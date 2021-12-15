#pragma once

#include <initializer_list>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <stdarg.h>
#include "keys.h"
#include "common.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::tuple;
using std::vector;
using std::ofstream;
using std::ifstream;
using std::ios;


const int TAB_STOP = 8;

class Editor {
private:			// for basic
  int screenrows;
  int screencols;
  int cursorx;
  int cursory;
  int renderx;

private:			// for read file
  vector<string> rows;
  int rowoffset;
  int coloffset;

private:			// for status
  string filename;
  string status;
  time_t status_time;

private:
  int windowsize();
  void enableraw();
  int cxtorx(string & chars);

public:
  Editor(); 			/* this is for initialize */
  ~Editor();

public:
  int readkey();
  void processkey(int key);
  void open(const char * filename);

public:
  void refreshScreen();
  void drawrows(string & appendbuf);
  void drawStatusBar(string & appendbuf);
  void drawMessageBar(string & appendbuf);
  void setStatus(const char * fmt, ...);
  void scroll();
  string updaterow(string & chars);
private:
  void moveleft();
  void moveright();
  void moveup();
  void movedown();
  void homekey();
  void endkey();
  void pageup();
  void pagedown();
};

