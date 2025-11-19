#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
  size_t buffer_len;
  char buffer[100];
  unsigned int i;

  fgets(buffer,100,stdin);
  i = 0;
  do {
    buffer_len = strlen(buffer);
    if (i >= buffer_len - 1) {
      printf(buffer);
      exit(0);
    }
    if ('@' < buffer[i] && buffer[i] < '[') {
      buffer[i] = buffer[i] ^ 0b00100000;
    }
    i++;
  } while( true );
}
