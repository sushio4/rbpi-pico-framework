#pragma once
#include "Render.h"
#include "Util.h"
#include "KeyboardController.h"
#include "pico/multicore.h"

int SnakeProgram(LCDRender& render, KeyboardController& kbctl) {
    const uint8_t height = 16;
    const uint8_t width = 28;
    const uint16_t size = 16 * 28;

    enum Direction : uint8_t {
        up = 0, 
        right = 1, 
        down = 2, 
        left = 3
    };
    
    int8_t headPosX = 14;
    int8_t headPosY = 8;
    uint8_t headVal = 2;
    //from 1 to headVal -> snake, -1 -> fruit
    int8_t board[size] = {0};
    board[headPosX + width * headPosY] = 2;
    board[headPosX + width * headPosY - 1] = 1;
    Direction direction = right;

    uint8_t fruitCount = 0;
    const uint8_t fruitDelay = 6000 / 300; //every 6 secs
    uint8_t framesFromLastFruit = 0;

    //game loop
    for(int32_t frames = 0; frames >= 0; frames++) {
        //draw the board
        render.fill(false);
        for(uint8_t x = 0; x < width; x++)
            for(uint8_t y = 0; y < height; y++) {
                //snake piece
                if(board[x + y * width] > 0)
                    render.drawRect(x * 3, y * 3, x * 3 + 2, y * 3 + 2, true);
                //fruit
                else if(board[x + y * width] < 0) {
                    render.drawRect(x * 3, y * 3 + 1, x * 3 + 2, y * 3 + 1, true);
                    render.drawRect(x * 3 + 1, y * 3, x * 3 + 1, y * 3 + 2, true);
                }
            }
        render.drawBuffer();

        sleep_ms(300);

        //get keyboard data and process it
        Direction newDir;
        while(kbctl.isEvent()) {
            auto event = kbctl.getEvent();
            if(event.type != KbEventType::Pressed) continue;

            switch(event.code) {
                case 8:
                    newDir = up;
                    break;
                case 9:
                    newDir = down;
                    break;
                case 1:
                    newDir = left;
                    break;
                case 17:
                    newDir = right;
                    break;
                case 0:
                    //exit
                    render.fill(false);
                    render.setCursor(0,0);
                    render.puts("You exited");
                    goto end_snake;
                    break;
                default:
                    newDir = direction;
                    break;
            }
        }
        //if it's not opposite
        if(newDir % 2 != direction % 2)
            direction = newDir;

        //update the board
        //move snake
        switch(direction) {
            case up: 
                headPosY--;
                break;
            case down:
                headPosY++;
                break;
            case left:
                headPosX--;
                break;
            case right:
                headPosX++;
                break;
        }
        //wrap around the map
        //yay torus topology! :D
        headPosX = (headPosX + width) % width;
        headPosY = (headPosY + height) % height;
        auto newHeadPos = headPosX + width * headPosY;

        //check if theres another snake piece
        if(board[newHeadPos] > 0 ) {
            break;
        }

        bool eatFruit = board[newHeadPos] == -1;

        board[newHeadPos] = headVal + 1;
        if(!eatFruit) {
            for(uint16_t i = 0; i < size; i++)
                if(board[i] > 0) board[i]--;
        }
        else {
            headVal++;
            fruitCount--;
        }
            

        //place fruit maybe
        if(fruitCount == 0 || framesFromLastFruit > fruitDelay) {
            uint32_t pos;
            do {
                pos = rnd() % size;
            } while(board[pos]);
            
            board[pos] = -1;
            framesFromLastFruit = 0;
            fruitCount++;
        }
        else {
            framesFromLastFruit++;
        }
    }

    render.fill(false);
    render.setCursor(0,0);
    render.puts("You lost!");
    
    end_snake:
    render.puts("\nScore: ");
    render.putint10(headVal);
    render.puts("\nPress any key");
    render.drawBuffer();

    sleep_ms(500);
    kbctl.clearEvents();
    while(!kbctl.isEvent()) {sleep_ms(100);}

    return 0;
}