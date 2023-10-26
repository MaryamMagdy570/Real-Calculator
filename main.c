/*
 * main.c
 *
 *  Created on: Oct 25, 2023
 *      Author: Maryam Magdy
 */

#include <util/delay.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "DIO_interface.h"
#include "CLCD_interface.h"
#include "KPD_interface.h"

#define MATHERROR() do { CLCD_voidGoToRowColumn(2,4);      \
						CLCD_voidSendString("MATH ERROR"); \
						INIT_VARS();}while(0)

#define REARRANGE_EXPRESSION(ResultItem,DeletedItemIndex) do { if(DeletedItemIndex!=-1)										\
																	ExpressionArray[DeletedItemIndex] = ResultItem	;		\
															   for (u8 k = DeletedItemIndex+1; k<ExpressionCounter; k++){   \
																	ExpressionArray[k] = ExpressionArray[k+1];}				\
																	ExpressionCounter -- ;}while(0)

#define REARRANGE_ARRAYS(DeletedItemIndex) do {	for (u8 k = DeletedItemIndex+1 ; k<Counter; k++){  		    \
														OperationsArray[k-1] = OperationsArray[k];			\
														NumbersArray[k] = NumbersArray[k+1];}				\
														Counter--;}while(0)


#define INIT_VARS()  do{for(u8 k = 0; k<10; k++){	    \
							OperationsArray[k] = '\0';  \
							NumbersArray[k]=0;			\
							NegativeFlags[k]=0;} 		\
						DecimalPlace = 0.1;				\
						DecimalFlag =0;					\
						ExpressionCounter = 0;			\
						Counter = 0;					\
						NegativeFlag =0;}while(0)


u8 Button = '\0';


u8 ExpressionArray[20];
u8 ExpressionCounter;

u8 OperationsArray[10];
u8 Counter;

f32 NumbersArray[10];

f32 DecimalPlace;  //up to 4 decimals
u8 DecimalFlag;
u8 NegativeFlag;		// for + - operations
u8 NegativeFlags[10];   // for * / operations



void CleanExpressionAndCheckErrors();
void GetExpression();
void TranslateExpression();

void PerformAdditionAndSubtraction();


void main(void)
{
	DIO_voidInit();
	CLCD_voidInit();

	INIT_VARS();

	GetExpression();

	CleanExpressionAndCheckErrors();

	TranslateExpression();


	for(u8 i =0; i<Counter; i++)
	{
		if (OperationsArray[i] == '*')
		{
			NumbersArray[i] = NumbersArray[i]*NumbersArray[i+1];
		}
		else if (OperationsArray[i] == '/')
		{
			if (NumbersArray[i+1] == (float)0)
			{
				MATHERROR();
				break;
			}
			else
			{
				NumbersArray[i] = NumbersArray[i]/NumbersArray[i+1];
			}
		}
		else
		{
			continue;
		}
		//REARRANGE
		REARRANGE_ARRAYS(i);
		i--;
	}

/*
	CLCD_voidGoToRowColumn(2,0);
	CLCD_voidSendDecimalNumber(NumbersArray[0]);
	for(int m=0;m<Counter; m++){
	CLCD_voidSendData(OperationsArray[m]);
	CLCD_voidSendDecimalNumber(NumbersArray[m+1]);}
*/

	PerformAdditionAndSubtraction();

	CLCD_voidGoToRowColumn(3,13);

if(NegativeFlag)
	CLCD_voidSendData('-');

CLCD_voidSendDecimalNumber(NumbersArray[0]);

//do not forget to clear operand and numbers
while(1){}
}

void GetExpression()
{
	while (Button != '=')
	{
		Button = KPD_u8GetPressedKey();
		if (Button != '\0')
		{
			CLCD_voidSendData(Button);
			ExpressionArray[ExpressionCounter] = Button;
			ExpressionCounter++;
		}
	}

	ExpressionCounter--;  // to exclude '=' character
}

