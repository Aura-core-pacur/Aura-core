package com.auracore;

import java.nio.file.Files;
import java.nio.file.Paths;

public class Main {
    static {
        System.loadLibrary("aura_core_native");
    }

    public static void main(String[] args) {
        AuraEngine engine = new AuraEngine();
        engine.initEngine();

        try {
            String xml = Files.readString(Paths.get("src/ui/layouts/test_dow.xml"));
            engine.createDow(xml);
        } catch (Exception ex) {
            System.out.println("[AuraMain] No se pudo cargar test_dow.xml: " + ex.getMessage());
            engine.createDowFromXML("<dow title=\"AuraTest\" width=\"1280\" height=\"720\" />");
        }

        engine.loadUrl("https://www.google.com");
        engine.renderFrame();

        String sample = args.length > 0 ? args[0] : "/tmp/test_media.mp4";
        byte[] rgba = new byte[1280 * 720 * 4];

        int opened = engine.mediaOpen(sample);
        if (opened != 0) {
            int frames = 0;
            while (frames < 3) {
                int ok = engine.mediaReadFrame(rgba, 1280, 720);
                if (ok == 0) {
                    break;
                }
                frames++;
                System.out.println("[AuraMain] Frame video RGBA: " + frames + " / 1280x720");
            }
            engine.mediaClose();
            System.out.println("[AuraMain] Buffers liberados y reproducción simulada completada.");
        } else {
            System.out.println("[AuraMain] No se pudo abrir la prueba de media: " + sample);
        }
        rgba = null;
        System.gc();
        System.out.println("[AuraMain] Flujo JNI -> C-ABI completado sin fallo de segmentación.");
    }
}
