#include"lcd.h"
#include <stdio.h> 
#include<stdlib.h>

uint8_t LCD_x_pos = 0;
uint8_t LCD_y_pos = 0;

void LCD_INIT()
{

    /*DIO_SetPortDirection(LCD_DATA_PORT,OUTPUT);
    DIO_SetPinDirection(LCD_CONTROL_PORT,RS,OUTPUT);
    DIO_SetPinDirection(LCD_CONTROL_PORT,RW,OUTPUT);
    DIO_SetPinDirection(LCD_CONTROL_PORT,E,OUTPUT);
    LCD_CMD(TWO_LINE_LCD_Eight_BIT_MODE);
    LCD_CMD(CURSOR_ON);
    LCD_CMD(CLEAR);*/
    DDR_LCD_data = 0xFF; /* set as output */
    DDR_LCD_control |= (1<<RW) | (1<<RS) | (1<<E); /* RS , RW , E as output */
    LCD_CMD(TWO_LINE_LCD_Eight_BIT_MODE);
    LCD_CMD(CURSOR_OFF);
    LCD_CMD(CLEAR);

}

void LCD_WRITE_CHAR(uint8_t c)
{
    /*DIO_SetPinValue(LCD_CONTROL_PORT,RS,HIGH);
    DIO_SetPinValue(LCD_CONTROL_PORT,RW,LOW);
    _delay_ms(2);
    DIO_SetPinValue(LCD_CONTROL_PORT,E,HIGH);
    _delay_ms(2);
    DIO_SetPortValue(LCD_DATA_PORT,c);
    _delay_ms(2);
    DIO_SetPinValue(LCD_CONTROL_PORT,E,LOW);
    _delay_ms(2);*/
    LCD_X_Y(LCD_x_pos,LCD_y_pos);
    
    PORT_LCD_control |= 1<<RS;
    PORT_LCD_control &= ~(1<<RW);
    _delay_ms(2);
    PORT_LCD_control |= 1<<E;
    _delay_ms(2);
    PORT_LCD_data = c;
    _delay_ms(2);
    PORT_LCD_control &= ~(1<<E);

    LCD_x_pos++;
    LCD_y_pos = (LCD_x_pos == 16) ? abs(1-LCD_y_pos) : LCD_y_pos;
    LCD_x_pos = (LCD_x_pos == 16) ? 0 : LCD_x_pos;



}
void LCD_WRITE_STR(char * str)
{
    /*DIO_SetPinValue(LCD_CONTROL_PORT,RS,HIGH);
    DIO_SetPinValue(LCD_CONTROL_PORT,RW,LOW);
    DIO_SetPinValue(LCD_CONTROL_PORT,E,HIGH);
    _delay_ms(2);*/
    PORT_LCD_control |= 1<<RS;
    PORT_LCD_control &= ~(1<<RW);
    _delay_ms(2);
    PORT_LCD_control |= 1<<E;
    _delay_ms(2);
    while(*str != '\0')
    {
        LCD_WRITE_CHAR(*str);
        str++;
    }
    _delay_ms(2);
    LCD_WRITE_CHAR(' '); /* to avoid the unwanted char */
    
}

void LCD_CMD(uint8_t cmd)
{
    /*DIO_SetPinValue(LCD_CONTROL_PORT,RS,LOW);
    DIO_SetPinValue(LCD_CONTROL_PORT,RW,LOW);
    _delay_ms(2);
    DIO_SetPinValue(LCD_CONTROL_PORT,E,HIGH);
    _delay_ms(2);
    DIO_SetPortValue(LCD_DATA_PORT,cmd);
    _delay_ms(2);
    DIO_SetPinValue(LCD_CONTROL_PORT,E,LOW);
    _delay_ms(2);*/
    PORT_LCD_control &= ~(1<<RS);
    PORT_LCD_control &= ~(1<<RW);
    _delay_ms(2);
    PORT_LCD_control |= 1<<E;
    _delay_ms(2);
    PORT_LCD_data = cmd;
    PORT_LCD_control &= ~(1<<E);
    _delay_ms(2);
}
void LCD_X_Y(uint8_t x_pos , uint8_t y_pos)
{
    uint8_t loc = 0;
    LCD_x_pos = x_pos;
    LCD_y_pos = y_pos;
    switch(y_pos)
    {
        case 0: loc = x_pos; break;
        case 1: loc = 0x40+x_pos; break;
    }
    LCD_CMD(SET_CURSOR_LOCATION | loc);
}

void LCD_CLEAR_SCREEN(void)
{
    LCD_CMD(CLEAR);
    LCD_x_pos = 0;
    LCD_y_pos = 0;
}

void LCD_DISPLAY_RIGHT_SHIFT(uint8_t r)
{
    for(r;r>0;r--)
        LCD_CMD(DISPLAY_RIGHT_SHIFT);
}

void LCD_DISPLAY_LEFT_SHIFT(uint8_t l)
{
    for(l;l>0;l--)
        LCD_CMD(DISPLAY_LEFT_SHIFT);
}

void LCD_WRITE_NUM(uint16_t n)
{
    /*uint8_t len = snprintf(NULL, 0, "%ld", n);
	char *str = malloc(len + 1);
	snprintf(str, len + 1, "%ld", n);
	LCD_WRITE_STR(str);
	free(str);*/
    char str[20];
    itoa(n,str,10);
    LCD_WRITE_STR(str); 
}
#if 0
void LCD_clrear_row(uint8_t x,uint8_t y,uint8_t until){
    //LCD_X_Y(x,y);
    for(int i = x ; i<=until ; i++){
        LCD_X_Y(i,y);
        LCD_WRITE_CHAR(' ');
        //_delay_ms(40);
    }
    LCD_X_Y(x,y);
}
#endif