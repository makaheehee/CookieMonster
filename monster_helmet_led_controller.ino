#include <Adafruit_NeoPixel.h>

//------------------ DO NOT MODIFY --------------//
#define LEFT_EYE_PIN      5
#define RIGHT_EYE_PIN     6
#define BUTTON_PIN        4
#define LED_STRIP_LENGTH  52

#define BLINK_TIME_MS 3000
#define BUTTON_DEBOUNCE_MS 30

#define BUTTON_PRESSED  1
#define BUTTON_HELD     2
#define BUTTON_RELEASED 0
//-----------------------------------------------//

// Change this to match the number of colors to switch
#define NUMBER_OF_COLORS 10

uint16_t blinkCounter = 0;
uint8_t buttonCounter = 0;
uint8_t buttonState = BUTTON_RELEASED;

uint32_t color = 0;
uint32_t colorArray[NUMBER_OF_COLORS];
uint8_t colorIndex = 0;
boolean changeColor = false;

// Array
uint8_t rowSize[] = { 4,
                      10,
                      18,
                      26,
                      34,
                      42,
                      48,
                      52};

// Frames
boolean eye[] = {    1,1,1,1,
                   1,1,1,1,1,1,
                 1,1,1,0,0,1,1,1,
                 1,1,0,0,0,0,1,1,
                 1,1,0,0,0,0,1,1,
                 1,1,1,0,0,1,1,1,
                   1,1,1,1,1,1,
                     1,1,1,1};

boolean evilEyeLeft[] = {    0,0,0,0,
                           0,0,0,0,0,1,
                         0,0,0,0,0,1,1,1,
                         0,0,0,0,1,1,1,1,
                         0,0,0,1,1,1,1,1,
                         0,0,1,1,1,0,0,0,
                           1,1,1,0,0,0,
                             1,1,0,0};

boolean evilEyeRight[] = {   0,0,0,0,
                           1,0,0,0,0,0,
                         1,1,1,0,0,0,0,0,
                         1,1,1,1,0,0,0,0,
                         1,1,1,1,1,0,0,0,
                         0,0,0,1,1,1,0,0,
                           0,0,0,1,1,1,
                             0,0,1,1};

boolean movingEye1[] = { 1,1,1,1,
                       0,0,1,1,1,1,
                     0,0,0,0,1,1,1,1,
                     0,0,0,0,1,1,1,1,
                     1,0,0,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                       1,1,1,1,1,1,
                         1,1,1,1};

boolean movingEye2[] = { 1,1,1,1,
                       1,1,1,1,1,1,
                     1,0,0,1,1,1,1,1,
                     0,0,0,0,1,1,1,1,
                     0,0,0,0,1,1,1,1,
                     1,0,0,1,1,1,1,1,
                       1,1,1,1,1,1,
                         1,1,1,1};

boolean movingEye3[] = { 1,1,1,1,
                       1,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                     1,0,0,1,1,1,1,1,
                     0,0,0,0,1,1,1,1,
                     0,0,0,0,1,1,1,1,
                       0,0,1,1,1,1,
                         1,1,1,1};

boolean movingEye4[] = { 1,1,1,1,
                       1,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                     1,1,0,0,1,1,1,1,
                     1,0,0,0,0,1,1,1,
                       0,0,0,0,1,1,
                         0,0,1,1};

boolean movingEye5[] = { 1,1,1,1,
                       1,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                     1,1,1,0,0,1,1,1,
                     1,1,0,0,0,0,1,1,
                       1,0,0,0,0,1,
                         1,0,0,1};

boolean movingEye6[] = { 1,1,1,1,
                       1,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                     1,1,1,1,0,0,1,1,
                     1,1,1,0,0,0,0,1,
                       1,1,0,0,0,0,
                         1,1,0,0};

boolean movingEye7[] = { 1,1,1,1,
                       1,1,1,1,1,1,
                     1,1,1,1,1,1,1,1,
                     1,1,1,1,1,0,0,1,
                     1,1,1,1,0,0,0,0,
                     1,1,1,1,0,0,0,0,
                       1,1,1,1,0,0,
                         1,1,1,1};

boolean movingEye8[] = { 1,1,1,1,
                       1,1,1,1,1,1,
                     1,1,1,1,1,0,0,1,
                     1,1,1,1,0,0,0,0,
                     1,1,1,1,0,0,0,0,
                     1,1,1,1,1,0,0,1,
                       1,1,1,1,1,1,
                         1,1,1,1};


