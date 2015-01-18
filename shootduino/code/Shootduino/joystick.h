#ifndef __SHOOTER_JOYSTICK__
#define __SHOOTER_JOYSTICK__

#include <Arduino.h>

const uint8_t JOY_BUTTON = 2;
const uint8_t LEFT_BUTTON = 6;
const uint8_t TOP_BUTTON = 4;
const uint8_t RIGHT_BUTTON = 3;
const uint8_t BOTTOM_BUTTON = 5;

extern int resting_pos_x;
extern int resting_pos_y;
extern boolean left, right, up, down;
extern boolean joy_button, top_button, bottom_button, left_button, right_button;

void init_joystick_shield();
void update_control();
void init_joystick_shield_button(const uint8_t pin);

#endif

