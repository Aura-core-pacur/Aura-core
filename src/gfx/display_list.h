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

#ifndef AURA_DISPLAY_LIST_H
#define AURA_DISPLAY_LIST_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

struct AuraRustRect {
    float x;
    float y;
    float width;
    float height;
    std::uint32_t color;
};

struct AuraRustText {
    float x;
    float y;
    std::uint32_t color;
    float size;
    const char* text_ptr;
};

struct AuraRustImage {
    float x;
    float y;
    float width;
    float height;
    const char* image_ptr;
};

struct AuraRustBorder {
    float x;
    float y;
    float width;
    float height;
    float stroke;
    std::uint32_t color;
};

struct AuraRustDisplayList {
    std::uint32_t rect_count;
    std::uint32_t text_count;
    std::uint32_t image_count;
    std::uint32_t border_count;
    AuraRustRect* rects;
    AuraRustText* texts;
    AuraRustImage* images;
    AuraRustBorder* borders;
};

const AuraRustDisplayList* aura_rust_bridge_build_display_list(const char* dom_ptr);
void aura_rust_bridge_free_display_list(AuraRustDisplayList* display);

#ifdef __cplusplus
}
#endif

#endif
