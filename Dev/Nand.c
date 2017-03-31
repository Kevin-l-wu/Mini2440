#include "GlobalDefine.h"
#include "Common.h"
#include "Error.h"
#include "Framework.h"
#include "String.h"
#include "Getopt.h"
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
	printf_string("SelectChip: NFCONT = 0x%x\n", NFCONT);
	printf_string("SelectChip: NFSTAT = 0x%x\n", NFSTAT);
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
	NFCONT |= (1 << 1);
	printf_string("DeselectChip: NFCONT = 0x%x\n", NFCONT);
	printf_string("DeselectChip: NFSTAT = 0x%x\n", NFSTAT);
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
	print_string("Wati RB\n");

	while(!(NFSTAT & 0x04U))
	{
		;
	}
}

/*******************************************************************
 * Function Name: 	 	NandPageRead
 * Inputs : 			unsigned pageAddr, char* readData
 * Outputs : 			None.
 * Description:			Read one page data from Nand
 * Author:				Kevin
 *******************************************************************/
void NandPageRead(unsigned pageAddr, char* readData)
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
	printf_string("NandPageRead before WaitRB: NFSTAT = 0x%x\n", NFSTAT);
	printf_string("NandPageRead before WaitRB: NFCONT = 0x%x\n", NFCONT);
	WaitRB();
	printf_string("NandPageRead After WaitRB: NFSTAT = 0x%x\n", NFSTAT);
	printf_string("NandPageRead After WaitRB: NFCONT = 0x%x\n", NFCONT);
	
	//Get data
	for(i = 0; i < BYTES_PER_PAGE; i++)
	{
		readData[i] = NFDATA;
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
int NandProgramPage(unsigned pageAddr, char* writeData)
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
	for(i = 0; i < BYTES_PER_PAGE; i++)
	{
		NFDATA = writeData[i];
	}
	//SendCommand 0x10
	SendCommand(0x10);
	
	//Wait RB
	printf_string("NandProgramPage before WaitRB: NFSTAT = 0x%x\n", NFSTAT);
	printf_string("NandProgramPage before WaitRB: NFCONT = 0x%x\n", NFCONT);
	WaitRB();
	printf_string("NandProgramPage After WaitRB: NFSTAT = 0x%x\n", NFSTAT);
	printf_string("NandProgramPage After WaitRB: NFCONT = 0x%x\n", NFCONT);

	//Send command 0x70
	SendCommand(0x70);

	//Read result;
	ret = NFDATA;

	//Chip deselect
	DeselectChip();
	
	return ret;
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
	printf_string("NandEraseBlock before WaitRB: NFSTAT = 0x%x\n", NFSTAT);
	printf_string("NandEraseBlock before WaitRB: NFCONT = 0x%x\n", NFCONT);
	WaitRB();
	printf_string("NandEraseBlock After WaitRB: NFSTAT = 0x%x\n", NFSTAT);
	printf_string("NandEraseBlock After WaitRB: NFCONT = 0x%x\n", NFCONT);
	
	//Send command 0x70
	SendCommand(0x70);
	
	//Read result
	ret = NFDATA;

	//Chip deselect
	DeselectChip();
	
	return ret;
}

int ReadID(char* chipIDBuf)
{
	int i = 0;
	
	//Chip select
	SelectChip();
	
	//Send Command 0x90
	SendCommand(0x90);
	
	//Send address 0x00
	SendAddress(0x00);

	for (i = 0; i < 10; i++);
	//Read 5 data from NFDATA
	chipIDBuf[0] = NFDATA;
	chipIDBuf[1] = NFDATA;
	chipIDBuf[2] = NFDATA;
	chipIDBuf[3] = NFDATA;
	chipIDBuf[4] = NFDATA;
/*	
	for(i =  0; i < 5; i++)
	{
		chipIDBuf[i] = NFDATA;
	}
*/	
	//Chip deselect
	DeselectChip();
}

void NandReset()
{
	SelectChip();

	ClearRB();
	
	SendCommand(0xff);
	
	WaitRB();
	
	DeselectChip();
}

//Enable NandFlash controller
void NandControllerInit()
{
	print_string("\n\nNandControllerInit\n\n");

	NFCONF = (TACLS << 12) | (TWRPH0 << 8) | (TWRPH1 << 3) & (~1);
	NFCONT |= 0x11;
	NFCONT &= ~(1 << 12);

	printf_string("\n\nBefore Reset NandControllerInit: NFCONF = 0x%x\n\n", NFCONF);
	printf_string("\n\nBefore Reset NandControllerInit: NFCONT = 0x%x\n\n", NFCONT);

	NandReset();

	printf_string("\n\nAfter Reset NandControllerInit: NFCONF = 0x%x\n\n", NFCONF);
	printf_string("\n\nAfter Reset NandControllerInit: NFCONT = 0x%x\n\n", NFCONT);
}


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
		printf_string("0x%x ", valuePool[index]);
	}
}


void NandBlockEraseTest(int address)
{
	printf_string("\n\nNand block erase, address = %d\n\n", address);
	NandEraseBlock(address);
	print_string("\n\nBlock erase complete\n\n");
}

