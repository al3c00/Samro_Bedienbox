/*
 Name:		Samro_Bedienbox.ino
 Created:	08.06.2025 23:10:32
 Author:	Alec Fankhauser
*/

#include "Arduino.h"


//Use "Release", otherwise the ATMega328P will try to establish a serial connection, rendering _Func1 unusuable.

//Version Samro_Bedienbox_V1_4





	//define Inputs
#define _Joystick_Left_X	A1
#define _Joystick_Left_Y	A0
#define _Joystick_Right_X	A3
#define _Joystick_Right_Y	A2
#define _Button_Input		A4



//define Outputs
#define _RollbodenAn		0
#define _Func1				1 //Dammaufnahme oder Bunker Hoch/Runter
#define _Func2				2 //Dammaufnahme oder Bunker Hoch/Runter
#define _Deichsel_Links		4
#define _Deichsel_Rechts	5
#define _Achse_Links		8
#define _Achse_Rechts		9
#define _Achse_Gerade		10
#define _Schnabel_Runter	A5
#define _Schnabel_Hoch		3




struct Inputs
{
	uint16_t	Buttons; //2.5V = SW1, 3V = SW2, 1.88V = SW1 & SW2

	uint16_t	Joystick_Left_X;
	uint16_t	Joystick_Left_Y;

	uint16_t	Joystick_Right_X;
	uint16_t	Joystick_Right_Y;
};
Inputs input;


uint16_t last_time;
uint16_t interval_time;

//Analog Inputs 0 - 1024
//Analog Active 0 - 204 and 820 - 1024
uint16_t Joystick_LowActive_Value = 204;
uint16_t Joystick_HighActive_Value = 820;

//Value: 1024/5*xV (Threshhold = 50)
//Button1 pressed: _Button_Input = 2.5V
//Button2 pressed: _Button_Input = 3.0V
//Both: 1.8V


// the setup function runs once when you press reset or power the board
void setup() 
{
	//Setup Inputs
	{
		pinMode(_Joystick_Left_X, INPUT);
		pinMode(_Joystick_Left_Y, INPUT);
		pinMode(_Joystick_Right_X, INPUT);
		pinMode(_Joystick_Right_Y, INPUT);
		pinMode(_Button_Input, INPUT);

		pinMode(_Func1, OUTPUT);
		pinMode(_Func2, OUTPUT);
		pinMode(_Deichsel_Links, OUTPUT);
		pinMode(_Deichsel_Rechts, OUTPUT);
		pinMode(_Achse_Links, OUTPUT);
		pinMode(_Achse_Rechts, OUTPUT);
		pinMode(_Achse_Gerade, OUTPUT);
		pinMode(_Schnabel_Hoch, OUTPUT);
		pinMode(_Schnabel_Runter, OUTPUT);
		pinMode(_RollbodenAn, OUTPUT);
	}

	last_time = 0;
	interval_time = 100;

}

// the loop function runs over and over again until power down or reset
void loop()
{
	


	uint16_t current_time = millis();
	if (current_time - last_time >= interval_time)
	{
		last_time = current_time;

		//Read left joystick
		input.Joystick_Left_X = analogRead(_Joystick_Left_X);
		input.Joystick_Left_Y = analogRead(_Joystick_Left_Y);
		//Read right joystick
		input.Joystick_Right_X = analogRead(_Joystick_Right_X);
		input.Joystick_Right_Y = analogRead(_Joystick_Right_Y);
		//Read button 
		input.Buttons = analogRead(_Button_Input);
		
		
		//Buttons
		{
			if (input.Buttons > 462 && input.Buttons < 562)
			{
				digitalWrite(_RollbodenAn, HIGH);
			}
			
			else if (input.Buttons > 564 && input.Buttons < 800)
			{
				digitalWrite(_Achse_Gerade, HIGH);
			}
			else
			{
				digitalWrite(_Achse_Gerade, LOW);
				digitalWrite(_RollbodenAn, LOW);
				di
			}
			
		}

		

		//Achse
		{
			if (input.Joystick_Left_X < Joystick_LowActive_Value)
			{
				digitalWrite(_Deichsel_Links, HIGH);
			}
			else if (input.Joystick_Left_X >= Joystick_LowActive_Value && input.Joystick_Left_X < Joystick_HighActive_Value)
			{
				digitalWrite(_Deichsel_Links, LOW);
				digitalWrite(_Deichsel_Rechts, LOW);
			}
			else if (input.Joystick_Left_X > Joystick_HighActive_Value)
			{
				digitalWrite(_Deichsel_Rechts, HIGH);
			}

			
		}

		
		//Deichsel
		{
			if (input.Joystick_Left_Y < Joystick_LowActive_Value)
			{
				digitalWrite(_Achse_Links, HIGH);
			}
			else if (input.Joystick_Left_Y >= Joystick_LowActive_Value && input.Joystick_Left_Y < Joystick_HighActive_Value)
			{
				digitalWrite(_Achse_Links, LOW);
				digitalWrite(_Achse_Rechts, LOW);
			}
			else if (input.Joystick_Left_Y > Joystick_HighActive_Value)
			{
				digitalWrite(_Achse_Rechts, HIGH);
			}
		}

		//Func--Dammaufnahme oder Bunker
		{
			if (input.Joystick_Right_Y < Joystick_LowActive_Value)
			{
				digitalWrite(_Func2, HIGH);
			}
			else if (input.Joystick_Right_Y >= Joystick_LowActive_Value && input.Joystick_Right_Y < Joystick_HighActive_Value)
			{
				digitalWrite(_Func1, LOW);
				digitalWrite(_Func2, LOW);
			}
			else if (input.Joystick_Right_Y > Joystick_HighActive_Value)
			{
				digitalWrite(_Func1, HIGH);
			}
		}

		//Schnabel
		{
			if (input.Joystick_Right_X < Joystick_LowActive_Value)
			{
				digitalWrite(_Schnabel_Runter, HIGH);
			}
			else if (input.Joystick_Right_X >= Joystick_LowActive_Value && input.Joystick_Right_X < Joystick_HighActive_Value)
			{
				digitalWrite(_Schnabel_Runter, LOW);
				digitalWrite(_Schnabel_Hoch, LOW);
			}
			else if (input.Joystick_Right_X > Joystick_HighActive_Value)
			{
				digitalWrite(_Schnabel_Hoch, HIGH);
			}
		}

	}
	


/*
//Debugging, set all outputs to high to measure amps: Result is 0.14A

digitalWrite(_Achse_Gerade, HIGH); //Set Achse to straight position
digitalWrite(_Func1, HIGH);
digitalWrite(_Func2, HIGH); //Set Func1 and Func2 to high (Dammaufnahme or Bunker)
digitalWrite(_Deichsel_Links, HIGH);
digitalWrite(_Deichsel_Rechts, HIGH); //Set Deichsel to straight position
digitalWrite(_Schnabel_Hoch, HIGH);
digitalWrite(_Schnabel_Runter, HIGH); //Set Schnabel to straight position
digitalWrite(_RollbodenAn, HIGH);
digitalWrite(_Achse_Links, HIGH);
digitalWrite(_Achse_Rechts, HIGH); //Set Achse to straight position
*/

}


