#include "../scrnc.h"
#include <stdlib.h>
#include <stdio.h>

#define printfn(x) printf(x "\n");

void help() {
  printfn("scrnc interface:");

  printfn("\nInteractive shell:");
  printfn("s_confirm_term\tConfirm what the user wants to do.");

  printfn("\nInteractive TUI");
  printfn("s_show_menu\tTakes a context and prompts the user for configuration.");

  exit(1);
}
