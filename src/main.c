#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <util/delay.h>


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

#define LCD_NOKIA_DATA 1
#define LCD_NOKIA_CMD 0


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


void LCD_Write(uint8_t mode, char cData)
{
  if(mode == LCD_NOKIA_CMD)
  {
    PORTB &= ~(1<<DC_pin);
  }

  else
  {
    PORTB |= (1<<DC_pin);
  }

  PORTB &= ~(1<<SCE_pin);


  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));

  PORTB |= (1<<SCE_pin);
}


void LCD_Clear(void){
  for (int i=0; i<504; i++){
    lcd_display[i] = 0b00000000;
    LCD_Write(LCD_NOKIA_DATA, lcd_display[i]);
  }
}


void LCD_Position(uint8_t l, uint8_t c) {  
  LCD_Write( LCD_NOKIA_CMD, 0b01000000 + l);    
  LCD_Write( LCD_NOKIA_CMD, 0b10000000 + c);    
}


void LCD_Set_Pixel(uint8_t l, uint8_t c, bool val){
  uint8_t bit = l%8;
  l = l/8;
  LCD_Position(l, c);
  
  if (val) { 
    lcd_display[l*84+c] |= (1 << bit); 
  } else { 
    lcd_display[l*84+c] &= ~(1 << bit);  
  }
  
  LCD_Write(LCD_NOKIA_DATA, lcd_display[l*84+c]);
}


void go_right(void)
{
  LCD_Set_Pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  LCD_Set_Pixel(snake_body[0][0], snake_body[0][1]+1, true);

  
  
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = snake_body[1][0];
  snake_body[0][1] = snake_body[1][1]+1;

}


void go_left(void)
{
  LCD_Set_Pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  LCD_Set_Pixel(snake_body[0][0], snake_body[0][1]-1, true);

  
  
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = snake_body[1][0];
  snake_body[0][1] = snake_body[1][1]-1;

}


void go_up(void)
{
  LCD_Set_Pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  LCD_Set_Pixel(snake_body[0][0]-1, snake_body[0][1], true);

  
  
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = snake_body[1][0]-1;
  snake_body[0][1] = snake_body[1][1];

}


void go_down(void)
{
  LCD_Set_Pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  LCD_Set_Pixel(snake_body[0][0]+1, snake_body[0][1], true);

  
  
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = snake_body[1][0]+1;
  snake_body[0][1] = snake_body[1][1];

}


void eat_fruit(void)
{
  body_size++;

  srand(time(NULL));

  for(i=(body_size-1); i>0; i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = food[0];
  snake_body[0][1] = food[1];

  food[0] = rand() % 46 + 1;
  food[1] = rand() % 82 + 1;

  LCD_Set_Pixel(food[0], food[1], true);
}


void game_over(void)
{
  for(i=0;i<84;i++)
  {
    for(j=0;j<6;j++)
    {
      LCD_Position(j,i);
    
      LCD_Write(LCD_NOKIA_DATA, 0b11111111);
    }
      
  }
   
  _delay_ms(5000);
   
}


int main(void)
{
  DDRB |= ((1<<LED_pin)|(1<<RST_pin)|(1<<DC_pin));

  DDRD &= ~((1<<left_button)|(1<<right_button)|(up_button)|(down_button));

  PORTD |= ((1<<left_button)|(1<<right_button)|(up_button)|(down_button));


  PORTB &= ~(1<<RST_pin);

  PORTB |= (1<<RST_pin);


  LCD_Write(LCD_NOKIA_CMD, 0b00100001);
  LCD_Write(LCD_NOKIA_CMD, 0b10101111);
  LCD_Write(LCD_NOKIA_CMD, 0b00000100);
  LCD_Write(LCD_NOKIA_CMD, 0b00010100);

  LCD_Write(LCD_NOKIA_CMD, 0b00100000);
  LCD_Write(LCD_NOKIA_CMD, 0b00001100);


  LCD_Position(0,0);

  LCD_Clear();


  for(i=0; i<6 ;i++)
  {
    LCD_Position(i,0);
    
    LCD_Write(LCD_NOKIA_DATA, 0b11111111);
  }

  for(i=0; i<6 ;i++)
  {
    LCD_Position(i,83);
    
    LCD_Write(LCD_NOKIA_DATA, 0b11111111);
  }

  for(i=1; i<83 ;i++)
  {
    LCD_Set_Pixel(0, i, true);
  }

  for(i=1; i<83 ;i++)
  { 
    LCD_Set_Pixel(47, i, true);
  }



  for(i=0; i<body_size ;i++)
  {
    LCD_Set_Pixel(snake_body[i][0], snake_body[i][1], true);
  }

  LCD_Set_Pixel(food[0], food[1], true);

  dir = 4;


  while(1)
  {
    _delay_ms(100);



    if(!(PIND & (1<<up_button)))
      dir = 1;

    if(!(PIND & (1<<down_button)))
      dir = 2;

    if(!(PIND & (1<<left_button)))
      dir = 3;

    if(!(PIND & (1<<right_button)))
      dir = 4;




    if (dir == 1)
    {
      for(i=1;i<body_size;i++)
      {
        if(((snake_body[0][0]-1) == (snake_body[i][0])) && ((snake_body[0][1]) == (snake_body[i][1])))
        {
          collision = true;
          break;
        }

        else
          collision = false;
      }


      if(collision == true)
        game_over();
      
    
      if((snake_body[0][0]-1)== 0)
        game_over();
    
      else if(((snake_body[0][0]-1)==(food[0])) && ((snake_body[0][1])==(food[1])))
        eat_fruit();
      
      else
        go_up();
    }

    if (dir == 2)
    {
      for(i=1;i<body_size;i++)
      {
        if(((snake_body[0][0]+1) == (snake_body[i][0])) && ((snake_body[0][1]) == (snake_body[i][1])))
        {
          collision = true;
          break;
        }

        else
          collision = false;
      }


      if(collision == true)
        game_over();
    
      if((snake_body[0][0]+1)== 47)
        game_over();
      
      else if(((snake_body[0][0]+1)==(food[0])) && ((snake_body[0][1])==(food[1])))
        eat_fruit();  
      
      else
        go_down();
    }

    if (dir == 3)
    {
      for(i=1;i<body_size;i++)
      {
        if(((snake_body[0][1]-1) == (snake_body[i][1])) && ((snake_body[0][0]) == (snake_body[i][0])))
        {
          collision = true;
          break;
        }

        else
          collision = false;
      }


      if(collision == true)
        game_over();

      
      if((snake_body[0][1]-1)== 0)
        game_over();
      
      else if(((snake_body[0][1]-1)==(food[1])) && ((snake_body[0][0])==(food[0])))
        eat_fruit();  
      else
        go_left();
    }

    if (dir == 4)
    {

      for(i=1;i<body_size;i++)
      {
        if(((snake_body[0][1]+1) == (snake_body[i][1])) && ((snake_body[0][0]) == (snake_body[i][0])))
        {
          collision = true;
          break;
        }

        else
          collision = false;
      }


      if(collision == true)
        game_over();

      
      if((snake_body[0][1]+1)== 83)
        game_over();
      
      else if(((snake_body[0][1]+1)==(food[1])) && ((snake_body[0][0])==(food[0])))
        eat_fruit();  
      else
        go_right();
    }
  }
}

