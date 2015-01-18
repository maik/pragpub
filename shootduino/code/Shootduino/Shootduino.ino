#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "gfx.h"
#include "joystick.h"

const uint8_t I2C_ADDRESS_DISPLAY = 0x3C;
const uint8_t OLED_RESET = 4;

const uint8_t MAX_LIVES = 3;
const uint8_t MAX_ASTEROIDS = 8;
const uint8_t MAX_BULLETS = 3;
const uint8_t MAX_STARS = 10;
const uint8_t MAX_MISSES = 5;
const uint16_t ASTEROID_DELAY = 200;
const uint16_t BULLET_DELAY = 600;
const uint16_t MIN_DELAY = 300;

const uint8_t MAX_SCORE_LEN = 7;

enum ObjectType {
  PLAYER, ASTEROID, BULLET, EXPLOSION
};

enum GameState {
  INTRO, RUNNING, PAUSED, LOST_LIVE, DONE
};

struct GameObject {
  int16_t x;
  int8_t y, vx, vy;
  boolean is_active;
  ObjectType type;
  uint8_t anim_frame;
  uint8_t frame_count;
};

GameObject bullets[MAX_BULLETS];
GameObject asteroids[MAX_ASTEROIDS];
GameObject player;

struct Star {
  int8_t x, y, vx;
};

Star starfield[MAX_STARS];

void init_objects(GameObject* objects, uint8_t max_objects);
void draw_objects(GameObject* objects, uint8_t max_objects);
void move_objects(GameObject* objects, uint8_t max_objects);
void change_state(GameState new_state);

unsigned long ticks = 0;  
unsigned long bullet_fired = 0; // Timestamp of the last bullet fired by player.
unsigned long asteroid_started = 0; // Timestamp of the last asteroid.

Adafruit_SSD1306 display(OLED_RESET);
boolean player_hit = false;
GameState state;
uint8_t lives = MAX_LIVES;
uint16_t score = 0;
uint8_t asteroids_missed = 0;
unsigned long state_changed = 0;

void change_state(GameState new_state) {
  state = new_state;
  state_changed = ticks;
}

uint8_t center_pos(const char* str, const uint8_t text_size) {
  return (display.width() - (strlen_P(str) * text_size * BASE_FONT_W)) / 2;
}

void pmem_print(uint8_t x, uint8_t y, uint8_t size, const char* str, uint16_t color = WHITE) {
  uint8_t font_width = size * BASE_FONT_W;
  char c;
  uint8_t i = 0;
  display.setTextSize(size);
  display.setTextColor(color);
  while (c = pgm_read_byte(str++)) {
    display.setCursor(x + i++ * font_width, y);
    display.print(c);
  }
}

void pmem_print_center(uint8_t y, uint8_t size, const char* str, uint16_t color = WHITE) {
  pmem_print(center_pos(str, size), y, size, str, color);
}

void init_objects(GameObject* objects, uint8_t max_objects) {
  for (uint8_t i = 0; i < max_objects; i++) {
    objects[i].is_active = false;
  }
}

void draw_objects(GameObject* objects, uint8_t max_objects) {
  const uint8_t* bmp = NULL;
  for (uint8_t i = 0; i < max_objects; i++) {
    if (objects[i].is_active) {
      switch (objects[i].type) {
        case ASTEROID:
          bmp = asteroid_anim1 + (2 * ASTEROID_H) * objects[i].anim_frame;
          display.drawBitmap(asteroids[i].x, asteroids[i].y, bmp, ASTEROID_W, ASTEROID_H, WHITE);
          if (state == RUNNING)
            objects[i].frame_count++;
          if (objects[i].frame_count == ANIM_FRAME_DELAY)
          {
            objects[i].anim_frame++;
            objects[i].anim_frame %= NUM_ASTEROID_FRAMES;
            objects[i].frame_count = 0;            
          }
          break;
        case EXPLOSION:
          bmp = explosion_anim1 + (2 * EXPLOSION_H) * objects[i].anim_frame;
          display.drawBitmap(asteroids[i].x, asteroids[i].y, bmp, EXPLOSION_W, EXPLOSION_H, WHITE);
          objects[i].frame_count++;
          if (objects[i].frame_count == ANIM_FRAME_DELAY)
          {
            objects[i].frame_count = 0;            
            objects[i].anim_frame++;
            if (objects[i].anim_frame == NUM_EXPLOSION_FRAMES)
              objects[i].is_active = false;
          }
          break;
        case BULLET:
          display.drawFastHLine(objects[i].x, objects[i].y, BULLET_W, WHITE);
          break;
      }
    }
  }
}

