#include "../scrnc.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

static struct termios orig_termios;

void disable_raw_mode(void);
void enable_raw_mode(void);
void clear_screen(void);

void s_show_menu(s_ctx *ctx) {
  enable_raw_mode();

  int selected = 0;

  while (1) {
    clear_screen();
    printf("== Config Menu ==\nUse ↑/↓ or w/s to navigate, space to toggle, "
           "enter to confirm.\n\n");

    for (int i = 0; i < ctx->length; ++i) {
      char check = ctx->sets[i] ? 'x' : ' ';
      printf("%c [%c] %s\n", (i == selected ? '>' : ' '), check,
             ctx->options[i]);
    }

    char c = getchar();
    if (c == '\033') { // escape sequence
      getchar();       // skip [
      switch (getchar()) {
      case 'A':
        selected = (selected - 1 + ctx->length) % ctx->length;
        break; // up
      case 'B':
        selected = (selected + 1) % ctx->length;
        break; // down
      }
    } else if (c == 'w') {
      selected = (selected - 1 + ctx->length) % ctx->length;
    } else if (c == 's') {
      selected = (selected + 1) % ctx->length;
    } else if (c == ' ') {
      ctx->sets[selected] ^= 1;
    } else if (c == '\r' || c == '\n') {
      break;
    }
  }

  disable_raw_mode();
}

void disable_raw_mode(void) { tcsetattr(1, TCSAFLUSH, &orig_termios); }

void enable_raw_mode(void) {
  tcgetattr(1, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ICANON | ECHO);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;
  tcsetattr(1, TCSAFLUSH, &raw);
}

void clear_screen(void) { write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7); }
