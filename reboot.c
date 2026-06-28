#include "reboot.h"
#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

typedef struct __attribute__((packed)) {
    uint16_t limit;
    uint32_t base;
} idt_descriptor_t;

void reboot_via_8042(void) {
    uint8_t good = 0x02;
    int timeout = 100000;
    while (timeout--) {
        good = inb(0x64);
        if (!(good & 0x02)) break;
    }
    outb(0x64, 0xFE);
    for (volatile int i = 0; i < 1000000; i++);
}

static void __attribute__((noreturn)) trigger_triple_fault(void) {
    idt_descriptor_t null_idt = {
        .limit = 0,
        .base  = 0
    };
    __asm__ volatile ("lidt %0" : : "m"(null_idt));
    __asm__ volatile ("cli");
    __asm__ volatile ("int $0x00");
    __builtin_unreachable();
}

void reboot_via_acpi(void) {
    outb(0xCF9, 0x00);
    outb(0xCF9, 0x04);
    for (volatile int i = 0; i < 100000; i++);
    outb(0xCF9, 0x06);
    for (volatile int i = 0; i < 1000000; i++);
}

void reboot_via_pci_reset(void) {
    outb(0xCF9, 0x0E);
    for (volatile int i = 0; i < 1000000; i++);
}

void reboot(void) {
    reboot_via_acpi();
    reboot_via_8042();
    for (volatile int i = 0; i < 5000000; i++);
    reboot_via_pci_reset();
    for (volatile int i = 0; i < 5000000; i++);
    trigger_triple_fault();
    __builtin_unreachable();
}
