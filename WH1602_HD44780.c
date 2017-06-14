#include <avr/io.h>
#include "WH1602_HD44780.h"
#define F_CPU 8000000UL
#include <util/delay.h>

void lcd_gate_up()
{
	LCD_PORT|=(1<<E);
	_delay_us(200);	
}

void lcd_gate_down()
{
	LCD_PORT&=~(1<<E);
	_delay_us(200);
}

void lcd_send_byte(uint8_t com, _Bool RS_PIN)
{
	if(RS_PIN)
	LCD_PORT|=(1<<RS);
	else
	LCD_PORT&=~(1<<RS);
	for (signed char i=0;i<=7;i++)
	{
		switch(7-i)
		{				
			case 0:
				if(com&(1<<(7-i))) LCD_PORT|=(1<<DB4);
				else LCD_PORT&=~(1<<DB4);
				lcd_gate_down();
			break;			
			case 1:
				if(com&(1<<(7-i))) LCD_PORT|=(1<<DB5);
				else LCD_PORT&=~(1<<DB5);
			break;
			case 2:
				if(com&(1<<(7-i))) LCD_PORT|=(1<<DB6);
				else LCD_PORT&=~(1<<DB6);
			break;
			case 3:
				lcd_gate_up();
				if(com&(1<<(7-i))) LCD_PORT|=(1<<DB7);
				else LCD_PORT&=~(1<<DB7);
			break;
			case 4:				
				if(com&(1<<(7-i))) LCD_PORT|=(1<<DB4);
				else LCD_PORT&=~(1<<DB4);
				lcd_gate_down();	
			break;
			case 5:
				if(com&(1<<(7-i))) LCD_PORT|=(1<<DB5);
				else LCD_PORT&=~(1<<DB5);
			break;
			case 6:
				if(com&(1<<(7-i))) LCD_PORT|=(1<<DB6);
				else LCD_PORT&=~(1<<DB6);
			break;
			case 7:
			lcd_gate_up();
				if(com&(1<<(7-i))) LCD_PORT|=(1<<DB7);
				else LCD_PORT&=~(1<<DB7);
			break;
		}
	}
	_delay_us(50);
}

void lcd_clear_display()
{
	lcd_send_byte(0b00000001, false);
	_delay_ms(2);
}

void lcd_init()
{
	LCD_DDR |= (1<<E)|(1<<RS)|(1<<DB4)|(1<<DB5)|(1<<DB6)|(1<<DB7);
	_delay_ms(50);
	for(char i = 0; i < 2; i++)
	{
		lcd_gate_up();
		LCD_PORT&=~(1<<RS)&~(1<<DB6)&~(1<<DB7);	
		LCD_PORT|=(1<<DB4)|(1<<DB5);
		lcd_gate_down();
	}
	lcd_gate_up();
	LCD_PORT&=~(1<<RS)&~(1<<DB4)&~(1<<DB6)&~(1<<DB7);
	LCD_PORT|=(1<<DB5);
	lcd_gate_down();

	lcd_send_byte(0b00101000, false);
	lcd_send_byte(0b00001000, false);	
	lcd_clear_display();
	lcd_send_byte(0b00000110, false);
	lcd_send_byte(0b00001100, false);
}

void lcd_set_cursor_position(char slot_number, char line_number)
{
	char adress;
	adress = 0x40 * line_number + slot_number;
	adress |= 0x80;
	lcd_send_byte(adress, false);
	_delay_ms(2);
}

void lcd_print_text(char *text, char slot_number, char line_number)
{	
	lcd_set_cursor_position(slot_number, line_number);					
	while( *text )													
	{		
		lcd_send_byte(*text++, true);
	}
}

void lcd_print_integer_number(int number, char string_length, char slot_number, char line_number)
{	
	lcd_set_cursor_position(slot_number, line_number);
	for(char i = 0; i<string_length; i++) lcd_send_byte(' ', true);
	char text[10];
	sprintf(text, "%i", number);
	lcd_print_text(text, slot_number, line_number);
}

void lcd_print_float_number(float number,char precision, char string_length, char slot_number, char line_number)
{
	lcd_set_cursor_position(slot_number, line_number);
	for(char i = 0; i<string_length; i++) lcd_send_byte(' ', true);
	char text[10];
	signed int digit_int = number;
	signed int digit_tenth_part, digit_hundred_part;
	if(precision > 0)
	{
		digit_tenth_part = (int)(number*10)%10;
		if(digit_tenth_part < 0) digit_tenth_part *= -1;
		if(precision > 1)
		{
			digit_hundred_part = (int)(number*100)%100%10;
			if(digit_hundred_part < 0) digit_hundred_part *= -1;
			if(number < 0 && digit_int == 0) sprintf(text, "-%i.%u%u", digit_int, digit_tenth_part, digit_hundred_part);
			else sprintf(text, "%i.%u%u", digit_int,digit_tenth_part, digit_hundred_part);			
		}
		else
		{
			if(number < 0 && digit_int == 0) sprintf(text, "-%i.%u", digit_int, digit_tenth_part);
			else sprintf(text, "%i.%u", digit_int,digit_tenth_part);
		}
		lcd_print_text(text, slot_number, line_number);
	}			
	else lcd_print_integer_number(digit_int, string_length, slot_number, line_number);
}