void CleanExpressionAndCheckErrors()
{

	if ( ExpressionArray[0] == '*' ||  ExpressionArray[0]== '/')
		MATHERROR();
	else if (ExpressionArray[0] == '+')
		REARRANGE_EXPRESSION(' ',-1);
	else if (ExpressionArray[0] == '-')
	{
		REARRANGE_EXPRESSION(' ',-1);
		NegativeFlags[0] = 1;
	}

	for (u8 i = 0; i<=ExpressionCounter; i++)
	{
		if (ExpressionArray[i] == '+' && ExpressionArray[i+1] == '*')
			MATHERROR();

		else if (ExpressionArray[i] == '+' && ExpressionArray[i+1] == '/')
			MATHERROR();

		else if (ExpressionArray[i] == '-' && ExpressionArray[i+1] == '*')
			MATHERROR();

		else if (ExpressionArray[i] == '-' && ExpressionArray[i+1] == '/')
			MATHERROR();

		else if (ExpressionArray[i] == '*' && ExpressionArray[i+1] == '*')
			MATHERROR();

		else if (ExpressionArray[i] == '/' && ExpressionArray[i+1] == '/')
			MATHERROR();

		else if (ExpressionArray[i] == '.' && ExpressionArray[i+1] == '.')
			MATHERROR();

		else if (ExpressionArray[i] == '*' && ExpressionArray[i+1] == '+')
		{
			REARRANGE_EXPRESSION('*',i);	i--;
		}
		else if (ExpressionArray[i] == '/' && ExpressionArray[i+1] == '+')
		{
			REARRANGE_EXPRESSION('/',i);	i--;
		}
		//////////////
		else if (ExpressionArray[i] == '*' && ExpressionArray[i+1] == '-')
		{
			REARRANGE_EXPRESSION('*',i);
			NegativeFlags[i+1] = 1;
			i--;
		}
		else if (ExpressionArray[i] == '/' && ExpressionArray[i+1] == '-')
		{
			REARRANGE_EXPRESSION('/',i);
			NegativeFlags[i+1] = 1;
			i--;
		}
		//////////////

		else if (ExpressionArray[i] == '+' && ExpressionArray[i+1] == '+')
		{
			REARRANGE_EXPRESSION('+',i);	i--;
		}

		else if (ExpressionArray[i] == '+' && ExpressionArray[i+1] == '-')
		{
			REARRANGE_EXPRESSION('-',i);	i--;
		}

		else if (ExpressionArray[i] == '-' && ExpressionArray[i+1] == '+')
		{
			REARRANGE_EXPRESSION('-',i);	i--;
		}
		else if (ExpressionArray[i] == '-' && ExpressionArray[i+1] == '-')
		{
			REARRANGE_EXPRESSION('+',i);	i--;
		}
		else if (ExpressionArray[i] == '-' && ExpressionArray[i+1] == '-')
		{
			REARRANGE_EXPRESSION('+',i);	i--;
		}
		else if (ExpressionArray[i] == '.' && ExpressionArray[i+1] == '+')
		{
			REARRANGE_EXPRESSION('+',i);	i--;
		}
		else if (ExpressionArray[i] == '.' && ExpressionArray[i+1] == '-')
		{
			REARRANGE_EXPRESSION('-',i);  	i--;
		}
		else if (ExpressionArray[i] == '.' && ExpressionArray[i+1] == '*')
		{
			REARRANGE_EXPRESSION('*',i);	i--;
		}
		else if (ExpressionArray[i] == '.' && ExpressionArray[i+1] == '/')
		{
			REARRANGE_EXPRESSION('/',i);	i--;
		}
	}

}

void TranslateExpression()
{
	for(u8 i =0; i<ExpressionCounter; i++)
	{
		if ((ExpressionArray[i] >= '0') && (ExpressionArray[i] <='9'))
		{
			if(!DecimalFlag)
			{
			NumbersArray[Counter] = NumbersArray[Counter] * 10 + (ExpressionArray[i]  - '0');
			DecimalPlace = 0.1;
			}
			else
			{
				DecimalFlag = 0;
				NumbersArray[Counter] = NumbersArray[Counter] + (ExpressionArray[i] - '0')*DecimalPlace;
				DecimalPlace *= 0.1;
			}
		}
		else
		{
			DecimalPlace = 0.1;
			DecimalFlag = 0;
			switch(ExpressionArray[i])
			{
				case '+' : 	OperationsArray [Counter] = '+';
							Counter ++;
							break;
				case '-' : 	OperationsArray [Counter] = '-';
							Counter ++;
							break;
				case '*' : 	OperationsArray [Counter] = '*';
							Counter ++;
							break;
				case '/' : 	OperationsArray [Counter] = '/';
							Counter ++;
							break;
				case '.' : 	DecimalFlag = 1;
							break;
				default  :	break;
			}
		}
	}

}

void PerformAdditionAndSubtraction()
{
	for(u8 i =0; i<Counter; i++)
	{
		if (OperationsArray[i] == '+')
		{
			if (NegativeFlag && (NumbersArray[i]>=NumbersArray[i+1]))
			{
				NumbersArray[i] = NumbersArray[i]-NumbersArray[i+1];
			}
			else if (NegativeFlag && (NumbersArray[i]<NumbersArray[i+1]))
			{
				NegativeFlag = 0;
				NumbersArray[i] = NumbersArray[i+1]-NumbersArray[i+1];
			}
			else if (!NegativeFlag)
			{
				NumbersArray[i] = NumbersArray[i]+NumbersArray[i+1];
			}
		}
		else if (OperationsArray[i] == '-')
		{
			if(!NegativeFlag && (NumbersArray[i]>=NumbersArray[i+1]))
			{
				NumbersArray[i] = NumbersArray[i]-NumbersArray[i+1];
			}
			else if (!NegativeFlag &&  (NumbersArray[i]<NumbersArray[i+1]))
			{
				NegativeFlag = 1;
				NumbersArray[i] = NumbersArray[i+1]-NumbersArray[i];
			}
			else if(NegativeFlag)
			{
				NumbersArray[i] = NumbersArray[i]+NumbersArray[i+1];
			}
		}
		else
		{
			continue;
		}
		//REARRANGE
		REARRANGE_ARRAYS(i);
		i--;
	}
}
