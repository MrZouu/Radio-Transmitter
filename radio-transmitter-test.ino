#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int joystick1VRX = A0;
const int joystick1VRY = A1;
const int joystick1SW = 2;

const int joystick2VRX = A2;
const int joystick2VRY = A3;
const int joystick2SW = 6;

const int encoderSW = 5;
const int encoderDT = 4;
const int encoderCLK = 3;

volatile int encoderPos = 0;
int lastEncoded = 0;

void setup() {
  pinMode(joystick1SW, INPUT_PULLUP);
  pinMode(joystick2SW, INPUT_PULLUP);
  pinMode(encoderSW, INPUT_PULLUP);
  pinMode(encoderDT, INPUT);
  pinMode(encoderCLK, INPUT);

  attachInterrupt(digitalPinToInterrupt(encoderCLK), updateEncoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderDT), updateEncoder, CHANGE);

  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Initialisation de l'écran OLED
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  int joystick1X = analogRead(joystick1VRX);
  int joystick1Y = analogRead(joystick1VRY);
  int joystick1Button = digitalRead(joystick1SW);

  int joystick2X = analogRead(joystick2VRX);
  int joystick2Y = analogRead(joystick2VRY);
  int joystick2Button = digitalRead(joystick2SW);

  int encoderButton = digitalRead(encoderSW);

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.print("Joystick 1: ");
  display.print("X=");
  display.print(joystick1X);
  display.print(" Y=");
  display.print(joystick1Y);
  display.print(" B=");
  display.println(joystick1Button == LOW ? "Pressed" : "Released");

  display.setCursor(0, 16);
  display.print("Joystick 2: ");
  display.print("X=");
  display.print(joystick2X);
  display.print(" Y=");
  display.print(joystick2Y);
  display.print(" B=");
  display.println(joystick2Button == LOW ? "Pressed" : "Released");

  display.setCursor(0, 32);
  display.print("Encoder: ");
  display.print("Pos=");
  display.print(encoderPos);
  display.print(" B=");
  display.println(encoderButton == LOW ? "Pressed" : "Released");

  display.display();

  delay(100);
}

void updateEncoder() {
  int MSB = digitalRead(encoderCLK);
  int LSB = digitalRead(encoderDT);

  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if(sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderPos--;
  if(sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderPos++;

  lastEncoded = encoded;
}
