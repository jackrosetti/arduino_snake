/** Have a great time with our new spin on a classic game. Every time you score a multiple of 5, the speed increases. Made by Jack Rosetti, Sam Bowers and Trevor deBernardi.

   Version 1.0.1



*/
#include <Wire.h>  // Include Wire if you're using I2C
#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <SPI.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>
#define PIN_RESET 9  // Connect RST to pin 9 (req. for SPI and I2C)
#define PIN_DC    8  // Connect DC to pin 8 (required for SPI)
#define PIN_CS    10 // Connect CS to pin 10 (required for SPI)
#define DC_JUMPER 0 // (I2C only) Set to either 0 or 1, matching the value of the DC Jumper
//// Declare a MicroOLED object. The parameters include:
//1 - Reset pin: Any digital pin
//2 - D/C pin: Any digital pin (SPI mode only)
//3 - CS pin: Any digital pin (SPI mode only, 10 recommended)
MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS); // Example SPI Declaration
//MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C Declaration
//////////////////////////
// MicroOLED Definition //
//////////////////////////

#define PIN_RESET 9
#define PIN_DC    8
#define PIN_CS    10
#define DC_JUMPER 0

//MicroOLED oled(PIN_RESET, PIN_DC, PIN_CS);

////////////////
// Game stuff //
////////////////

//general variables
int score = 0; //keeps track of score
bool gameStarted = false; //check if game has started or not
const int SCREEN_WIDTH = oled.getLCDWidth(); //width of screen
const int SCREEN_HEIGHT = oled.getLCDHeight(); //height of screen
const int DISPLAY_RATE = 24;

const int BUTTONPIN_LEFT = 4;
const int BUTTONPIN_RIGHT = 3;

int buttonPressed = false;


void setup() //set up program
{
  randomSeed(analogRead(5)); //set random seed
  pinMode(BUTTONPIN_RIGHT, INPUT);
  pinMode(BUTTONPIN_LEFT, INPUT);

  oled.begin(); //initialize screen
  oled.clear(PAGE); //initially clears the display buffer
  oled.display(); //initial display
}

//declare classes
class Snake { //player code
  public:
    int x = SCREEN_WIDTH / 2; //start in middle of screen
    int y = SCREEN_HEIGHT / 2;
    double speed = 1; //how many pixels the snake moves every frame
    int positions[];

    String dir = "up"; //direction the snake is facing ("left", "down", "up", "right")
    void draw() {
      oled.rect(x, y, 5, 5);
    }
    void die() {
      score = 0;
      speed = 1;
      x = SCREEN_WIDTH / 2;
      y = SCREEN_HEIGHT / 2;
    }
};

class Food { //food code
  public:
    int x = random(0, SCREEN_WIDTH - 5); //random starting position
    int y = random(0, SCREEN_HEIGHT - 5);
    void draw() {
      oled.rect(x, y, 5, 5);
    }
    void jump() {
      x = random(5, SCREEN_WIDTH - 5); //random starting position
      y = random(5, SCREEN_HEIGHT - 5);
    }
};

//set up instances
Snake snake;
Food food;

void gameUpdate() {
  //button setup
  //button setup
  int buttonStateRight, buttonStateLeft;
  buttonStateRight = digitalRead(BUTTONPIN_RIGHT);
  buttonStateLeft = digitalRead(BUTTONPIN_LEFT);

  //buttons
  if (gameStarted) {

    if (buttonStateRight == LOW) {
      if (buttonPressed == false) {
        if (snake.dir == "up") {
          snake.dir = "right";
        } else if (snake.dir == "right") {
          snake.dir = "down";
        } else if (snake.dir == "down") {
          snake.dir = "left";
        } else if (snake.dir == "left") {
          snake.dir = "up";
        }
        buttonPressed = true;
      }
    }
    else if (buttonStateLeft == LOW) {
      if (buttonPressed == false) {
        if (snake.dir == "up") {
          snake.dir = "left";
        } else if (snake.dir == "left") {
          snake.dir = "down";
        } else if (snake.dir == "down") {
          snake.dir = "right";
        } else if (snake.dir == "right") {
          snake.dir = "up";
        }
        buttonPressed = true;
      }
    } else {
      buttonPressed = false;
    }

    //movement
    if (snake.dir == "left") {
      snake.x -= snake.speed;
    } else if (snake.dir == "right") {
      snake.x += snake.speed;
    } else if (snake.dir == "up") {
      snake.y -= snake.speed;
    } else if (snake.dir == "down") {
      snake.y += snake.speed;
    }
  } else {
    //start off left or right
    if (buttonStateRight == LOW) {
      snake.dir = "right";
    }
    if (buttonStateLeft == LOW) {
      snake.dir = "left";
    }
    //set gamestarted
    gameStarted = true;
  }


  if (snake.x >= food.x && snake.x <= food.x + 5) {
    if (snake.y >= food.y && snake.y <= food.y + 5) {
      score += 1;
      food.jump();
    }
  }

  if (score >= 5 && score < 10) {
    snake.speed = 2;
  } else if (score >= 10 && score < 15) {
    snake.speed = 3;
  } else if (score >= 15 && score < 20) {
    snake.speed = 4;
  }

  if (snake.x >= SCREEN_WIDTH || snake.x <= -5) {
    snake.die();
  }
  if (snake.y >= SCREEN_HEIGHT || snake.y <= -5) {
    snake.die();
  }
}

//main game loop
void loop()
{
  oled.clear(PAGE);
  oled.setCursor(0, 0); //draw score

  //update
  gameUpdate();

  //drawing player/food
  snake.draw();
  food.draw();
  //drawing score
  oled.print(score);

  oled.display(); //display all
  delay(DISPLAY_RATE);


}
