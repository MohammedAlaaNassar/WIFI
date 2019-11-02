/*******************************************************************************
 *                      	Included Libraries                                 *
 *******************************************************************************/
#include "LCD.h"
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*******************************************************************************
 * Function Name:	LCD_init
 *
 * Description: 	
 * 			
 * Inputs:			
 *
 * Outputs:			
 *
 * Return:			
 *******************************************************************************/
void LCD_init(void) {

	/* Data Direction Configuration */

	DDRA = 0xFF;

	_delay_us(50);

	/* Initialize LCD to work in 4_bit mode */
	LCD_sendCommand(Init_4bit_1);
	_delay_us(50);

	LCD_sendCommand(Init_4bit_2);
	_delay_us(50);

	LCD_sendCommand(FunctionSet4bit);
	_delay_us(50);

	/* LCD Initialization */
	LCD_sendCommand(Return_home); /* Start from home Position */
	_delay_us(50);

	LCD_sendCommand(Displayon_Cursoroff); /* LCD display On */
	_delay_us(50);

	LCD_sendCommand(Display_noshift); /*  mode set */
	_delay_us(50);

	LCD_sendCommand(Clear_display); /* Clear LCD display */
	_delay_us(2000);

}

/*******************************************************************************
 * Function Name:	LCD_sendCommand
 *
 * Description: 	
 * 			
 * Inputs:			
 *
 * Outputs:			
 *
 * Return:			
 *******************************************************************************/
void LCD_sendCommand(uint8_t command) {
	/* Control Signals Configuration */

	PORTA &= (~(1 << LCD_RS));
	PORTA &= (~(1 << LCD_RW));
	PORTA |= (1 << LCD_E);

	_delay_us(50);

	/* Sending High nibble */
	PORTA = (PORTA & 0x0F) | (command & 0xF0);
	_delay_us(50);

	PORTA &= (~(1 << LCD_E));/* Disable Signal */
	_delay_us(50);

	PORTA |= (1 << LCD_E); /* Enable Signal */
	_delay_us(50);

	/* Sending Low nibble */
	PORTA = (PORTA & 0x0F) | ((command & 0x0F) << 4);
	_delay_us(50);

	PORTA &= (~(1 << LCD_E));/* Disable Signal */
	_delay_us(50);

	PORTA |= (1 << LCD_E); /* Enable Signal */
	_delay_us(50);
}

/*******************************************************************************
 * Function Name:	LCD_displayChar
 *
 * Description: 	
 * 			
 * Inputs:			
 *
 * Outputs:			
 *
 * Return:			
 *******************************************************************************/
void LCD_displayChar(uint8_t character) {
	/* Control Signals Configuration */

	PORTA |= (1 << LCD_RS); /* Select data register */
	PORTA &= (~(1 << LCD_RW)); /*disable Write operation */
	PORTA |= (1 << LCD_E);/* Enable Signal */
	_delay_us(50);

	/* Sending High nibble */
	PORTA = (PORTA & 0x0F) | (character & 0xF0);
	_delay_us(50);

	PORTA &= (~(1 << LCD_E));/* Disable Signal */
	_delay_us(50);

	PORTA |= (1 << LCD_E);/* Enable Signal */
	_delay_us(50);

	/* Sending low nibble */
	PORTA = (PORTA & 0x0F) | ((character & 0x0F) << 4);
	_delay_us(50);

	PORTA &= (~(1 << LCD_E));/* Disable Signal */
	_delay_us(50);

	PORTA |= (1 << LCD_E);/* Enable Signal */
	_delay_us(50);

}

/*******************************************************************************
 * Function Name:	LCD_gotoRowColumn
 *
 * Description: 	
 * 			
 * Inputs:			
 *
 * Outputs:			
 *
 * Return:			
 *******************************************************************************/
void LCD_gotoRowColumn(uint8_t LINE, uint8_t COLUMN) {
	if ((COLUMN > COLUMN_MIN) && (COLUMN <= COLUMN_MAX)) {
		switch (LINE) {
		case LINE_1:
			LCD_sendCommand(( lcd_line_1_address) + (COLUMN));
			break;

		case LINE_2:
			LCD_sendCommand(( lcd_line_2_address) + (COLUMN));
			break;

		default:
			break;
		}
	} else {
		/* Do Nothing */
	}

}

/*******************************************************************************
 * Function Name:	LCD_displayString
 *
 * Description: 	
 * 			
 * Inputs:			
 *
 * Outputs:			
 *
 * Return:			
 *******************************************************************************/
void LCD_displayString(uint8_t string[]) {
	uint8_t iterator = 0;
	while (string[iterator] != '\0') {
		LCD_displayChar(string[iterator]);
		iterator++;
	}
}

/*******************************************************************************
 * Function Name:	LCD_displayStringRowColumn
 *
 * Description: 	
 * 			
 * Inputs:			
 *
 * Outputs:			
 *
 * Return:			
 *******************************************************************************/
void LCD_displayStringRowColumn(uint8_t string[], uint8_t row, uint8_t column) {
	LCD_gotoRowColumn(row, column);
	LCD_displayString(string);
}

/*******************************************************************************
 * Function Name:	LCD_clear
 *
 * Description: 	
 * 			
 * Inputs:			
 *
 * Outputs:			
 *
 * Return:			
 *******************************************************************************/
void LCD_clear(void) {
	LCD_sendCommand(Clear_display);
	//_delay_us(2000);
	//Timer_enmDelayBlocking(TIMER_GENERAL_DELAY, TIMER_PERIOD_10_MICRO_SECOND, 200);
	//LCD_sendCommand(Return_home);
}

// a function to reverse a string
void reverse(uint8_t *str, uint8_t length) {
	uint8_t start = 0;
	uint8_t end = length - 1;
	while (start < end) {
		uint8_t temp = str[start];

		str[start] = str[end];

		str[end] = temp;

		start++;
		end--;
	}

}

uint8_t* integer_to_string(sint32 number, uint8_t *str, uint8_t base) {
	// index for length

	uint8_t i = 0;
	uint8_t isNegative = 0;

	// Handle 0 explicitely

	if (number == 0) {
		str[i++] = '0';
		str[i] = '\0';  // the end of the string
		return str;
	}

	// negative numbers are handled only with
	// base 10. Otherwise numbers are considered unsigned

	if (number < 0 && base == 10) {
		isNegative = 1;
		number = -number; // to get the absolute value of the number
	}

	while (number != 0) {

		uint8_t remainder = number % base;
		if (remainder > 9) {
			str[i++] = (remainder - 10) + 'A';  // for hexa-decimal only

		} else {
			str[i++] = remainder + '0';
		}
		number = number / base;
	}

	if (isNegative) {
		str[i++] = '-';
	}

	str[i] = '\0';  // the end of the string

	reverse(str, i); // where i  index for the length of the string

	return str;
}

void LCD_Write_Integer_Number(sint32 number) {
	uint8_t str[16];
	integer_to_string(number, str, 10);

	LCD_displayString((uint8_t*) str);

}
