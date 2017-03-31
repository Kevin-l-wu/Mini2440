#include <stdio.h>
#include "GlobalDefine.h"
#include "Common.h"
#include "Error.h"
#include "Framework.h"
#include "String.h"
#include "GetoptTemp.h"
#include "Nand.h"


/*******************************************************************
 * Function Name: 	 	SelectChip
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Select Nand Chip
 * Author:				Kevin
 *******************************************************************/
void SelectChip()
{
	NFCONT &= (~(1 << 1));
}

/*******************************************************************
 * Function Name: 	 	DeselectChip
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Deselect Nand Chip
 * Author:				Kevin
 *******************************************************************/
void DeselectChip()
{
	delay(10);
	NFCONT |= (1 << 1);
}

/*******************************************************************
 * Function Name: 	 	ClearRB
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Clear RB
 * Author:				Kevin
 *******************************************************************/
void ClearRB()
{
	delay(10);
	NFSTAT |= 0x04U;
	printf_string("ClearRB: NFCONT = 0x%x\n", NFCONT);
	printf_string("ClearRB: NFSTAT = 0x%x\n", NFSTAT);
}

/*******************************************************************
 * Function Name: 	 	SendCommand
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Send command to Nand
 * Author:				Kevin
 *******************************************************************/
void SendCommand(unsigned command)
{
	NFCMMD = command;
}

/*******************************************************************
 * Function Name: 	 	SendAddress
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Send address to Nand
 * Author:				Kevin
 *******************************************************************/
void SendAddress(unsigned address)
{
	NFADDR = address;
}

/*******************************************************************
 * Function Name: 	 	WaitRB
 * Inputs : 			None
 * Outputs : 			None.
 * Description:			Wait RB is not busy
 * Author:				Kevin
 *******************************************************************/
void WaitRB()
{
	while(!(NFSTAT & 0x04U))
	{
		;
	}
}

void InitEcc()
{
	NFCONT |= (1 << 4); 
//	printf_string("\nInitEcc: NFCONT = 0x%x\n", NFCONT);
}

void LockMainEcc()
{
	NFCONT |= (1 << 5); 
//	printf_string("\nLockMainEcc: NFCONT = 0x%x\n", NFCONT);
}

void UnlockMainEcc()
{
	NFCONT &= ~(1 << 5); 
//	printf_string("\nUnlockMainEcc: NFCONT = 0x%x\n", NFCONT);
}

void LockSpareEcc()
{
	NFCONT |= (1 << 6); 
//	printf_string("\nLockSpareEcc: NFCONT = 0x%x\n", NFCONT);
}

void UnlockSpareEcc()
{
	NFCONT &= ~(1 << 6); 
//	printf_string("\nUnlockSpareEcc: NFCONT = 0x%x\n", NFCONT);
}

/*******************************************************************
 * Function Name: 	 	NandPageRead
 * Inputs : 			unsigned pageAddr, char* readData
 * Outputs : 			None.
 * Description:			Read one page data from Nand
 * Author:				Kevin
 *******************************************************************/
void NandPageRead(unsigned pageAddr, unsigned* readData)
{
	int i = 0;
	
	//Select NandFlash
	SelectChip();

	//Clear R/B
	ClearRB();
	
	//Send page read start command 0x00
	SendCommand(0x00);

	//Send column address
	SendAddress(0x00);
	SendAddress(0x00);
	
	//Send row address
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);
	
	//Send page read end command 0x30
	SendCommand(0x30);
	
	//Wait R/B
//	printf_string("NandPageRead before WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf_string("NandPageRead before WaitRB: NFCONT = 0x%x\n", NFCONT);
	WaitRB();
//	printf_string("NandPageRead After WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf_string("NandPageRead After WaitRB: NFCONT = 0x%x\n", NFCONT);
	
	//Get data
	for(i = 0; i < (BYTES_PER_PAGE / 4); i++)
	{
		readData[i] = NFDATA;
		data_adjustment((unsigned*)(&readData[i])); //Adjustment data
	}

	//Deselect NandFlash
	DeselectChip();
}


/*******************************************************************
 * Function Name: 	 	NandRandomPageRead
 * Inputs : 			unsigned pageAddr, char* readData
 * Outputs : 			None.
 * Description:			Read one page data from Nand
 * Author:				Kevin
 *******************************************************************/

