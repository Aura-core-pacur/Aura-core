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

package com.auracore;

public class AuraEngine {
    static {
        System.loadLibrary("aura_core_native");
    }

    private final AuraWindow window;
    private final AuraJSEngine jsEngine;

    public AuraEngine() {
        this.window = new AuraWindow();
        this.jsEngine = new AuraJSEngine();
    }

    // Métodos nativos expuestos hacia C/Rust/C++
    public native void initEngine();
    public native void renderFrame();
    public native void loadUrl(String url);
    public native void evalJs(String script);
    public native void createDow(String xmlConfig);
    public native int mediaOpen(String resourcePath);
    public native int mediaReadFrame(byte[] rgbaBuffer, int width, int height);
    public native void mediaClose();
    public native void triggerUiAction(String elementId);

    // Gestión de JavaScript desde Java
    public void executeJS(String script) {
        System.out.println("[AuraJS Engine] Ejecutando código JavaScript...");
        jsEngine.execute(script);
    }

    // Creación de ventanas (Dows)
    public void createDowFromXML(String xmlPath) {
        System.out.println("[AuraUI] Cargando ventana 'Dow' desde: " + xmlPath);
        window.loadFromXml(xmlPath);
    }

    public AuraWindow getWindow() {
        return window;
    }

    public AuraJSEngine getJsEngine() {
        return jsEngine;
    }
}
