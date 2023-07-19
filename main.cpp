#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/binary_info.h"

#include "Render.h"
#include "KeyboardController.h"
#include <math.h>

const uint8_t LED_PIN = 25;

int main()
{
    stdio_init_all();

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    LCDRender s(21, 20);
    const uint8_t ipins[] = {11,12,13,14,6};
    const uint8_t opins[] = {10, 9, 8, 7};
    KeyboardController ctl(opins, ipins);
    s.setTextWrap(false);
    if(!ctl.startListener()){
        s.puts("ERROR\n");
        s.drawBuffer();
    }

    while(true)
    {
        s.fill(false);
        s.setCursor(0,0);
        uint32_t state = ctl.getKeyboardState();
        s.putint16(state);

        s.drawBuffer();
        sleep_ms(50);
    }
}