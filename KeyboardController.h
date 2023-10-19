#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/multicore.h"

enum KbEventType : uint8_t {
    None = 0,
    Pressed = 1,
    Released = 2
};

struct KeyboardEvent {
    KbEventType type : 3;
    uint8_t code : 5;
};

class EventQueue 
{
private:
    KeyboardEvent events[128];
    uint8_t begin = 0;
    uint8_t end = 0;
public:

    inline bool isEmpty() {return begin == end;}
    inline bool isFull() {return (end + 1) % 128 == begin; }
    
    bool push(KeyboardEvent e) 
    {
        if(isFull()) return false;
        events[end] = e;
        end = (end + 1) % 128;
        return true;
    }

    auto pop() -> KeyboardEvent 
    {
        if(isEmpty()) return {None, 0};
        auto res = events[begin];
        begin = (begin + 1) % 128;
        return res;
    }

    inline void clear() {
        begin = end = 0;
    }
};

class KeyboardController 
{
private:
    uint8_t outpins[4];
    uint8_t inpins[5];
    uint32_t kbstate;
    bool keys[20];
    bool listenerStarted = false;
    bool threaded;
    bool paused = false;
    EventQueue queue;

public:
    KeyboardController(const uint8_t outputpins[4], const uint8_t inputpins[5], bool doThreading);
    bool startListener();
    bool stopListener();

    uint32_t getKeyboardState();

    inline auto getEvent() -> KeyboardEvent
    {
        return queue.pop();
    }
    inline bool isEvent()
    {
        return !queue.isEmpty();
    }
    inline void clearEvents() {
        queue.clear();
    }

    inline bool* getKeys()
    {
        return keys;
    }
    bool getAsyncKeyState(uint8_t outputPin, uint8_t inputPin);
    inline bool getAsyncKeyState(uint8_t keycode)
    {
        return getAsyncKeyState((keycode & 0b11000) >> 3, keycode & 0b111);
    }
    void listenerFunction();

private:
    void getKbStateHelper();

};

void core1function() {
    auto kbctl = (KeyboardController*)multicore_fifo_pop_blocking();
    kbctl->listenerFunction();
}

bool KeyboardController::getAsyncKeyState(uint8_t outputPin, uint8_t inputPin) 
{
    paused = true;

    //avoid race condition
    sleep_ms(10);
    gpio_put(outpins[outputPin], true);
    auto res = gpio_get(inpins[inputPin]);

    paused = false;

    return res;
}

bool KeyboardController::startListener() 
{
    if(!threaded) return false;
    multicore_launch_core1(core1function);
    multicore_fifo_push_blocking((uint32_t)this);

    uint32_t res;
    if(multicore_fifo_pop_timeout_us(1000000, &res)) 
        return listenerStarted = true;
    
    return false;
}

bool KeyboardController::stopListener()
{
    if(!threaded) return false;
    multicore_reset_core1();
    return true;
}

KeyboardController::KeyboardController(const uint8_t outputpins[4], const uint8_t inputpins[5], bool doThreading = true) 
{
    for(uint8_t i = 0; i < 4; i++) 
    {
        outpins[i] = outputpins[i];
        gpio_init(outpins[i]);
        gpio_set_dir(outpins[i], GPIO_OUT);
        gpio_put(outpins[i], false);
    }

    for(uint8_t i = 0; i < 5; i++) 
    {
        inpins[i] = inputpins[i];
        gpio_init(inpins[i]);
        gpio_set_dir(inpins[i], GPIO_IN);
        gpio_pull_down(inpins[i]);
    }

    threaded = doThreading;
}

uint32_t KeyboardController::getKeyboardState() 
{
    if(!threaded) getKbStateHelper();

    return kbstate;
}

void KeyboardController::getKbStateHelper()
{
    kbstate = 0;
    uint8_t offset = 0;

    for(uint8_t out = 0; out < 4; out++)
    {
        gpio_put(outpins[out], true);

        for(uint8_t in = 0; in < 5; in++)
        {
            if(keys[out*5+in] = gpio_get(inpins[in])) kbstate += (1 << offset);
            offset++;
            sleep_us(100);
        }
        offset+=3;

        gpio_put(outpins[out], false);
        sleep_us(100);
    }
}

void KeyboardController::listenerFunction() 
{
    multicore_fifo_push_blocking(1);

    while(true) if(!paused)
    {
        gpio_put(outpins[0], false);
        gpio_put(outpins[1], false);
        gpio_put(outpins[2], false);
        gpio_put(outpins[3], false);

        uint32_t newkbstate = 0;
        uint8_t offset = 0;

        for(uint8_t out = 0; out < 4; out++)
        {
            gpio_put(outpins[out], true);

            for(uint8_t in = 0; in < 5; in++)
            {
                if(keys[out*5+in] = gpio_get(inpins[in])) newkbstate += (1 << offset);
                //check if it's changed
                if((newkbstate & (1 << offset)) != (kbstate & (1 << offset)))
                {
                    KeyboardEvent e;
                    if(newkbstate & (1 << offset)) 
                        e.type = Pressed;
                    else 
                        e.type = Released;

                    e.code = in | (out << 3);

                    queue.push(e);
                }

                offset++;
                sleep_us(100);
            }

            gpio_put(outpins[out], false);
            offset += 3;
            sleep_us(100);
        }
        kbstate = newkbstate;
    }
}