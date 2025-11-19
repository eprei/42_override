#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void clear_stdin(void){
  int i;

  do {
    i = getchar();
    if ((char)i == '\n') {
      return;
    }
  } while ((char)i != -1);
}

int get_unum(void){
  int user_input;

  user_input = 0;
  fflush(stdout);
  scanf("%d",&user_input);
  clear_stdin();
  return user_input;
}

int store_number(int *store){
  unsigned int number_input;
  unsigned int index_input;
  int ret;

  printf(" Number: ");
  number_input = get_unum();
  printf(" Index: ");
  index_input = get_unum();
  if (index_input % 3 == 0 || number_input >> 24 == 0xb7) {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    ret = 1;
  }
  else {
    store[index_input] = number_input;
    ret = 0;
  }
  return ret;
}

int read_number(int *store){
  printf(" Index: ");
  const int user_input_num = get_unum();
  printf(" Number at data[%u] is %u\n", user_input_num,store[user_input_num]);
  return 0;
}

int main(int argc,char **argv,char **env){
  int ret_command;
  int store_array [100];
  char input_buffer [20];
  char **argv_local = argv;
  char **env_local = env;

  memset(input_buffer, 0, sizeof(input_buffer));
  memset(store_array, 0, sizeof(store_array));

  for (; *argv_local != (char *)0; argv_local++) {
    memset(*argv_local, 0, strlen(*argv_local));
  }
  for (; *env_local != (char *)0x0; env_local++) {
    memset(*env_local, 0, strlen(*env_local));
  }

  puts(
       "----------------------------------------------------\n\
         Welcome to wil\'s crappy number storage service!   \n\
        ----------------------------------------------------\n\
         Commands:                                          \n\
            store - store a number into the data storage    \n\
            read  - read a number from the data storage     \n\
            quit  - exit the program                        \n\
        ----------------------------------------------------\n\
           wil has reserved some storage :>                 \n\
        ----------------------------------------------------\n"
      );
    do {
      printf("Input command: ");
      fgets(input_buffer, sizeof(input_buffer), stdin);

      int input_len = strcspn(input_buffer, "\n");
      input_buffer[input_len] = '\0';

      if (strncmp(input_buffer, "store", 5) == 0) {
        ret_command = store_number(store_array);
      } else if (strncmp(input_buffer, "read", 4) == 0) {
        ret_command = read_number(store_array);
      } else if (strncmp(input_buffer, "quit", 4) == 0) {
        break;
      } else {
        ret_command = -1;
      }

      if (ret_command == 0) {
        printf("Completed %s command successfully\n", input_buffer);
      } else {
        printf("Failed to do %s command\n", input_buffer);
      }

      memset(input_buffer, 0, sizeof(input_buffer));

  } while( true );
}
