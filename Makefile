CC = C:\Users\komin\Documents\i686-elf-tools\bin\i686-elf-gcc.exe
AS = C:\Users\komin\Documents\NASM\nasm.exe
LD = C:\Users\komin\Documents\i686-elf-tools\bin\i686-elf-ld.exe

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -fno-pie
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

all: kernel.elf

kernel.elf: boot.o kernel.o reboot.o font.o fb_text.o ata.o
	$(LD) $(LDFLAGS) -o kernel.elf $^

boot.o: boot.asm
	$(AS) -f elf32 $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

reboot.o: reboot.c
	$(CC) $(CFLAGS) -c $< -o $@

font.o: font.c
	$(CC) $(CFLAGS) -c $< -o $@

fb_text.o: fb_text.c
	$(CC) $(CFLAGS) -c $< -o $@

ata.o: ata.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q *.o kernel.elf kekeos_native.iso
	if exist isodir rmdir /S /Q isodir

iso: kernel.elf
	if exist isodir rmdir /S /Q isodir
	mkdir isodir
	mkdir isodir\boot
	mkdir isodir\boot\grub
	copy kernel.elf isodir\boot\kernel.elf
	copy grub.cfg isodir\boot\grub\grub.cfg
	wsl -- grub-mkrescue -o kekeos_native.iso isodir

hdd: kernel.elf
	if exist isodir rmdir /S /Q isodir
	mkdir isodir
	copy kernel.elf isodir\kernel.elf
	copy C:\Users\komin\ldlinux.c32 isodir\ldlinux.c32
	copy C:\Users\komin\libcom32.c32 isodir\libcom32.c32
	copy C:\Users\komin\menu.c32 isodir\menu.c32
	copy C:\Users\komin\mboot.c32 isodir\mboot.c32
	copy C:\Users\komin\libmenu.c32 isodir\libmenu.c32
	copy C:\Users\komin\libutil.c32 isodir\libutil.c32
	echo DEFAULT kekeos > isodir\syslinux.cfg
	echo PROMPT 0 >> isodir\syslinux.cfg
	echo TIMEOUT 0 >> isodir\syslinux.cfg
	echo. >> isodir\syslinux.cfg
	echo LABEL kekeos >> isodir\syslinux.cfg
	echo     KERNEL mboot.c32 >> isodir\syslinux.cfg
	echo     APPEND kernel.elf >> isodir\syslinux.cfg
	wsl dd if=/dev/zero of=kekeos_native.img bs=1M count=100
	wsl mkfs.vfat kekeos_native.img
	wsl mcopy -i kekeos_native.img -s isodir/* ::
	C:\Users\komin\Documents\syslinux\syslinux-6.03\bios\win32\syslinux.exe --directory isodir kekeos_native.img

run: iso
	qemu-system-i386 -cdrom kekeos_native.iso -vga std