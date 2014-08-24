/* pacmirror - uses reflector to update your pacman mirrorlist automagically!
     author:  blake bartenbach
     version: 0.0.1   
 */

#include <stdio.h>
#include <stdlib.h>

main() {
  backup_mirrorlist();
  sort_speed();
  exit(0);
}

backup_mirrorlist(){
  int ret = system("cp /etc/pacman.d/mirrorlist /etc/pacman.d/mirrorlist.bak &> /dev/null");
  if (ret != 0) {
    printf("error: failed to backup mirrorlist...arre you root?\n");
    exit(1);
  }
}

sort_speed() {
  int ret = system("type reflector &> /dev/null");
  if (ret != 0) {
    printf("error: pacmirror requires that reflector be installed.\n");
    exit(1);
  }
  ret = system("reflector --verbose -l 5 --sort rate --save /etc/pacman.d/mirrorlist &> /dev/null");
  if (ret != 0) {
    printf("Failed to sort mirrors with reflector...please contact me if this happens.\n");
    exit(1);
  } else {
    printf("Mirrors updated successfully!\n");
    system("cat /etc/pacman.d/mirrorlist");
  }
}
