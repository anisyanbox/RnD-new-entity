# UID Changer

The module can change UID for any process to root-UID.
After loading module to kernel it can happen by two ways:

1. Just open device file from your current bash
```sh
$ ls -l /dev/uid_changer0 
crw------- 1 root root 239, 0 апр  8 00:36 /dev/uid_changer0

$ sudo chmod 664 /dev/uid_changer0
$ whoami

# after this you and bash-process are root
$ source /dev/uid_changer0
$ whoami

# create new bash process (from current root parent)
# after this symbol `$` will change to symbol `#`
$ bash
```

2. Point PID for some process through sysfs attributes
```sh
# show all process's PIDs in system
# pick PID from the lish (f.e. 3105)
# for current bash you can use $BASHPID
cat /sys/class/uid_changer/uid_changer0/list_all_pids

# change UID
# after this process with PID=3105 is root
sudo bash -c "echo 3105 > /sys/class/uid_changer/uid_changer0/substitute_uid_for_pid"
whoami

# create new bash process (from current root parent)
# after this symbol `$` will change to symbol `#`
$ bash
```
