# Walkthrough for Level 03

The `level03` binary gets a password from the user reading in STDIN and using scanf.

```asm
...SNIP...
   0x080488c1 <+103>:   call   0x8048530 <__isoc99_scanf@plt>
   0x080488c6 <+108>:   mov    eax,DWORD PTR [esp+0x1c]
   0x080488ca <+112>:   mov    DWORD PTR [esp+0x4],0x1337d00d
   0x080488d2 <+120>:   mov    DWORD PTR [esp],eax
   0x080488d5 <+123>:   call   0x8048747 <test>
...SNIP...
```

It passes the user input as the first argument to the function `test`. As a second parameter it passes the constant, 
`0x1337d00d` (322424845 in decimal). 

The function `test` calculates the difference between `322424845` and the user's input. If the resulting value is
between 1 and 9 or 16 and 21, it will call the `decrypt` function with the resulting value as parameter. Otherwise
`decrypt` function will be called but with a random number as a parameter instead of the user's input.

Here is a list of all inputs that will make the function `test` call `decrypt`:
`322424844, 322424843, 322424842, 322424841, 322424840, 322424839, 322424838, 322424837, 322424836, 322424828, 322424827,
322424826, 322424825, 322424824, 322424823.`

`decrypt` will XOR the string `Q}|u``sfg~sf{}|a30` and the input number passed to it. If the results match the string
`Congratulations!`, a shell will be spawned with the privileges of `level04`.

Trying all possible inputs, we find that `322424827` is the correct password.

```shell
level03@OverRide:~$ ./level03 
***********************************
*               level03         **
***********************************
Password:322424827
$ whoami
level04
```

There isn't a `.pass` file in level04's home directory.

```shell
$ cd ../level04
$ ls -l
total 8
-rwsr-s---+ 1 level05 users 7797 Sep 10  2016 level04
$ ls -a
.  ..  .bash_logout  .bash_profile  .bashrc  .pass  .profile  level04
```