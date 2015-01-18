#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const uint8_t I2C_ADDRESS_DISPLAY = 0x3C;
const uint8_t OLED_RESET = 4;
const uint8_t MAX_STARS = 10;

struct Star {  int8_t x, y, vx; };

Star starfield[MAX_STARS];
Adafruit_SSD1306 display(OLED_RESET);

void init_starfield() {
  for (uint8_t i = 0; i < MAX_STARS; i++) {
    starfield[i].x = random(display.width());
    starfield[i].y = random(display.height());
    starfield[i].vx = random(3) + 1;
  }
}

void move_stars() {
  for (uint8_t i = 0; i < MAX_STARS; i++) {
    starfield[i].x -= starfield[i].vx;
    if (starfield[i].x < 0) {
      starfield[i].x = display.width();
      starfield[i].y = random(display.height());
      starfield[i].vx = random(3) + 1;
    }
  }
}

void draw_stars() {
  for (uint8_t i = 0; i < MAX_STARS; i++) {
    display.drawPixel(starfield[i].x, starfield[i].y, WHITE);
  }
}

void setup()  {
  randomSeed(analogRead(A0));
  init_starfield();
  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS_DISPLAY);
  display.display();
  delay(1000);
  display.clearDisplay();
}

void loop() {
  display.clearDisplay();
  move_stars();
  draw_stars();
  display.display();
}

