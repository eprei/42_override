# Walkthrough for Level 06

This binary takes a `login` and a `serial number` as input. It calculates a hash (serial) from the `login` and compares it to
the provided `serial number`. If they match, it authenticates the user and spawns a shell with the privileges of the
`level07` user, since the binary is owned by `level07` and has the setuid bit set.

If we try to trace the binary with `strace`, `ltrace` or `gdb` the authentication will always fail. Therefore, these
tools are not useful here. To solve this level, we need to reverse engineer the hash function to find a valid login and
serial pair. First, we decompile the binary using [ghidra](https://github.com/NationalSecurityAgency/ghidra). After
cleaning up the code, we add a line to print the serial value right after it is computed (see line 24). The resulting
code can be found in the file [level06_debug.c](level06_debug.c), with the login hardcoded as "AAAAAA".

Compiling and executing the code gives us the serial number for the login "AAAAAA".

```shell
╰─ gcc level06_debug.c && ./a.out
Serial calculated: 6229050
```

The computed serial is `6229050`. We can now run the original binary with the login "AAAAAA" and the serial `6229050`.

```shell
level06@OverRide:~$ ./level06 
***********************************
*               level06           *
***********************************
-> Enter Login: AAAAAA
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6229050
Authenticated!
$ whoami
level07
```

Now we are logged in as `level07` and can read the password for the next level.

```shell
$ cd ../level07
$ cat .pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```