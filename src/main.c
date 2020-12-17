#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "nokia_lcd.h"

#define select_button PD3
#define right_button PD4
#define up_button PD5
#define down_button PD6
#define left_button PD7

uint8_t i, j;


//Array containing the positions of the blocks that belong to the snake body
uint8_t snake_body[100][2];

//size of the snake's body
uint8_t body_size;

//Position of the food
uint8_t food[2];

uint8_t dir, init=1;

//Collision Variable
bool collision;


//Score Variable
uint8_t score;

//Declaring functions
void go_right(void);
void go_left(void);
void go_up(void);
void go_down(void);
void eat_fruit(void);
void game_over(void);
void pause_menu(void);
void snake_game(void);
void quit_menu(void);
void continue_menu(void);
void start_menu(void);
void reset(void);

//Function to allow snake to move to the right
void go_right(void)
{
  //Erase a pixel of the tail
  nokia_lcd_set_pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  //Add a pixel to the head
  nokia_lcd_set_pixel(snake_body[0][0]+1, snake_body[0][1], true);

  
  
  //Adjust the positions of the body in the array
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  //Add the position of the new head
  snake_body[0][0] = snake_body[1][0]+1;
  snake_body[0][1] = snake_body[1][1];

}


//Function to allow snake to move to the left
void go_left(void)
{
  //Erase a pixel of the tail
  nokia_lcd_set_pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  //Add a pixel to the head
  nokia_lcd_set_pixel(snake_body[0][0]-1, snake_body[0][1], true);

  

  //Adjust the positions of the body in the array
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  //Add the position of the new head
  snake_body[0][0] = snake_body[1][0]-1;
  snake_body[0][1] = snake_body[1][1];

}


//Function to allow snake to move up
void go_up(void)
{
  //Erase a pixel of the tail
  nokia_lcd_set_pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  //Add a pixel to the head
  nokia_lcd_set_pixel(snake_body[0][0], snake_body[0][1]-1, true);

  
  //Adjust the positions of the body in the array
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  //Add the position of the new head
  snake_body[0][0] = snake_body[1][0];
  snake_body[0][1] = snake_body[1][1]-1;

}

