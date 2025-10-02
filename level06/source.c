#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>

int auth(char *login,unsigned int serial_user_input){
    size_t login_len;
    int is_auth;
    long ptrace_ret;
    int i;
    unsigned int serial;

    login_len = strcspn(login,"\n");
    login[login_len] = '\0';
    login_len = strnlen(login,32);
    if ((int)login_len < 6) {
        is_auth = 1;
    }
    else {
        ptrace_ret = ptrace(PTRACE_TRACEME);
        if (ptrace_ret == -1) {
            puts("\x1b[32m.---------------------------.");
            puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
            puts("\x1b[32m\'---------------------------\'");
            is_auth = 1;
        }
        else {
            serial = ((int)login[3] ^ 0x1337U) + 0x5eeded;
            for (i = 0; i < (int)login_len; i = i + 1) {
                if (login[i] < ' ') {
                    return 1;
                }
                serial = serial + ((int)login[i] ^ serial) % 1337;
            }
            if (serial_user_input == serial) {
                is_auth = 0;
            }
            else {
                is_auth = 1;
            }
        }
    }
    return is_auth;
}

int main(void){
    int is_auth;
    int in_GS_OFFSET;
    unsigned int serial_user_input;
    char login [32];
    int canary;

    canary = *(int *)(in_GS_OFFSET + 0x14);
    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    printf("-> Enter Login: ");
    fgets(login,0x20,stdin);
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    printf("-> Enter Serial: ");
    scanf("%u", &serial_user_input);
    is_auth = auth(login,serial_user_input);
    if (is_auth == 0) {
        puts("Authenticated!");
        system("/bin/sh");
    }
    if (canary != *(int *)(in_GS_OFFSET + 0x14)) {
        /* WARNING: Subroutine does not return */
        __stack_chk_fail();
    }
    return (unsigned int)(is_auth != 0);
}
