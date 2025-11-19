#include <signal.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/prctl.h>

char *gets(char *s);

int main(void){
  int child_status;
  char array [128];
  long child_syscall;
  int pid;
  
  pid = fork();
  bzero(array, sizeof(array));
  child_syscall = 0;
  child_status = 0;

  if (pid == 0) {
    prctl(1,1);
    ptrace(PTRACE_TRACEME,0,0,0);
    puts("Give me some shellcode, k");
    gets(array);
  } else {
    do {
      wait(&child_status);
      if (WIFEXITED(child_status) || WIFSIGNALED(child_status)) {
        puts("child is exiting...");
        return 0;
      }
      WIFEXITED(child_status);
      WIFSIGNALED(child_status);
      child_syscall = ptrace(PTRACE_PEEKUSER,pid, 44, 0);
    } while (child_syscall != 11);
    puts("no exec() for you");
    kill(pid,9);
  }

  return 0;
}
