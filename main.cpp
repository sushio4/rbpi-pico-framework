#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "Render.h"
#include "KeyboardController.h"
#include <math.h>
#include "Snake.hpp"
#include "KbThreadTest.h"

const uint8_t LED_PIN = 25;

int KeyboardTestProgram(LCDRender& render, KeyboardController& kbctl);

struct Program {
    int (*ptr)(LCDRender&,KeyboardController&);
    char name[15];
};

void processCode(LCDRender& s, KeyboardController& ctl, int exitCode) {
    if(exitCode != 0) {
        s.fill(false);
        s.setCursor(0,0);
        s.puts("Program failed\nCode: ");
        s.putint10(exitCode);
        s.puts("\nPress any key");
        s.drawBuffer();
        while(!ctl.getKeyboardState()) {
            sleep_ms(2);
        }
    }
}

int main()
{
    //initializing board
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    LCDRender s(21, 20);
    s.setTextWrap(false);

    const uint8_t ipins[] = {11,12,13,14,6};
    const uint8_t opins[] = {10, 9, 8, 7};
    KeyboardController ctl(opins, ipins, true);
    if(!ctl.startListener()) {
        s.fill(0);
        s.setCursor(0,0);
        s.puts("Keyboard\nerror!");
        s.drawBuffer();
        return 0;
    }

    const uint8_t progCount = 3;
    Program programs[progCount] = {
        {SnakeProgram, "Snake"}, 
        {KeyboardTestProgram, "Kb debug"},
        {KbThreadTest, "Kb thread dbg"}
    };
    uint8_t chosen = 0;

    while(true) {
        s.fill(false);
        s.setCursor(0,0);
        for(uint8_t i = 0; i < progCount; i++) {
            s.putchar(chosen == i ? '>' : ' ');
            s.puts(programs[i].name);
            s.putchar('\n');
        }
        s.drawBuffer();

        uint32_t state;
        while((state = ctl.getKeyboardState()) == 0) {
            sleep_ms(10);
        }
        switch (state)
        {
        case 0x100: //up
            chosen--;
            break;
        case 0x200: //down
            chosen++;
            break;
        case 0x20000: 
            //execute
            ctl.clearEvents();
            int exitCode = programs[chosen].ptr(s, ctl);
            ctl.clearEvents();
            processCode(s, ctl, exitCode);
            break;
        }
        chosen = (chosen + progCount) % progCount;
        sleep_ms(300);
    }
    s.fill(false);
    s.setCursor(0,0);
    s.puts("Finished.");
    s.drawBuffer();
}

int KeyboardTestProgram(LCDRender& s, KeyboardController& ctl) {
    while(true)
    {
        s.fill(false);
        s.setCursor(0,0);
        uint32_t state = ctl.getKeyboardState();
        s.putint16(state);

        s.drawBuffer();
        sleep_ms(50);
        if(state == 0x10000001) return 0;
    }
}