#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "nokia5110.h"

#define pause_button PD3
#define right_button PD4
#define up_button PD5
#define down_button PD6
#define left_button PD7

uint8_t i, j;

uint8_t snake_body[100][2];

uint8_t body_size;

uint8_t food[2];

uint8_t dir;

bool collision;


uint8_t score;


static void write(uint8_t bytes, uint8_t is_data)
{
	/* Enable controller */
	PORT_LCD &= ~(1 << LCD_SCE);

	/* We are sending data */
	if (is_data)
		PORT_LCD |= (1 << LCD_DC);
	/* We are sending commands */
	else
		PORT_LCD &= ~(1 << LCD_DC);

	/* Send bytes */
	for (i = 0; i < 8; i++) {
		/* Set data pin to byte state */
		if ((bytes >> (7-i)) & 0x01)
			PORT_LCD |= (1 << LCD_DIN);
		else
			PORT_LCD &= ~(1 << LCD_DIN);

		/* Blink clock */
		PORT_LCD |= (1 << LCD_CLK);
		PORT_LCD &= ~(1 << LCD_CLK);
	}

	/* Disable controller */
	PORT_LCD |= (1 << LCD_SCE);
}

static void write_cmd(uint8_t cmd)
{
	write(cmd, 0);
}

static void write_data(uint8_t data)
{
	write(data, 1);
}


void go_right(void)
{
  nokia_lcd_set_pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  nokia_lcd_set_pixel(snake_body[0][0]+1, snake_body[0][1], true);

  
  
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = snake_body[1][0]+1;
  snake_body[0][1] = snake_body[1][1];

}


void go_left(void)
{
  nokia_lcd_set_pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  nokia_lcd_set_pixel(snake_body[0][0]-1, snake_body[0][1], true);

  
  
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = snake_body[1][0]-1;
  snake_body[0][1] = snake_body[1][1];

}


void go_up(void)
{
  nokia_lcd_set_pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  nokia_lcd_set_pixel(snake_body[0][0], snake_body[0][1]-1, true);

  
  
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = snake_body[1][0];
  snake_body[0][1] = snake_body[1][1]-1;

}


void go_down(void)
{
  nokia_lcd_set_pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  nokia_lcd_set_pixel(snake_body[0][0], snake_body[0][1]+1, true);

  
  
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = snake_body[1][0];
  snake_body[0][1] = snake_body[1][1]+1;

}


