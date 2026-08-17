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

package main

/*
#include <stdlib.h>
*/
import "C"

import "fmt"

//export AuraNet_FetchURL
func AuraNet_FetchURL(url *C.char) *C.char {
    if url == nil {
        return C.CString("")
    }

    output := fmt.Sprintf("[AuraNet] Fetching URL: %s\n", C.GoString(url))
    return C.CString(output)
}

//export AuraNet_DownloadFile
func AuraNet_DownloadFile(url *C.char, destPath *C.char) *C.char {
    if url == nil || destPath == nil {
        return C.CString("error: missing params")
    }

    output := fmt.Sprintf("[AuraNet] Downloaded %s -> %s\n", C.GoString(url), C.GoString(destPath))
    return C.CString(output)
}

//export AuraNet_UploadFile
func AuraNet_UploadFile(url *C.char, filePath *C.char) *C.char {
    if url == nil || filePath == nil {
        return C.CString("error: missing params")
    }

    output := fmt.Sprintf("[AuraNet] Uploaded %s <- %s\n", C.GoString(url), C.GoString(filePath))
    return C.CString(output)
}

func main() {}
