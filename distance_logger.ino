#include <SoftwareSerial.h>
// For Serial
#include <SPI.h>
#include <SD.h>
// For SD card
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// For LCD display

// Libraries

#define PULS1 2
#define PULS2 3
#define IN_LED 13

#define TRIG_PIN 5
#define ECHO_PIN 4

#define CS 10

#define BUZZER 8

#define USART_RX 6
#define USART_TX 7

#define LCD_ROWS 2
#define LCD_COLUMNS 16
#define I2C_ADDR 0x27

// DEFINEs

int mode, distance;

SoftwareSerial usart(USART_RX, USART_TX);

LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLUMNS, LCD_ROWS);

// Variables

void write_on_display(int distance) {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Distance Logger");
  lcd.setCursor(0, 1);
  lcd.print(String(distance));
}

void sound_buzzer() {
  /*
  Plays a frequency with buzzer.
  */
  tone(BUZZER, 1000, 1000);
  // Buzzer Pin, Tone Frequency, Tone Duration
}

void write_SD(int distance) {
  /*
  Write distance in SD, a value for row.
  */
  String dataString = String(distance);

  File dataFile = SD.open("distance_log.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
}

int read_distance() {
  /*
  Reads distance using ultrasonic sensor.
  */
  // Clears the trigPin condition
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(ECHO_PIN, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor

  return distance;
}

void change_mode() {
  /*
  Switch ISR, read state and selects mode.
  */
  if (!digitalRead(PULS1) && !digitalRead(PULS2)) {
    // 0 - 0 - Normal mode
    mode = 0;
  } else if (digitalRead(PULS1) && !digitalRead(PULS2) || !digitalRead(PULS1) && digitalRead(PULS2)) {
    // 1 - 0 o 0 - 1 - Debug mode
    mode = 1;
  } else {
    // 1 - 1 - Com mode
    mode = 2;
  }
}

void setup() {
  change_mode();

  pinMode(PULS1, INPUT_PULLUP);
  pinMode(PULS2, INPUT_PULLUP);

  pinMode(IN_LED, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(BUZZER, OUTPUT);

  // Pin Config

  attachInterrupt(digitalPinToInterrupt(PULS1), change_mode, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PULS2), change_mode, CHANGE);

  // Pin Interrupt

  Serial.begin(9600);  
  usart.begin(9600);

  // Serial begin

  if (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
  } else {
    Serial.println("Card initialized.");
  }

  // SD Inits

  write_on_display(0);
  // LCD i2c Init
}

void loop() {
  distance = read_distance();

  if (distance < 10) {
    sound_buzzer();
  }

  write_SD(distance);

  // Measure and write distance, always done

  if (mode == 1) {
    // Debug mode, write on display
    write_on_display(distance);
  } else if (mode == 2) {
    // Communication mode, writes on serial USB
    usart.println(distance);
  }

  delay(100);
}