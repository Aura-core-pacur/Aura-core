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

public class AuraJSEngine {
    private String currentContext;
    private boolean initialized;

    public AuraJSEngine() {
        this.currentContext = "aura-js-runtime";
        this.initialized = false;
    }

    public native void nativeEval(String script);

    public void initialize() {
        this.initialized = true;
        System.out.println("[AuraJS Engine] Contexto inicializado: " + currentContext);
    }

    public void execute(String script) {
        if (!initialized) {
            initialize();
        }

        if (script == null || script.isBlank()) {
            System.out.println("[AuraJS Engine] Script vacío, no se ejecuta ninguna acción.");
            return;
        }

        System.out.println("[AuraJS Engine] Ejecutando script: " + script.substring(0, Math.min(script.length(), 80)));
        nativeEval(script);
    }

    public Object evaluate(String script) {
        execute(script);
        return null;
    }

    public void loadFromPage(String pageSource) {
        System.out.println("[AuraJS Engine] Procesando contenido de página web para evaluación.");
        if (pageSource != null && !pageSource.isBlank()) {
            nativeEval(pageSource);
        }
    }

    public String getCurrentContext() {
        return currentContext;
    }

    public boolean isInitialized() {
        return initialized;
    }
}
