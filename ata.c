#include "ata.h"

// Čtení 8-bit hodnoty z I/O portu
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Zápis 8-bit hodnoty do I/O portu
static inline void outb(uint16_t port, uint8_t data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

// Čtení 16-bit hodnoty z I/O portu (pro DATA port)
static inline uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Čekání na status bit
static void ata_wait(uint8_t mask, uint8_t value) {
    // Kontrola zda disk existuje - pokud status port vrací 0xFF nebo 0x00, disk není připojen
    uint8_t status = inb(ATA_STATUS_REG);
    if (status == 0xFF || status == 0x00) {
        return;
    }
    while ((inb(ATA_STATUS_REG) & mask) != value);
}

void ata_read_sector(uint32_t lba, uint16_t* buffer) {
    // Kontrola zda disk existuje - pokud status port vrací 0xFF nebo 0x00, disk není připojen
    uint8_t status = inb(ATA_STATUS_REG);
    if (status == 0xFF || status == 0x00) {
        // Disk neexistuje - vyplnit buffer nulami a vrátit
        for (int i = 0; i < 256; i++) {
            buffer[i] = 0;
        }
        return;
    }

    // 1. Čekat na BSY = 0 (disk není zaneprázdněn)
    uint8_t status_check;
    while (((status_check = inb(ATA_STATUS_REG)) & ATA_STATUS_BSY) && (status_check != 0xFF) && (status_check != 0x00));

    // 2. Nastavit počet sektorů (1)
    outb(ATA_SECTOR_COUNT, 1);

    // 3. Nastavit LBA adresu
    outb(ATA_LBA_LOW, lba & 0xFF);
    outb(ATA_LBA_MID, (lba >> 8) & 0xFF);
    outb(ATA_LBA_HIGH, (lba >> 16) & 0xFF);

    // 4. Nastavit drive select (master, LBA mode)
    // Bit 4 = 0 (master), Bit 6 = 1 (LBA mode), Bit 7 = 1 (LBA28)
    outb(ATA_DRIVE_SELECT, 0xE0 | ((lba >> 24) & 0x0F));

    // 5. Poslat READ SECTORS příkaz
    outb(ATA_COMMAND_REG, ATA_CMD_READ_SECTORS);

    // 6. Čekat na BSY = 0 a DRQ = 1 (data připravena)
    while (1) {
        uint8_t status = inb(ATA_STATUS_REG);
        if (status == 0xFF || status == 0x00) {
            // Disk není připojen - vyplnit buffer nulami a vrátit
            for (int i = 0; i < 256; i++) {
                buffer[i] = 0;
            }
            return;
        }
        if (!(status & ATA_STATUS_BSY) && (status & ATA_STATUS_DRQ)) {
            break;
        }
        if (status & ATA_STATUS_ERR) {
            // Chyba - vyplnit buffer nulami
            for (int i = 0; i < 256; i++) {
                buffer[i] = 0;
            }
            return;
        }
    }

    // 7. Přečíst 256 slov (512 bajtů) z DATA portu
    for (int i = 0; i < 256; i++) {
        buffer[i] = inw(ATA_DATA_REG);
    }

    // 8. Čekat na BSY = 0 (operace dokončena)
    while (((status_check = inb(ATA_STATUS_REG)) & ATA_STATUS_BSY) && (status_check != 0xFF) && (status_check != 0x00));
}
