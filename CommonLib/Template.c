#include <stdio.h>
#include "Uart.h"
#include "Template.h"

Template global_ascii_template[] = {
	{'A', TEMPLATE_A},
	{'B', TEMPLATE_B},
	{'C', TEMPLATE_C},
	{'D', TEMPLATE_D},
	{'E', TEMPLATE_E},
	{'F', TEMPLATE_F},
	{'G', TEMPLATE_G},
	{'H', TEMPLATE_H},
	{'I', TEMPLATE_I},
	{'J', TEMPLATE_J},
	{'K', TEMPLATE_K},
	{'L', TEMPLATE_L},
	{'M', TEMPLATE_M},
	{'N', TEMPLATE_N},
	{'O', TEMPLATE_O},
	{'P', TEMPLATE_P},
	{'Q', TEMPLATE_Q},
	{'R', TEMPLATE_R},
	{'S', TEMPLATE_S},
	{'T', TEMPLATE_T},
	{'U', TEMPLATE_U},
	{'V', TEMPLATE_V},
	{'W', TEMPLATE_W},
	{'X', TEMPLATE_X},
	{'Y', TEMPLATE_Y},
	{'Z', TEMPLATE_Z},
};

void fill_template_color(unsigned short color, TEMPLATE8X16* ascii_template)
{
	unsigned index = 0;
	
	if(ascii_template != NULL && color != 0x00)
	{
		for(index = 0; index < (8 * 16); index++)
		{
			if((*ascii_template)[index] != 0x00)
			{
				(*ascii_template)[index] = color;
			}
			else
			{
				(*ascii_template)[index] = 0x00;
			}
		}
	}
}

unsigned get_template(unsigned char ascii_char, unsigned short color, TEMPLATE8X16** ascii_template)
{
	unsigned index = 0;
	print_string("In function get_template\n\n");
	printf_string("&global_ascii_template = 0x%x\n\n", &global_ascii_template);
	printf_string("global_ascii_template = 0x%x\n\n", global_ascii_template);
	
	for(index = 0; index < (sizeof(global_ascii_template) / sizeof(global_ascii_template[0])); index++)
	{
		if(global_ascii_template[index].ascii_char == ascii_char)
		{
			printf_string("index = %d\n\n", index);
			printf_string("&(global_ascii_template[index].ascii_template) = 0x%x\n\n", &(global_ascii_template[index].ascii_template));
			*ascii_template = &(global_ascii_template[index].ascii_template);
			break;
		}
	}
	
	fill_template_color(color, *ascii_template);

	return index;
}