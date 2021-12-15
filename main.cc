#include "editor.h"

int main(int argc, char * argv[]) {
  Editor editor;
  
  if(argc >= 2) {
    editor.open(argv[1]);
  }

  editor.setStatus("HELP: Ctrl-Q = quit");

  while (true) {
    editor.refreshScreen();
    int key = editor.readkey();
    editor.processkey(key);
  }

  return 0;
}