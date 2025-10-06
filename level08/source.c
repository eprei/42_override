#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
void log_wrapper(FILE *file,char *log_msg,char *argv_1);

int main(int argc,char **argv){
  char buf [104];

  if (argc != 2) {
    printf("Usage: %s filename\n",*argv);
  }

  FILE *log_file = fopen("./backups/.log", "w");
  if (log_file == (FILE *)0) {
    printf("ERROR: Failed to open %s\n","./backups/.log");
    exit(1);
  }

  log_wrapper(log_file,"Starting back up: ",argv[1]);

  FILE *file_to_be_backuped = fopen(argv[1], "r");
  if (file_to_be_backuped == (FILE *)0) {
    printf("ERROR: Failed to open %s\n",argv[1]);
    exit(1);
  }

  strncpy(buf, "./backups/", 11);

  strncat(buf, argv[1], sizeof(buf) - strlen(buf) - 1);

  int fd_backup = open(buf, 193, 432);
  if (fd_backup < 0) {
    printf("ERROR: Failed to open %s%s\n","./backups/",argv[1]);
    exit(1);
  }

  char c = -1;
  while( true ) {
    int char_read_int = fgetc(file_to_be_backuped);
    c = (char)char_read_int;
    if (c == -1) break;
    write(fd_backup, &c, 1);
  }

  log_wrapper(log_file,"Finished back up ",argv[1]);
  fclose(file_to_be_backuped);
  close(fd_backup);

  return 0;
}

void log_wrapper(FILE *file, char *log_msg, char *argv_1){
  size_t array_len;
  unsigned long max_u_long_1;
  unsigned long max_u_long_2;
  char *pcVar1;
  long in_FS_OFFSET;
  byte zero;
  undefined8 local_120;
  char array [264];
  long canary;
  char c;

  zero = 0;
  canary = *(long *)(in_FS_OFFSET + 40);
  local_120 = file;
  strcpy(array,log_msg);
  max_u_long_1 = 0xffffffffffffffff;
  pcVar1 = array;
  do {
    if (max_u_long_1 == 0) break;
    max_u_long_1 = max_u_long_1 - 1;
    c = *pcVar1;
    pcVar1 = pcVar1 + (unsigned long)zero * -2 + 1;
  } while (c != '\0');
  max_u_long_2 = 0xffffffffffffffff;
  pcVar1 = array;
  do {
    if (max_u_long_2 == 0) break;
    max_u_long_2 = max_u_long_2 - 1;
    c = *pcVar1;
    pcVar1 = pcVar1 + (unsigned long)zero * -2 + 1;
  } while (c != '\0');
  snprintf(array + (~max_u_long_2 - 1),254 - (~max_u_long_1 - 1),argv_1);
  array_len = strcspn(array,"\n");
  array[array_len] = '\0';
  fprintf(local_120,"LOG: %s\n",array);
  if (canary != *(long *)(in_FS_OFFSET + 40)) {
    __stack_chk_fail();
  }
  return;
}

