#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#define MAX_COMMAND_NUMBER 10
#define MAX_COMMAND_LENGTH 10

typedef struct _diags_commonds
{
	char* cmmond_string;
	char* message;
	void(*function)(int argc, char(*argv)[MAX_COMMAND_LENGTH]);
} Diasg_Commonds;

#endif