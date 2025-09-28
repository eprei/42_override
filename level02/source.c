int main(void){
  int is_valid_password;
  size_t ret_value;
  long index;
  char *buf_pointer;
  char user_password_input [112];
  char password_from_file [48];
  char buffer_username [100];
  int characters_in_pass;
  FILE *file;

  buf_pointer = buffer_username;
  for (index = 12; index != 0; index = index + -1) {
    buf_pointer[0] = '\0';
    buf_pointer[1] = '\0';
    buf_pointer[2] = '\0';
    buf_pointer[3] = '\0';
    buf_pointer[4] = '\0';
    buf_pointer[5] = '\0';
    buf_pointer[6] = '\0';
    buf_pointer[7] = '\0';
    buf_pointer = buf_pointer + 8;
  }
  buf_pointer[0] = '\0';
  buf_pointer[1] = '\0';
  buf_pointer[2] = '\0';
  buf_pointer[3] = '\0';
  buf_pointer = password_from_file;
  for (index = 5; index != 0; index = index + -1) {
    buf_pointer[0] = '\0';
    buf_pointer[1] = '\0';
    buf_pointer[2] = '\0';
    buf_pointer[3] = '\0';
    buf_pointer[4] = '\0';
    buf_pointer[5] = '\0';
    buf_pointer[6] = '\0';
    buf_pointer[7] = '\0';
    buf_pointer = buf_pointer + 8;
  }
  *buf_pointer = '\0';
  buf_pointer = user_password_input;
  for (index = 12; index != 0; index = index + -1) {
    buf_pointer[0] = '\0';
    buf_pointer[1] = '\0';
    buf_pointer[2] = '\0';
    buf_pointer[3] = '\0';
    buf_pointer[4] = '\0';
    buf_pointer[5] = '\0';
    buf_pointer[6] = '\0';
    buf_pointer[7] = '\0';
    buf_pointer = buf_pointer + 8;
  }
  buf_pointer[0] = '\0';
  buf_pointer[1] = '\0';
  buf_pointer[2] = '\0';
  buf_pointer[3] = '\0';
  file = (FILE *)0x0;
  characters_in_pass = 0;
  file = fopen("/home/users/level03/.pass","r");
  if (file == (FILE *)0) {
    fwrite("ERROR: failed to open password file\n",1,36,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  ret_value = fread(password_from_file,1,41,file);
  characters_in_pass = (int)ret_value;
  ret_value = strcspn(password_from_file,"\n");
  password_from_file[ret_value] = '\0';
  if (characters_in_pass != 41) {
    fwrite("ERROR: failed to read password file\n",1,36,stderr);
    fwrite("ERROR: failed to read password file\n",1,36,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  fclose(file);
  puts("===== [ Secure Access System v1.0 ] =====");
  puts("/***************************************\\");
  puts("| You must login to access this system. |");
  puts("\\**************************************/");
  printf("--[ Username: ");
  fgets(buffer_username,100,stdin);
  ret_value = strcspn(buffer_username,"\n");
  buffer_username[ret_value] = '\0';
  printf("--[ Password: ");
  fgets(user_password_input,100,stdin);
  ret_value = strcspn(user_password_input,"\n");
  user_password_input[ret_value] = '\0';
  puts("*****************************************");
  is_valid_password = strncmp(password_from_file,user_password_input,41);
  if (is_valid_password == 0) {
    printf("Greetings, %s!\n",buffer_username);
    system("/bin/sh");
    return 0;
  }
  printf(buffer_username);
  puts(" does not have access!");
                    /* WARNING: Subroutine does not return */
  exit(1);
}
