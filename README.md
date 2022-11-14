# distance_logger
Simple project with Arduino to measure distance with ultrasonic sensor, save it in a memory card or send it via serial port.

Also, it has a buzzer that sounds when distance is less than 10 cm. For debug mode, it adds and i2c LCD display, that shows
current distance on it.

## Schematic

[Schematic](distance_logger_bb.png)

## Connections table

|   Component  | Connection |
|:------------:|:----------:|
| HC-SR04 Trig |     D5     |
| HC-Sr04 Echo |     D4     |
|    i2c SDA   |     A4     |
| i2c SCL      | A5         |
| USART RX     | D6         |
| USART TX     | D7         |
| BUZZER       | D8         |
| SWITCH 1     | D2         |
| SWITCH 2     | D3         |
| SD Card MISO | D12        |
| SD Card MOSI | D11        |
| SD Card CS   | D10        |
| SD Card SCK  | D9         |