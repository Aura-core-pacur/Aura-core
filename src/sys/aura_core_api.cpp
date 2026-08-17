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

#include "aura_core_api.h"

#include "display_list.h"
#include "event_router.h"
#include "media_decoder.h"
#include "os_paths.h"
#include "xml_parser.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <sstream>

extern "C" {
char* AuraNet_DownloadFile(char* url, char* dest_path);
char* AuraNet_UploadFile(char* url, char* file_path);
}

static bool g_aura_initialized = false;
static AuraMediaDecoder g_media_decoder;

void aura_init(void) {
    g_aura_initialized = true;

    char data_dir[512];
    if (aura_os_get_user_data_dir(data_dir, sizeof(data_dir)) == 0) {
        std::printf("[Aura Core] User data dir: %s\n", data_dir);
    }

    std::puts("[Aura Core] Sistema inicializado.");
}

void aura_load_url(const char* url) {
    if (url == nullptr || std::strlen(url) == 0U) {
        std::puts("[Aura Core] URL vacía. No se inicia la carga.");
        return;
    }

    std::printf("[Aura Core] Cargando URL: %s\n", url);
}

void aura_render_frame(void) {
    if (!g_aura_initialized) {
        std::puts("[Aura Core] Engine no inicializado; arrancando render con initial setup.");
        aura_init();
    }

    const AuraRustDisplayList* list = aura_rust_bridge_build_display_list("<window><button id=\"submit\"/></window>");
    if (list != nullptr) {
        std::printf("[Aura Core] Render pipeline ejecutado con %u rects y %u textos.\n",
                    list->rect_count, list->text_count);
        aura_rust_bridge_free_display_list(const_cast<AuraRustDisplayList*>(list));
    } else {
        std::puts("[Aura Core] Render pipeline ejecutado.");
    }
}

void aura_eval_js(const char* script) {
    if (script == nullptr || std::strlen(script) == 0U) {
        std::puts("[Aura Core] Script JS vacío.");
        return;
    }

    std::printf("[Aura Core] Ejecutando JS: %s\n", script);
}

void aura_create_dow(const char* xml_config) {
    if (xml_config == nullptr || std::strlen(xml_config) == 0U) {
        std::puts("[Aura Core] XML de Dow vacío.");
        return;
    }

    const auto components = AuraXmlParser::parse(xml_config);
    std::printf("[Aura Core] Creando Dow desde XML con %zu componentes.\n", components.size());
    for (const auto& component : components) {
        std::printf("[Aura UI] %s id=%s action=%s source=%s\n", component.type.c_str(), component.id.c_str(), component.action.c_str(), component.source.c_str());
    }

    const AuraRustDisplayList* list = aura_rust_bridge_build_display_list(xml_config);
    if (list != nullptr) {
        std::printf("[Aura Core] DisplayList generada por Rust: %u rects.\n", list->rect_count);
        aura_rust_bridge_free_display_list(const_cast<AuraRustDisplayList*>(list));
    }
}

void aura_trigger_ui_action(const char* element_id) {
    if (element_id == nullptr || std::strlen(element_id) == 0U) {
        return;
    }

    std::ifstream layout_file("src/ui/layouts/test_dow.xml");
    std::stringstream buffer;
    if (layout_file.good()) {
        buffer << layout_file.rdbuf();
    }

    const std::string xml = buffer.str();
    if (xml.empty()) {
        std::printf("[Aura UI] No se pudo abrir %s para el evento %s.\n", "src/ui/layouts/test_dow.xml", element_id);
        return;
    }

    const auto elements = AuraXmlParser::parse(xml);
    const auto found = AuraXmlParser::find_element(elements, element_id);
    if (found.id.empty()) {
        std::printf("[Aura UI] No existe el elemento %s en el layout.\n", element_id);
        return;
    }

    std::printf("[Aura UI] Disparando acción para %s id=%s action=%s\n", found.type.c_str(), found.id.c_str(), found.action.c_str());
    AuraEventRouter::dispatch_event(found.id.c_str(), found.action.c_str(), found.data_url.c_str());
    if (!found.source.empty()) {
        AuraEventRouter::dispatch_event(found.id.c_str(), "media_player", found.source.c_str());
    }
}

int aura_get_user_data_dir(char* out_path, size_t max_len) {
    return aura_os_get_user_data_dir(out_path, max_len);
}

int aura_net_download_file(const char* url, const char* dest_path) {
    if (url == nullptr || dest_path == nullptr) {
        return -1;
    }

    char* url_copy = const_cast<char*>(url);
    char* path_copy = const_cast<char*>(dest_path);
    char* result = AuraNet_DownloadFile(url_copy, path_copy);
    if (result == nullptr) {
        return -1;
    }

    std::printf("[Aura Net] Download result: %s\n", result);
    return 0;
}

int aura_net_upload_file(const char* url, const char* file_path) {
    if (url == nullptr || file_path == nullptr) {
        return -1;
    }

    char* url_copy = const_cast<char*>(url);
    char* file_copy = const_cast<char*>(file_path);
    char* result = AuraNet_UploadFile(url_copy, file_copy);
    if (result == nullptr) {
        return -1;
    }

    std::printf("[Aura Net] Upload result: %s\n", result);
    return 0;
}

int aura_media_open(const char* resource_path) {
    if (resource_path == nullptr || std::strlen(resource_path) == 0U) {
        std::puts("[Aura Core] Ruta de media vacía.");
        return 0;
    }

    return g_media_decoder.open_stream(resource_path) ? 1 : 0;
}

int aura_media_read_frame(void* rgba_buffer, int width, int height) {
    if (rgba_buffer == nullptr || width <= 0 || height <= 0) {
        return 0;
    }

    return g_media_decoder.read_frame(static_cast<uint8_t*>(rgba_buffer), width, height) ? 1 : 0;
}

void aura_media_close(void) {
    g_media_decoder.close_stream();
}