void NandRandomPageRead(unsigned pageAddr, unsigned columnAddr, unsigned* readData, unsigned length)
{
	int index = 0;
	
	unsigned realReadLenth = length;
	
	//If read data > one page, should reduce length
	if(realReadLenth > (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) - columnAddr)
	{
		realReadLenth = (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) - columnAddr;
	}
	
	printf_string("\n\nNandRandomPageRead pageAddress(hex) = 0x%x\n\n", pageAddr);
	printf_string("\n\nNandRandomPageRead columnAddress(hex) = 0x%x\n\n", columnAddr);
	printf_string("\n\nNandRandomPageRead realReadLenth(hex) = 0x%x\n\n", realReadLenth);
	
	//Select NandFlash
	SelectChip();

	//Clear R/B
	ClearRB();
	
	//Send page read start command 0x00
	SendCommand(0x00);
	
	//Send column address
	SendAddress(columnAddr & 0xff);
	SendAddress((columnAddr >> 8) & 0xff);
	
	//Send row address
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);
	
	//Send page read end command 0x30
	SendCommand(0x30);
	
	//Wait R/B
//	printf_string("NandPageRead before WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf_string("NandPageRead before WaitRB: NFCONT = 0x%x\n", NFCONT);
	WaitRB();
//	printf_string("NandPageRead After WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf_string("NandPageRead After WaitRB: NFCONT = 0x%x\n", NFCONT);

	//Send command 0x05
	SendCommand(0x05);
	
	//Send column address
	SendAddress(columnAddr & 0xff);
	SendAddress((columnAddr >> 8) & 0xff);
	
	//Send command 0xE0
	SendCommand(0xE0);
	
	//Get data
	for(index = 0; index < MAX_MAGNIFICATION(realReadLenth, 4); index++)
	{
		readData[index] = NFDATA;
		data_adjustment((unsigned*)(&readData[index])); //Adjustment data
	}

	//Deselect NandFlash
	DeselectChip();
}

/*******************************************************************
 * Function Name: 	 	NandProgramPage
 * Inputs : 			unsigned pageAddr, char* writeData
 * Outputs : 			int.
 * Description:			Write one page data to nand
 * Author:				Kevin
 *******************************************************************/
int NandProgramPage(unsigned pageAddr, unsigned* writeData)
{
	int ret;
	int i = 0;

	//Chip select
	SelectChip();
	
	//Clear RB
	ClearRB();
	
	//Send command 0x80
	SendCommand(0x80);
	
	//Send Address
	SendAddress(0x00);
	SendAddress(0x00);
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);
	
	//Write data
	for(i = 0; i < BYTES_PER_PAGE / 4; i++)
	{
		NFDATA = writeData[i];
	}
	//SendCommand 0x10
	SendCommand(0x10);
	
	//Wait RB
//	printf_string("NandProgramPage before WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf_string("NandProgramPage before WaitRB: NFCONT = 0x%x\n", NFCONT);
	WaitRB();
//	printf_string("NandProgramPage After WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf_string("NandProgramPage After WaitRB: NFCONT = 0x%x\n", NFCONT);

	//Send command 0x70
	SendCommand(0x70);

	//Read result;
	ret = NFDATA;

	//Chip deselect
	DeselectChip();
	
	return ret;
}


/*******************************************************************
 * Function Name: 	 	NandRandomPageProgram
 * Inputs : 			unsigned pageAddr, char* readData
 * Outputs : 			None.
 * Description:			Read one page data from Nand
 * Author:				Kevin
 *******************************************************************/

void NandRandomPageProgram(unsigned pageAddr, unsigned columnAddr, unsigned* writeData, unsigned length)
{
	int index = 0;
	
	unsigned realProgramLenth = length;
	
	//If read data > one page, should reduce length
	if(realProgramLenth > (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) - columnAddr)
	{
		realProgramLenth = (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) - columnAddr;
	}
	
	printf_string("\n\npageAddress = 0x%x\n\n", pageAddr);
	printf_string("\n\ncolumnAddress = 0x%x\n\n", columnAddr);
	printf_string("\n\nrealProgramLenth = 0x%x\n\n", realProgramLenth);
	
	//Select NandFlash
	SelectChip();

	//Clear R/B
	ClearRB();
	
	//Send page program start command 0x80
	SendCommand(0x80);
	
	//Send column address
	SendAddress(columnAddr & 0xff);
	SendAddress((columnAddr >> 8) & 0xff);
	
	//Send row address
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);

	//Send command 0x85
	SendCommand(0x85);
	
	//Send column address
	SendAddress(columnAddr & 0xff);
	SendAddress((columnAddr >> 8) & 0xff);
	
	//Get data
	for(index = 0; index < MAX_MAGNIFICATION(realProgramLenth, 4); index++)
	{
		NFDATA = writeData[index];
	}

	//Send command 0x10
	SendCommand(0x10);
	
	//Deselect NandFlash
	DeselectChip();
}

