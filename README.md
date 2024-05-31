# Multiboot Basic Graphics
Very Basic example of Booting with GRUB, and requesting a 32bit Graphics mode with Multiboot 1024 x 768 x 32

* the master branch is the bare bones in C++
* main-c is slightly more advanged but in C

# Overview
The purpose of this is a basic sample of using a GRUB boot and requesting a graphics mode with MultiBoot, and then doing some very basic drawing functions.

Included is a loyalty free font, with example use so you can write stuff to screen in your OS.

The idea is that it will be a reference for people who have created their own OS and would like to add graphics to it, and want to use MultiBoot to figure it out for them instead of coding all the required steps them selfs. 

Tested and working with Virtualbox 7.0 inwindows 11 using wsl 2.

# MultiBoot basics
The multiboot works by setting specific values at specific offsets in the boot executable file, so we are requesting the graphics mode with the following:
```asm
.set WIDTH, 1024  /* requested width */
.set HEIGHT, 768  /* requested height */
.set DEPTH, 32    /* requested depth */
```

GRUB will read those values and set the Graphics mode prior to handing execution over to our application, so when our application starts, if the requested mode is valid, it will already be in that mode.

GRUB will also pass through the multiboot header containing what the Graphics mode is set to and other values of interest.

The framebuffer_addr is where you will write your graphics operations to.

Things to note, request a valid Graphics Mode, In the GRUB menu [https://askubuntu.com/questions/16042/how-to-get-to-the-grub-menu-at-boot-time] CLI the `videoinfo` will list the available and valid modes.

I would recommend working with 32Bit graphics it is simplier and I would have thought as fast or faster than other options. Before you go thinking but 8bit graphics will be faster measure it.

# Basic principles
* Don't read from the framebuffer_addr it will be slow.
* Don't fiddle around writing bits and peices to the framebuffer_addr write in bulk in a single loop from a buffer, the reason for this is, in the underlying architecture you are transfering data from your main RAM to the PCI-E bus, if you do it as single opertions you will be using burst writing in Cache which makes a huge difference.
* If you run directly against Hardware ie not in a Virtual machine you will need to setup up write combining for the framebuffer_addr in the Cache so it will busrt write in blocks to the PCI-E bus (in a VM you get this for free as the underlying OS has already done it).

# Features:
* Boots into 1024 x 768 x 32 Graphics mode.
* 8 x 8 loyalty free Font
* Basic draw functions
* 32bit not x64

The basic 8x8 font was created in TempleOS using the FontEd example.

# Building
Needs to be built under Linux (I tried WLS but it doesn't create the ISO image)
The Make script will create an ISO, 
```
g++ 
binutils 
libc6-dev-i386
VirtualBox 
grub-legacy 
xorriso
```


# Running
Create a new virtual machine in VirtualBox
Mount the ISO
Boot

# What it is not
It has no drivers for anything, no memory managment, no keyboard or mouse IO, not Disk IO, nothing.

# Boot sequence
* Grub loads
* Grub is configured to load our exec
* Grub switchs from 16bit mode to 32bit mode
* Grub read our exe, sees the request for the graphics mode
* Grub sets the graphics mode
* Grub starts executing our exe, (loader.s writen in ASM)
* Loader.s calls teh CPP KernalMain

# Toolchain install
sudo apt install gcc g++ make grub-pc-bin xorriso

# Virtual box
Once Virtualbox is configed, there is a profile which I created under an earlier version of VirtualBox and Just retested in 7.0 in the [Releases](https://github.com/Slapparoo/MultibootBasicGraphics/releases)

From the command prompt (VBoxManage.exe will be installed in a different location for you)
/mnt/d/VirtualBox/VBoxManage.exe startvm Multiboot
