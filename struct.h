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
	ButtopDown		

};