/*******************************************************************
 * Function Name: 	 	NandEraseBlock
 * Inputs : 			unsigned long pageAddr
 * Outputs : 			int
 * Description:			Erase the specify nand block
 * Author:				Kevin
 *******************************************************************/
int NandEraseBlock(unsigned pageAddr)
{
	int ret;
	//Chip select
	SelectChip();
	
	//Clear RB
	ClearRB();
	
	//Send command 0x60
	SendCommand(0x60);
	
	//Send page address
	SendAddress(pageAddr & 0xff);
	SendAddress((pageAddr >> 8) & 0xff);
	SendAddress((pageAddr >> 16) & 0xff);

	//Send command 0xd0
	SendCommand(0xd0);
	
	//Wait RB
//	printf_string("NandEraseBlock before WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf_string("NandEraseBlock before WaitRB: NFCONT = 0x%x\n", NFCONT);
	WaitRB();
//	printf_string("NandEraseBlock After WaitRB: NFSTAT = 0x%x\n", NFSTAT);
//	printf_string("NandEraseBlock After WaitRB: NFCONT = 0x%x\n", NFCONT);
	
	//Send command 0x70
	SendCommand(0x70);
	
	//Read result
	delay(10);
	ret = NFDATA;

	//Chip deselect
	DeselectChip();
	
	return ret;
}

/*******************************************************************
 * Function Name: 	 	ReadID
 * Inputs : 			unsigned* chipIDBuf
 * Outputs : 			int
 * Description:			Read nand flash ID
 * Author:				Kevin
 *******************************************************************/
int ReadID(unsigned* chipIDBuf)
{
	int i = 0;
	
	//Chip select
	SelectChip();
	
	//Send Command 0x90
	SendCommand(0x90);
	
	//Send address 0x00
	SendAddress(0x00);

	//Read id data from NFDATA(2 cycle and read 8 Byte, while the id is 5 Byte)
	for(i = 0; i < 2; i++)
	{
		chipIDBuf[i] = NFDATA;
	}
	
	//Chip deselect
	DeselectChip();
	
	return 0;
}

/*******************************************************************
 * Function Name: 	 	NandReset
 * Inputs : 			None
 * Outputs : 			void
 * Description:			Reset nand flash
 * Author:				Kevin
 *******************************************************************/
void NandReset()
{
	SelectChip();

	ClearRB();
	
	SendCommand(0xff);
	
	WaitRB();
	
	DeselectChip();
}

/*******************************************************************
 * Function Name: 	 	NandControllerInit
 * Inputs : 			None
 * Outputs : 			void
 * Description:			Enable NandFlash controller
 * Author:				Kevin
 *******************************************************************/
void NandControllerInit()
{
//	print_string("\n\nNandControllerInit\n\n");

	/*
	1. NFCONF: 
		Set TACLS , TWRPH0, TWRPH1, duration should >0ns, >12ns, >5ns; While HCLK = 100MHz, One cycle is 10 ns
	2. NFCONT: 
		bit0 set to 1, enable Nand flash controller
		bit1 set to 1, disable chip select
		bit12 set to 0, disable soft lock
	3. Reset Nand flash
	*/
	NFCONF = (TACLS << 12) | (TWRPH0 << 8) | (TWRPH1 << 3) & (~1);
	NFCONT |= 0x11;
	NFCONT &= ~(1 << 12);

//	printf_string("\n\nBefore Reset NandControllerInit: NFCONF = 0x%x\n\n", NFCONF);
//	printf_string("\n\nBefore Reset NandControllerInit: NFCONT = 0x%x\n\n", NFCONT);

	NandReset();

//	printf_string("\n\nAfter Reset NandControllerInit: NFCONF = 0x%x\n\n", NFCONF);
//	printf_string("\n\nAfter Reset NandControllerInit: NFCONT = 0x%x\n\n", NFCONT);
}

/*******************************************************************
 * Function Name: 	 	valueDump
 * Inputs : 			char *valuePool, int dumpLength
 * Outputs : 			void
 * Description:			Dump values
 * Author:				Kevin
 *******************************************************************/
