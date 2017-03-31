#ifndef TFTP_H_
#define TFTP_H_

typedef struct _TFTP_PACKAGE
{
	u16 opcode;
	u16 blocknum;
	u8 data[0];
} TFTP_PACKAGE;

MINI2440_STATUS tftp_test(int argc, char(*argv)[MAX_COMMAND_LENGTH]);

#endif