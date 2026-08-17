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

#include "display_list.h"

#include <cstdio>

extern "C" {
const AuraRustDisplayList* aura_rust_native_build_display_list(const char* dom_ptr);
void aura_rust_native_free_display_list(AuraRustDisplayList* display);
}

const AuraRustDisplayList* aura_rust_bridge_build_display_list(const char* dom_ptr) {
    const AuraRustDisplayList* list = aura_rust_native_build_display_list(dom_ptr);
    if (list == nullptr) {
        return nullptr;
    }

    std::printf("[Aura Core] DisplayList Rust bridge initialized with %u rects, %u texts, %u borders.\n",
                list->rect_count, list->text_count, list->border_count);
    return list;
}

void aura_rust_bridge_free_display_list(AuraRustDisplayList* display) {
    if (display == nullptr) {
        return;
    }

    aura_rust_native_free_display_list(display);
}