void valueDump(char *valuePool, int dumpLength)
{
	int index = 0;
	int lineNumber = 0;
	
	for(index = 0; index < dumpLength; index++)
	{
		if(index % 8 == 0)
		{
			printf_string("\nLine%d :", lineNumber);
			lineNumber++;
		}
		printf_string("0x%C ", valuePool[index]);
	}
}

void AddrConvert(unsigned address, unsigned* pageAddr, unsigned* columnAddr, int isRandomOperation)
{
	unsigned base = BYTES_PER_PAGE;
	
	if(pageAddr != NULL && columnAddr != NULL)
	{
		if(isRandomOperation)
		{
			base = base + SPARE_BYTES_PER_PAGE;
		}
		
		*pageAddr = address / base;
		*columnAddr = address % base;
		printf_string("\n\nAfter convert, pageAddr(hex) = 0x%x\n\n", *pageAddr);
		printf_string("\n\nAfter convert, columnAddr(hex) = 0x%x\n\n", *columnAddr);
	}
}

void NandReadChipId()
{
	print_string("\n\nStart read ID\n\n");
	int index = 0;
	unsigned chipIDBuf[2] = {0};
	
	ReadID(chipIDBuf);
	
	print_string("chipID = ");
	
	for(index = 0; index < 5; index++)
	{
		printf_string("0x%C ", ((char*)chipIDBuf)[index]);
	}
	print_string("\n");
	print_string("\n\nRead ID complete\n\n");
}

void NandBlockEraseTest(int address)
{
	printf_string("\n\nNandBlockEraseTest, address(hex) = 0x%x\n\n", address);
	
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	NandEraseBlock(pageAddr);
	print_string("\n\nBlock erase complete\n\n");
}

void NandPageWriteTest(int address, unsigned writeValue)
{
	printf_string("\n\nNandPageWriteTest, address(hex) = 0x%x\n\n", address);
	printf_string("\n\nProgram value(hex) = 0x%x\n\n", writeValue);
	
	unsigned writeBuff[(BYTES_PER_PAGE / 4)] = {0};
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	int index = 0;
	
	for(index = 0; index < (BYTES_PER_PAGE / 4); index++)
	{
		writeBuff[index] = writeValue;
	}
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	NandProgramPage(pageAddr, writeBuff);
	
	print_string("\n\nPage program complete\n\n");
}

void NandPageReadTest(int address)
{
	printf_string("\n\nNandPageReadTest, address(hex) = %x\n\n", address);
	
	unsigned readBuff[(BYTES_PER_PAGE / 4)] = {0};
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	
	
	memset(readBuff, 0, (BYTES_PER_PAGE / 4));
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	NandPageRead(pageAddr, readBuff);
	
	printf_string("\n\nDump page(hex) %x value:\n\n", pageAddr);
	
	valueDump((char*)readBuff, BYTES_PER_PAGE);
	
	
	print_string("\n\nPage read complete\n\n");
}


void NandRandomPageReadTest(int address, int length)
{
	printf_string("\n\nNand Random Page Read, address(hex) = 0x%x\n\n", address);
	
	unsigned readBuff[((BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) / 4)] = {0};
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;

	memset(readBuff, 0, (BYTES_PER_PAGE + SPARE_BYTES_PER_PAGE) / 4);
	
	AddrConvert(address, &pageAddr, &columnAddr, 1);
	NandRandomPageRead(pageAddr, columnAddr, readBuff, length);
	
	printf_string("\n\nDump page(hex) 0x%x value:\n\n", pageAddr);
	
	valueDump((char*)readBuff, length);
	
	print_string("\n\nPage random read complete\n\n");
}

void NandRandomPageProgramTest(int address, unsigned value)
{
	printf_string("\n\nNand Random Page program, address(hex) = 0x%x\n\n", address);

	unsigned pageAddr = 0;
	unsigned columnAddr = 0;

	AddrConvert(address, &pageAddr, &columnAddr, 1);
	NandRandomPageProgram(pageAddr, columnAddr, &value, 1);
	
	print_string("\n\nPage random program complete\n\n");
}

