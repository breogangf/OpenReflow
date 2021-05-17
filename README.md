# OpenReflow
![reflow gif][gif]

**Description**:  
- OpenSource reflow solder control.
- Arduino platform compatible written in C++. 

**Features**:
- PID temperature control.
- Oled display shows: 
  - currentTemperature in °C.
  - targetTemperature in °C.
  - heatingStatus icon.
- PWM controlled SSR (Solid State Relay)
- Rotary encoder input (Using external microcontroller interruptions)

**Software stack**: 
- Arduino platform, C++.

**Hardware requirements**: 

- Arduino Nano.
- 0.96" Oled Display.
- K-type Thermocouple.
- MAX5575 K-Thermocouple to Digital Converter module.
- Rotary Encoder.
- DC - AC Solid State Relay.
- AC Hot plate.

**Status**: 
- Alpha 0.0.2, basic PID for temperature control. 

## Dependencies
- Oled Screen: https://github.com/adafruit/Adafruit_SSD1306
- Graphics: https://github.com/adafruit/Adafruit-GFX-Library
- PID: https://github.com/br3ttb/Arduino-PID-Library
- Rotary Encoder: https://github.com/mathertel/RotaryEncoder
- SPI: https://www.arduino.cc/en/reference/SPI
- I2C: https://www.arduino.cc/en/reference/wire

## Installation

- Install all the library dependencies using your Arduino IDE and compile/upload your code to you arduino device.

## Hardware Configuration
- Rotary Encoder:
  - D2 (INT) -> Rotary DT
  - D3 (INT) -> Rotary CLK
  - VCC
  - GND

- Thermocouple module:
  - D7 -> MAX6675 SO
  - D8 -> MAX6675 CS
  - D9 -> MAX6675 SCK
  - VCC
  - GND

- Oled Display:
  - A4 (SDA) -> SDA
  - A5 (SCL) -> SCK
  - VCC
  - GND

- Solid State Relay:
  - GND -> SSR VDC-
  - D11 (PWM) -> SSR VDC+
  - Note1: D11 PWM frecuency will be set to 30.64 Hz.
  - Note2: Connect your AC Hotplate to VAC terminals.

## Software Configuration

- You can setup 3 basic things: 
1. PID: A proportional–integral–derivative controller.

|              | Kp   | Ki   | Kd   | Description                                                     |
|--------------|------|------|------|-----------------------------------------------------------------|
| Conservative | 0.30 | 0.02 | 0.10 | This will be used to stabilise close to the target temperature  |
| Aggressive   | 0.80 | 0.00 | 0.00 | This will help reach far temperatures faster                    |
  
2. Target Temperature

|                   | Default | Description |
|-------------------|---------|-------------|
| targetTemperature | 235.00  | Your default desired temperature in °C |

3. Temperature gap trigger

|            | Default | Description |
|------------|---------|-------------|
| gapTrigger | 20.00   | It will use the conservative PID parameters when the temperature gap gets closer than this value. In °C |

## Usage

1. Turn on the device, you will see a splash screen for two seconds, and after that, the main control screen.
2. The hot plate temperature will begin to raise following your targetTemperature.
3. You can rotate clockwise the rotary encoder to increase the targetTemperature.
4. You can rotate counter clockwise the rotary encoder to reduce the targetTemperature.

## Known issues
None yet.
Have you found something wrong? please file an issue in this repository, or help me by sending a pull request.

## Getting help

If you have questions, concerns, bug reports, etc you can contact me at info@breogangf.com.

## Open source licensing info

- [LICENSE](LICENSE)

----

## Credits and references
For more information about reflow soldering profiles -> https://www.compuphase.com/electronics/reflowsolderprofiles.htm

[gif]: ./images/reflow.gif "Open Reflow"