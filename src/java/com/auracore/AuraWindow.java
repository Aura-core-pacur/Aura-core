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

public class AuraWindow {
    private String title;
    private int width;
    private int height;
    private boolean visible;

    public AuraWindow() {
        this.title = "Aura Core Window";
        this.width = 1280;
        this.height = 720;
        this.visible = true;
    }

    public native void nativeCreateDow(String xmlLayout);
    public native void nativeCloseDow();

    public void loadFromXml(String xmlPath) {
        System.out.println("[AuraUI] Carga de layout XML: " + xmlPath);
        nativeCreateDow(xmlPath);
    }

    public void show() {
        this.visible = true;
        System.out.println("[AuraUI] Ventana visible: " + title);
    }

    public void hide() {
        this.visible = false;
        System.out.println("[AuraUI] Ventana oculta: " + title);
    }

    public void resize(int width, int height) {
        this.width = width;
        this.height = height;
        System.out.println("[AuraUI] Resize -> " + width + "x" + height);
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public int getWidth() {
        return width;
    }

    public int getHeight() {
        return height;
    }

    public boolean isVisible() {
        return visible;
    }
}
