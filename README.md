#### About

Source codes playing with Raspberry Pi and Raspberry Pi 2


#### Prerequisite

##### Build tools

Install RPi build tools
```
cd ~
mkdir -p bin
cd bin
git clone https://github.com/raspberrypi/tools rpitools
```

Edit `.profile` file add below lines at the end
```
# raspberry pi
PATH=$PATH:$HOME/bin/rpitools/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi/bin
```

Logout and login


##### ssh key

When using `scp` command to copy binary files, it would be convenient if you don't have to
type passwords each time. Adding host public key to RPis authorized keys will do this.

1) In host PC, generate rsa key if you don't have it
```
host$ ssh-keygen -t rsa
```

2) copy or add `~/.ssh/id_rsa.pub` file to Raspberry Pi
```
host$ scp ~/.ssh/id_rsa.pub pi@(rpi-ip-address):/home/pi/.ssh/authorized_keys.temp
```
replace `(rpi-ip-address)` with IP address of your RPi.

3) in RPi, append to `authorized_keys` file
```
pi$ cat ~/.ssh/authorized_keys.temp >> ~/.ssh/authorized_keys
```
