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

#ifndef AURA_MEDIA_DECODER_H
#define AURA_MEDIA_DECODER_H

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
#include <libswscale/swscale.h>
}

#include <cstdint>

class AuraMediaDecoder {
public:
    AuraMediaDecoder();
    ~AuraMediaDecoder();

    bool open_stream(const char* resource_path);
    bool read_frame(uint8_t* out_rgba_buffer, int target_width, int target_height);
    void close_stream();

private:
    AVFormatContext* format_context_ = nullptr;
    AVCodecContext* video_codec_context_ = nullptr;
    AVCodecContext* audio_codec_context_ = nullptr;
    AVFrame* video_frame_ = nullptr;
    AVFrame* rgba_frame_ = nullptr;
    SwsContext* sws_context_ = nullptr;
    AVPacket* packet_ = nullptr;

    int video_stream_index_ = -1;
    int audio_stream_index_ = -1;
    bool initialized_ = false;
};

#endif
