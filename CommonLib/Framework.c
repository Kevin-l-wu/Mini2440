#include "Framework.h"
#include "Led.h"

void print_diags_ver(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	print_string("Ver: 20150625\n");
}

void print_diags_sn(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	print_string("SN: C39HGFG00FF\n");
}

void print_help_message();


Diasg_Commonds gdiags_commonds[] = {
	{"ver", 	"diags version", 	print_diags_ver		},
	{"sn", 		"diags sn", 		print_diags_sn		},
	{"led", 	"Led test", 		led_test			},
	{"help", 	"Help message", 	print_help_message	},
};

void print_help_message(int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	unsigned index = 0;
	
	for(index = 0; index < sizeof(gdiags_commonds)/sizeof(gdiags_commonds[0]); index++)
	{
		print_string(gdiags_commonds[index].cmmond_string);
		print_string("<-->");
		print_string(gdiags_commonds[index].message);
		print_string("\n");
	}
	
}

void handle_event(char receive_from_terminal)
{
	switch(receive_from_terminal)
	{
		case 'h':
			print_string("1<-->Led test\n2<-->Interrupt test\n");
			break;
		case '1':
			print_string("Led test...\n");
			break;
		case '2':
			print_string("Interrupt test...\n");
			break;
		default:
			print_string("Input error...\n");
			break;
	}
}

void handle_event_commond(char* commond, int argc, char(*argv)[MAX_COMMAND_LENGTH])
{
	unsigned index = 0;
	int compare_result = 0;
	
	for(index = 0; index < sizeof(gdiags_commonds)/sizeof(gdiags_commonds[0]); index++)
	{
		compare_result = strcmp(commond, gdiags_commonds[index].cmmond_string);
		if(compare_result == 0)
		{
			gdiags_commonds[index].function(argc, argv);
			break;
		}
	}
}

void GenerateArgv(int* argc, char(*argv)[MAX_COMMAND_NUMBER][MAX_COMMAND_LENGTH], char* receive_string)
{
	char* receive_string_temp = receive_string;
	int str_length = strlen(receive_string) + 1;
	char* ptr_head = receive_string;
	int char_count = 0;
	int index = 0;
	
	*argc = 0;

	for(index = 0; index < str_length; index++)
	{
		char_count++;
		if(receive_string_temp[index] == ' ' || receive_string_temp[index] == '\0')
		{
			memcpy((*argv)[*argc], ptr_head, char_count);
			(*argv)[*argc][char_count - 1] = '\0';
			ptr_head = ptr_head + char_count;
			char_count = 0;
			*argc = *argc + 1;
			
			if(receive_string_temp[index] == '\0')
			{
				break;
			}
		}
	}
}

