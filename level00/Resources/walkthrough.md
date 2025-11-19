# Walkthrough for Level 00

## Understanding the Program

The assembly code for `level00` compares the user input against a hardcoded value. The program expects a 5726 as the
input. If the input matches, it prints "Authenticated!" and spawns a shell with the `level01` user's privileges. This
allows us to read the password for the next level.

```assembly
0x080484e7 <+83>:    cmp    eax,0x149c                         ; 5276 in decimal
```

## Exploit the Program

```
level00@OverRide:~$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
```