#define _GNU_SOURCE
#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct flag {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flag;

int check_args(int argc, char **argv, flag *flags) {
  int i;
  int errflag = 0;
  while ((i = getopt_long(argc, argv, "beEnstTv", NULL, NULL)) != -1) {
    switch (i) {
      case 'b':
        flags->b = flags->n = 1;
        break;
      case 'e':
        flags->e = flags->v = 1;
        break;
      case 'E':
        flags->e = 1;
        break;
      case 'n':
        flags->n = 1;
        break;
      case 's':
        flags->s = 1;
        break;
      case 't':
        flags->t = flags->v = 1;
        break;
      case 'T':
        flags->t = 1;
        break;
      case 'v':
        flags->v = 1;
        break;
      case 0:
        break;
      default:
        errflag = 1;
        break;
    }
    if (errflag == 1) {
      printf("usage: cat [-benstuv] [file ...]\n");
      break;
    }
  }
  return errflag;
}

void cat_action(char **argv, int argc, flag *flags) {
  char ch, prev;
  int lineIsEmpty = 0;
  int lineNum = 0;
  for (int i = optind; i < argc; i++) {
    FILE *file;
    if ((file = fopen(argv[i], "r")) == NULL) {
      printf("cat: %s No such file or directory ", argv[i]);
      exit(0);
    } else {
      for (prev = '\n'; (ch = getc(file)) != EOF; prev = ch) {
        if (prev == '\n') {
          if (flags->s) {
            if (ch == '\n') {
              if (lineIsEmpty) continue;
              lineIsEmpty = 1;
            } else {
              lineIsEmpty = 0;
            }
          }
          if ((!(flags->b) || ch != '\n') && flags->n) {
            printf("%6.d\t", ++lineNum);
          }
        }
        if (ch == '\n' && flags->e) {
          printf("%c", '$');
        } else if (ch == '\t' && flags->t) {
          printf("^I");
          continue;
        } else if (flags->v) {
          if (ch < 9 || (ch > 10 && ch <= 31)) {
            ch += 64;
            printf("^");
          } else if (ch == 127) {
            ch -= 64;
            printf("^");
          }
          if (!isascii(ch)) {
            printf("M-");
            ch = toascii(ch);
          }
        }
        printf("%c", ch);
      }
    }
  }
}

int main(int argc, char **argv) {
  flag flags = {0};
  if (check_args(argc, argv, &flags) == 1) exit(0);
  cat_action(argv, argc, &flags);
}