void eat_fruit(void)
{
  body_size++;

  score++;

  uint8_t check = 0;

  for(i=(body_size-1); i>0; i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  snake_body[0][0] = food[0];
  snake_body[0][1] = food[1];

  while(check==0)
  {
    food[0] = rand() % 82 + 1;
    food[1] = rand() % 46 + 1;
  
    for(i=0; i<body_size; i++)
    {
      if((food[0]==snake_body[i][0]) && (food[1]==snake_body[i][1]))
      {
        check = 0;
        break;
      }

      else
      {
        check = 1;
      }
      

      
    }

  }


  nokia_lcd_set_pixel(food[0], food[1], true);
}



void game_over(void)
{
  uint8_t score_aux;

  nokia_lcd_clear();

  nokia_lcd_set_cursor(17, 10);

  nokia_lcd_write_string("Game Over",1);

  nokia_lcd_set_cursor(16, 29);

  nokia_lcd_write_string("Score: ",1);

  score_aux = score/10;

  for(i=2; i>0; i--)
  {
    if(score_aux==0)
      nokia_lcd_write_string("0",1);

    if(score_aux==1)
      nokia_lcd_write_string("1",1);

    if(score_aux==2)
      nokia_lcd_write_string("2",1);

    if(score_aux==3)
      nokia_lcd_write_string("3",1);

    if(score_aux==4)
      nokia_lcd_write_string("4",1);

    if(score_aux==5)
      nokia_lcd_write_string("5",1);

    if(score_aux==6)
      nokia_lcd_write_string("6",1);

    if(score_aux==7)
      nokia_lcd_write_string("7",1);

    if(score_aux==8)
      nokia_lcd_write_string("8",1);

    if(score_aux==9)
      nokia_lcd_write_string("9",1);

    score_aux= score-(score_aux*10);

    nokia_lcd_render();

  }
  
  while((PIND & (1<<right_button)));
  

  reset();
   
}



void pause_menu(void)
{
  for(i=0; i<48 ;i++)
    nokia_lcd_set_pixel(0, i, false);

  for(i=0; i<48 ;i++)
    nokia_lcd_set_pixel(83, i, false);

  for(i=1; i<83 ;i++)
    nokia_lcd_set_pixel(i, 0, false);

  for(i=1; i<83 ;i++)
    nokia_lcd_set_pixel(i, 47, false);


  for(i=0; i<body_size ;i++)
  {
    nokia_lcd_set_pixel(snake_body[i][0], snake_body[i][1], false);
  }

  nokia_lcd_set_pixel(food[0], food[1], false);

  nokia_lcd_set_cursor(29, 2);

  nokia_lcd_write_string("Pause",1);

  nokia_lcd_set_cursor(20, 20);

  nokia_lcd_write_string("Continue",1);

  nokia_lcd_set_cursor(20, 38);

  nokia_lcd_write_string("Quit",1);

  continue_menu();
}



void snake_game(void)
{
  //time_t t;

  while(!(PIND & (1<<right_button)));

  nokia_lcd_clear();

  for(i=0; i<48 ;i++)
    nokia_lcd_set_pixel(0, i, true);

  for(i=0; i<48 ;i++)
    nokia_lcd_set_pixel(83, i, true);

  for(i=1; i<83 ;i++)
    nokia_lcd_set_pixel(i, 0, true);

  for(i=1; i<83 ;i++)
    nokia_lcd_set_pixel(i, 47, true);


  for(i=0; i<body_size ;i++)
  {
    nokia_lcd_set_pixel(snake_body[i][0], snake_body[i][1], true);
  }

  nokia_lcd_set_pixel(food[0], food[1], true);

  dir = 4;
    
  srand(time(NULL));

  nokia_lcd_render();

  while(!(PIND & (1<<right_button)));


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

    if(!(PIND & (1<<pause_button)))
      pause_menu();

        
    

    if (dir == 1)
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
        go_up();
    }

    if (dir == 2)
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

      if((snake_body[0][1]+1)== 47)
        game_over();
  
      else if(((snake_body[0][1]+1)==(food[1])) && ((snake_body[0][0])==(food[0])))
        eat_fruit();  
  
      else
        go_down();
    }

    if (dir == 3)
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
        go_left();
    }

    if (dir == 4)
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

  
      if((snake_body[0][0]+1)== 83)
        game_over();
  
      else if(((snake_body[0][0]+1)==(food[0])) && ((snake_body[0][1])==(food[1])))
        eat_fruit();  
      else
        go_right();
    }

    nokia_lcd_render();  
  }
}



void quit_menu(void)
{
  nokia_lcd_set_cursor(10, 20);

  nokia_lcd_write_string(" ",1);

  nokia_lcd_set_cursor(10, 38);

  nokia_lcd_write_string(">",1);

  nokia_lcd_render();

  while(1)
  {
    if(!(PIND & (1<<up_button)))
    {
      continue_menu();
    }


    if(!(PIND & (1<<right_button)))
    {
      reset();
    }
  }

}



void continue_menu(void)
{
  nokia_lcd_set_cursor(10, 38);

  nokia_lcd_write_string(" ",1);

  nokia_lcd_set_cursor(10, 20);

  nokia_lcd_write_string(">",1);

  nokia_lcd_render();

  while(1)
  {
    if(!(PIND & (1<<down_button)))
    {
      quit_menu();
    }


    if(!(PIND & (1<<right_button)))
    {
      snake_game();
    }
  }

}



void start_menu(void)
{
  nokia_lcd_set_cursor(15, 20);

  nokia_lcd_write_string("Start Game",1);

  nokia_lcd_render();

  while((PIND & (1<<right_button)));

  snake_game();
  
}


void reset(void)
{
  while(!(PIND & (1<<right_button)));

  snake_body[0][0]= 22; 
  snake_body[0][1]=24;
  snake_body[1][0]=21; 
  snake_body[1][1]=24;
  snake_body[2][0]=20; 
  snake_body[2][1]=24;
  snake_body[3][0]=19; 
  snake_body[3][1]=24;
  snake_body[4][0]=18; 
  snake_body[4][1]=24;
  
  body_size = 5;

  food[0] = 47; 
  food[1] = 24;

  collision = false;

  score = 0;

  nokia_lcd_init();
  
  nokia_lcd_clear();

  start_menu();
}


int main(void)
{
    DDRD &= ~((1<<left_button)|(1<<right_button)|(1<<up_button)|(1<<down_button)|(1<<pause_button));

    PORTD |= ((1<<left_button)|(1<<right_button)|(1<<up_button)|(1<<down_button)|(1<<pause_button));

    reset();

}
