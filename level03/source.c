#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int decrypt(int substraction_result);
void test(int user_input,int magic_number);

int main(void){
  unsigned int __seed;
  int password_input;

  __seed = time(0);
  srand(__seed);
  puts("***********************************");
  puts("*\t\tlevel03\t\t**");
  puts("***********************************");
  printf("Password:");
  scanf("%d", &password_input);
  test(password_input, 322424845);
  return 0;
}

void test(int user_input, int magic_number){

    switch ( magic_number - user_input ){
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            decrypt(magic_number - user_input);
            break;
        default:
            decrypt(rand());
            break;
    }
}

int decrypt(int substraction_result){
    char encrypted[17] = "Q}|u`sfg~sf{}|a3";
    unsigned int len = strlen(encrypted);

    for (int i = 0; i < len; i++) {
        encrypted[i] = (unsigned char)substraction_result ^ encrypted[i];
    }

    if (strcmp(encrypted, "Congratulations!") == 0) {
        return system("/bin/sh");
    }

    return puts("\nInvalid Password");
}



