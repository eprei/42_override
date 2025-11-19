#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool main(void){
  int user_input [4];
  
  puts("***********************************");
  puts("* \t     -Level00 -\t\t  *");
  puts("***********************************");
  printf("Password:");
  scanf(%d,user_input);

  if (user_input[0] != 5276) {
    puts("\nInvalid Password!");
  }
  else {
    puts("\nAuthenticated!");
    system("/bin/sh");
  }

  return user_input[0] != 5276;
}
