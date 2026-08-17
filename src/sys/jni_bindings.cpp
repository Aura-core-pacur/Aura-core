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

#include <cstdio>
#include <jni.h>

extern "C" {

JNIEXPORT void JNICALL Java_com_auracore_AuraEngine_initEngine(JNIEnv* env, jobject obj) {
    (void)env;
    (void)obj;
    aura_init();
}

JNIEXPORT void JNICALL Java_com_auracore_AuraEngine_renderFrame(JNIEnv* env, jobject obj) {
    (void)env;
    (void)obj;
    aura_render_frame();
}

JNIEXPORT void JNICALL Java_com_auracore_AuraEngine_loadUrl(JNIEnv* env, jobject obj, jstring url) {
    (void)obj;
    if (url == nullptr) {
        return;
    }

    const char* uri = env->GetStringUTFChars(url, nullptr);
    if (uri != nullptr) {
        aura_load_url(uri);
        env->ReleaseStringUTFChars(url, uri);
    }
}

JNIEXPORT void JNICALL Java_com_auracore_AuraEngine_evalJs(JNIEnv* env, jobject obj, jstring script) {
    (void)obj;
    if (script == nullptr) {
        return;
    }

    const char* js = env->GetStringUTFChars(script, nullptr);
    if (js != nullptr) {
        aura_eval_js(js);
        env->ReleaseStringUTFChars(script, js);
    }
}

JNIEXPORT void JNICALL Java_com_auracore_AuraEngine_createDow(JNIEnv* env, jobject obj, jstring xmlConfig) {
    (void)obj;
    if (xmlConfig == nullptr) {
        return;
    }

    const char* xml = env->GetStringUTFChars(xmlConfig, nullptr);
    if (xml != nullptr) {
        aura_create_dow(xml);
        env->ReleaseStringUTFChars(xmlConfig, xml);
    }
}

JNIEXPORT void JNICALL Java_com_auracore_AuraWindow_nativeCreateDow(JNIEnv* env, jobject obj, jstring xmlLayout) {
    (void)obj;
    if (xmlLayout == nullptr) {
        return;
    }

    const char* xml = env->GetStringUTFChars(xmlLayout, nullptr);
    if (xml != nullptr) {
        aura_create_dow(xml);
        env->ReleaseStringUTFChars(xmlLayout, xml);
    }
}

JNIEXPORT void JNICALL Java_com_auracore_AuraWindow_nativeCloseDow(JNIEnv* env, jobject obj) {
    (void)env;
    (void)obj;
    std::puts("[Aura Core] Ventana Dow cerrada.");
}

JNIEXPORT void JNICALL Java_com_auracore_AuraJSEngine_nativeEval(JNIEnv* env, jobject obj, jstring script) {
    (void)obj;
    if (script == nullptr) {
        return;
    }

    const char* js = env->GetStringUTFChars(script, nullptr);
    if (js != nullptr) {
        aura_eval_js(js);
        env->ReleaseStringUTFChars(script, js);
    }
}

JNIEXPORT jint JNICALL Java_com_auracore_AuraEngine_mediaOpen(JNIEnv* env, jobject obj, jstring resourcePath) {
    (void)obj;
    if (resourcePath == nullptr) {
        return 0;
    }

    const char* path = env->GetStringUTFChars(resourcePath, nullptr);
    if (path == nullptr) {
        return 0;
    }

    const int result = aura_media_open(path);
    env->ReleaseStringUTFChars(resourcePath, path);
    return result;
}

JNIEXPORT jint JNICALL Java_com_auracore_AuraEngine_mediaReadFrame(JNIEnv* env, jobject obj, jbyteArray rgbaBuffer, jint width, jint height) {
    (void)obj;
    if (rgbaBuffer == nullptr || width <= 0 || height <= 0) {
        return 0;
    }

    jsize len = env->GetArrayLength(rgbaBuffer);
    jbyte* buffer = env->GetByteArrayElements(rgbaBuffer, nullptr);
    if (buffer == nullptr) {
        return 0;
    }

    const int result = aura_media_read_frame(buffer, width, height);
    env->ReleaseByteArrayElements(rgbaBuffer, buffer, 0);
    return result;
}

JNIEXPORT void JNICALL Java_com_auracore_AuraEngine_mediaClose(JNIEnv* env, jobject obj) {
    (void)env;
    (void)obj;
    aura_media_close();
}

JNIEXPORT void JNICALL Java_com_auracore_AuraEngine_triggerUiAction(JNIEnv* env, jobject obj, jstring elementId) {
    (void)obj;
    if (elementId == nullptr) {
        return;
    }

    const char* element = env->GetStringUTFChars(elementId, nullptr);
    if (element != nullptr) {
        aura_trigger_ui_action(element);
        env->ReleaseStringUTFChars(elementId, element);
    }
}

} // extern "C"
