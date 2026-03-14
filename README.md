# 🧱 Brick Breaker — ESP32 + OLED

A classic Brick Breaker arcade game built on a breadboard using an ESP32,
a 0.96" OLED display, joystick, TM1637 score display, and LEDs.

---

## Demo

Break all 24 bricks to win. You have 3 lives. Score updates live on the
4-digit display every time you destroy a brick.

---

## Hardware Required

| Component | Details |
|-----------|---------|
| ESP32 | Dev board (ESP32-D0WDQ6 or similar) |
| OLED Display | 0.96 inch SSD1306, I2C (GND VDD SCK SDA) |
| Joystick Module | Analog XY + button (GND +5V VRx VRy SW) |
| TM1637 Display | 4-digit 7-segment V1.2 (CLK GND DIO VCC) |
| Green LED | With 220Ω resistor |
| Red LED | With 220Ω resistor |
| Breadboard | Full size recommended |
| Jumper wires | Male to male |
| USB cable | Data capable (not charge only) |

---

## Wiring

### OLED (I2C) → ESP32
| OLED | ESP32 |
|------|-------|
| VDD | 3V3 |
| GND | GND |
| SCK | GPIO22 |
| SDA | GPIO21 |

### Joystick → ESP32
| Joystick | ESP32 |
|----------|-------|
| +5V | 3V3 |
| GND | GND |
| VRx | GPIO34 |
| VRy | GPIO35 |
| SW | GPIO15 |

### TM1637 → ESP32
| TM1637 | ESP32 |
|--------|-------|
| VCC | 3V3 |
| GND | GND |
| CLK | GPIO13 |
| DIO | GPIO12 |

### LEDs → ESP32
| Component | ESP32 | Note |
|-----------|-------|------|
| Green LED (+) | GPIO26 | 220Ω resistor in series |
| Red LED (+) | GPIO27 | 220Ω resistor in series |
| Both LED (-) | GND | Common ground |

> All modules powered from 3.3V. Do not use 5V —
> it will damage the ESP32 ADC pins.

---

## Libraries Required

Install via Arduino IDE:
**Sketch → Include Library → Manage Libraries**

| Library | Author |
|---------|--------|
| Adafruit SSD1306 | Adafruit |
| Adafruit GFX Library | Adafruit |
| TM1637 | Avishay Orpaz |

---

## Arduino IDE Settings

| Setting | Value |
|---------|-------|
| Board | ESP32 Dev Module |
| Upload Speed | 115200 |
| Port | Your COM port |

---

## How to Upload

1. Open `brick_breaker.ino` in Arduino IDE
2. Select board and port under **Tools**
3. Click **Upload**
4. If you see `Connecting....` hold the **BOOT button** on ESP32
5. Release once uploading starts

---

## How to Play

| Action | Control |
|--------|---------|
| Move paddle left | Tilt joystick left |
| Move paddle right | Tilt joystick right |
| Restart after game over | Press joystick button |

### Tips
- Hit the left edge of the paddle to send ball left
- Hit the right edge to send ball right
- Center hits go straight up
- Score shows live on TM1637 display
- Lives shown bottom left on OLED

### LED feedback
| LED | Meaning |
|-----|---------|
| Green flash | Brick destroyed |
| Red flash | Life lost |
| Red solid | Game over |
| Green solid | You win! |

---

## Game Details

| Setting | Value |
|---------|-------|
| Bricks | 24 (8 columns x 3 rows) |
| Lives | 3 |
| Points per brick | 10 |
| Max score | 240 |
| Ball speed | 1.5 (increase for harder) |
| Paddle width | 20px (decrease for harder) |

---

## Make it Harder

In the code find these lines and tweak them:
```cpp
// Faster ball
ballDX = 2.0;
ballDY = -2.0;

// Smaller paddle
#define PADDLE_W 14

// More brick rows
#define BRICK_ROWS 4
```

---

## Project Structure
```
brick_breaker/
└── brick_breaker.ino
```

---

## Built With

- ESP32 Arduino Core
- Adafruit SSD1306 + GFX libraries
- TM1637 library by Avishay Orpaz

---

## Author

Built on a breadboard with jumper wires and spare LEDs.
Fork it, hack it, make it better!
