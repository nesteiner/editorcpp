#include "editor.h"

int Editor::readkey() {
  int nread;
  char c;
  while (true) {
    nread = read(STDIN_FILENO, &c, 1);
    // nread = cin >> c;
    if (nread != 1) {
      if (nread == -1 && errno != EAGAIN) {
        perror("read");
        exit(1);
      } else {
        continue;
      }
    } else {
      break;
    }
  }

  if (c == '\e') {
    char seq[3];

    if (read(STDIN_FILENO, &seq[0], 1) != 1)
      return '\e';
    if (read(STDIN_FILENO, &seq[1], 1) != 1)
      return '\e';

    if (seq[0] == '[') {
      if (seq[1] >= '0' && seq[1] <= '9') {
        if (read(STDIN_FILENO, &seq[2], 1) != 1)
          return '\e';
        if (seq[2] == '~') {
          switch (seq[1]) {
          case '1':
            return HOME_KEY;
          case '4':
            return END_KEY;
          case '5':
            return PAGE_UP;
          case '6':
            return PAGE_DOWN;
          case '7':
            return HOME_KEY;
          case '8':
            return END_KEY;
          }
        }
      } else {
        switch (seq[1]) {
        case 'A':
          return ARROW_UP;
        case 'B':
          return ARROW_DOWN;
        case 'C':
          return ARROW_RIGHT;
        case 'D':
          return ARROW_LEFT;
        case 'H':
          return HOME_KEY;
        case 'F':
          return END_KEY;
        }
      }
    } else if (seq[0] == 'O') {
      switch (seq[1]) {
      case 'H':
        return HOME_KEY;
      case 'F':
        return END_KEY;
      }
    }

    return '\e';
  } else {
    return c;
  }
}

void Editor::processkey(int key) {
  switch (key) {
  case CTRL_KEY('q'):
    cout << "\e[2J" << std::flush;
    cout << "\e[H" << std::flush;

    exit(0);
    break;

  case HOME_KEY:
    homekey();
    break;
  case END_KEY:
    endkey();
    break;
  case PAGE_UP:
    pageup();
    break;
  case PAGE_DOWN:
    pagedown();
    break;

  case ARROW_UP:
    moveup();
    break;
  case ARROW_DOWN:
    movedown();
    break;
  case ARROW_LEFT:
    moveleft();
    break;
  case ARROW_RIGHT:
    moveright();
    break;
  }

  string row = (cursory >= rows.size()) ? "" : rows[cursory];
  int rowlen = row.length();
  if(cursorx > rowlen) {
    cursorx = rowlen;
  }
}

void Editor::open(const char * path) {
  filename = path;
  ifstream is(filename, ios::in);

  string buffer;
  std::getline(is, buffer);
  
  while(is) {
    rows.push_back(buffer);
    std::getline(is, buffer);
  }

  is.close();
  // STUB
  // string buffer, render;
  // std::getline(is, buffer);

  // while(is) {
  //   render = updaterow(buffer);
  //   rows.push_back(render);
  //   std::getline(is, buffer);
  // }
  
}