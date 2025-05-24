#include "../scrnc.h"
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static void set_raw_mode(int enable)
{
  static struct termios old;
  struct termios new;

  if(enable)
    {
      tcgetattr(STDIN_FILENO, &old);
      new = old;
      new.c_lflag &= ~(ICANON | ECHO);
      tcsetattr(STDIN_FILENO, TCSANOW, &new);
    }
  else { tcsetattr(STDIN_FILENO, TCSANOW, &old); }
}

static void clear_screen() { write(STDOUT_FILENO, "\033[2J\033[H", 7); }

static void render_menu(s_ctx *ctx, int selected)
{
  clear_screen();
  printf("Navigate: j/k, ^N/^P, arrows | Toggle: space, t | Confirm: enter, "
         "^M\n\n");
  for(int i = 0; i < ctx->length; i++)
    {
      printf("%s [%c] %s\n", (i == selected ? "->" : "  "),
             ctx->sets[i] ? 'x' : ' ', ctx->options[i]);
    }
}

void s_show_menu(s_ctx *ctx)
{
  int selected = 0;
  char buf[4];

  set_raw_mode(1);

  while(1)
    {
      render_menu(ctx, selected);

      ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
      buf[n] = '\0';

      if(n == 1)
        {
          char c = buf[0];
          if(c == '\n' || c == '\r')
            break; // Enter or Ctrl-M
          else if(c == ' ')
            ctx->sets[selected] ^= 1; // Space = toggle
          else if(c == 'j')
            selected = (selected + 1) % ctx->length; // Vim down
          else if(c == 'k')
            selected = (selected - 1 + ctx->length) % ctx->length; // Vim up
          else if(c == 't')
            ctx->sets[selected] ^= 1; // Alt toggle
          else if(c == 14)
            selected = (selected + 1) % ctx->length; // Ctrl-N = down
          else if(c == 16)
            selected
              = (selected - 1 + ctx->length) % ctx->length; // Ctrl-P = up
        }
      else if(n == 3 && buf[0] == '\033' && buf[1] == '[')
        {
          if(buf[2] == 'A')
            selected = (selected - 1 + ctx->length) % ctx->length; // Arrow up
          else if(buf[2] == 'B')
            selected = (selected + 1) % ctx->length; // Arrow down
        }
    }

  set_raw_mode(0);
  clear_screen();
}
