#include <stdio.h>
#include <string.h>

int auth(char *login, unsigned int serial_user_input){
    size_t login_len;
    int is_auth;
    int i;
    unsigned int serial;

    login_len = strcspn(login,"\n");
    login[login_len] = '\0';
    login_len = strnlen(login,32);
    if ((int)login_len < 6) {
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
        printf("Serial calculated: %d\n", serial); // Debug line to show calculated serial
        if (serial_user_input == serial) {
            is_auth = 0;
        }
        else {
            is_auth = 1;
        }
    }
    return is_auth;
}

int main() {
    char login[] = "AAAAAA";
    const unsigned int serial_user_input = 424242; // Example serial number

    auth(login, serial_user_input);

    return 0;
}