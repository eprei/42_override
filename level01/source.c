#include <stdio.h>
#include <string.h>

char a_user_name[100];

int verify_user_name(void){
  puts("verifying username....\n");
  return memcmp(a_user_name, "dat_wil", 7);
}

int verify_user_pass(char *pass){
  return memcmp(pass, "admin", 5);
}

int main(void){
  int return_value;
  char buffer [64];
  int is_authenticated = 0;

  memset(buffer, 0, sizeof(buffer));
  puts("********* ADMIN LOGIN PROMPT *********");
  printf("Enter Username: ");
  fgets(a_user_name,256,stdin);
  is_authenticated = verify_user_name();

  if (is_authenticated == 0) {
    puts("Enter Password: ");
    fgets(buffer,100,stdin);
    is_authenticated = verify_user_pass(buffer);
    puts("nope, incorrect password...\n");
    return_value = 1;
  } else {
    puts("nope, incorrect username...\n");
    return_value = 1;
  }
  return return_value;
}
