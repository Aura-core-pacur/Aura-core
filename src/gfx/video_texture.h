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

#ifndef AURA_VIDEO_TEXTURE_H
#define AURA_VIDEO_TEXTURE_H

#include <array>
#include <cstdint>
#include <vector>

class AuraVideoTexture {
public:
    AuraVideoTexture();
    ~AuraVideoTexture();

    void upload_rgba(const uint8_t* rgba_buffer, int width, int height);
    void bind();
    void unbind();
    bool is_ready() const;

    int width() const;
    int height() const;
    const uint8_t* cpu_buffer() const;

private:
    void resize_staging(int width, int height);

    int width_ = 0;
    int height_ = 0;
    uint32_t gpu_texture_id_ = 0;
    std::vector<uint8_t> staging_buffer_;
    std::array<std::vector<uint8_t>, 2> double_buffers_{};
    std::size_t write_index_ = 0;
    std::size_t read_index_ = 1;
    bool ready_ = false;
};

#endif
