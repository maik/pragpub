#include "joystick.h"

int resting_pos_x;
int resting_pos_y;
boolean left, right, up, down;
boolean joy_button, top_button, bottom_button, left_button, right_button;

void init_joystick_shield() {
  init_joystick_shield_button(JOY_BUTTON);
  init_joystick_shield_button(TOP_BUTTON);
  init_joystick_shield_button(LEFT_BUTTON);
  init_joystick_shield_button(RIGHT_BUTTON);
  init_joystick_shield_button(BOTTOM_BUTTON);
  for (uint8_t i = 0; i < 10; i++) {
    resting_pos_x = analogRead(A0);
    resting_pos_y = analogRead(A1);
  }
}

void update_control() {
  left = analogRead(A0) < resting_pos_x;
  right = analogRead(A0) > resting_pos_x;
  up = analogRead(A1) > resting_pos_y;
  down = analogRead(A1) < resting_pos_y;
  joy_button = digitalRead(JOY_BUTTON) == 0;
  top_button = digitalRead(TOP_BUTTON) == 0;
  bottom_button = digitalRead(BOTTOM_BUTTON) == 0;
  left_button = digitalRead(LEFT_BUTTON) == 0;
  right_button = digitalRead(RIGHT_BUTTON) == 0;
}

void init_joystick_shield_button(const uint8_t pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

