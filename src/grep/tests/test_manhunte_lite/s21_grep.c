#define _GNU_SOURCE
#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flag {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int empty;
} flag;

void check_args(int argc, char **argv, flag *flags) {
  int i = 0;
  int errflag = 0;
  while ((i = getopt_long(argc, argv, "eivcln", NULL, NULL)) != -1) {
    switch (i) {
    case 'i':
      flags->i = 1;
      break;
    case 'e':
      flags->e = 1;
      break;
    case 'n':
      flags->n = 1;
      break;
    case 'l':
      flags->l = 1;
      break;
    case 'c':
      flags->c = 1;
      break;
    case 'v':
      flags->v = 1;
      break;
    default:
      errflag = 1;
      break;
    }
    if (errflag == 1) {
      printf("usage: grep [-eivcln]");
      break;
    }
  }
}

void how_many_files(int argc, char **argv, int *filesCount) {
  for (int i = optind + 1; i < argc; i++) {
    FILE *file;
    if (argv[i][0] == '-') {
      continue;
    } else if ((file = fopen(argv[i], "r")) != NULL) {
      *filesCount = *filesCount + 1;
    }
  }
}

void grep_action(int argc, char **argv, flag *flags, int filesCount) {
  regex_t regStruct;
  int errflag, coincidence, lineCount = 0, ltrue = 0, filenum = 0, lines = 0;
  FILE *file;
  size_t len;
  char *line = NULL;
  regmatch_t pm;
  if (flags->i) {
    errflag = regcomp(&regStruct, argv[optind], REG_ICASE);
  }
  if (!flags->i) {
    errflag = regcomp(&regStruct, argv[optind], REG_EXTENDED);
  }

  if (errflag != 0) {
    char errbuff[512];
    regerror(errflag, &regStruct, errbuff, sizeof(errbuff));
    for (long unsigned i = 0; i < sizeof(errbuff); i++) {
      printf("%c", errbuff[i]);
    }
  }

  for (int i = optind + 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      continue;
    } else if ((file = fopen(argv[i], "r")) == NULL) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
      exit(0);
    } else {
      filenum++;
      while (getline(&line, &len, file) != EOF) {
        coincidence = regexec(&regStruct, line, 0, &pm, 0);
        lines++;
        if (!flags->v) {
          if (!coincidence) {
            lineCount++;
            if (flags->c) {
              if (flags->l)
                ltrue = 1;
              continue;
            } else {
              if (flags->l) {
                printf("%s\n", argv[i]);
                break;
              } else if (flags->n) {
                if (filesCount > 1)
                  printf("%s:%d:%s", argv[i], lines, line);
                else
                  printf("%d:%s", lines, line);
              } else {
                if (filesCount > 1)
                  printf("%s:%s", argv[i], line);
                else
                  printf("%s", line);
              }
            }
          }
        }
        if (flags->v) {
          if (coincidence) {
            lineCount++;
            if (flags->c) {
              if (flags->l)
                ltrue = 1;
              continue;

            } else {
              if (flags->l) {
                printf("%s\n", argv[i]);
                break;
              } else if (flags->n) {
                if (filesCount > 1)
                  printf("%s:%d:%s", argv[i], lines, line);
                else
                  printf("%d:%s", lines, line);
              } else {
                if (filesCount > 1)
                  printf("%s:%s", argv[i], line);
                else
                  printf("%s", line);
              }
            }
          }
        }
      }
      if (flags->c && flags->l && ltrue) {
        if (filesCount > 1 && lineCount > 0) {
          printf("%s:%d\n", argv[i], 1);
          printf("%s\n", argv[i]);
        } else if (filesCount > 1 && lineCount == 0) {
          printf("%s:%d\n", argv[i], 0);
        } else {
          printf("1\n%s\n", argv[i]);
        }
      } else if (flags->c) {
        if (filesCount > 1) {
          printf("%s:%d\n", argv[i], lineCount);
        } else {
          printf("%d\n", lineCount);
        }
      }
    }
    lineCount = 0;
    fclose(file);
  }
  if (line) {
    free(line);
  }
  regfree(&regStruct);
}

int main(int argc, char **argv) {
  flag flags = {0};
  int filesCount = 0;
  check_args(argc, argv, &flags);
  how_many_files(argc, argv, &filesCount);
  grep_action(argc, argv, &flags, filesCount);
}
