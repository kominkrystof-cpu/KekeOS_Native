#ifndef REBOOT_H
#define REBOOT_H

#include <stdint.h>

void reboot(void);
void reboot_via_8042(void);
void reboot_via_triple_fault(void);
void reboot_via_acpi(void);
void reboot_via_pci_reset(void);

#endif
