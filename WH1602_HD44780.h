#define F_CPU 8000000UL
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>

//This define micro controllers port for LCD connection
 //The mode is 4 bits
#define LCD_PORT PORTB 
#define LCD_DDR DDRB
#define E 1
#define RS 7
#define DB4 2
#define DB5 3
#define DB6 4
#define DB7 5


//Setting gating signal
void lcd_gate_up();

//Reset gating signal
void lcd_gate_down();

//Clear display
void lcd_clear_display();

//Send byte: command(false)/data(true)
void lcd_send_byte(uint8_t com, _Bool RS_PIN);

//Display initialization
void lcd_init();

//Set cursor position
void lcd_set_cursor_position(char slot_number, char line_number);

//Display text
void lcd_print_text(char *text, char slot_number, char line_number);

//Display integer number
void lcd_print_integer_number(int number, char string_length, char slot_number, char line_number);

//Display float number(precision must be 0, 1 or 2 - its the number of digits after the decimal point)
void lcd_print_float_number(float number, char precision, char string_length, char slot_number, char line_number);