void NandPageWriteTest(int address, char writeValue)
{
	printf_string("\n\nNand Page Program, address = %d\n\n", address);
	printf_string("\n\nProgram value = %d\n\n", writeValue);
	
	int index = 0;
	char writeBuff[BYTES_PER_PAGE] = {0};
	
	for(index = 0; index < BYTES_PER_PAGE; index++)
	{
		writeBuff[index] = writeValue;
	}
	
	NandProgramPage(address, writeBuff);
	
	print_string("\n\nPage program complete\n\n");
}

void NandPageReadTest(int address)
{
	printf_string("\n\nNand Page Read, address = %d\n\n", address);
	char readBuff[BYTES_PER_PAGE] = {0};
	
	memset(readBuff, 0, BYTES_PER_PAGE);
	NandPageRead(address, readBuff);
	
	print_string("\n\nDump page value:\n\n", address);
	
	valueDump(readBuff, BYTES_PER_PAGE);
	
	
	print_string("\n\nPage read complete\n\n");
}

void NandReadChipId()
{
	print_string("\n\nStart read ID\n\n");
	int index = 0;
	char chipIDBuf[5] = {0};
	
	ReadID(chipIDBuf);
	print_string("chipID = ");
	for(index = 0; index < 5; index++)
	{
		printf_string("%x ", chipIDBuf[index]);
	}
	print_string("\n");
	
	print_string("\n\nRead ID complete\n\n");
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
	int value = 0;
	
	printf_string("\n\nargc = %d\n\n", argc);
		
	for(index = 0; index < argc; index++)
	{
		printf_string("argv[%d] = %s\n", index, argv[index]);
	}
	
	NandControllerInit();
	
	GetoptInit();
	
	while ((option = Getopt(argc, argv, "epri")) != 0)
	{
		switch(option)
		{
			case 'e':
				address = string_to_int(argv[optInd]);
				NandBlockEraseTest(address);
				break;
			
			case 'p':
				if(argc - optInd >= 1)
				{
					address = string_to_int(argv[optInd]);
					value = string_to_int(argv[optInd + 1]);
					
					NandPageWriteTest(address, value);
				}
				break;
				
			case 'r':
				if(argc - optInd >= 0)
				{
					address = string_to_int(argv[optInd]);
					NandPageReadTest(address);
				}
				break;
				
			case 'i':
				NandReadChipId();
				break;
				
			default:
				break;
		}
	}




/*

	char write_to_nand[BYTES_PER_PAGE] = {0};
	char write_to_nand1[BYTES_PER_PAGE] = {0xD0, 0xD5, 0xD7};
	char write_to_nand2[BYTES_PER_PAGE] = {0x3C};
	char read_from_nand[BYTES_PER_PAGE] = {0x00};
	int index = 0;
	int page_index = 0;
	int lineNumber = 0;
	char chipIDBuf[5] = {0};
	
	/************************** Read Nand ID ***********************
	print_string("\n\nStart Nand Test...\n\n");
	NandControllerInit();
	
	ReadID(chipIDBuf);
	print_string("chipID = ");
	for(index = 0; index < 5; index++)
	{
		printf_string("%x ", chipIDBuf[index]);
	}
	print_string("\n");
	
	/************************** Nand Block Test ***********************
	print_string("\n\nBlock 100\n\n");
	
	for(index = 0; index < BYTES_PER_PAGE; index++)
	{
		write_to_nand[index] = 0x55;
	}
	
	NandEraseBlock(BYTES_PER_PAGE * 99);
	NandEraseBlock(BYTES_PER_PAGE * 100);
	NandEraseBlock(BYTES_PER_PAGE * 101);
	
	for(page_index = 0; page_index < PAGES_PER_BLOCK + 4; page_index++)
	{
		printf_string("\n\nWrite to page %d\n\n", BYTES_PER_PAGE * 100 + page_index);
		NandProgramPage(BYTES_PER_PAGE * 100 + page_index, write_to_nand);
	}
	
	for(page_index = 0; page_index < PAGES_PER_BLOCK + 4; page_index++)
	{
		printf_string("\n\nWrite to page %d\n\n", BYTES_PER_PAGE * 100 + page_index);
		NandProgramPage(BYTES_PER_PAGE * 100 + page_index, write_to_nand);
	}

	for(page_index = 0; page_index < PAGES_PER_BLOCK + 4; page_index++)
	{
		memcpy(read_from_nand, '\0', BYTES_PER_PAGE);
		NandPageRead(BYTES_PER_PAGE * 100 + page_index - 2, read_from_nand);
		printf_string("\n\nAfter read from nand page %d\n\n", BYTES_PER_PAGE * 100 + page_index);
		lineNumber = 0;
		for(index = 0; index < BYTES_PER_PAGE; index++)
		{
			if(index % 8 == 0)
			{
				printf_string("\nLine%d :", lineNumber);
				lineNumber++;
			}
			printf_string("0x%x ", read_from_nand[index]);
		}
	}
	
*/
	print_string("Nand Test end\n");

	return status;
}
