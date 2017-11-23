/* pacmirror - uses reflector to update your pacman mirrorlist automagically!
     Copyright (c) 2014-2016  Blake Bartenbach
     pacmirror.c
     version: 0.0.2 
*/

/* glibc */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

/* macros */
#ifndef PACMAN_MIRRORLIST
    #define PACMAN_MIRRORLIST    "/etc/pacman.d/mirrorlist"
#endif

#define PACMIRROR_VERSION        "0.0.3"

#define RED                      "\033[0;31m"
#define GREEN                    "\033[0;32m"

/* function prototypes */
static int parse_options(int, char*[]);
static void backup_mirrorlist();
static void check_reflector();
static void sort_speed();
static void show_version();
static void show_help();

int main(int argc, char *argv[]) {
  parse_options(argc, argv);
  exit(0);
}

int parse_options(int argc, char *argv[]) {
  int opt, option_index = 0;

  static const struct option opts[] = {
    {"version",  no_argument,  0, 'V'},
    {"update",   no_argument,  0, 'u'},
    {"help",     no_argument,  0, 'h'},
    {0, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "Vuh", opts, &option_index)) != -1) {
    switch (opt) {
      case 'V':
        show_version();
        exit(0);
        break; 
      case 'u':
        check_reflector();
        backup_mirrorlist();
        sort_speed();
        exit(0);
        break;
      case 'h':
        show_help();
        exit(0);
        break;
      default:
        break;
    } 
  }
  show_help();
}

void backup_mirrorlist(){
  int ret = system("cp /etc/pacman.d/mirrorlist /etc/pacman.d/mirrorlist.bak &> /dev/null");
  if (ret != 0) {
    printf("error: failed to backup mirrorlist...are you root?\n");
    exit(1);
  } else {
    printf("Backed up current mirrorlist to /etc/pacman.d/mirrorlist.bak\n");
  }
}

void show_help() {
  printf("Usage: pacmirror [OPTION]\n");
  printf("Updates the pacman mirrorlist automagically using reflector\n");
  printf("NEEDS ROOT PERMISSION TO WRITE TO /etc/pacman.d/\n\n");
  printf("  -u, --update            Update the pacman mirrorlist with new mirrorlist - backs up the old one\n");
  printf("  -V, --version           Show version information\n");
  printf("  -h, --help              Show the help menu you are reading right now\n");
}

void check_reflector() {
  int ret = system("type reflector &> /dev/null");
  if (ret != 0) {
    printf("error: pacmirror requires that reflector be installed.\n");
    exit(1);
  }
}

void sort_speed() {
  printf("Sorting new mirrors.....\n");
  int ret = system("reflector --verbose -l 5 --sort rate --save /etc/pacman.d/mirrorlist &> /dev/null");
  if (ret != 0) {
    printf("error: failed to sort mirrors with reflector\n");
    exit(1);
  } else {
    system("cat /etc/pacman.d/mirrorlist");
    printf("Mirrors updated successfully!\n");
  }
}

void show_version() {
  printf("pacmirror version %s\n", PACMIRROR_VERSION);
  system("repo-elephant");
  printf("  by blake bartenbach\n");
}
