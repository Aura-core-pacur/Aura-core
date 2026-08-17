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

#include "event_router.h"

#include "aura_core_api.h"

#include <cstdio>
#include <cstring>

void AuraEventRouter::dispatch_event(const char* element_id, const char* action, const char* payload) {
    if (element_id == nullptr || action == nullptr) {
        return;
    }

    if (std::strcmp(action, "aura_net_download_file") == 0) {
        const char* source_url = payload != nullptr ? payload : "https://example.com/file.zip";
        const char* default_dest = "/tmp/aura_download.zip";
        std::printf("[Aura EventRouter] Dispatching Go download from %s -> %s\n", source_url, default_dest);
        aura_net_download_file(source_url, default_dest);
        return;
    }

    if (std::strcmp(action, "media_player") == 0 || std::strcmp(action, "play") == 0) {
        const char* source = payload != nullptr ? payload : "test_media.mp4";
        std::printf("[Aura EventRouter] Initializing FFmpeg media source: %s\n", source);
        aura_media_open(source);
        return;
    }

    std::printf("[Aura EventRouter] Unhandled action '%s' for element '%s' payload='%s'\n",
                action, element_id, payload != nullptr ? payload : "");
}
