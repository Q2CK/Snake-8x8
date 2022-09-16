#include <LedControl.h>

byte DIN = 11;
byte CS = 7;
byte CLK = 13;
LedControl lc=LedControl(DIN, CLK, CS,0);

byte snake_x[64];
byte snake_y[64];

byte snake_length;

byte dir_x;
byte dir_y;

byte dir_choice;

byte food_x;
byte food_y;

bool loss;

bool push_l;
bool push_r;

void initialize() {
  for(byte i = 0; i < 8; i++) {
    lc.setRow(0, i, 0);
  }

  lc.shutdown(0,false);
  lc.setIntensity(0,7);
  
  food_x = analogRead(A0) % 8;
  food_y = analogRead(A0) % 8;

  dir_x = 0;
  dir_y = 1;

  dir_choice = 1;
  push_l = false;
  push_r = false;

  loss = false;

  snake_length = 4;

  for(byte i = 0; i < 5; i++) {
    snake_x[i] = 3;
    snake_y[i] = i;
  }
}

void setup() {
  
  initialize();

  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  
}
  
void loop() {
  if(!loss) {
    if(push_l == false and digitalRead(8) == LOW) {
      push_l = true;
      dir_choice--;
    }
    
    if(push_r == false and digitalRead(9) == LOW) {
      push_r = true;
      dir_choice++;
    }
    
    if(millis() % 150 < 2) {
      if(dir_choice < 1)
      dir_choice = 4;
      
      else if(dir_choice > 4)
        dir_choice = 1;
  
      switch(dir_choice) {
        
        case 1:
          dir_y = 1;
          dir_x = 0;
          break;
          
        case 2:
          dir_y = 0;
          dir_x = 1;
          break;
  
        case 3:
          dir_y = -1;
          dir_x = 0;
          break;
  
        case 4:
          dir_y = 0;
          dir_x = -1;
          break;
      }

      if(push_r == true and digitalRead(9) == HIGH) {
        push_r = false;
      }

      if(push_l == true and digitalRead(8) == HIGH) {
        push_l = false;
      }
      
      lc.setLed(0, snake_y[0], snake_x[0], false);
      
      lc.setLed(0, food_y, food_x, true);
      
      if(snake_x[snake_length - 1] == food_x && snake_y[snake_length - 1] == food_y) {
        
        snake_length++;
        
        snake_x[snake_length - 1] = food_x;
        snake_y[snake_length - 1] = food_y;
  
        food_x = analogRead(A0) % 8;
        food_y = analogRead(A0) % 8;
        
      }    
      
      else {
        for(byte i = 0; i < snake_length - 1; i++) {
            
          snake_y[i + 1] = snake_y[i + 1] % 8;
          snake_x[i + 1] = snake_x[i + 1] % 8;
        
          snake_x[i] = snake_x[i + 1];
          snake_y[i] = snake_y[i + 1];
          
          lc.setLed(0, snake_y[i], snake_x[i], true);
    
        }
      }
      
      snake_x[snake_length - 1] = snake_x[snake_length - 2] + dir_x;
      snake_y[snake_length - 1] = snake_y[snake_length - 2] + dir_y;
  
      for(byte i = 0; i < snake_length - 2; i++) {
        
        if(snake_x[i] == snake_x[snake_length - 1] && snake_y[i] == snake_y[snake_length - 1]) {
           
          loss = true;
          
        }       
      } 
    }
  }
  else {
    initialize();
  }
}
