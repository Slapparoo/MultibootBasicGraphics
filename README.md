# MultibootBasicGraphics
Very Basic example of Booting with GRUB, and requesting a graphics mode with Multiboot

# Overview
The purpose of this is a basic sample of using a GRUB boot and requesting a graphics mode with MultiBoot, and then doing some very basic drawing functions.

The idea is that it will be a reference for people who have created their own OS and would like to add graphics to it, and want to use MUltiBoot to figure it out for them instead of coding all the required steps them selfs. 

# Features:
* Boots into 1024 x 768 x 32 Graphics mode.
* 8 x 8 loyalty free Font

Included is a basic 8x8 font, the font was created in TempleOS using the FontEd example.

# Building
Needs to be built under Linux (I tried WLS but it doesn't create the ISO image)
The Make script will create an ISO

# Running
Create a new virtual machine in VirtualBox
Mount the ISO
Boot

# What it is not
It has no drivers for anything, no memory managment, no keyboard or mouse IO, not Disk IO, nothing.
