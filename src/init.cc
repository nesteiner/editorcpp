#include "editor.h"
void Editor::disableraw() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &originTermios); }

Editor::Editor() {
  cursorx = cursory = renderx = 0;
  screencols = screenrows = 0;
  rowoffset = coloffset = 0;
  filename = "";
  status = "";
  status_time = 0;

  enableraw();
  windowsize();
  screenrows -= 2;
}

Editor::~Editor() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &originTermios);
  cout << "\e[2J" << "\e[H" << std::flush;
}

int Editor::windowsize() {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    screencols = ws.ws_col;
    screenrows = ws.ws_row;
    return 0;
  }
}

void Editor::enableraw() {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &originTermios);

  atexit(Editor::disableraw);
  // atexit(Editor::~Editor);
  raw = originTermios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


