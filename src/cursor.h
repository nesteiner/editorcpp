#pragma once
#include "common.h"
enum Key {
  ARROW_LEFT = 1000,
  ARROW_RIGHT,
  ARROW_UP,
  ARROW_DOWN,
  HOME_KEY,
  END_KEY,
  PAGE_UP,
  PAGE_DOWN
};

class Cursor {
private:
  int x = 0;
  int y = 0;

public:
  void display();
  void hidden();
  void moveto(int rows, int cols);
  void move(Key key, int n);

public:
  void left(int n);
  void right(int n);
  void up(int n);
  void down(int n);
};