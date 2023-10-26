#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"

#include "KPD_config.h"
#include "KPD_private.h"
#include "KPD_interface.h"




u8 KPD_u8GetPressedKey(void)
{
	u8 Local_u8PressedKey = NO_PRESSED_KEY;
	u8 Local_u8RowIndex,Local_u8ColumnIndex;
	u8 Local_u8SwitchValue;
	u8 Local_u8Array[NUMBER_OF_ROWS][NUMBER_OF_COLUMNS] = KPD_ARRAY;
	u8 Local_u8RowsArray[NUMBER_OF_ROWS] = {KPD_ROW0_PIN,KPD_ROW1_PIN,KPD_ROW2_PIN,KPD_ROW3_PIN};
	u8 Local_u8ColumnsArray[NUMBER_OF_COLUMNS] = {KPD_COL0_PIN,KPD_COL1_PIN,KPD_COL2_PIN,KPD_COL3_PIN};


	//deactivate all rows
	for (int i = 0; i<NUMBER_OF_ROWS; i++)
	{
		DIO_u8SetPinValue(KPD_ROWS_PORT,Local_u8RowsArray[i],DIO_u8PIN_HIGH);
	}

	//looping
	for (Local_u8RowIndex=0; Local_u8RowIndex < NUMBER_OF_ROWS; Local_u8RowIndex++)
	{
		// ACTIVATE THE ROW
		DIO_u8SetPinValue(KPD_ROWS_PORT,Local_u8RowsArray[Local_u8RowIndex],DIO_u8PIN_LOW);

		for (Local_u8ColumnIndex=0; Local_u8ColumnIndex<NUMBER_OF_COLUMNS; Local_u8ColumnIndex++)
		{
			DIO_u8GetPinValue(KPD_COLS_PORT,Local_u8ColumnsArray[Local_u8ColumnIndex],&Local_u8SwitchValue);
			if (!Local_u8SwitchValue)
			{
				while(!Local_u8SwitchValue)
				{
					DIO_u8GetPinValue(KPD_COLS_PORT,Local_u8ColumnsArray[Local_u8ColumnIndex],&Local_u8SwitchValue);
				}
					Local_u8PressedKey= Local_u8Array[Local_u8RowIndex][Local_u8ColumnIndex];
					return Local_u8PressedKey;
			}
		}
		//DEACTIVATE THE ROW
		DIO_u8SetPinValue(KPD_ROWS_PORT,Local_u8RowsArray[Local_u8RowIndex],DIO_u8PIN_HIGH);
	}
	return Local_u8PressedKey;
}


