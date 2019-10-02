
# sudo apt-get install g++ binutils libc6-dev-i386
# sudo apt-get install VirtualBox grub-legacy xorriso

GCCPARAMS = -m32 -Iinclude -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -O3
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = obj/loader.o \
          obj/vga.o \
          obj/kernel.o


run: mykernel.iso
#	(killall VirtualBox && sleep 1) || true
#	VirtualBox --startvm 'My Operating System' &

obj/%.o: src/%.cpp
	mkdir -p $(@D)
	gcc $(GCCPARAMS) -c -o $@ $<

obj/%.o: src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

mykernel.iso: mykernel.bin
	rm -rf iso
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	
	echo ''		  > iso/boot/grub/grub.cfg
	echo 'set timeout=0'                     >> iso/boot/grub/grub.cfg 
	echo 'set default=0'                     >> iso/boot/grub/grub.cfg
	echo 'set gfxmode=1024x768x32'		 >> iso/boot/grub/grub.cfg
	echo 'set gfxpayload=keep'               >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "My Operating System" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	cp mykernel.iso /mnt/share/mykernel.iso
	

install: mykernel.bin
	sudo cp $< /boot/mykernel.bin

.PHONY: clean
clean:
	rm -rf obj mykernel.bin mykernel.iso
