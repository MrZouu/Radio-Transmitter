#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RF24.h>
#include <MPU6050.h>

// Définition des broches
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define JOY_X A0
#define JOY_Y A1
#define JOY_SW 7
#define SWITCH_PIN1 5
#define SWITCH_PIN2 6
#define ENC_SW 2
#define ENC_CLK 3
#define ENC_DT 4

MPU6050 mpu;
RF24 radio(9, 10);
const byte address[6] = "00001";

struct Data {
  int16_t joyX;
  int16_t joyY;
  int16_t accX;
  int16_t accY;
  int16_t accZ;
};

Data data;

void setup() {
  pinMode(JOY_SW, INPUT_PULLUP);
  pinMode(SWITCH_PIN1, INPUT_PULLUP);
  pinMode(SWITCH_PIN2, INPUT_PULLUP);
  pinMode(ENC_SW, INPUT_PULLUP);
  pinMode(ENC_CLK, INPUT_PULLUP);
  pinMode(ENC_DT, INPUT_PULLUP);
  
  Serial.begin(9600);
  Wire.begin();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();

  mpu.initialize();
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
}

void loop() {
  if (digitalRead(SWITCH_PIN1) == LOW) {
    // Lire joystick
    data.joyX = analogRead(JOY_X);
    data.joyY = analogRead(JOY_Y);
    displayJoystick();
  } else {
    // Lire MPU6050
    mpu.getAcceleration(&data.accX, &data.accY, &data.accZ);
    displayMPU();
  }

  // Envoyer les données
  radio.write(&data, sizeof(data));
  delay(100);
}

void displayJoystick() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("Joystick:");
  display.setCursor(0,10);
  display.print("X: "); display.print(data.joyX);
  display.setCursor(0,20);
  display.print("Y: "); display.print(data.joyY);
  display.display();
}

void displayMPU() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print("MPU6050:");
  display.setCursor(0,10);
  display.print("AccX: "); display.print(data.accX);
  display.setCursor(0,20);
  display.print("AccY: "); display.print(data.accY);
  display.setCursor(0,30);
  display.print("AccZ: "); display.print(data.accZ);
  display.display();
}