void BadBlockCheck()
{
	unsigned blockIndex = 0;
	unsigned pageIndex = 0;
	unsigned invidFlag1 = 0;
	unsigned invidFlag2 = 0;
	unsigned badBlockTable[100] = {0};
	unsigned badBlockIndex = 0;
	
	// Check Initial Invalid Block BLOCK_PER_DEVICE
	for(blockIndex = 0; blockIndex < BLOCK_PER_DEVICE; blockIndex++)
	{
		NandEraseBlock(blockIndex * PAGES_PER_BLOCK);

		NandRandomPageRead(blockIndex * PAGES_PER_BLOCK, 2048, &invidFlag1, 1);
		
		NandRandomPageRead(blockIndex * PAGES_PER_BLOCK, 2049, &invidFlag2, 1);
		
		
		printf_string("\n\n\nBlock : %x\n\n\n", blockIndex);
		printf_string("\ninvidFlag1 = %x\n", invidFlag1);
		printf_string("\ninvidFlag2 = %x\n", invidFlag2);
		
		if((invidFlag1 & 0xff) != 0xff || (invidFlag2 & 0xff) != 0xff)
		{
			badBlockTable[badBlockIndex] = blockIndex;
			badBlockIndex++;
		}
		
		if(badBlockIndex >= 100)
		{
			break;
		}
	}
	
	//List init bad block
	print_string("Bad block: ");
	for(blockIndex = 0; blockIndex < badBlockIndex; blockIndex++)
	{
		printf_string("0x%x ", badBlockTable[blockIndex]);
	}
	print_string("\n");
}



void CopyNandToRam(int address, char* sdramAddr, int size)
{
	unsigned pageAddr = 0;
	unsigned columnAddr = 0;
	unsigned pageIndex = 0;	
	unsigned copyPages = (size / BYTES_PER_PAGE) + 1;
	
	AddrConvert(address, &pageAddr, &columnAddr, 0);
	
	for(pageIndex = 0; pageIndex < copyPages; pageIndex++)
	{	
		NandPageRead(pageIndex + pageAddr, (unsigned*)(sdramAddr + (BYTES_PER_PAGE * pageIndex)));
	}
}


/*******************************************************************
 * Function Name: 	 	NandTest
 * Inputs : 			argc, argv
 * Outputs : 			Status
 * Description:			Run nand flash test
 * Author:				Kevin
 *******************************************************************/
MINI2440_STATUS NandTest(int argc, char (*argv)[MAX_COMMAND_LENGTH])
{
	MINI2440_STATUS status = MINI2440_SUCCESS;
	char option = 0;
	int index = 0;
	
	int address = 0;
	int length = 0;
	int value = 0;
	
	printf_string("\n\nargc = %d\n\n", argc);
		
	for(index = 0; index < argc; index++)
	{
		printf_string("argv[%d] = %s\n", index, argv[index]);
	}
	
	NandControllerInit();
	
	GetoptInit();
	
	while ((option = Getopt(argc, argv, "epritca")) != 0)
	{
		switch(option)
		{
			case 'e':
				address = hex_string_to_int(argv[optInd]);
				NandBlockEraseTest(address);
				break;
			
			case 'p':
				if(argc - optInd >= 1)
				{
					address = hex_string_to_int(argv[optInd]);
					printf_string("\n\nargv[optInd + 1] = %s\n\n", argv[optInd + 1]);
					value = hex_string_to_int(argv[optInd + 1]);
					printf_string("\n\nwrite value(hex) = 0x%x\n\n", value);
					NandPageWriteTest(address, value);
				}
				break;
				
			case 'r':
				if(argc - optInd >= 0)
				{
					address = hex_string_to_int(argv[optInd]);
					NandPageReadTest(address);
				}
				break;
				
			case 'i':
				NandReadChipId();
				break;
				
			case 't':
				if(argc - optInd >= 1)
				{
					address = hex_string_to_int(argv[optInd]);
					printf_string("Address(hex) = 0x%x\n", address);
					
					length = hex_string_to_int(argv[optInd + 1]);
					printf_string("Length(hex) = 0x%x\n", length);
					
					NandRandomPageReadTest(address, length);
				}
				break;
				
			case 'c':
				BadBlockCheck();
				break;

			case 'a':
				if(argc - optInd >= 1)
				{
					address = hex_string_to_int(argv[optInd]);
					
					printf_string("\n\nargv[optInd + 1] = %s\n\n", argv[optInd + 1]);
					value = hex_string_to_int(argv[optInd + 1]);
					printf_string("\n\nwrite value(hex) = %x\n\n", value);
				
					NandRandomPageProgramTest(address, value);
				}
				break;
				
			default:
				printf_string("Invalid Parameter\n");
				break;
		}
	}

	print_string("Nand Test end\n");

	return status;
}