#include <Wire.h>
#include <SPI.h>
#include <PID_v1.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>

//Solid State Relay
#define SSR 11 

//Oled Display
#define MAX6675_SO 7 
#define MAX6675_CS 8
#define MAX6675_SCK 9
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Rotary Encoder
#define PIN_IN1 2
#define PIN_IN2 3

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); 

static const unsigned char PROGMEM logo_bmp[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x7f, 0xff, 0xfc, 0x3f, 0xff, 0xf0, 0x07, 0xff, 0xff, 0xc0, 0x00, 0x7f, 0xe0, 0x00, 0x00,
  0x00, 0x3f, 0xff, 0xff, 0x1f, 0xff, 0xfc, 0x03, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xfc, 0x00, 0x00,
  0x00, 0x1f, 0xff, 0xff, 0x9f, 0xff, 0xfe, 0x01, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xfe, 0x00, 0x00,
  0x00, 0x0f, 0xff, 0xff, 0xcf, 0xff, 0xff, 0x00, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0x80, 0x00,
  0x00, 0x07, 0xff, 0xff, 0xc7, 0xff, 0xff, 0x80, 0x7f, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xc0, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x3f, 0xc0, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x3f, 0xe0, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x0f, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x0f, 0xc0, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x07, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x03, 0xf8, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x01, 0xf8, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0xfc, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0xfc, 0x00,
  0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x7c, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x0f, 0xcf, 0xff, 0xff, 0xc3, 0xf0, 0x00, 0x00, 0x7c, 0x00,
  0x00, 0xff, 0xff, 0xff, 0x80, 0x00, 0x1f, 0xcf, 0xff, 0xff, 0xc3, 0xf0, 0x00, 0x00, 0x7e, 0x00,
  0x00, 0xff, 0xff, 0xff, 0x80, 0x0f, 0xff, 0x8f, 0xff, 0xff, 0xc3, 0xf0, 0x00, 0x00, 0x7e, 0x00,
  0x00, 0xff, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0x0f, 0xff, 0xff, 0xc3, 0xf0, 0x00, 0x00, 0x7e, 0x00,
  0x00, 0x7f, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0x0f, 0xff, 0xff, 0xc3, 0xf0, 0x00, 0x00, 0x7c, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0xe0, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x7c, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xe0, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0xfc, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xe0, 0x03, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xf8, 0x00, 0x00, 0xfc, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xf0, 0x01, 0xf8, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x01, 0xf8, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xe0, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x03, 0xf8, 0x00,
  0x00, 0x00, 0x00, 0x03, 0xe0, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x07, 0xf0, 0x00,
  0x00, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x1f, 0xe0, 0x00,
  0x00, 0x00, 0x00, 0x1f, 0xe0, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xf0, 0x7f, 0xe0, 0x00,
  0x00, 0x03, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0x00, 0x7f, 0xff, 0xc0, 0x1f, 0xff, 0xff, 0xc0, 0x00,
  0x00, 0x03, 0xff, 0xff, 0x80, 0x00, 0x3f, 0x80, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x03, 0xff, 0xff, 0x00, 0x00, 0x1f, 0x83, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xfe, 0x00, 0x00,
  0x00, 0x03, 0xff, 0xfe, 0x00, 0x00, 0x0f, 0xc7, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xf8, 0x00, 0x00,
  0x00, 0x03, 0xff, 0xf8, 0x00, 0x00, 0x0f, 0xcf, 0xff, 0xff, 0xc0, 0x00, 0x3f, 0xc0, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x4b, 0x77, 0xff, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0xec, 0x44, 0xa4, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0xfc, 0x67, 0x27, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x45, 0xde, 0x44, 0x21, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x33, 0x4b, 0xf0, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

double currentTemperature;
double targetTemperature = 235.00;
double gapTrigger = 20.00;
double PWM_Output;
boolean heatingPreviousState = false;
boolean heatingCurrentState = false;

//Define the aggressive and conservative PID Tuning Parameters

//////////////////////////////////////////////

double aggKp=15, aggKi=0.00, aggKd=0.00;       
                                            
double consKp=15, consKi=0.02, consKd=0.1;

//////////////////////////////////////////////


PID myPID(&currentTemperature, &PWM_Output, &targetTemperature, consKp, consKi, consKd, DIRECT);
RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);

 void checkPosition()
 {
   encoder.tick(); // just call tick() to check the state.
 }

void showText(String text, int textSize, int x, int y) {
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.println(text);
} 

void showSplashScreen(void) {
  display.clearDisplay();
  display.drawBitmap(0, 2, logo_bmp, 128, 64, 1);
  display.display();
}

void setup() {
  pinMode(MAX6675_CS, OUTPUT);
  pinMode(MAX6675_SO, INPUT);
  pinMode(MAX6675_SCK, OUTPUT);
  pinMode(SSR, OUTPUT);
  Serial.begin(9600);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  TCCR2B = TCCR2B & B11111000 | B00000111;    // D11 PWM is now 30.64 Hz
  showSplashScreen();
  delay(2000);
  myPID.SetMode(AUTOMATIC); //turn the PID on

  attachInterrupt(digitalPinToInterrupt(PIN_IN1), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_IN2), checkPosition, CHANGE);
  encoder.setPosition((long) targetTemperature);
}

void showTemperature(float target, float current) {
  showText("Control", 2, 22, 0);
  display.drawRect(0, 16, 120, 20, WHITE);
  showText("Target: " + String(target) + " C", 1, 4, 22);
  display.drawRect(0, 43, 120, 20, WHITE);
  showText("Current: " + String(current) + " C", 1, 4, 50);
}

void showHeatingStatus(bool heating) {
  heatingCurrentState = heating;
  if (!(heatingPreviousState == heatingCurrentState) && (heating == true)) {
    heatingCurrentState = true;
    display.fillCircle(10, 6, 3, WHITE);
  }
}

void loop() {

  encoder.tick();
  long newTemperature = encoder.getPosition();
  if ((int) targetTemperature != (int) newTemperature) {
    targetTemperature = (double) newTemperature;
  }

  currentTemperature = read_termocouple();
  double gap = abs(targetTemperature-currentTemperature); //distance away from setpoint

  if (gap < gapTrigger) {  //we're close to target temperature, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else { //we're far from target temperature, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
  }
  myPID.Compute();
  analogWrite(SSR, PWM_Output); 
  display.clearDisplay();
 
  showTemperature(targetTemperature, currentTemperature);
  if (currentTemperature <= targetTemperature) {
    showHeatingStatus(true);
  }
  else if (currentTemperature >= targetTemperature) {
    showHeatingStatus(false);
  }

  Serial.println("\n------------------------------------------");
  Serial.println("Current: " + String(currentTemperature, 2));
  Serial.println("Target: " + String(targetTemperature));
  Serial.println("PWM_Output: " + String(PWM_Output));
  Serial.println("PID: (" + String(myPID.GetKp()) + ")" + " " + "(" + String(myPID.GetKi()) + ")" + " " + "(" + String(myPID.GetKd()) + ")");
  Serial.println("------------------------------------------");

  display.display();
  delay(200);
}

double read_termocouple() {
  uint16_t v;
  digitalWrite(MAX6675_CS, LOW);
  delay(1);

  v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
  v <<= 8;
  v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);

  digitalWrite(MAX6675_CS, HIGH);
  if (v & 0x4)
  {
    return NAN;
  }
  v >>= 3;
  return v * 0.25;
}
