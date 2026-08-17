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

#ifndef AURA_XML_PARSER_H
#define AURA_XML_PARSER_H

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

struct AuraUIElement {
    std::string id;
    std::string type;
    std::string action;
    std::string data_url;
    std::string source;
    std::unordered_map<std::string, std::string> attributes;
};

class AuraXmlParser {
public:
    static std::vector<AuraUIElement> parse(const std::string& xml_text);
    static AuraUIElement find_element(const std::vector<AuraUIElement>& elements, const std::string& element_id);
};

#endif
