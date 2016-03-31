#ifndef ERROR_H_
#define ERROR_H_

typedef enum _MINI2440_STATUS
{
	MINI2440_SUCCESS,
	MINI2440_INVALID_PARAMETER,
} MINI2440_STATUS;

void ErrorCheck(MINI2440_STATUS mini2440Status); 

#endif