/*
 * Aura Core
 * Copyright (C) 2026 Aura-core-pacur
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "media_decoder.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

AuraMediaDecoder::AuraMediaDecoder() = default;

AuraMediaDecoder::~AuraMediaDecoder() {
    close_stream();
}

bool AuraMediaDecoder::open_stream(const char* resource_path) {
    if (resource_path == nullptr || std::strlen(resource_path) == 0U) {
        return false;
    }

    close_stream();

    if (avformat_open_input(&format_context_, resource_path, nullptr, nullptr) < 0) {
        std::puts("[Aura Media] No se pudo abrir el recurso.");
        return false;
    }

    if (avformat_find_stream_info(format_context_, nullptr) < 0) {
        std::puts("[Aura Media] No se pudo localizar información del stream.");
        avformat_close_input(&format_context_);
        return false;
    }

    for (unsigned int i = 0; i < format_context_->nb_streams; ++i) {
        const AVCodecParameters* codec_params = format_context_->streams[i]->codecpar;
        const AVCodec* codec = avcodec_find_decoder(codec_params->codec_id);
        if (codec == nullptr) {
            continue;
        }

        if (codec_params->codec_type == AVMEDIA_TYPE_VIDEO && video_stream_index_ < 0) {
            video_stream_index_ = static_cast<int>(i);
            AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
            if (codec_ctx == nullptr) {
                continue;
            }
            if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0) {
                avcodec_free_context(&codec_ctx);
                continue;
            }
            if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
                avcodec_free_context(&codec_ctx);
                continue;
            }
            video_codec_context_ = codec_ctx;
        } else if (codec_params->codec_type == AVMEDIA_TYPE_AUDIO && audio_stream_index_ < 0) {
            audio_stream_index_ = static_cast<int>(i);
            AVCodecContext* codec_ctx = avcodec_alloc_context3(codec);
            if (codec_ctx == nullptr) {
                continue;
            }
            if (avcodec_parameters_to_context(codec_ctx, codec_params) < 0) {
                avcodec_free_context(&codec_ctx);
                continue;
            }
            if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
                avcodec_free_context(&codec_ctx);
                continue;
            }
            audio_codec_context_ = codec_ctx;
        }
    }

    if (video_codec_context_ == nullptr) {
        std::puts("[Aura Media] No se encontró stream de video H.264 compatible.");
        close_stream();
        return false;
    }

    packet_ = av_packet_alloc();
    video_frame_ = av_frame_alloc();
    rgba_frame_ = av_frame_alloc();

    if (packet_ == nullptr || video_frame_ == nullptr || rgba_frame_ == nullptr) {
        std::puts("[Aura Media] No se pudo reservar memoria para el decoder.");
        close_stream();
        return false;
    }

    initialized_ = true;
    std::printf("[Aura Media] Recurso abierto: %s\n", resource_path);
    return true;
}

bool AuraMediaDecoder::read_frame(uint8_t* out_rgba_buffer, int target_width, int target_height) {
    if (!initialized_ || out_rgba_buffer == nullptr || target_width <= 0 || target_height <= 0) {
        return false;
    }

    if (video_codec_context_ == nullptr) {
        return false;
    }

    while (av_read_frame(format_context_, packet_) >= 0) {
        if (packet_->stream_index != video_stream_index_) {
            av_packet_unref(packet_);
            continue;
        }

        int decode_result = avcodec_send_packet(video_codec_context_, packet_);
        av_packet_unref(packet_);

        if (decode_result < 0) {
            continue;
        }

        while ((decode_result = avcodec_receive_frame(video_codec_context_, video_frame_)) >= 0) {
            if (sws_context_ == nullptr) {
                const AVPixelFormat source_format = static_cast<AVPixelFormat>(video_frame_->format);
                sws_context_ = sws_getContext(
                    video_frame_->width,
                    video_frame_->height,
                    source_format,
                    target_width,
                    target_height,
                    AV_PIX_FMT_RGBA,
                    SWS_BILINEAR,
                    nullptr,
                    nullptr,
                    nullptr);

                if (sws_context_ == nullptr) {
                    std::puts("[Aura Media] Error al crear contexto de conversión de color.");
                    return false;
                }
            }

            const int stride = target_width * 4;
            const int expected_size = stride * target_height;
            if (rgba_frame_->data[0] == nullptr || rgba_frame_->width != target_width || rgba_frame_->height != target_height) {
                av_frame_unref(rgba_frame_);
                rgba_frame_->format = AV_PIX_FMT_RGBA;
                rgba_frame_->width = target_width;
                rgba_frame_->height = target_height;

                if (av_frame_get_buffer(rgba_frame_, 32) < 0) {
                    std::puts("[Aura Media] Error al reservar framebuffer RGBA.");
                    return false;
                }
            }

            sws_scale(
                sws_context_,
                video_frame_->data,
                video_frame_->linesize,
                0,
                video_frame_->height,
                rgba_frame_->data,
                rgba_frame_->linesize);

            std::memcpy(out_rgba_buffer, rgba_frame_->data[0], static_cast<size_t>(expected_size));
            av_frame_unref(video_frame_);
            return true;
        }

        if (decode_result == AVERROR(EAGAIN) || decode_result == AVERROR_EOF) {
            continue;
        }
    }

    return false;
}

void AuraMediaDecoder::close_stream() {
    if (sws_context_ != nullptr) {
        sws_freeContext(sws_context_);
        sws_context_ = nullptr;
    }

    if (video_frame_ != nullptr) {
        av_frame_free(&video_frame_);
        video_frame_ = nullptr;
    }

    if (rgba_frame_ != nullptr) {
        av_frame_free(&rgba_frame_);
        rgba_frame_ = nullptr;
    }

    if (packet_ != nullptr) {
        av_packet_free(&packet_);
        packet_ = nullptr;
    }

    if (video_codec_context_ != nullptr) {
        avcodec_free_context(&video_codec_context_);
        video_codec_context_ = nullptr;
    }

    if (audio_codec_context_ != nullptr) {
        avcodec_free_context(&audio_codec_context_);
        audio_codec_context_ = nullptr;
    }

    if (format_context_ != nullptr) {
        avformat_close_input(&format_context_);
        format_context_ = nullptr;
    }

    video_stream_index_ = -1;
    audio_stream_index_ = -1;
    initialized_ = false;
}
