#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>


#define LED_pin PB0
#define RST_pin PB1
#define SCE_pin PB2
#define MOSI_pin PB3
#define DC_pin PB4
#define SCLK_pin PB5


#define right_button PD4
#define up_button PD5
#define down_button PD6
#define left_button PD7


char lcd_display[504];

uint8_t i, j;

uint8_t snake_body[100][2]= {{24, 22},
  {24, 21},
  {24, 20},
  {24, 19},
  {24, 18}
  
};

uint8_t body_size = 5;

uint8_t food[2] = {24, 47};

uint8_t dir;

bool collision = false;



void SPI_init(void){

  DDRB |= ((1<<SCE_pin)|(1<<MOSI_pin)|(1<<SCLK_pin)); //spi pins on port b MOSI SCK,SS outputs

  SPCR = ((1<<SPE)|(1<<MSTR)|(1<<SPR0));  // SPI enable, Master, f/16

}


void SPI_MasterTransmit(char cData)
{
/* Start transmission */
SPDR = cData;
/* Wait for transmission complete */
while(!(SPSR & (1<<SPIF)))
;
}

int main(void)
{
  DDRB |= ((1<<LED_pin)|(1<<RST_pin)|(1<<DC_pin));

  DDRD &= ~((1<<left_button)|(1<<right_button)|(up_button)|(down_button));

  PORTD |= ((1<<left_button)|(1<<right_button)|(up_button)|(down_button));


  PORTB &= ~(1<<RST_pin);

  PORTB |= (1<<RST_pin);


  while(1)
  {

  }

}

