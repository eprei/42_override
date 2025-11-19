#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>

void log_wrapper(FILE *file,char *log_msg, char *argv_1);

int main(int argc, char **argv){
  char buffer[104];

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

  strncpy(buffer, "./backups/", 11);
  strncat(buffer, argv[1], sizeof(buffer) - strlen(buffer) - 1);

  int fd_backup = open(buffer, 193, 432);
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
  size_t array_len, log_msg_len;
  char buffer[264];

  strcpy(buffer, log_msg);
  log_msg_len = strlen(buffer);

  snprintf(buffer + log_msg_len, 254 - log_msg_len, argv_1);
  array_len = strcspn(buffer, "\n");
  buffer[array_len] = '\0';
  fprintf(file, "LOG: %s\n", buffer);
}