//Function to allow snake to move down
void go_down(void)
{
  //Erase a pixel of the tail
  nokia_lcd_set_pixel(snake_body[body_size-1][0], snake_body[body_size-1][1], false);

  //Add a pixel to the head
  nokia_lcd_set_pixel(snake_body[0][0], snake_body[0][1]+1, true);

  
  //Adjust the positions of the body in the array
  for(i=(body_size-1);i>0;i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  //Add the position of the new head
  snake_body[0][0] = snake_body[1][0];
  snake_body[0][1] = snake_body[1][1]+1;

}

//Function to add the block of the fruit to the snake's body
void eat_fruit(void)
{
  //Increase the body size by one after eating the food
  body_size++;

  //Increase Score by one after eating the food
  score++;

  uint8_t check = 0;

  //Adjust the positions of the body in the array
  for(i=(body_size-1); i>0; i--)
  {
    snake_body[i][0] = snake_body[i-1][0];
    snake_body[i][1] = snake_body[i-1][1];
  }

  //Add food pixel as the new head
  snake_body[0][0] = food[0];
  snake_body[0][1] = food[1];


  //Generate Position for the new food 
  while(check==0)
  {
    // Random Position within the walls
    food[0] = rand() % 82 + 1;
    food[1] = rand() % 46 + 1;
  
    //Makes sure that the food position isn't in one of the body positions
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

  //Add new food to the screen
  nokia_lcd_set_pixel(food[0], food[1], true);
}



//Game Over Menu and Score Display
void game_over(void)
{
  uint8_t score_aux;

  //Cleans the Game Screen
  nokia_lcd_clear();

  nokia_lcd_set_cursor(17, 10);

  //Shows the Game Over message
  nokia_lcd_write_string("Game Over",1);

  nokia_lcd_set_cursor(16, 29);

  //Shows the score
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

    //Updates display
    nokia_lcd_render();

  }
  
  //If the user clicks in the select button it resets the game back to the start menu
  while((PIND & (1<<select_button)));
  

  reset();
   
}


//Pause Menu
void pause_menu(void)
{
  //Erases the walls pixels without erasing theirs values from the Display array
  for(i=0; i<48 ;i++)
    nokia_lcd_set_pixel(0, i, false);

  for(i=0; i<48 ;i++)
    nokia_lcd_set_pixel(83, i, false);

  for(i=1; i<83 ;i++)
    nokia_lcd_set_pixel(i, 0, false);

  for(i=1; i<83 ;i++)
    nokia_lcd_set_pixel(i, 47, false);


  //Erases the snake body pixels without erasing theirs values from the Display array
  for(i=0; i<body_size ;i++)
  {
    nokia_lcd_set_pixel(snake_body[i][0], snake_body[i][1], false);
  }

  //Erases the food pixel without erasing its value from the Display array
  nokia_lcd_set_pixel(food[0], food[1], false);

  nokia_lcd_set_cursor(29, 2);

  //Shows the Pause message and the messages related to the options
  nokia_lcd_write_string("Pause",1);

  nokia_lcd_set_cursor(20, 20);

  nokia_lcd_write_string("Continue",1);

  nokia_lcd_set_cursor(20, 38);

  nokia_lcd_write_string("Quit",1);

  //Points to the continue option
  continue_menu();
}


//Function that controls the snake game
void snake_game(void)
{
  //Software debouncing instruction
  while(!(PIND & (1<<select_button)));

  //Clears the display
  nokia_lcd_clear();

  //Creates the walls pixels
  for(i=0; i<48 ;i++)
    nokia_lcd_set_pixel(0, i, true);

  for(i=0; i<48 ;i++)
    nokia_lcd_set_pixel(83, i, true);

  for(i=1; i<83 ;i++)
    nokia_lcd_set_pixel(i, 0, true);

  for(i=1; i<83 ;i++)
    nokia_lcd_set_pixel(i, 47, true);


  //Creates the snake's body pixels
  for(i=0; i<body_size ;i++)
  {
    nokia_lcd_set_pixel(snake_body[i][0], snake_body[i][1], true);
  }

  //Creates the food pixel
  nokia_lcd_set_pixel(food[0], food[1], true);
    
  //Sets the srand seed
  srand(time(NULL));


  //Updates the displays
  nokia_lcd_render();

  //Software debouncing instruction
  while(!(PIND & (1<<select_button)));


  while(1) 
  {
    //Period of the snake movement
    _delay_ms(100);
        
    //Getting Input from the direction buttons
    if(!(PIND & (1<<up_button)))
      dir = 1;

    if(!(PIND & (1<<down_button)))
      dir = 2;
        
    if(!(PIND & (1<<left_button)))
      dir = 3;

    if(!(PIND & (1<<right_button)))
      dir = 4;

    //Choosing Pause Option
    if(!(PIND & (1<<select_button)))
      pause_menu();

        
    
    //Choosing the action based on the direction and the body position
    if (dir == 1)
    { 
      //Deteting Colision with the body
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

      //Game over if the colision is verified
      if(collision == true)
        game_over();
      
      //Deteting Colision with the wall
      if((snake_body[0][1]-1)== 0)
        game_over();
    
      //Deteting if food is in the next pixel
      else if(((snake_body[0][1]-1)==(food[1])) && ((snake_body[0][0])==(food[0])))
        eat_fruit();
      
      //Otherwise keep moving
      else
        go_up();
    }

    //Choosing the action based on the direction and the body position
    if (dir == 2)
    {
      //Deteting Colision with the body
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

      //Game over if the colision is verified
      if(collision == true)
        game_over();

      //Deteting Colision with the wall
      if((snake_body[0][1]+1)== 47)
        game_over();

      //Deteting if food is in the next pixel
      else if(((snake_body[0][1]+1)==(food[1])) && ((snake_body[0][0])==(food[0])))
        eat_fruit();  
  
      //Otherwise keep moving
      else
        go_down();
    }

    //Choosing the action based on the direction and the body position
    if (dir == 3)
    {
      //Deteting Colision with the body
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

      //Game over if the colision is verified
      if(collision == true)
        game_over();

      //Deteting Colision with the wall
      if((snake_body[0][0]-1)== 0)
        game_over();

      //Deteting if food is in the next pixel
      else if(((snake_body[0][0]-1)==(food[0])) && ((snake_body[0][1])==(food[1])))
        eat_fruit();  
      
      //Otherwise keep moving
      else
        go_left();
    }

    //Choosing the action based on the direction and the body position
    if (dir == 4)
    {
      //Deteting Colision with the body
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

      //Game over if the colision is verified
      if(collision == true)
        game_over();

  
      //Deteting Colision with the wall
      if((snake_body[0][0]+1)== 83)
        game_over();

      //Deteting if food is in the next pixel
      else if(((snake_body[0][0]+1)==(food[0])) && ((snake_body[0][1])==(food[1])))
        eat_fruit();  
      
      //Otherwise keep moving
      else
        go_right();
    }

    //Update Display
    nokia_lcd_render();  
  }
}


//Quit Option in the Pause Menu
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


    if(!(PIND & (1<<select_button)))
    {
      reset();
    }
  }

}


//Option to continue the game in the Pause Menu
void continue_menu(void)
{
  //Software Debouncing Instruction
  while(!(PIND & (1<<select_button)));

  nokia_lcd_set_cursor(10, 38);

  //Cleans the pointer to the quit option
  nokia_lcd_write_string(" ",1);

  nokia_lcd_set_cursor(10, 20);

  //Shows pointer to the continue option
  nokia_lcd_write_string(">",1);

  //Display Update
  nokia_lcd_render();

  while(1)
  { 
    //Changes the pointed option by clicking in the down button
    if(!(PIND & (1<<down_button)))
    {
      //Points to quit option
      quit_menu();
    }

    //Selects Continue Option and gets back to the game
    if(!(PIND & (1<<select_button)))
    {
      snake_game();
    }
  }

}


//Start Menu
void start_menu(void)
{
  nokia_lcd_set_cursor(15, 20);

  //Shows the Start Game message
  nokia_lcd_write_string("Start Game",1);

  //Display Update
  nokia_lcd_render();

  //Starts the game by clicking the select button
  while((PIND & (1<<select_button)));

  snake_game();
  
}

// Reset -  Called before starting a new game
void reset(void)
{
  //Software Debouncing Instruction
  while(!(PIND & (1<<select_button)));

  // Resets Information of the variables
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

  dir = 4;
  
  body_size = 5;
  
  dir = 4;

  food[0] = 47; 
  food[1] = 24;

  collision = false;

  score = 0;

  if (init == 1)
  {
    //Initialize the lcd in the first reset
    nokia_lcd_init();
    init=0;
  }

  nokia_lcd_clear();

  //Moves to the start menu
  start_menu();
}


int main(void)
{   
    //Declares Buttons as inputs
    DDRD &= ~((1<<left_button)|(1<<right_button)|(1<<up_button)|(1<<down_button)|(1<<select_button));


    //Activate the respective Pull Up Resistors
    PORTD |= ((1<<left_button)|(1<<right_button)|(1<<up_button)|(1<<down_button)|(1<<select_button));

    //Resets the variables to get the game ready to start
    reset();

}
