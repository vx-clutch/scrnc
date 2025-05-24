#include "../scrnc.h"
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define ESC "\033"
#define CSI "\033["

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
  else
    {
      tcsetattr(STDIN_FILENO, TCSANOW, &old);
    }
}

static void clear_screen() { write(STDOUT_FILENO, "\033[2J\033[H", 7); }

static void move_cursor(int row, int col)
{
  printf(CSI "%d;%dH", row, col);
}

static void render_menu(s_ctx *ctx, int selected)
{
  clear_screen();
  int menu_width = 50;
  int menu_height = ctx->length + 4;

  // Draw top border
  printf("┌");
  for(int i = 0; i < menu_width - 2; i++) putchar('─');
  printf("┐\n");

  // Title bar
  printf("│ %-*s │\n", menu_width - 2, "Configuration");

  // Divider
  printf("├");
  for(int i = 0; i < menu_width - 2; i++) putchar('─');
  printf("┤\n");

  // Options
  for(int i = 0; i < ctx->length; i++)
    {
      char line[menu_width + 1];
      snprintf(line, sizeof(line), "[%c] %s", ctx->sets[i] ? '*' : ' ', ctx->options[i]);

      // Truncate or pad line
      line[menu_width - 4] = '\0';

      printf("│ ");
      if(i == selected)
        {
          printf(ESC "[7m%-*s" ESC "[0m", menu_width - 4, line); // Reverse video
        }
      else
        {
          printf("%-*s", menu_width - 4, line);
        }
      printf(" │\n");
    }

  // Bottom border
  printf("└");
  for(int i = 0; i < menu_width - 2; i++) putchar('─');
  printf("┘\n");

  // Footer
  printf("\nUse ↑/↓ or j/k to navigate, Space/t to toggle, Enter to confirm\n");
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
            break;
          else if(c == ' ' || c == 't')
            ctx->sets[selected] ^= 1;
          else if(c == 'j' || c == 14)
            selected = (selected + 1) % ctx->length;
          else if(c == 'k' || c == 16)
            selected = (selected - 1 + ctx->length) % ctx->length;
        }
      else if(n == 3 && buf[0] == '\033' && buf[1] == '[')
        {
          if(buf[2] == 'A') // up
            selected = (selected - 1 + ctx->length) % ctx->length;
          else if(buf[2] == 'B') // down
            selected = (selected + 1) % ctx->length;
        }
    }

  set_raw_mode(0);
  clear_screen();
}
