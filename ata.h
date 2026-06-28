#ifndef ATA_H
#define ATA_H

#include <stdint.h>

// ATA I/O porty (Primary bus)
#define ATA_DATA_REG       0x1F0
#define ATA_ERROR_REG      0x1F1
#define ATA_SECTOR_COUNT   0x1F2
#define ATA_LBA_LOW       0x1F3
#define ATA_LBA_MID        0x1F4
#define ATA_LBA_HIGH       0x1F5
#define ATA_DRIVE_SELECT   0x1F6
#define ATA_COMMAND_REG    0x1F7
#define ATA_STATUS_REG     0x1F7

// ATA příkazy
#define ATA_CMD_READ_SECTORS 0x20

// ATA status bity
#define ATA_STATUS_BSY  0x80  // Busy
#define ATA_STATUS_DRQ  0x08  // Data Request Ready
#define ATA_STATUS_ERR  0x01  // Error

// MBR boot signatura
#define MBR_SIGNATURE 0xAA55

// Přečte jeden 512-bajtový sektor z disku do bufferu
void ata_read_sector(uint32_t lba, uint16_t* buffer);

#endif