// Function prototypes
void drawEye(Adafruit_NeoPixel * ledStrip, boolean * frameArray, uint32_t color);
void blinkEye(Adafruit_NeoPixel * leftEye, Adafruit_NeoPixel * rightEye, boolean * frameArray, uint32_t color, uint32_t fps);
void animateEye1(Adafruit_NeoPixel *ledStrip, uint32_t color, uint32_t fps);

// Pixel object declarations
Adafruit_NeoPixel leftEyeStrip = Adafruit_NeoPixel(LED_STRIP_LENGTH, LEFT_EYE_PIN, NEO_GRBW + NEO_KHZ800);
Adafruit_NeoPixel rightEyeStrip = Adafruit_NeoPixel(LED_STRIP_LENGTH, RIGHT_EYE_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  leftEyeStrip.begin();
  leftEyeStrip.show(); // Initialize all pixels to 'off'

  rightEyeStrip.begin();
  rightEyeStrip.show(); // Initialize all pixels to 'off'

  pinMode(BUTTON_PIN, INPUT);

  // Setup color array
  colorArray[0] = leftEyeStrip.Color(25,7,15,0); // PINK
  colorArray[1] = leftEyeStrip.Color(20,0,0,0); // RED
  colorArray[2] = leftEyeStrip.Color(20,0,20,0); // PURPLE
  colorArray[3] = leftEyeStrip.Color(0,0,20,0); // BLUE
  colorArray[4] = leftEyeStrip.Color(0,20,20,0); // CYAN
  colorArray[5] = leftEyeStrip.Color(0,20,0,0); // GREEN
  colorArray[6] = leftEyeStrip.Color(22,18,0,0); // YELLOW
  colorArray[7] = leftEyeStrip.Color(30,10,0,0); // ORANGE
  colorArray[8] = leftEyeStrip.Color(0,0,0,20); // WHITE
  colorArray[9] = leftEyeStrip.Color(0,0,0,0); // OFF

  // Set Initial color
  color = colorArray[colorIndex];
}

void loop() {
  // put your main code here, to run repeatedly:

//------------------ DO NOT MODIFY --------------//
  // Fill button buffer
  buttonCounter <<= 1;
  buttonCounter |= digitalRead(BUTTON_PIN);

  // Handle button press
  if (buttonState == BUTTON_RELEASED && buttonCounter == 255)
  {
    buttonState = BUTTON_PRESSED;
    //Serial.println("Button Pressed!");
    changeColor = true;
  }
  else if (buttonState == BUTTON_PRESSED)
  {
    buttonState = BUTTON_HELD;
    //Serial.println("Button Held!");
  }
  else if (buttonState == BUTTON_HELD)
  {
    if (buttonCounter == 0)
    {
      buttonState = BUTTON_RELEASED;
      //Serial.println("Button Released!");
    }
  }

  // Reset color at end of cycle
  if (changeColor == true)
  {
    colorIndex++;
    if (colorIndex >= NUMBER_OF_COLORS)
    {
      colorIndex = 0;
    }
    
    color = colorArray[colorIndex];
    drawEye(&leftEyeStrip, eye, color);
    drawEye(&rightEyeStrip, eye, color);

    changeColor = false;
  }
//-----------------------------------------------// 

  // Blink Eyes
  if (blinkCounter == BLINK_TIME_MS)
  {
    blinkEye(&leftEyeStrip, &rightEyeStrip, eye, color, 60);
    blinkCounter = 0;
  }
  else
  {
    blinkCounter++;
  }

  delay(1);

  /*
  color = leftEyeStrip.Color(20,0,0,0);
  blinkEye(&leftEyeStrip, &rightEyeStrip, eye, color, 60);
  delay(3000);

  color = leftEyeStrip.Color(0,20,0,0);
  blinkEye(&leftEyeStrip, &rightEyeStrip, eye, color, 60);
  delay(3000);

  color = leftEyeStrip.Color(0,0,20,0);
  blinkEye(&leftEyeStrip, &rightEyeStrip, eye, color, 60);
  delay(3000);

  color = leftEyeStrip.Color(0,0,0,20);
  blinkEye(&leftEyeStrip, &rightEyeStrip, eye, color, 60);
  delay(3000);
  */

/*
  // evil eye
  color = leftEyeStrip.Color(20,0,0,0);
  blinkEye(&leftEyeStrip, evilEyeLeft, color, 24);
  blinkEye(&rightEyeStrip, evilEyeRight, color, 24);
  delay(1000);

  color = leftEyeStrip.Color(0,20,0,0);
  blinkEye(&leftEyeStrip, evilEyeLeft, color, 24);
  blinkEye(&rightEyeStrip, evilEyeRight, color, 24);
  delay(1000);

  color = leftEyeStrip.Color(0,0,20,0);
  blinkEye(&leftEyeStrip, evilEyeLeft, color, 24);
  blinkEye(&rightEyeStrip, evilEyeRight, color, 24);
  delay(1000);

  color = leftEyeStrip.Color(0,0,0,20);
  blinkEye(&leftEyeStrip, evilEyeLeft, color, 24);
  blinkEye(&rightEyeStrip, evilEyeRight, color, 24);
  delay(1000);
*/

/*
  // Animate eye 1
  color = leftEyeStrip.Color(20,0,0,0);
  animateEye1(&leftEyeStrip, color, 24);
  animateEye1(&rightEyeStrip, color, 24);
  delay(1000);

  color = leftEyeStrip.Color(0,20,0,0);
  animateEye1(&leftEyeStrip, color, 24);
  animateEye1(&rightEyeStrip, color, 24);
  delay(1000);

  color = leftEyeStrip.Color(0,0,20,0);
  animateEye1(&leftEyeStrip, color, 24);
  animateEye1(&rightEyeStrip, color, 24);
  delay(1000);

  color = leftEyeStrip.Color(0,0,0,20);
  animateEye1(&leftEyeStrip, color, 24);
  animateEye1(&rightEyeStrip, color, 24);
  delay(1000);
*/

}


