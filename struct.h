#include <stdio.h>

/*
	Struct: Time
	Description: Describe the calendar and current time 
*/
typedef struct Time{

	int year;
	int month;
	int date;
	int week;
	int hour;
	int minute;
	int second;

}Time;

/*
	Enum: ShowList
	Description: Describe the state of the display driver 
*/
enum ShowList{
	
	MainWin,		
	Menu,			
	SetTime,		
	Schedule

};


/*
	Enum: Week
	Description: Describe which day of a week 
*/
enum Week {
	Sun,
	Mon,
	Tue,
	Wed,
	Tru,
	Fri,
	Sat
};


/*
	Enum: Buttons
	Description: Describe which button is clicked 
*/
enum Buttons{
	
	Home,		
	ButtonUP,			
	ButtonDown		

};

/*
	Enum: SetState
	Description: Describe the state of setting time 
*/
enum SetState{
	
	ERROR,
	YEAR,
	MONTH,
	DATE,
	HOUR,
	MINUTE,
	NEXT,
	CONTINUE,
	FINISH
};