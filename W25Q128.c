#include "stm32f10x.h"
#include "W25Q128JV.h"
#include "gpio_drive.h"



void SPI_Configuration(void)
{
    // Enable SPI1 and GPIOA clocks
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

    // Configure SPI pins (SCK, MISO, MOSI)
    GPIOA->CRL &= ~GPIO_CRL_CNF5; // Clear CNF5 bits
    GPIOA->CRL |= GPIO_CRL_CNF5_1; // Set CNF5[1] bit to 1 (Alternate function, push-pull)
    GPIOA->CRL |= GPIO_CRL_MODE5; // Set MODE5 bits to 11 (Output mode, 50 MHz)

    GPIOA->CRL &= ~GPIO_CRL_CNF6; // Clear CNF6 bits
    GPIOA->CRL |= GPIO_CRL_CNF6_0; // Set CNF6[0] bit to 0 (Input floating)
    GPIOA->CRL &= ~GPIO_CRL_MODE6; // Clear MODE6 bits (Input mode)

    GPIOA->CRL &= ~GPIO_CRL_CNF7; // Clear CNF7 bits
    GPIOA->CRL |= GPIO_CRL_CNF7_1; // Set CNF7[1] bit to 1 (Alternate function, push-pull)
    GPIOA->CRL |= GPIO_CRL_MODE7; // Set MODE7 bits to 11 (Output mode, 50 MHz)

    // Configure CS pin
    GPIOA->CRL &= ~GPIO_CRL_CNF4; // Clear CNF4 bits
    GPIOA->CRL |= GPIO_CRL_MODE4; // Set MODE4 bits to 11 (Output mode, 50 MHz)

    // SPI configuration
    SPI1->CR1 = 0; // Clear CR1 register
    SPI1->CR1 |= SPI_CR1_MSTR; // Set MSTR bit (Master mode)
    SPI1->CR1 |= SPI_CR1_SSM; // Set SSM bit (Software slave management)
    SPI1->CR1 |= SPI_CR1_SSI; // Set SSI bit (Internal slave select)
    SPI1->CR1 |= SPI_CR1_BR_2; // Set BR[2] bit to 1 (fPCLK/256 baud rate)
    SPI1->CR1 |= SPI_CR1_CPOL; // Set CPOL bit (Clock polarity: Idle low)
    SPI1->CR1 |= SPI_CR1_CPHA; // Set CPHA bit (Clock phase: First edge)
    SPI1->CR1 |= SPI_CR1_SPE; // Set SPE bit (SPI enable)
}

void SPI_SelectChip(void)
{
    GPIOA->BRR = SPI_CS_PIN; // Reset CS pin
		
}

void SPI_DeselectChip(void)
{
    GPIOA->BSRR = SPI_CS_PIN; // Set CS pin
	
}

void SPI_SendByte(uint8_t byte)
{
    SPI1->DR = byte; // Write data to the SPI data register
    while (!(SPI1->SR & SPI_SR_TXE)) // Wait for TXE bit (Transmit buffer empty)
        ;
    while (SPI1->SR & SPI_SR_BSY) // Wait for BSY bit (SPI is busy)
        ;
    volatile uint8_t dummy = SPI1->DR; // Read the received data to clear the RXNE flag
}

uint8_t SPI_ReceiveByte(void)
{
    SPI_SendByte(0xFF); // Send dummy byte to receive data
    return SPI1->DR; // Read the received data from the SPI data register
}

void SPI_SendCommand(uint8_t command)
{
    SPI_SelectChip();
    SPI_SendByte(command);
    SPI_DeselectChip();
}

uint8_t SPI_ReadStatusRegister1(void)
{
    uint8_t status;
    SPI_SelectChip();
    SPI_SendByte(CMD_READ_STATUS_REG1);
    status = SPI_ReceiveByte();
    SPI_DeselectChip();
    return status;
}

void SPI_WriteEnable(void)
{
    SPI_SelectChip();
    SPI_SendByte(CMD_WRITE_ENABLE);
    SPI_DeselectChip();
}

void SPI_WriteDisable(void)
{
    SPI_SelectChip();
    SPI_SendByte(CMD_WRITE_DISABLE);
    SPI_DeselectChip();
}

void SPI_WaitForReady(void)
{
    while (SPI_ReadStatusRegister1() & STATUS_REG1_BUSY)
        ;
}

void SPI_PageProgram(uint32_t address, uint8_t *data, uint32_t length)
{
    SPI_WriteEnable();
    SPI_SelectChip();
    SPI_SendByte(CMD_PAGE_PROGRAM);
    SPI_SendByte((address >> 16) & 0xFF);
    SPI_SendByte((address >> 8) & 0xFF);
    SPI_SendByte(address & 0xFF);
    while (length--)
    {
        SPI_SendByte(*data++);
    }
    SPI_DeselectChip();
    SPI_WaitForReady();
}

void SPI_SectorErase(uint32_t address)
{
    SPI_WriteEnable();
    SPI_SelectChip();
    SPI_SendByte(CMD_SECTOR_ERASE);
    SPI_SendByte((address >> 16) & 0xFF);
    SPI_SendByte((address >> 8) & 0xFF);
    SPI_SendByte(address & 0xFF);
    SPI_DeselectChip();
    SPI_WaitForReady();
}

void SPI_ChipErase(void)
{
    SPI_WriteEnable();
    SPI_SelectChip();
    SPI_SendByte(CMD_CHIP_ERASE);
    SPI_DeselectChip();
    SPI_WaitForReady();
}

void SPI_PowerDown(void)
{
    SPI_SelectChip();
    SPI_SendByte(CMD_POWER_DOWN);
    SPI_DeselectChip();
}

void SPI_ReleasePowerDown(void)
{
    SPI_SelectChip();
    SPI_SendByte(CMD_RELEASE_POWER_DOWN);
    SPI_DeselectChip();
    // Wait for flash to exit power-down mode
    for (uint32_t i = 0; i < 1000; i++)
        __NOP();
}

void SPI_Init(void)
{
    SPI_Configuration();
    SPI_ReleasePowerDown();
}


