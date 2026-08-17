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

package com.auracore

import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

class AuraEngineKotlin {
    companion object {
        init {
            System.loadLibrary("aura_core_native")
        }

        external fun initEngine(): Unit
        external fun renderFrame(): Unit
        external fun loadUrl(url: String): Unit
        external fun createDow(xmlConfig: String): Unit
        external fun mediaOpen(resourcePath: String): Int
        external fun mediaReadFrame(buffer: ByteArray, width: Int, height: Int): Int
        external fun mediaClose(): Unit
        external fun triggerUiAction(elementId: String): Unit
    }

    fun start() {
        GlobalScope.launch(Dispatchers.IO) {
            Companion.initEngine()
        }
    }

    fun render() {
        CoroutineScope(Dispatchers.Default).launch {
            Companion.renderFrame()
        }
    }

    fun onDownloadComplete(url: String) {
        CoroutineScope(Dispatchers.Main).launch {
            println("[Aura Kotlin] Download finalized: $url")
        }
    }
}
