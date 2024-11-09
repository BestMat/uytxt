// ©2024 - UYtxt - BestMat, Inc. - All rights reserved.
// --------------------------------------------
// ██    ██ ██    ██ ████████ ██   ██ ████████ 
// ██    ██  ██  ██     ██     ██ ██     ██    
// ██    ██   ████      ██      ███      ██    
// ██    ██    ██       ██     ██ ██     ██    
//  ██████     ██       ██    ██   ██    ██    
// --------------------------------------------

#include <stdio.h>
#include <curses.h>
#include <stdlib.h>

#define ctrl(x) ((x) & 0x1f)
#define MAC_LINUX_BACKSPACE     127
#define LINUX_DEBIAN_BACKSPACE  263
#define WIN_BACKSPACE           8
#define ESCAPE                  27
#define ENTER                   10 

typedef enum {
  NORMAL,
  INSERT,
} Mode;

typedef struct {
  size_t index;
  size_t size;
  char *contents;
} Row;

typedef struct {
  char *buf;
  Row rows[1024];
  size_t row_index;
  size_t cur_pos;
  size_t row_s;
} Buffer;

Mode mode = NORMAL;
int QUIT = 0;

char *stringifyMode() {
  switch (mode) {

    case NORMAL:
      return "UVtxt: Normal Mode";
      break;

    case INSERT:
      return "UVtxt: Insert Mode";
      break;

    default:
      return "Normal Mode";
      break;

  }
}

int main (void) {
  printf("Nagapillaiyar");

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  Buffer buffer = {0};

  for (size_t i = 0; i < 1024; i++) {
    buffer.rows[i].contents = calloc(1024, sizeof(char)); 
  }

  int row, col;
  (void)col;

  getmaxyx(stdscr, row, col);

  mvprintw(row - 1, 0, stringifyMode(mode));
  move(0, 0);

  int ch = 0;
  int x, y = 0;

  while (ch != ctrl('q') && QUIT != 1) {
    refresh();
    mvprintw(row - 1, 0, stringifyMode(mode));
    for (size_t i = 0; i <= buffer.row_s; i++) {
      mvprintw(i, 0, buffer.rows[i].contents);
    }
    move(y, x);

    ch = getch();

    switch (mode) {
  if (x > buffer.rows[buffer.row_index].size - 1) x = buffer.rows[buffer.row_index].size - 1;

      case NORMAL: {
        if (ch == 'i') {
          mode = INSERT;
        } else if (ch == 'l') { 
          move(y, x + 1);
          buffer.cur_pos += 1;
        } else if (ch == 'h') {
          move(y, x - 1);
          buffer.cur_pos -= 1;
        } else if (ch == 'j') {
          move(y + 1, x);
          buffer.row_index++;
        } else if (ch == 'k') {
          move(y - 1, x);
          buffer.row_index--;
        } else if (ch == ctrl('s')) {
          FILE *file = fopen("out.txt", "w");
          
          for (size_t i = 0; i <= buffer.row_s; i++) {
            fwrite(buffer.rows[i].contents, buffer.rows[i].size, 1, file);
          }

          fclose(file);

          QUIT = 1;
        }
        
        break;
      }

      case INSERT: {
        keypad(stdscr, FALSE);

        if (
          ch == MAC_LINUX_BACKSPACE || 
          ch == WIN_BACKSPACE ||
          ch == LINUX_DEBIAN_BACKSPACE
        ) {
          getyx(stdscr, y, x);

          if (buffer.cur_pos == 0) {
            //if (buffer.row_index != 0) {
              Row *cur = &buffer.rows[--buffer.row_index];
              buffer.cur_pos = cur->size;
              move(buffer.row_index, buffer.cur_pos);
            //}
          } else {
            Row *cur = &buffer.rows[buffer.row_index];
            cur->contents[--buffer.cur_pos] = ' ';
            cur->size = buffer.cur_pos;
            move(y, buffer.cur_pos);
          }
        } else if (ch == ESCAPE) {
          mode = NORMAL;
          keypad(stdscr, TRUE);
        } else if (ch == ENTER) {
          buffer.rows[buffer.row_index].contents[buffer.rows[buffer.row_index].size++] = '\n';
          buffer.row_index ++; 
          buffer.row_s++;
          buffer.cur_pos = 0;
          move(buffer.row_index, buffer.cur_pos);
        } else {
          Row *cur = &buffer.rows[buffer.row_index];
          cur->contents[buffer.cur_pos++] = ch;
          cur->size = buffer.cur_pos;
          move(y, buffer.cur_pos);
        }

        break;
      }
    }

    getyx(stdscr, y, x);
  }

  refresh();
  endwin();

  return 0;
}


// --------------------------------------------
// ██    ██ ██    ██ ████████ ██   ██ ████████ 
// ██    ██  ██  ██     ██     ██ ██     ██    
// ██    ██   ████      ██      ███      ██    
// ██    ██    ██       ██     ██ ██     ██    
//  ██████     ██       ██    ██   ██    ██    
// --------------------------------------------
// ©2024 - UYtxt - BestMat, Inc. - All rights reserved.
