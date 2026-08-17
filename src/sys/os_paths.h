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

#ifndef AURA_OS_PATHS_H
#define AURA_OS_PATHS_H

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

int aura_os_get_user_data_dir(char* out_path, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif
