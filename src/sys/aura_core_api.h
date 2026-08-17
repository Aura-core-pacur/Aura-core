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

#ifndef AURA_CORE_API_H
#define AURA_CORE_API_H

#include <cstddef>

#ifdef __cplusplus
extern "C" {
#endif

// Funciones expuestas por el motor Aura Core
void aura_init(void);
void aura_load_url(const char* url);
void aura_render_frame(void);
void aura_eval_js(const char* script);
void aura_create_dow(const char* xml_config);
void aura_trigger_ui_action(const char* element_id);
int aura_get_user_data_dir(char* out_path, size_t max_len);
int aura_net_download_file(const char* url, const char* dest_path);
int aura_net_upload_file(const char* url, const char* file_path);
int aura_media_open(const char* resource_path);
int aura_media_read_frame(void* rgba_buffer, int width, int height);
void aura_media_close(void);

#ifdef __cplusplus
}
#endif

#endif
