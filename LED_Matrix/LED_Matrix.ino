#include <LedControl.h>

byte DIN = 11;
byte CS = 7;
byte CLK = 13;
LedControl lc=LedControl(DIN, CLK, CS,0);

byte snake_x[64] = {};
byte snake_y[64] = {};

byte snake_length;

byte dir_x;
byte dir_y;

byte food_x;
byte food_y;

bool loss;

void initialize() {
  for(byte i = 0; i < 8; i++) {
    lc.setRow(0, i, 0);
  }
  
  randomSeed(analogRead(A0));

  lc.shutdown(0,false);
  lc.setIntensity(0,7);
  
  food_x = random(0, 8);
  food_y = random(0, 8);

  dir_x = 0;
  dir_y = 1;

  loss = false;

  snake_length = 4;
  
  snake_x[0] = 3;
  snake_x[1] = 3;
  snake_x[2] = 3;
  snake_x[3] = 3;
  snake_x[4] = 4;
  
  snake_y[0] = 0;
  snake_y[1] = 1;
  snake_y[2] = 2;
  snake_y[3] = 3;
  snake_y[4] = 4;
}

void setup() {
  
  initialize();
  
  Serial.begin(9600);
}
  
void loop() {
  if(!loss) {
  
    if(Serial.available()) {   
      switch(Serial.read()) {     
        case 'w':
          dir_x = 0;
          dir_y = 1;
          break;
          
        case 'a':
          dir_x = 1;
          dir_y = 0;
          break;
          
        case 's':
          dir_x = 0;
          dir_y = -1;
          break;
          
        case 'd':
          dir_x = -1;
          dir_y = 0;
          break;
      }
    }
    
    if(millis() % 200 == 0) {
      
      lc.setLed(0, snake_y[0], snake_x[0], false);
      
      lc.setLed(0, food_y, food_x, true);
      
      if(snake_x[snake_length - 1] % 8 == food_x && snake_y[snake_length - 1] % 8 == food_y) {
        
        snake_length++;
        
        snake_x[snake_length - 1] = food_x;
        snake_y[snake_length - 1] = food_y;
  
        food_x = random(0, 8);
        food_y = random(0, 8);
        
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
    delay(2000);
    initialize();
  }
}
