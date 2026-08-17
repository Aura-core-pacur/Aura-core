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

#include "xml_parser.h"

#include <cctype>
#include <cstdio>

extern "C" void aura_trigger_ui_action(const char* element_id);

static bool g_ui_event_test_triggered = false;

std::vector<AuraUIElement> AuraXmlParser::parse(const std::string& xml_text) {
    std::vector<AuraUIElement> components;
    std::size_t pos = 0;

    while (pos < xml_text.size()) {
        const std::size_t lt = xml_text.find('<', pos);
        if (lt == std::string::npos) {
            break;
        }

        const std::size_t gt = xml_text.find('>', lt + 1);
        if (gt == std::string::npos) {
            break;
        }

        const std::string tag = xml_text.substr(lt + 1, gt - lt - 1);
        if (!tag.empty() && tag[0] != '/' && tag[0] != '!') {
            AuraUIElement element;

            std::size_t name_end = 0;
            while (name_end < tag.size() && !std::isspace(static_cast<unsigned char>(tag[name_end])) && tag[name_end] != '/') {
                ++name_end;
            }
            element.type = tag.substr(0, name_end);

            std::size_t cursor = name_end;
            while (cursor < tag.size()) {
                while (cursor < tag.size() && std::isspace(static_cast<unsigned char>(tag[cursor]))) {
                    ++cursor;
                }
                if (cursor >= tag.size() || tag[cursor] == '/') {
                    break;
                }

                const std::size_t eq = tag.find('=', cursor);
                if (eq == std::string::npos) {
                    break;
                }

                std::size_t key_start = cursor;
                std::size_t key_end = eq;
                while (key_end > key_start && std::isspace(static_cast<unsigned char>(tag[key_end - 1]))) {
                    --key_end;
                }

                const std::string key = tag.substr(key_start, key_end - key_start);
                const std::size_t value_start = tag.find('"', eq + 1);
                if (value_start == std::string::npos) {
                    break;
                }
                const std::size_t value_end = tag.find('"', value_start + 1);
                if (value_end == std::string::npos) {
                    break;
                }

                const std::string value = tag.substr(value_start + 1, value_end - value_start - 1);
                element.attributes[key] = value;
                if (key == "id") {
                    element.id = value;
                } else if (key == "action") {
                    element.action = value;
                } else if (key == "data-url") {
                    element.data_url = value;
                } else if (key == "source") {
                    element.source = value;
                }
                cursor = value_end + 1;
            }

            if (element.type == "media_player" && !element.source.empty()) {
                std::printf("[Aura XML] media_player detected: source=%s\n", element.source.c_str());
            }

            components.push_back(element);
        }

        pos = gt + 1;
    }

    return components;
}

AuraUIElement AuraXmlParser::find_element(const std::vector<AuraUIElement>& elements, const std::string& element_id) {
    for (const auto& element : elements) {
        if (element.id == element_id) {
            return element;
        }
    }

    return AuraUIElement{};
}
