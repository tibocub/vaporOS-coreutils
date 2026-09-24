# VaporOS-coreutils

> The GNU coreutils equivalent (and more) of [vaporOS](https://github.com/tibocub/vaporOS)

forked from [toybox](https://codeberg.org/landley/toybox) (another coreutils alternative) and copied 
some [nsh](https://nuttx.apache.org/docs/latest/applications/nsh/index.html) (NuttX's shell) code.


## GOALS
- Provide NuttX with a wider and better set of POSIX tools and commands than the
nsh builtins by porting toybox to NuttX (and even implementing our own tools that
toybox or nsh doesn't already).
- Compile in a single binary (like toybox and nsh already do)
- Keep and adapt toybox's menuconfig-based configuration to easily select which
commands to include in the binary
- 


# Frequently (never) Asked Questions

> why NuttX ?

It offers a narrower hardware support than its main alternative zephyr, but debugging
and porting new boards/chips seems much easier. NuttX's philosophy also match with
VaporOS on a lot of points: it's aiming for POSIX compliance (zephyr isn't), and being
less used in production than zephyr, it have a more community-friendly ecosystem where
hobyists are welcome and can play an important role in development.

> why toybox?

Mostly because of its good ratio of size/POSIX (some alternatives offers better POSIX-
compliance but are heavier and some are much lighter but with poor compliance).
It also uses a more permissive license (BSD) than most alternatives.


## FEATURES

### VAPOR NATIVE
nothing yet - all current commands come from toybox or nsh

### TOYBOX PORTS
- [x] true
- [x] false
- [x] echo
- [x] pwd
- [x] cat
- [x] mkdir
- [x] rmdir
- [x] touch
- [x] printf
- [x] rm
- [x] ls
- [x] cp
- [x] mv
- [x] printenv
- [x] basename
- [x] dirname
- [x] sleep
- [x] which
- [x] find
- [x] head
- [x] tail
- [x] cut
- [x] ln (hard/sym links: ENOSYS on the sim's FAT /tmp)
- [ ] acpi
- [x] arch
- [ ] blkdiscard
- [ ] blkid
- [ ] blockdev
- [ ] cal
- [ ] chattr
- [ ] chgrp
- [x] chmod (ENOSYS on the sim's FAT /tmp)
- [ ] chown
- [ ] chroot
- [ ] chrt
- [ ] clear
- [x] cmp
- [x] comm
- [ ] cpio
- [ ] cut
- [x] date
- [ ] dd
- [ ] devmem
- [ ] df
- [ ] diff
- [ ] dmesg
- [ ] dos2unix
- [ ] du
- [x] egrep
- [x] env
- [x] expand
- [x] expr
- [ ] fallocate
- [ ] false
- [x] fgrep
- [ ] file
- [ ] flock
- [ ] fmt
- [not planned] free
- [not planned] freeramdisk
- [ ] fsfreeze
- [ ] fsync
- [ ] getconf
- [ ] getopt
- [x] grep
- [ ] groups
- [ ] gunzip
- [ ] gzip
- [ ] head
- [ ] help
- [ ] hostname
- [ ] hwclock
- [ ] id
- [ ] ifconfig
- [ ] inotifyd
- [ ] insmod
- [ ] ionice
- [ ] iorienice
- [ ] iotop
- [ ] kill
- [ ] killall
- [ ] link
- [ ] ln
- [ ] logger
- [ ] login
- [ ] logname
- [ ] losetup
- [ ] ls
- [ ] lsattr
- [ ] lsmod
- [ ] lspci
- [ ] lsusb
- [ ] makedevs
- [ ] mcookie
- [x] md5sum
- [ ] mkdir
- [ ] mkfifo
- [ ] mknod
- [ ] mkpasswd
- [ ] mkswap
- [x] mktemp
- [ ] modinfo
- [ ] mount
- [ ] mountpoint
- [ ] mv
- [ ] nc
- [ ] netcat
- [ ] netstat
- [ ] nice
- [x] nl
- [x] nohup
- [ ] nproc
- [ ] nsenter
- [x] od
- [ ] partprobe
- [ ] passwd
- [x] paste
- [ ] patch
- [ ] pgrep
- [ ] pidof
- [ ] ping
- [ ] pivot_root
- [ ] pkill
- [ ] pmap
- [not planned] poweroff
- [ ] printenv
- [ ] printf
- [ ] prlimit
- [ ] ps
- [ ] pwd
- [ ] pwdx
- [ ] readahead
- [ ] readlink
- [ ] realpath
- [ ] reboot
- [ ] renice
- [ ] reset
- [x] rev
- [ ] rfkill
- [ ] rm
- [ ] rmdir
- [ ] rmmod
- [ ] rtcwake
- [x] sed
- [ ] seq
- [ ] setfattr
- [ ] setsid
- [x] sha1sum
- [x] sha224sum
- [x] sha256sum
- [x] sha384sum
- [ ] sha3sum
- [x] sha512sum
- [ ] shred
- [ ] sleep
- [ ] sntp
- [x] sort
- [x] split
- [ ] stat
- [ ] strings
- [ ] su
- [ ] swapoff
- [ ] swapon
- [ ] switch_root
- [ ] sync
- [ ] sysctl
- [x] tac
- [ ] tail
- [ ] tar
- [ ] taskset
- [x] tee
- [ ] test
- [ ] time
- [ ] timeout
- [ ] top
- [ ] touch
- [ ] true
- [x] truncate
- [x] tty
- [ ] tunctl
- [ ] ulimit
- [ ] umount
- [x] uname
- [x] uniq
- [ ] unix2dos
- [x] unlink
- [ ] unshare
- [ ] uptime
- [ ] usleep
- [ ] uudecode
- [ ] uuencode
- [ ] uuidgen
- [ ] vconfig
- [ ] vmstat
- [ ] watch
- [ ] watchdog
- [x] wc
- [ ] which
- [ ] who
- [ ] whoami
- [x] xargs
- [x] xxd
- [x] yes
- [x] tr
- [ ] zcat

## NSH PORTS
- [ ] poweroff
- [ ] ps
- [ ] free
- [ ] df
- [ ] mount/umount
- [ ] dmesg
- [ ] 
- [ ] 
- [ ] 
- [ ] 

# LICENSE
MIT
