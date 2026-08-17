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

#include "video_texture.h"

#include <cstring>

AuraVideoTexture::AuraVideoTexture() = default;

AuraVideoTexture::~AuraVideoTexture() {
    if (gpu_texture_id_ != 0) {
        // En una implementación completa esto se liberaría con el contexto OpenGL/Vulkan.
        gpu_texture_id_ = 0;
    }
    double_buffers_[0].clear();
    double_buffers_[1].clear();
    staging_buffer_.clear();
}

void AuraVideoTexture::resize_staging(int width, int height) {
    width_ = width;
    height_ = height;
    const std::size_t bytes = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4U;
    staging_buffer_.resize(bytes);

    for (auto& buffer : double_buffers_) {
        buffer.resize(bytes);
    }
}

void AuraVideoTexture::upload_rgba(const uint8_t* rgba_buffer, int width, int height) {
    if (rgba_buffer == nullptr || width <= 0 || height <= 0) {
        return;
    }

    if (width_ != width || height_ != height) {
        resize_staging(width, height);
    }

    std::vector<uint8_t>& current = double_buffers_[write_index_];
    std::memcpy(current.data(), rgba_buffer, current.size());

    std::swap(write_index_, read_index_);
    ready_ = true;

    if (gpu_texture_id_ == 0) {
        gpu_texture_id_ = 1;
    }
}

void AuraVideoTexture::bind() {
    // Placeholder: en un backend real se enlazaría con OpenGL/Vulkan.
    (void)gpu_texture_id_;
}

void AuraVideoTexture::unbind() {
    // Placeholder: cierre de la liga de textura.
}

bool AuraVideoTexture::is_ready() const {
    return ready_;
}

int AuraVideoTexture::width() const {
    return width_;
}

int AuraVideoTexture::height() const {
    return height_;
}

const uint8_t* AuraVideoTexture::cpu_buffer() const {
    return double_buffers_[read_index_].empty() ? nullptr : double_buffers_[read_index_].data();
}
