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

#include "os_paths.h"

#include <cstdlib>
#include <cstring>
#include <string>

int aura_os_get_user_data_dir(char* out_path, size_t max_len) {
    if (out_path == nullptr || max_len == 0U) {
        return -1;
    }

    const char* env = std::getenv("XDG_DATA_HOME");
    std::string base;

    if (env != nullptr && env[0] != '\0') {
        base = env;
    } else {
        const char* home = std::getenv("HOME");
        if (home != nullptr && home[0] != '\0') {
            base = std::string(home) + "/.local/share";
        } else {
            base = "/tmp/aura";
        }
    }

    std::string final_path = base + "/aura-core";
    if (final_path.length() + 1U > max_len) {
        return -1;
    }

    std::strncpy(out_path, final_path.c_str(), max_len);
    out_path[max_len - 1U] = '\0';
    return 0;
}
