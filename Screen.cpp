#include "Screen.h"

LCDScreen::LCDScreen(uint8_t LCD_RST, uint8_t LCD_DC) : RST(LCD_RST), DC(LCD_DC)
{
    spi_init(spi0, 4000000);
    spi_set_format(spi0, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);

    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    //make pins available for the protocol
    bi_decl(bi_3pins_with_func(PICO_DEFAULT_SPI_TX_PIN, PICO_DEFAULT_SPI_SCK_PIN, PICO_DEFAULT_SPI_CSN_PIN, GPIO_FUNC_SPI));

    gpio_init(LCD_RST);
    gpio_init(LCD_DC);
    gpio_set_dir(LCD_RST, GPIO_OUT);
    gpio_set_dir(LCD_DC, GPIO_OUT);

    initScreen();
}

void LCDScreen::initScreen()
{
    sleep_ms(100);
    gpio_put(RST, 1);
    gpio_put(RST, 0);
    sleep_ms(50);
    gpio_put(RST, 1);

    writeCommand(LCD_FUNCTIONSET | LCD_ENTRYMODE);
    writeCommand(LCD_DISPLAYCONTROL | LCD_DISPLAYNORMAL);
}

void LCDScreen::fill(bool colour)
{
    uint32_t fillValue = colour ? 0xFFFFFFFF : 0;
    for(uint8_t i = 0; i < 126; i++) //126 = LCD_WIDTH * LCD_HEIGHT / 32 bits
        *((uint32_t*)buffer + i) = fillValue;
}

void LCDScreen::setPixel(uint8_t x, uint8_t y, bool colour)
{
    if(x >= LCD_WIDTH || y >= LCD_HEIGHT) return;

    if(colour)
        buffer[x * (LCD_HEIGHT / 8) + (y / 8)] |= 1 << (y & 0b111);
    else
        buffer[x * (LCD_HEIGHT / 8) + (y / 8)] &= ~(1 << (y & 0b111));
}

inline void LCDScreen::writeCommand(uint8_t command) const
{
    gpio_put(DC, 0);
    spi_write_blocking(spi0, &command, 1);
}

void LCDScreen::drawBuffer() const
{
    writeCommand(LCD_SETXADDR);
    writeCommand(LCD_SETYADDR);

    gpio_put(DC, 1);
    spi_write_blocking(spi0, buffer, LCD_BUFFER_SIZE);
}