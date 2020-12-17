#include "nokia_lcd.h"
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <util/delay.h>
#include "lcd_chars.h"


static struct {
    //Screen Array - with the byte of each one of 504 columns of the 6 banks
    uint8_t screen[504];

    /// Cursor Postion
    uint8_t cursor_x;
    uint8_t cursor_y;

} nokia_lcd = {
    .cursor_x = 0,
    .cursor_y = 0
};


void write(uint8_t bytes, uint8_t is_data)
{
	register uint8_t i;
	// Enable controller
	PORT_LCD &= ~(1 << LCD_SCE);

	//If sending Data
	if (is_data)
		PORT_LCD |= (1 << LCD_DC);

	//If sending Commands
	else
		PORT_LCD &= ~(1 << LCD_DC);

	//Sending bit by bit
	for (i = 0; i < 8; i++) {

		//Writing bit on the serial pin
		if ((bytes >> (7-i)) & 0x01)
			PORT_LCD |= (1 << LCD_DIN);
		else
			PORT_LCD &= ~(1 << LCD_DIN);

		//Making clock transitions
		PORT_LCD |= (1 << LCD_CLK);
		PORT_LCD &= ~(1 << LCD_CLK);
	}

	// Disable controller
	PORT_LCD |= (1 << LCD_SCE);
}

//Writing command
void write_cmd(uint8_t cmd)
{
	write(cmd, 0);
}

//Writing data
void write_data(uint8_t data)
{
	write(data, 1);
}


//LCD Initialization
void nokia_lcd_init(void)
{
	register unsigned i;
	// Set LCD pins as outputs
	DDR_LCD |= (1 << LCD_SCE);
	DDR_LCD |= (1 << LCD_RST);
	DDR_LCD |= (1 << LCD_DC);
	DDR_LCD |= (1 << LCD_DIN);
	DDR_LCD |= (1 << LCD_CLK);
	DDR_LCD |= (1 << LED_pin);



	// Reset display
	PORT_LCD |= (1 << LED_pin);

	PORT_LCD |= (1 << LCD_RST);
	PORT_LCD |= (1 << LCD_SCE);
	_delay_ms(10);
	PORT_LCD &= ~(1 << LCD_RST);
	_delay_ms(70);
	PORT_LCD |= (1 << LCD_RST);


	//Initialization Instructions
	
	// Enable controller
	PORT_LCD &= ~(1 << LCD_SCE);
	
	// -LCD Extended Commands mode
	write_cmd(0x21);

	// LCD bias mode 1:48
	write_cmd(0x13);

	// Set temperature coefficient
	write_cmd(0x06);

	// Default VOP (3.06 + 66 * 0.06 = 7V)
	write_cmd(0xC2);

	// Standard Commands mode, powered down
	write_cmd(0x20);

	// LCD in normal mode
	write_cmd(0x09);

	

	// Clear LCD
	write_cmd(0x80);
	write_cmd(LCD_CONTRAST);
	for (i = 0; i < 504; i++)
		write_data(0xFF);

	// Activate LCD
	write_cmd(0x08);
	write_cmd(0x0C);
}

//Clear the display
void nokia_lcd_clear(void)
{
	register unsigned i;
	
	// Set column and row to 0
	write_cmd(0x80);
	write_cmd(0x40);

	// Set cursor to position (0,0)
	nokia_lcd.cursor_x = 0;
	nokia_lcd.cursor_y = 0;

	// Sending 0x00 to every pixel in the display
	for(i = 0;i < 504; i++)
		nokia_lcd.screen[i] = 0x00;
}

//Setting pixel in determined position in the Screen array
void nokia_lcd_set_pixel(uint8_t x, uint8_t y, bool value)
{
	uint8_t *byte = &nokia_lcd.screen[y/8*84+x];
	if (value)
		*byte |= (1 << (y % 8)); //Setting pixel to 1
	else
		*byte &= ~(1 << (y %8 )); //Setting pixel to 0
}

//Writing Character
void nokia_lcd_write_char(char code, uint8_t scale)
{
	register uint8_t x, y;

	//Choosing values for the charater in the matrix of HEX values in the lcd_chars.h
	for (x = 0; x < 5*scale; x++)
		for (y = 0; y < 7*scale; y++)
			if (pgm_read_byte(&CHARSET[code-32][x/scale]) & (1 << y/scale))
				nokia_lcd_set_pixel(nokia_lcd.cursor_x + x, nokia_lcd.cursor_y + y, true);
			else
				nokia_lcd_set_pixel(nokia_lcd.cursor_x + x, nokia_lcd.cursor_y + y, false);
	
	//updating cursor position
	nokia_lcd.cursor_x += 5*scale + 1;
	if (nokia_lcd.cursor_x >= 84) {
		nokia_lcd.cursor_x = 0;
		nokia_lcd.cursor_y += 7*scale + 1;
	}
	if (nokia_lcd.cursor_y >= 48) {
		nokia_lcd.cursor_x = 0;
		nokia_lcd.cursor_y = 0;
	}
}

//Writing a string
void nokia_lcd_write_string(const char *str, uint8_t scale)
{
	//Writing character by character
	while(*str)
		nokia_lcd_write_char(*str++, scale);
}

//Changing cursor position
void nokia_lcd_set_cursor(uint8_t x, uint8_t y)
{
	nokia_lcd.cursor_x = x;
	nokia_lcd.cursor_y = y;
}

//Updating Display
void nokia_lcd_render(void)
{
	register unsigned i;
	// Set column and row to 0
	write_cmd(0x80);
	write_cmd(0x40);

	//Writing every value of the Screen Array in the display one by one 
	for (i = 0; i < 504; i++)
		write_data(nokia_lcd.screen[i]);
}
