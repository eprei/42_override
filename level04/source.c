int main(void){
  int i;
  char *char_ptr;
  byte zero;
  uint child_status;
  char array [128];
  uint local_20;
  uint local_1c;
  long child_syscall;
  int fork_ret;
  
  zero = 0;
  fork_ret = fork();
  char_ptr = array;
  for (i = 32; i != 0; i = i + -1) {
    char_ptr[0] = '\0';
    char_ptr[1] = '\0';
    char_ptr[2] = '\0';
    char_ptr[3] = '\0';
    char_ptr = char_ptr + ((uint)zero * -2 + 1) * 4;
  }
  child_syscall = 0;
  child_status = 0;
  if (fork_ret == 0) {
    prctl(1,1);
    ptrace(PTRACE_TRACEME,0,0,0);
    puts("Give me some shellcode, k");
    gets(array);
  }
  else {
    do {
      wait(&child_status);
      local_20 = child_status;
      if (((child_status & 0x7f) == 0) ||
         (local_1c = child_status, '\0' < (char)(((byte)child_status & 0x7f) + 1) >> 1)) {
        puts("child is exiting...");
        return 0;
      }
      child_syscall = ptrace(PTRACE_PEEKUSER,fork_ret,44,0);
    } while (child_syscall != 11);
    puts("no exec() for you");
    kill(fork_ret,9);
  }
  return 0;
}
