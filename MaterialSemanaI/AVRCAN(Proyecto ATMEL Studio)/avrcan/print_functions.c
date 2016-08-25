/****************************************************************** 
 * print_functions.c
 * Created on: Jul 30, 2015
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *© 2015
 *****************************************************************/
#ifndef print_functions_C_
#define print_functions_C_
#endif
/**
 * \addtogroup print_functions_module
 * @{
 */

/******************************************************************/
/*    I N C L U D E   F I L E S                                   */
/******************************************************************/
#include <print_functions.h>


/*******************************************************************************************/
/*    M A C R O S                                                                          */
/*******************************************************************************************/


void HExChar(uint8_t inval);
void HexByte(uint8_t val);


/*******************************************************************************************/
/*    F U N C T I O N   P R O T O T Y P E S                                                */
/*******************************************************************************************/
void HExChar(uint8_t inval){
	uint8_t ret_hex='0';
	switch(inval){
	 case 0: ret_hex='0'; break;
	 case 1: ret_hex='1'; break;
	 case 2: ret_hex='2'; break;
	 case 3: ret_hex='3'; break;
	 case 4: ret_hex='4'; break;
	 case 5: ret_hex='5'; break;
	 case 6: ret_hex='6'; break;
	 case 7: ret_hex='7'; break;
	 case 8: ret_hex='8'; break;
	 case 9: ret_hex='9'; break;
	case 10: ret_hex='A'; break;
	case 11: ret_hex='B'; break;
	case 12: ret_hex='C'; break;
	case 13: ret_hex='D'; break;
	case 14: ret_hex='E'; break;
	case 15: ret_hex='F'; break;
	};

	PUT_CHAR_STDOUT(ret_hex);
	return;
}
void HexByte(uint8_t val){
	HExChar( (val>>4)& 0x0F );
	HExChar(0x0F&val);
	PUT_CHAR_STDOUT('H');
	PUT_CHAR_STDOUT(32);
	return;
}
void Print_Buffer(unsigned char * data){
	while(*data){
		PUT_CHAR_STDOUT(*data++);
	}
}

void Print_BufferHEX(unsigned char * data,uint8_t ico){
	uint8_t coun;
	for(coun=0;coun<ico;coun++){
		HexByte(data[coun]);
		Print_Buffer((unsigned char*)" ");
	}
}
/**
 * @}
 */