void move_objects(GameObject* objects, uint8_t max_objects) {
  for (uint8_t i = 0; i < max_objects; i++) {
    if (objects[i].is_active) {
      objects[i].x += objects[i].vx;      
      switch (objects[i].type) {
        case ASTEROID:
          if (objects[i].x + (int)ASTEROID_W <= 0) {
            objects[i].is_active = false;
            asteroids_missed++;
          }          
          break;
        case BULLET:
          if (objects[i].x >= display.width()) {
            objects[i].is_active = false;
          }
          break;
      }
    }
  }
}

void fire_bullet() {
  if (ticks - bullet_fired < BULLET_DELAY) {
    return;
  }
  bullet_fired = ticks;
  
  for (uint8_t i = 0; i < MAX_BULLETS; i++) {
    if (!bullets[i].is_active) {
      bullets[i].type = BULLET;
      bullets[i].is_active = true;
      bullets[i].x = player.x + PLAYER_W;
      bullets[i].y = player.y + (PLAYER_H / 2);
      bullets[i].vx = 3;
      break;
    }
  }
}

void start_asteroid() {
  if (ticks - asteroid_started < ASTEROID_DELAY) {
    return;
  }
  asteroid_started = ticks;
  
  for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {
    if (!asteroids[i].is_active) {
      asteroids[i].type = ASTEROID;
      asteroids[i].is_active = true;
      asteroids[i].x = display.width();
      asteroids[i].y = random(display.height() - ASTEROID_H) + 1;
      asteroids[i].vx = -1 * random(2) - 2;
      asteroids[i].anim_frame = random(NUM_ASTEROID_FRAMES);
      asteroids[i].frame_count = 0;
      break;
    }
  }
}

void move_player() {
  if (left) {
    player.x -= player.vx;
    if (player.x < 0) {
      player.x = 0;
    }
  }
  if (right) {
    player.x += player.vx;
    if (player.x + PLAYER_W > display.width()) {
      player.x = display.width() - PLAYER_W;
    }
  }
  if (up) {
    player.y -= player.vy;
    if (player.y < 0) {
      player.y = 0;
    }
  }
  if (down) {
    player.y += player.vy;
    if (player.y + PLAYER_W > display.height()) {
      player.y = display.height() - PLAYER_H;
    }
  }
}

void draw_player() {
  if (!player.is_active)
    return;
  if (player.type == PLAYER) {
    display.drawBitmap(player.x, player.y, spaceship_bmp, PLAYER_W, PLAYER_H, WHITE);
  } else {
    const uint8_t* bmp = explosion_anim1 + (2 * EXPLOSION_H) * player.anim_frame;
    display.drawBitmap(player.x, player.y, bmp, EXPLOSION_W, EXPLOSION_H, WHITE);
    player.frame_count++;
    if (player.frame_count == ANIM_FRAME_DELAY)
    {
      player.frame_count = 0;            
      player.anim_frame++;
      if (player.anim_frame == NUM_EXPLOSION_FRAMES)
        player.is_active = false;
    }
  }
}

void check_collisions() {
  for (uint8_t i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].is_active) {
      for (uint8_t j = 0; j < MAX_ASTEROIDS; j++) {
        if (asteroids[j].is_active && asteroids[j].type == ASTEROID) {
          if (bullets[i].x < asteroids[j].x + ASTEROID_W && bullets[i].x + BULLET_W > asteroids[j].x &&
              bullets[i].y < asteroids[j].y + ASTEROID_H && bullets[i].y + BULLET_H > asteroids[j].y)
          {
            asteroids[j].type = EXPLOSION;
            asteroids[j].vx = 0;
            bullets[i].is_active = false;
            score += 100;
            break;
          }
        }        
      }
    }
  }
  
  for (uint8_t i = 0; i < MAX_ASTEROIDS; i++) {          
    if (asteroids[i].is_active && asteroids[i].type == ASTEROID) {
      if (player.x < asteroids[i].x + ASTEROID_W && player.x + PLAYER_W > asteroids[i].x &&
          player.y < asteroids[i].y + ASTEROID_H && player.y + PLAYER_H > asteroids[i].y)
      {
        player_hit = true;
        player.type = EXPLOSION;
        player.vx = player.vy = 0;
        asteroids[i].type = EXPLOSION;
        asteroids[i].vx = 0;
        break;
      }
    }
  }
}

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

void init_game() {
  init_starfield();
  init_objects(bullets, MAX_BULLETS);
  init_objects(asteroids, MAX_ASTEROIDS);
  change_state(INTRO);
  player_hit = false;
  player.x = 0;
  player.y = display.height() / 2;
  player.vx = player.vy = 2;
  player.type = PLAYER;
  player.is_active = true;
  player.anim_frame = 0;
  lives = MAX_LIVES;
  score = 0;
  asteroids_missed = 0;
}

