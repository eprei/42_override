#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
  char user_password_input [112];
  char password [48];
  char buffer_username [100];
  int pass_len = 0;


  bzero(buffer_username, sizeof(buffer_username));
  bzero(password, sizeof(password));
  bzero(user_password_input, sizeof(user_password_input));

  FILE* file = fopen("/home/users/level03/.pass", "r");
  if (file == (FILE *)0) {
    fwrite("ERROR: failed to open password file\n",1,36,stderr);
    exit(1);
  }

  pass_len = (int)fread(password,1,41,file);
  password[strcspn(password,"\n")] = '\0';
  if (pass_len != 41) {
    fwrite("ERROR: failed to read password file\n",1,36,stderr);
    fwrite("ERROR: failed to read password file\n",1,36,stderr);
    exit(1);
  }

  fclose(file);
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");
  printf("--[ Username: ");
  fgets(buffer_username, 100, stdin);
  buffer_username[strcspn(buffer_username,"\n")] = '\0';
  printf("--[ Password: ");
  fgets(user_password_input, 100, stdin);
  user_password_input[strcspn(user_password_input,"\n")] = '\0';
  puts("*****************************************");

  const int is_valid_password = strncmp(password, user_password_input, 41);
  if (is_valid_password == 0) {
    printf("Greetings, %s!\n",buffer_username);
    system("/bin/sh");
    return 0;
  }

  printf(buffer_username);
  puts(" does not have access!");
  exit(1);
}
