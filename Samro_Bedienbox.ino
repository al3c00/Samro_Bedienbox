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
#define _Signal_Active_IN	7



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
#define _Signal_Active_OUT	6			


//Analog Inputs 0 - 1024
//Analog Active 0 - 204 and 820 - 1024
uint16_t Joystick_LowActive_Value = 204;
uint16_t Joystick_HighActive_Value = 820;

//Value: 1024/5*xV (Threshhold = 50)
//Button1 pressed: _Button_Input = 2.5V
//Button2 pressed: _Button_Input = 3.0V
//Both: 1.8V
uint16_t Button_1_LowActive_Value = 462;
uint16_t Button_1_HighActive_Value = 562;

uint16_t Button_2_LowActive_Value = 564;
uint16_t Button_2_HighActive_Value = 800;

uint16_t Button_BOTH_LowActive_Value = 318;
uint16_t Button_BOTH_HighActive_Value = 408;

struct Inputs
{
	uint16_t	Buttons_Value; //2.5V = SW1, 3V = SW2, 1.88V = SW1 & SW2
	uint16_t	Buttons_Measured;

	uint16_t	Joystick_Left_X_Value;
	uint16_t	Joystick_Left_X_Measured;
	uint16_t	Joystick_Left_Y_Value;
	uint16_t	Joystick_Left_Y_Measured;

	uint16_t	Joystick_Right_X_Value;
	uint16_t	Joystick_Right_X_Measured;
	uint16_t	Joystick_Right_Y_Value;
	uint16_t	Joystick_Right_Y_Measured;
};
Inputs input;


uint16_t last_time;
uint16_t interval_time;

bool loop_New_Values = false;

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
		pinMode(_Signal_Active_IN, INPUT);

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
		pinMode(_Signal_Active_OUT, OUTPUT);
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
		if ((input.Joystick_Left_X_Measured = analogRead(_Joystick_Left_X)) != input.Joystick_Left_X_Value)
		{
			loop_New_Values = true;
		}
		if ((input.Joystick_Left_Y_Measured = analogRead(_Joystick_Left_Y)) != input.Joystick_Left_Y_Value)
		{
			loop_New_Values = true;
		}
		//Read right joystick
		if ((input.Joystick_Right_X_Measured = analogRead(_Joystick_Right_X)) != input.Joystick_Right_X_Value)
		{
			loop_New_Values = true;
		}
		if ((input.Joystick_Right_Y_Measured = analogRead(_Joystick_Right_Y)) != input.Joystick_Right_Y_Value)
		{
			loop_New_Values = true;
		}
		//Read button 
		if ((input.Buttons_Measured = analogRead(_Button_Input)) != input.Buttons_Value)
		{
			loop_New_Values = true;
		}
		
		
		
		//If there are any new Input Values, go refresh everything
		if (loop_New_Values)
		{
		
			//Buttons
			{
				if (input.Buttons_Measured > Button_1_LowActive_Value && input.Buttons_Measured < Button_1_HighActive_Value)
				{
					digitalWrite(_RollbodenAn, HIGH);
					digitalWrite(_Achse_Gerade, LOW);
				}

				else if (input.Buttons_Measured > Button_2_LowActive_Value && input.Buttons_Measured < Button_2_HighActive_Value)
				{
					digitalWrite(_Achse_Gerade, HIGH);
					digitalWrite(_RollbodenAn, LOW);
				}

				else if (input.Buttons_Measured > Button_BOTH_LowActive_Value && input.Buttons_Measured < Button_BOTH_HighActive_Value)
				{
					digitalWrite(_Achse_Gerade, HIGH);
					digitalWrite(_RollbodenAn, HIGH);
				}
				else
				{
					digitalWrite(_Achse_Gerade, LOW);
					digitalWrite(_RollbodenAn, LOW);
				}

			}



			//Achse
			{
				if (input.Joystick_Left_X_Measured < Joystick_LowActive_Value)
				{
					digitalWrite(_Deichsel_Links, HIGH);
				}
				else if (input.Joystick_Left_X_Measured >= Joystick_LowActive_Value && input.Joystick_Left_X_Measured < Joystick_HighActive_Value)
				{
					digitalWrite(_Deichsel_Links, LOW);
					digitalWrite(_Deichsel_Rechts, LOW);
				}
				else if (input.Joystick_Left_X_Measured > Joystick_HighActive_Value)
				{
					digitalWrite(_Deichsel_Rechts, HIGH);
				}


			}


			//Deichsel
			{
				if (input.Joystick_Left_Y_Measured < Joystick_LowActive_Value)
				{
					digitalWrite(_Achse_Links, HIGH);
				}
				else if (input.Joystick_Left_Y_Measured >= Joystick_LowActive_Value && input.Joystick_Left_Y_Measured < Joystick_HighActive_Value)
				{
					digitalWrite(_Achse_Links, LOW);
					digitalWrite(_Achse_Rechts, LOW);
				}
				else if (input.Joystick_Left_Y_Measured > Joystick_HighActive_Value)
				{
					digitalWrite(_Achse_Rechts, HIGH);
				}
			}

			//Func--Dammaufnahme oder Bunker
			{
				if (input.Joystick_Right_Y_Measured < Joystick_LowActive_Value)
				{
					digitalWrite(_Func2, HIGH);
				}
				else if (input.Joystick_Right_Y_Measured >= Joystick_LowActive_Value && input.Joystick_Right_Y_Measured < Joystick_HighActive_Value)
				{
					digitalWrite(_Func1, LOW);
					digitalWrite(_Func2, LOW);
				}
				else if (input.Joystick_Right_Y_Measured > Joystick_HighActive_Value)
				{
					digitalWrite(_Func1, HIGH);
				}
			}

			//Schnabel
			{
				if (input.Joystick_Right_X_Measured < Joystick_LowActive_Value)
				{
					digitalWrite(_Schnabel_Runter, HIGH);
				}
				else if (input.Joystick_Right_X_Measured >= Joystick_LowActive_Value && input.Joystick_Right_X_Measured < Joystick_HighActive_Value)
				{
					digitalWrite(_Schnabel_Runter, LOW);
					digitalWrite(_Schnabel_Hoch, LOW);
				}
				else if (input.Joystick_Right_X_Measured > Joystick_HighActive_Value)
				{
					digitalWrite(_Schnabel_Hoch, HIGH);
				}
			}

			input.Buttons_Value = input.Buttons_Measured;
			input.Joystick_Left_X_Value = input.Joystick_Left_X_Measured;
			input.Joystick_Left_Y_Value = input.Joystick_Left_Y_Measured;
			input.Joystick_Right_X_Value = input.Joystick_Right_X_Measured;
			input.Joystick_Right_Y_Value = input.Joystick_Right_Y_Measured;

			loop_New_Values = false;


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


