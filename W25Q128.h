#include "stm32f10x.h"

#ifndef W25Q128JV_H
#define W25Q128JV_H
#define SPI_TIMEOUT 1000 // Timeout for SPI operations in milliseconds

// Definitions for the SPI peripheral used
#define SPI_PORT GPIOA
#define SPI_CS_PIN GPIO_Pin_4
#define SPI_SCK_PIN GPIO_Pin_5
#define SPI_MISO_PIN GPIO_Pin_6
#define SPI_MOSI_PIN GPIO_Pin_7

// W25Q128JV Instructions
#define CMD_WRITE_ENABLE 0x06
#define CMD_WRITE_DISABLE 0x04
#define CMD_READ_STATUS_REG1 0x05
#define CMD_WRITE_STATUS_REG1 0x01
#define CMD_READ_DATA 0x03
#define CMD_PAGE_PROGRAM 0x02
#define CMD_SECTOR_ERASE 0x20
#define CMD_CHIP_ERASE 0xC7
#define CMD_POWER_DOWN 0xB9
#define CMD_RELEASE_POWER_DOWN 0xAB
#define STATUS_REG1_BUSY (1 << 0)



void SPI_Configuration(void);
void SPI_SelectChip(void);
void SPI_DeselectChip(void);
void SPI_SendByte(uint8_t byte);
uint8_t SPI_ReceiveByte(void);
void SPI_SendCommand(uint8_t command);
uint8_t SPI_ReadStatusRegister1(void);
void SPI_WriteEnable(void);
void SPI_WriteDisable(void);
void SPI_WaitForReady(void);
void SPI_PageProgram(uint32_t address, uint8_t *data, uint32_t length);
void SPI_SectorErase(uint32_t address);
void SPI_ChipErase(void);
void SPI_PowerDown(void);
void SPI_ReleasePowerDown(void);
void SPI_Init(void);


#endif /* W25Q128JV_H */