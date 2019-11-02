#ifndef LCD_H_
#define LCD_H_

/*******************************************************************************
 *                      	Included Libraries                                 *
 *******************************************************************************/
//#include "register_lib.h" //<avr/io.h>
#include <util/delay.h>
#include <avr/io.h>

#include "std_types.h"
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

#define Clear_display 0x01
#define Return_home 0x02
#define Display_noshift 0x06
#define Display_shift 0x07
#define Display_off 0x08
#define Displayon_Cursoroff	0x0C
#define Display_on 0x0E
#define Init_4bit_1 0x33
#define Init_4bit_2 0x32
#define FunctionSet4bit 0x28
#define FunctionSet8bit 0x38
#define SET_CGRAM_addr 0x40
#define SET_DDRAM_addr 0x80

#define Register_Select 1
#define Read_Write      2
#define Enable_LCD      3

#define LCD_RS    1
#define LCD_RW    2
#define LCD_E     3

#define lcd_line_1_address  0x7F       
#define lcd_line_2_address  0xBF       

#define LINE_1      1
#define LINE_2      2

#define COLUMN_MIN   0
#define COLUMN_MAX   16

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_displayChar(uint8 character);
void LCD_displayString(uint8 string[]);
void LCD_displayStringRowColumn(uint8 string[], uint8 row, uint8 column);
void LCD_gotoRowColumn(uint8 LINE, uint8 COLUMN);
void LCD_clear(void);
void LCD_Write_Integer_Number(sint32 number);

#endif /* LCD_H_ */