void drawEye(Adafruit_NeoPixel * ledStrip, boolean * frameArray, uint32_t color)
{
  int ledCounter = 0;

  for (ledCounter = 0; ledCounter < LED_STRIP_LENGTH; ledCounter++)
  {
    if (frameArray[ledCounter])
    {
      ledStrip->setPixelColor(ledCounter, color);
    }
    else
    {
      ledStrip->setPixelColor(ledCounter, 0);
    }
  }
  ledStrip->show();
}

void blinkEye(Adafruit_NeoPixel * leftEye, Adafruit_NeoPixel * rightEye, boolean * frameArray, uint32_t color, uint32_t fps)
{
  int frameCounter = 0;
  int ledCounter = 0;
  int frameTime = 1000/fps;

  // Close eye
  for (frameCounter = 0; frameCounter <= 8; frameCounter++)
  {
    for (ledCounter = LED_STRIP_LENGTH - 1; ledCounter >= 0; ledCounter--)
    {
      if (frameArray[ledCounter] && ledCounter < rowSize[7-frameCounter])
      {
        leftEye->setPixelColor(ledCounter, color);
        rightEye->setPixelColor(ledCounter, color);
      }
      else
      {
        leftEye->setPixelColor(ledCounter, 0);
        rightEye->setPixelColor(ledCounter, 0);
      }
    }
    leftEye->show();
    rightEye->show();
    delay(frameTime);
  }

  delay(frameTime * 10);
  
  // Delay eye opening
  // Open eye
  for (frameCounter = 0; frameCounter < 8; frameCounter++)
  {
    for (ledCounter = LED_STRIP_LENGTH - 1; ledCounter >= 0; --ledCounter)
    {
      if (frameArray[ledCounter] && ledCounter < rowSize[frameCounter])
      {
        leftEye->setPixelColor(ledCounter, color);
        rightEye->setPixelColor(ledCounter, color);
      }
      else
      {
        leftEye->setPixelColor(ledCounter, 0);
        rightEye->setPixelColor(ledCounter, 0);
      }
    }
    leftEye->show();
    rightEye->show();
    delay(frameTime);
  }

  drawEye(leftEye, frameArray, color);
  drawEye(rightEye, frameArray, color);
}

void animateEye1(Adafruit_NeoPixel *ledStrip, uint32_t color, uint32_t fps)
{
  int frameTime = 1000/fps;

  drawEye(ledStrip, movingEye1, color);
  delay(frameTime*5);

  drawEye(ledStrip, movingEye2, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye3, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye4, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye5, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye6, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye7, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye8, color);
  delay(frameTime*5);

  drawEye(ledStrip, movingEye7, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye6, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye5, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye4, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye3, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye2, color);
  delay(frameTime);

  drawEye(ledStrip, movingEye1, color);
  delay(frameTime*5);
}
