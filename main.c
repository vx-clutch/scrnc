#include "scrnc.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  s_ctx ctx;
  ctx.length = 3;

  ctx.options = malloc(ctx.length * sizeof(char *));
  ctx.sets = calloc(ctx.length, sizeof(int));

  ctx.options[0] = "Enable Logging";
  ctx.options[1] = "Use Experimental Mode";
  ctx.options[2] = "Verbose Output";

  s_show_menu(&ctx);

  // Debug: show final selections
  puts("debug: conf: ");
  for(int i = 0; i < ctx.length; i++)
    {
      printf("[%c] %s\n", ctx.sets[i] ? 'x' : ' ', ctx.options[i]);
    }

  free(ctx.options);
  free(ctx.sets);
  return 0;
}
