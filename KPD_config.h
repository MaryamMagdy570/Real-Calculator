#ifndef _KPD_CONFIG_H_
#define _KPD_CONFIG_H_



#define KPD_ROWS_PORT 	DIO_u8PORTC

#define KPD_ROW0_PIN 	DIO_u8PIN5
#define KPD_ROW1_PIN	DIO_u8PIN4
#define KPD_ROW2_PIN	DIO_u8PIN3
#define KPD_ROW3_PIN	DIO_u8PIN2

#define KPD_COLS_PORT 	DIO_u8PORTD

#define KPD_COL0_PIN 	DIO_u8PIN7
#define KPD_COL1_PIN	DIO_u8PIN6
#define KPD_COL2_PIN	DIO_u8PIN5
#define KPD_COL3_PIN	DIO_u8PIN3

#define KPD_ARRAY  {{'1','2','3','+'},{'4','5','6','-'},{'7','8','9','*'},{'.','0','=','/'}}

#define NO_PRESSED_KEY 	'\0'



#endif