int free_ram() {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
    
void setup()  {
  Serial.begin(9600);
  Serial.println(free_ram());
  init_joystick_shield();
  randomSeed(analogRead(A0));
  init_game();
  display.begin(SSD1306_SWITCHCAPVCC, I2C_ADDRESS_DISPLAY);
  display.display();
  delay(1000);
  display.clearDisplay();
}

void pause_game() {
  move_stars();
  draw_stars();
  draw_player();
  draw_objects(bullets, MAX_BULLETS);
  draw_objects(asteroids, MAX_ASTEROIDS);
  pmem_print_center(5, 2, PSTR("Pause"));
  pmem_print(0, 48, 1, PSTR("Lives: "));
  pmem_print(0, 56, 1, PSTR("Score: "));
  display.setCursor(42, 48);
  display.print(lives);
  display.setCursor(42, 56);
  display.print(score);
  if (bottom_button && (ticks - state_changed > MIN_DELAY)) {
    change_state(RUNNING);
    bottom_button = false;
    delay(200);
  }
}

void lost_live() {
  move_stars();
  draw_stars();
  draw_objects(bullets, MAX_BULLETS);
  draw_objects(asteroids, MAX_ASTEROIDS);
  draw_player();
  pmem_print_center(5, 2, PSTR("Don't"));
  pmem_print_center(25, 2, PSTR("Give"));
  pmem_print_center(45, 2, PSTR("Up!"));
  if (right_button && (ticks - state_changed > MIN_DELAY)) {
    change_state(RUNNING);
    player.vx = player.vy = 2;
    player.type = PLAYER;
    player.is_active = true;
    player.anim_frame = 0;
    right_button = false;
    delay(200);
  }
}

void print_score(const uint8_t y) {
  char tmp[MAX_SCORE_LEN];
  snprintf(tmp, MAX_SCORE_LEN, "%d", score);
  const uint8_t score_txt_len = 7;
  const uint8_t score_len = strlen(tmp);
  const uint8_t x = (display.width() - (score_txt_len + score_len) * BASE_FONT_W) / 2;
  pmem_print(x, y, 1, PSTR("Score: "));
  display.setCursor(x + score_txt_len * BASE_FONT_W, y);
  display.print(score);
}

void game_over() {
  move_stars();
  draw_stars();
  pmem_print_center(10, 2, PSTR("Game Over"));
  
  if (lives == 0) {
    pmem_print_center(30, 1, PSTR("You have lost"));
    pmem_print_center(40, 1, PSTR("all your lives."));
  } else {
    pmem_print_center(30, 1, PSTR("You have missed."));
    pmem_print_center(40, 1, PSTR("too many asteroids."));
  }
  
  print_score(56);
  
  if (right_button && (ticks - state_changed > MIN_DELAY)) {
    init_game();
    right_button = false;
    delay(200);
  }
}

void update_game() {
  if (bottom_button) {
    change_state(PAUSED);
    return;
  }
  if (player_hit) {
    player_hit = false;
    if (--lives == 0) {
      change_state(DONE);
    } else {
      change_state(LOST_LIVE);
    }    
    return;
  }
  if (asteroids_missed >= MAX_MISSES) {
      change_state(DONE);
      return;
  }
  if (right_button) {
    fire_bullet();
  }
  if (millis() - asteroid_started > 1000) {
    start_asteroid();
  }
  check_collisions();
  move_player();
  move_objects(bullets, MAX_BULLETS);
  move_objects(asteroids, MAX_ASTEROIDS);
  move_stars();
  draw_stars();
  draw_player();
  draw_objects(bullets, MAX_BULLETS);
  draw_objects(asteroids, MAX_ASTEROIDS);
}

void intro() {
  move_stars();
  draw_stars();
  pmem_print_center(10, 2, PSTR("Shootduino"));
  pmem_print_center(35, 1, PSTR("Press right button"));
  pmem_print_center(45, 1, PSTR("to start!"));
  if (right_button) {
    change_state(RUNNING);
    right_button = false;
    delay(200);
  }
}

void loop() {
  ticks = millis();
  update_control();
  display.clearDisplay();
  switch (state) {
    case INTRO:     intro();       break;
    case PAUSED:    pause_game();  break;
    case RUNNING:   update_game(); break;
    case LOST_LIVE: lost_live();   break;
    case DONE:      game_over();   break;
  }
  display.display();
}

