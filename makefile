
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso

GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -O2
# GCCPARAMS = -m32 -Iinclude -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore -Wno-write-strings -O2
ASPARAMS = --32
LDPARAMS = -melf_i386

SRC_DIRS := src src/boot

SRC_FILES_C := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
SRC_FILES_S := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.s))
SRC_FILES := $(SRC_FILES_C) $(SRC_FILES_S)

run: mykernel.iso
	@echo "\033[0;32mRun VirtualBox\033[0m"
	@cp mykernel.iso /mnt/d/VirtualBox\ VMs/
	@/mnt/d/VirtualBox/VBoxManage.exe startvm Multiboot

mykernel.bin: linker.ld 
# just compile all the source files together	
	@echo "\033[0;32mCompile mykernel.bin\033[0m"
	gcc $(GCCPARAMS) -T linker.ld -o mykernel.bin $(SRC_FILES)

mykernel.iso: mykernel.bin
	@echo "\033[0;32mMake mykernel.iso\033[0m"
	@mkdir iso
	@mkdir iso/boot
	@mkdir iso/boot/grub
	@cp mykernel.bin iso/boot/mykernel.bin
	@cp mykernel.bin iso/boot/module1.bin
	
	@echo ''		  > iso/boot/grub/grub.cfg
	@echo 'set timeout=0'                     >> iso/boot/grub/grub.cfg 
	@echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	@echo 'set gfxmode=1024x768x32'	    	  >> iso/boot/grub/grub.cfg
	@echo 'set gfxpayload=keep'               >> iso/boot/grub/grub.cfg
	@echo ''                                  >> iso/boot/grub/grub.cfg
	@echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	@echo '  multiboot /boot/mykernel.bin mykernel_arg1=myvalue'    >> iso/boot/grub/grub.cfg
	@echo '  module /boot/module1.bin'       >> iso/boot/grub/grub.cfg
	# @echo '  multiboot2 /boot/mykernel.bin mykernel_arg1=myvalue'    >> iso/boot/grub/grub.cfg
	# @echo '  module2 /boot/module1.bin'       >> iso/boot/grub/grub.cfg
	@echo '  boot'                            >> iso/boot/grub/grub.cfg
	@echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso

# install: mykernel.bin
# 	sudo cp $< /boot/mykernel.bin

.PHONY: clean 
clean:
	@echo "\033[0;32mClean mykernel.bin mykernel.iso iso\033[0m"
	rm -rf mykernel.bin mykernel.iso iso
