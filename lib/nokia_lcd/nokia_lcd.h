#ifndef __NOKIA_LCD__
#define __NOKIA_LCD__

#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>


#define PORT_LCD PORTB
#define DDR_LCD DDRB

//LCD PINS
#define LCD_SCE PB2
#define LCD_RST PB1
#define LCD_DC PB4
#define LCD_DIN PB3
#define LCD_CLK PB5
#define LED_pin PB0

#define LCD_CONTRAST 0x40



void write(uint8_t bytes, uint8_t is_data);

//Writing command
void write_cmd(uint8_t cmd);

//Writing data
void write_data(uint8_t data);

//LCD Initialization
void nokia_lcd_init(void);

//Clear the display
void nokia_lcd_clear(void);


//Setting pixel in determined position in the Screen array
void nokia_lcd_set_pixel(uint8_t x, uint8_t y, bool value);

//Writing Character
void nokia_lcd_write_char(char code, uint8_t scale);

//Writing a string
void nokia_lcd_write_string(const char *str, uint8_t scale);

//Changing cursor position
void nokia_lcd_set_cursor(uint8_t x, uint8_t y);

//Updating Display
void nokia_lcd_render(void);



#endif
