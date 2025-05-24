#include "scrnc.h"

int
main(void)
{
  s_ctx ctx;
  ctx.options[0] = "foo";
  ctx.length = 1;
  s_show_menu(&ctx);
  return 0;
}
