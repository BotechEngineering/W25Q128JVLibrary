# W25Q128JVLibrary
Basic Library


Usage Example

SPI_Init();
SPI_WriteEnable();

	uint32_t sectorAddress = 0x0000000; // Address of the sector to erase
	SPI_SectorErase(sectorAddress);
	uint32_t pageAddress = 0x00000000; // Address of the page to write


	
	char data[] = "Testing String"; // Data to write
	uint32_t dataSize = sizeof(data);
	SPI_PageProgram(pageAddress, data, dataSize);
	Print("Set Value:\n");
	Print(1,data);
	
	//-***************************
	uint32_t readAddress = 0x00000000; // Address to read from
	char readData[50]; // Buffer to store read data
	SPI_SelectChip();
	SPI_SendByte(CMD_READ_DATA);
	SPI_SendByte((readAddress >> 16) & 0xFF);
	SPI_SendByte((readAddress >> 8) & 0xFF);
	SPI_SendByte(readAddress & 0xFF);
	for (uint32_t i = 0; i < sizeof(readData)+500; i++)
	{
		readData[i] = SPI_ReceiveByte();
	}
	
	SPI_DeselectChip();

	// Disable write operations
	SPI_WriteDisable();
	

	
	Print(readData);
