int verify_user_name(void){
  int username_len;
  char *buffer;
  char *username;
  bool bool_1;
  bool bool_2;
  byte bVar1;
  
  bVar1 = 0;
  bool_1 = &stack0xfffffff4 < (undefined1 *)16;
  bool_2 = &stack0x00000000 == (undefined1 *)28;
  puts("verifying username....\n");
  username_len = 7;
  buffer = &a_user_name;
  username = "dat_wil";
  do {
    if (username_len == 0) break;
    username_len = username_len + -1;
    username = username + (uint)bVar1 * -2 + 1;
    buffer = buffer + (uint)bVar1 * -2 + 1;
    bool_1 = (byte)*buffer < (byte)*username;
    bool_2 = *buffer == *username;
    buffer = buffer;
    username = username;
  } while (bool_2);
  return (int)(char)((!bool_1 && !bool_2) - bool_1);
}

int verify_user_pass(char *param_1){
  int password_len;
  char *password;
  bool in_CF;
  bool in_ZF;

  password_len = 5;
  password = "admin";
  do {
    if (password_len == 0) break;
    password_len = password_len + -1;
    in_CF = (byte)*param_1 < (byte)*password;
    in_ZF = *param_1 == *password;
    param_1 = (char *)((byte *)param_1 + 1);
    password = password + 1;
  } while (in_ZF);
  return (int)(char)((!in_CF && !in_ZF) - in_CF);
}

int main(void){
  int return_value;
  char *buf_pointer;
  char buffer [64];
  int verification_result;

  buf_pointer = buffer;
  for (return_value = 16; return_value != 0; return_value = return_value + -1) {
    buf_pointer[0] = '\0';
    buf_pointer[1] = '\0';
    buf_pointer[2] = '\0';
    buf_pointer[3] = '\0';
    buf_pointer = buf_pointer + 4;
  }
  verification_result = 0;
  puts("********* ADMIN LOGIN PROMPT *********");
  printf("Enter Username: ");
  fgets(&a_user_name,256,stdin);
  verification_result = verify_user_name();
  if (verification_result == 0) {
    puts("Enter Password: ");
    fgets(buffer,100,stdin);
    verification_result = verify_user_pass(buffer);
    if ((verification_result == 0) || (verification_result != 0)) {
      puts("nope, incorrect password...\n");
      return_value = 1;
    }
    else {
      return_value = 0;
    }
  }
  else {
    puts("nope, incorrect username...\n");
    return_value = 1;
  }
  return return_value;
}
