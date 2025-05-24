#include "../scrnc.h"
#include <assert.h>
#include <stdio.h>

int s_confirm_term(char *src, int def)
{
  if(def)
    {
      printf("%s (Y/n)", src);
      char ch = getchar();
      if(ch == 'n' || ch == 'N') return 0;
      return 1;
    }
  printf("%s (y/N)", src);
  char ch = getchar();
  if(ch == 'y' || ch == 'Y') return 0;
  return 0;
}
