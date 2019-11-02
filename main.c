/* 
* main.c
*
*  Created on: Oct 30, 2019
*      Author: Mohammed Nassar
*/

#include "common_macros.h"
#include "DIO.h"
#include "motor.h"
#include "PWM.h"
#include "micro_config.h"
#include "std_types.h"
#include "uart_driver.h"
#include "LCD.h"

uint8 PORT[]="1234";                //port number
uint8 SSID[]="\"NASSAR\"";         //ssid: String, ESP8266’s softAP SSID
uint8 PASSWORD[]=",\"12345678\""; // Password, no longer than 64 characters
volatile uint8 command=0;

/*
 * ESP8266, in it’s default configuration,
 * boots up into the serial modem mode.
 * In this mode you can communicate with it using a set of AT commands.
*/

void MY_WIFI_Service_Start(uint8 Server,uint8*PORT,uint8*SSID,uint8* PASSWORD)
{

	/*
	 * mode： An integer designating the mode of operation either 1, 2, or 3.
1 = Station mode (client)
2 = AP mode (host)
3 = AP + Station mode (Yes, ESP8266 has a dual mode!)
	 * */
	 UART_Write_String("AT+CWMODE_DEF=2"); //AT+CWMODE - WIFI mode（station, AP, station + AP）
	 UART_Write_Byte(0xD);
	 UART_Write_Byte(0xA);
	 _delay_ms(1000);
/*
 * mode:
0: Single connection
1: Multiple connections (MAX 4)
 * */
	 UART_Write_String("AT+CIPMUX=1"); // AT+CIPMUX - Enable multiple connections or not
	 UART_Write_Byte(0xD);
	 UART_Write_Byte(0xA);
	 _delay_ms(1000);
/*
 * mode:
0: Delete server (need to follow by restart)
1: Create server
port: port number, default is 333

NOTE:
Server can only be created when AT+CIPMUX=1
Server monitor will automatically be created when Server is created.
When a client is connected to the server, it will take up one connection，be gave an id
*/
	 UART_Write_String("AT+CIPSERVER=1,");// AT+CIPSERVER - Configure as server
	 UART_Write_String(PORT);
	 UART_Write_Byte(0xD);
	 UART_Write_Byte(0xA);
	 _delay_ms(1000);
/*
 * ssid: String, ESP8266’s softAP SSID
pwd: String, Password, no longer than 64 characters
ch: channel id
ecn:
0 = OPEN
2 = WPA_PSK
3 = WPA2_PSK
4 = WPA_WPA2_PSK
*/
	 UART_Write_String("AT+CWSAP_DEF="); // AT+CWSAP - Configuration of softAP mode
	 UART_Write_String(SSID);
	 UART_Write_String(PASSWORD);
	 UART_Write_String(",1,3");
	 UART_Write_Byte(0xD);
	 UART_Write_Byte(0xA);
	 _delay_ms(1000);
}


void UART_callBack(uint8 data)    // UART CALL BACK FUNCTION
{

command=data;  // SAVING DATA IMMEDIATELY

}


int main()
{

Disable_Interrupts();
UART_Init(9600,UART_callBack); // UART initialization  Baud rate=9600
	Enable_Interrupts();
	
	MY_WIFI_Service_Start(0,PORT,SSID, PASSWORD);  // SETUP WIFI module

		 
DIO_init();                                   // DIGITAL INPUT / OUTPut PORTS initialization

Phase_Correct_PWM(20);                       // SET MOTOR SPEED
	 
	
	while(1)
	{


switch (command)                          // Checking the command
{
case 'A':
case 'a':MOTOR_CarForward();
LCD_clear();
LCD_displayString("   FORWARD");
break;
case 'B':
case 'b':MOTOR_CarBackward();
LCD_clear();
LCD_displayString("     Backward");
break;
case 'C':
case 'c':MOTOR_CarStop();
LCD_clear();
LCD_displayString("  Car STOPED!!");
break;
case 'D':
case 'd':MOTOR_CarRight(1000);
LCD_clear();
LCD_displayString("     Right");
break;
case 'E':
case 'e':MOTOR_CarLeft(1000);
LCD_clear();
LCD_displayString("     Left");
break;
default:MOTOR_CarStop();
LCD_clear();
LCD_displayString("  Car STOPED!!");
	break;
}

	}

	return 0;
}
