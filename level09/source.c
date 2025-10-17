#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void handle_msg(void);
void set_msg(char *storage);
void set_username(char *storage);

void secret_backdoor(void){
    char command [128];

    fgets(command, 128, STDIN_FILENO);
    system(command);
}

int main(void){
    puts(
        "--------------------------------------------\n|   ~Welcome to l33t-m$n ~    v1337        |\n--------------------------------------------"
        );
    handle_msg();
    return 0;
}

void handle_msg(void){
    char storage [140];

    set_username(storage);
    set_msg(storage);
    puts(">: Msg sent!");
}

void set_msg(char *storage){
    char buf_msg [1024];

    memset(buf_msg, 0, 1024);

    puts(">: Msg @Unix-Dude");
    printf(">>: ");
    fgets(buf_msg, 1024, STDIN_FILENO);

    size_t msg_len = (long)*(int *)(storage + 180);
    strncpy(storage, buf_msg, msg_len);
}

void set_username(char *storage){
    char buf_username [140];

    memset(buf_username, 0, 128);

    puts(">: Enter your username");
    printf(">>: ");
    fgets(buf_username, 128, STDIN_FILENO);

    for (int i = 0; i < 41 && buf_username[i]; i++) {
        storage[140 + i] = buf_username[i];
    }

    printf(">: Welcome, %s", storage + 140);
}
