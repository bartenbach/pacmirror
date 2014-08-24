/* pacmirror - uses reflector to update your pacman mirrorlist automagically!
     Copyright (c) 2014  Blake Bartenbach
     pacmirror.c
     version: 0.0.1   
*/

/* glibc */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* macros */
#ifndef PACMAN_MIRRORLIST
    #define PACMAN_MIRRORLIST    "/etc/pacman.d/mirrorlist"
#endif

#define PACMIRROR_VERSION        "0.0.2"

#define RED                      "\033[0;31m"
#define GREEN                    "\033[0;32m"

/* function prototypes */
static int parse_options(int, char*[]);
static void backup_mirrorlist();
static void sort_speed();
static void show_version();

main(int argc, char *argv[]) {
  parse_options(argc, argv);
  exit(0);
}

int parse_options(int argc, char *argv[]) {
  int opt, option_index = 0;

  static const struct option opts[] = {
    {"version",  no_argument,  0, 'V'},
    {"update",   no_argument,  0, 'u'},
    {0, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "Vu", opts, &option_index)) != -1) {
    switch (opt) {
      case 'V':
        show_version();
        exit(0);
        break; 
      case 'u':
        backup_mirrorlist();
        sort_speed();
        break;
      default:
        break;
    } 
  }
}

void backup_mirrorlist(){
  int ret = system("cp /etc/pacman.d/mirrorlist /etc/pacman.d/mirrorlist.bak &> /dev/null");
  if (ret != 0) {
    printf("error: failed to backup mirrorlist...are you root?\n");
    exit(1);
  }
}

void sort_speed() {
  int ret = system("type reflector &> /dev/null");
  if (ret != 0) {
    printf("error: pacmirror requires that reflector be installed.\n");
    exit(1);
  }
  ret = system("reflector --verbose -l 5 --sort rate --save /etc/pacman.d/mirrorlist &> /dev/null");
  if (ret != 0) {
    printf("error: failed to sort mirrors with reflector\n");
    exit(1);
  } else {
    printf("Mirrors updated successfully!\n");
    system("cat /etc/pacman.d/mirrorlist");
  }
}

void show_version() {
  printf("pacmirror version %s\n", PACMIRROR_VERSION);
  system("repo-elephant");
  printf("  by blake bartenbach\n");
}
