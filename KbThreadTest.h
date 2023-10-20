#pragma once
#include "Render.h"
#include "KeyboardController.h"

int KbThreadTest(LCDRender& render, KeyboardController& kbctl) {
    render.fill(false);
    render.setCursor(0,0);
    render.puts("Waiting for\nevent...");
    render.drawBuffer();
    while(true) {
        if(kbctl.isEvent()) {
            auto event = kbctl.getEvent();
            render.fill(false);
            render.setCursor(0,0);
            render.puts("Last event:\nkeycode: ");
            render.putint10(event.code);
            render.puts("\ntype: ");
            render.putint10(event.type);
            render.drawBuffer();
        }
        sleep_ms(100);
        if(kbctl.getKeyboardState() == 0x10000001) break;
    }
    return 0;
}