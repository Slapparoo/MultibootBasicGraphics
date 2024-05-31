
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso

GCCPARAMS =  -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -O2
# GCCPARAMS =  -Iinclude -fno-use-cxa-atexit -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -O2
# GCCPARAMS = -m32 -Iinclude -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -Wno-write-strings -O2
# ASPARAMS = --32
# LDPARAMS = -melf_i386

SRC_DIRS := src src/boot

SRC_FILES_C := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
SRC_FILES_S := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))
SRC_FILES := $(SRC_FILES_C) $(SRC_FILES_S)

run: mykernel.iso
	@echo "\033[0;32mRun VirtualBox\033[0m"
	# @cp mykernel.iso /mnt/d/VirtualBox\ VMs/
	# @/mnt/d/VirtualBox/VBoxManage.exe startvm Multiboot
	qemu-system-x86_64 -m 500 -cdrom mykernel.iso


mykernel.bin: linker.ld 
# just compile all the source files together	
	@echo "\033[0;32mCompile mykernel.bin\033[0m"
	gcc -m32 $(GCCPARAMS) -T linker.ld -o mykernel.bin $(SRC_FILES)

	# gcc -Iinclude -fPIE -ffreestanding -T linker.ld -o mykernel2.bin $(SRC_FILES)

	mkdir obj32 obj64
	gcc -m32 -c $(GCCPARAMS) $(SRC_FILES)
	mv *.o obj32
	gcc -c $(GCCPARAMS) $(SRC_FILES)
	mv *.o obj64

	# ld -m elf_i386 $(GCCPARAMS) -nostdlib -T linker.ld -o mykernel2.bin obj/*

mykernel.iso: mykernel.bin
	@echo "\033[0;32mMake mykernel.iso\033[0m"
	@mkdir iso
	@mkdir iso/boot
	@mkdir iso/boot/grub
	@cp mykernel.bin iso/boot/mykernel.bin
	@cp mykernel.bin iso/boot/module1.bin
	
	@cp grub.cfg iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso

# install: mykernel.bin
# 	sudo cp $< /boot/mykernel.bin

.PHONY: clean 
clean:
	@echo "\033[0;32mClean mykernel.bin mykernel.iso iso\033[0m"
	rm -rf obj32 obj64 mykernel.bin mykernel.iso iso *.o
