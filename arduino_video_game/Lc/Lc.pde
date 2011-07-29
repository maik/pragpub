#include <Wire.h>
#include <stdint.h>
#include <TVout.h>
#include <fontALL.h>
#include "nunchuk.h"

class Player {
  public:
  enum Direction {
    NORTH, WEST, EAST, SOUTH
  };

  enum Behavior {
    MANUALLY, PREFERRED_ORDER
  };

  uint8_t   px, py;
  Direction direction;
  Behavior  behavior;

  void move(void) {
    switch (direction) {
      case NORTH: py -= 1; break;
      case WEST:  px -= 1; break;
      case EAST:  px += 1; break;
      case SOUTH: py += 1; break;
    }
  }

  bool draw(TVout& tv) {
    if (tv.get_pixel(px, py) == 1)
      return true;
    tv.set_pixel(px, py, 1);
    return false;
  }

  void control(TVout& tv, const Nunchuk& controller) {
    switch (behavior) {
      case MANUALLY:
        control_manually(controller);
        break;
      case PREFERRED_ORDER:
        control_preferred_order(tv);
        break;
    }
  }

  private:

  void control_manually(const Nunchuk& controller) {
    if (controller.left()) {
      direction = WEST;
    } else if (controller.right()) {
      direction = EAST;
    } else if (controller.up()) {
      direction = NORTH;
    } else if (controller.down()) {
      direction = SOUTH;
    }
  }

  void control_preferred_order(TVout& tv) {
    if (is_passable(tv, NORTH))
      direction = NORTH;
    else if (is_passable(tv, WEST))
      direction = WEST;
    else if (is_passable(tv, EAST))
      direction = EAST;
    else if (is_passable(tv, SOUTH))
      direction = SOUTH;
    else
      direction = NORTH;
  }

  bool is_passable(TVout& tv, const uint8_t direction) {
    switch (direction) {
      case NORTH: return (tv.get_pixel(px, py - 1) == 0);
      case WEST:  return (tv.get_pixel(px - 1, py) == 0);
      case EAST:  return (tv.get_pixel(px + 1, py) == 0);
      case SOUTH: return (tv.get_pixel(px, py + 1) == 0);
    }
  }
};

class Game {
  public:  
  static const uint8_t SCREEN_WIDTH = 128;
  static const uint8_t SCREEN_HEIGHT = 96;
  static const uint8_t FONT_HEIGHT = 6;

  enum GameState {
    INTRO, STARTING, RUNNING, PAUSED, DONE
  };

  void initialize() {
    reset_game();
    _tv.begin(PAL, SCREEN_WIDTH, SCREEN_HEIGHT);
    _tv.select_font(font4x6);
    _controller.initialize();
  }
  
  void reset_game(void) {
    _game_state = INTRO;
    _player1.px = SCREEN_WIDTH / 2 - 4;
    _player1.py = SCREEN_HEIGHT / 2;
    _player1.direction = Player::NORTH;
    _player1.behavior = Player::MANUALLY;
    _player2.px = SCREEN_WIDTH / 2 + 4;
    _player2.py = SCREEN_HEIGHT / 2;
    _player2.direction = Player::NORTH;
    _player2.behavior = Player::PREFERRED_ORDER;
  }

  void intro(void) {
    reset_game();
    _tv.print(0, 20, "    Arduino Light Cycle Race");
    _tv.print(0, 46, "        by  Maik Schmidt");
    _tv.print(0, 72, "     Press Button to Start");
    if (_controller.c_button()) {
      _game_state = STARTING;
      _tv.clear_screen();
      _tv.draw_rect(
        0, 0, 
        SCREEN_WIDTH - 1, SCREEN_HEIGHT - 8,
        1
      );
      delay(150);
    }
  }

  void pause(void) {
    if (_controller.c_button()) {
      _game_state = RUNNING;
      print_message("       ");
      delay(150);
    }
  }

  void done(void) {
    if (_controller.c_button()) {
      _game_state = INTRO;
      _tv.clear_screen();
      delay(150);
    }
  }

  void start(void) {
    _player1.draw(_tv);
    _player2.draw(_tv);
    print_message("3");
    delay(1000);
    print_message("2");
    delay(1000);
    print_message("1");
    delay(1000);
    print_message("Go!");
    delay(1000);
    print_message("   ");
    _game_state = RUNNING;
  }

  void play(void) {
    _player1.control(_tv, _controller);
    _player1.move();
    const bool player1_hit = _player1.draw(_tv);
    _player2.control(_tv, _controller);
    _player2.move();
    const bool player2_hit = _player2.draw(_tv);
    if (player1_hit && player2_hit) {
      _game_state = DONE;
      print_message("Tie Game");
      delay(1000);
    } else if (player1_hit) {
      _game_state = DONE;
      print_message("You Lose!");
      delay(1000);
    } else if (player2_hit) {
      _game_state = DONE;
      print_message("You Win!");
      delay(1000);
    }
    if (_controller.c_button()) {
      _game_state = PAUSED;
      print_message("Paused");
      delay(150);
    }
  }

  void run(void) {
    _controller.update();
    switch (_game_state) {
      case INTRO:    intro(); break;
      case PAUSED:   pause(); break;
      case DONE:     done();  break;
      case STARTING: start(); break;
      default:       play();  break;
    }
    _tv.delay_frame(1);
  }

  private:

  void print_message(const char* message) {
    _tv.print(
      0,
      SCREEN_HEIGHT - FONT_HEIGHT,
      message
    );
  }

  GameState _game_state;
  TVout     _tv;
  Nunchuk   _controller;
  Player    _player1, _player2;
};

Game game;

void setup() {
  game.initialize();
}

void loop() {
  game.run();
}
