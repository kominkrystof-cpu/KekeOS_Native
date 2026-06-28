; boot.asm - Minimal multiboot test kernel
section .multiboot
align 4
magic equ 0x1BADB002
flags equ 0x00000003
checksum equ -(magic + flags)

dd magic            ; +0: magic number
dd flags            ; +4: flags
dd checksum         ; +8: checksum
dd 0                ; +12: header_addr
dd 0                ; +16: load_addr
dd 0                ; +20: load_end_addr
dd 0                ; +24: bss_end_addr
dd 0                ; +28: entry_addr
dd 0                ; +32: mode_type
dd 0                ; +36: width
dd 0                ; +40: height
dd 0                ; +44: depth

section .text
global _start
_start:
    ; Write 'A' to VGA buffer at 0xB8000
    mov word [0xB8000], 0x0F41
    cli
    hlt
    jmp $