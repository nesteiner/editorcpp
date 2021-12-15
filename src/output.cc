#include "editor.h"
#include <string.h>

void Editor::refreshScreen() {
  scroll();

  string appendbuf = "";
  appendbuf += "\e[?25l";
  appendbuf += "\e[H";
  drawrows(appendbuf);
  drawStatusBar(appendbuf);
  drawMessageBar(appendbuf);

  stringstream ss;
  // ss << "\e[" << cursory + 1 << ";" << cursorx + 1 << "H";
  ss << "\e["
     << cursory - rowoffset + 1
     << ";"
     << renderx - coloffset + 1
     << "H";
  appendbuf += ss.str();
  appendbuf += "\e[?25h";
  cout << appendbuf << std::flush;
}

void Editor::drawrows(string &appendbuf) {
  for (int y = 0; y < screenrows; y += 1) {
    int filerow = y + rowoffset;
    if(y < rows.size()) {
      string thisrow = updaterow(rows[filerow]);
      int len = thisrow.length() - coloffset;
      if(len < 0) len = 0;
      if(len > screencols) len = screencols;

      appendbuf += thisrow.substr(coloffset, len);
    }

    // appendbuf += "~";
    appendbuf += "\e[K";
    appendbuf += "\r\n";
  }
}

void Editor::moveleft() {
  if (cursorx != 0) {
    cursorx -= 1;
  } else if(cursory > 0) {
    cursory -= 1;
    cursorx = rows[cursory].length();
  }
}

void Editor::moveright() {
  string row = (cursory >= rows.size()) ? "" : rows[cursory];
  if(row.length() != 0 && cursorx < row.length()) {
    cursorx += 1;
  } else if(row.length() != 0 && cursorx == row.length()) {
    cursory += 1;
    cursorx = 0;
  } else if(cursory < row.length()) {
    cursory += 1;
    cursorx = 0;
  }

  // const char * row = (cursory >= rows.size()) ? nullptr : rows[cursory].data();
  // if(row != nullptr && cursorx < strlen(row)) {
  //   cursorx += 1;
  // } else if(row != nullptr && cursorx == strlen(row)) {
  //   cursory += 1;
  //   cursorx = 0;
  // }
}

void Editor::moveup() {
  if (cursory != 0) {
    cursory -= 1;
  }
}

void Editor::movedown() {
  if (cursory < rows.size()) {
    cursory += 1;
  }
}

void Editor::homekey() { cursorx = 0; }

void Editor::endkey() {
  if(cursory < rows.size()) {
    cursorx = rows[cursory].length();
  }
}

void Editor::pageup() {
  cursory = rowoffset;

  int times = screenrows;
  while (times != 0) {
    moveup();
    times -= 1;
  }
}
void Editor::pagedown() {
  cursory = rowoffset + screenrows - 1;
  int times = screenrows;
  while (times != 0) {
    movedown();
    times -= 1;
  }
}

void Editor::scroll() {
  renderx = 0;
  if(cursory < rows.size()) {
    renderx = cxtorx(rows[cursory]);
  }

  if(cursory < rowoffset) {
    rowoffset = cursory;
  }

  if(cursory >= rowoffset + screenrows) {
    rowoffset = cursory - screenrows + 1;
  }

  if(renderx < coloffset) {
    coloffset = renderx;
  }

  if(renderx >= coloffset + screencols) {
    coloffset = renderx - screencols + 1;
  }
}

string Editor::updaterow(string & chars) {
  string render = "";

  int tabs = 0;
  for(int j = 0; j < chars.length(); j += 1) {
    if(chars[j] == '\t') {
      tabs += 1;
    }
  }

  int idx = 0;
  for(int j = 0; j < chars.length(); j += 1) {
    if(chars[j] == '\t') {
      render += ' ';
      idx += 1;
      while(idx % 8 != 0) {
	render += ' ';
	idx += 1;
      }
    } else {
      render += chars[j];
      idx += 1;
    }
  }

  return render;
}

int Editor::cxtorx(string & chars) {
  int rx = 0;
  for(int j = 0; j < cursorx; j += 1) {
    if(chars[j] == '\t') {
      rx += (TAB_STOP - 1) - (rx % TAB_STOP);
    }

    rx += 1;
  }

  return rx;
}

void Editor::drawStatusBar(string & appendbuf) {
  appendbuf += "\e[7m";

  char status[80], rstatus[80];
  int len = snprintf(status, sizeof(status),
		     "%.20s - %d lines",
		     filename != "" ? filename.data() : "[No Name]",
		     rows.size());

  int rlen = snprintf(rstatus, sizeof(rstatus),
		      "%d/%d",
		      cursory + 1,
		      rows.size());

  if(len > screencols) len = screencols;

  appendbuf += status;

  while(len < screencols) {
    if(screencols - len == rlen) {
      appendbuf += rstatus;
      break;
    } else {
      appendbuf += " ";
      len += 1;
    }
  }

  appendbuf += "\e[m";
}

void Editor::setStatus(const char * fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buffer[80];
  vsnprintf(buffer, sizeof(buffer), fmt, ap);
  va_end(ap);

  status = buffer;
  status_time = time(NULL);
}

void Editor::drawMessageBar(string & appendbuf) {
  appendbuf += "\e[K";
  int msglen = status.length();
  if(msglen > screencols)
    msglen = screencols;

  if(msglen != 0 && time(NULL) - status_time < 5)
    appendbuf += status;
}