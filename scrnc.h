#ifndef SCRNC_H
#define SCRNC_H

/* term */

int s_confirm_term(char *, int def);

/* tui */

typedef struct
{
  char **options;
  int *sets;
  int length;
} s_ctx;

void s_show_menu(s_ctx *);

#endif // scrn_h
