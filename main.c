#include <xc.h>
#include <stdio.h>

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = ON        // Internal/External Switchover (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)




/************Struct Define***********/
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
/***********************************/

/************Enum Define************/
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
	Gloable Variable Used:keyState
*/
enum Buttons{
	
	Home,		
	ButtonUP,			
	ButtonDown,		
	HomeL,
	ButtonUPL,
	ButtonDownL
	
};

/*
	Enum: SetState
	Description: Describe the state of setting time 
	Gloable Variable Used:setState
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

/*
	Enum: StateMachine
	Description: Judge which button is clicked and return the state 
	Gloable Variable Used:setState current keyCurrent keyState
*/
enum StateMachine{
    NONE = 0xf0,
    CLICKED,
    LCLICKED
};
/************************************/

/************Constant Define*********/
#define high 1
#define low 0 
#define HIGH 1
#define LOW 0 
#define MINSCAN 1
#define MAXSCAN 10

#define FITST_LINE 0B00001110
/***********************************/

/************Timer******************/
#define TIMER1CNT 1
#define TIMERMUL 1
#define TMRVALUE (65536 - TIMER1CNT) 
#define TMRHVALUE (TMRVALUE>>8) 
#define TMRLVALUE ((char)TMRVALUE)
/***********************************/

/*************Pin Define*************/
#define RES (PORTAbits.RA2)
#define DC (PORTAbits.RA3)
#define SCL (PORTAbits.RA0)
#define SDA (PORTAbits.RA1)
#define UPBUT (PORTAbits.RA4)
#define HOME (PORTAbits.RA5)
#define DOWNBUT (PORTAbits.RA6)
/***********************************/


/*************Functions****************/
/**************SPI**************/
void Initial_LY096BG30();
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture_MainWin();
void Write_SPI_Command(unsigned char ucCmd);
void Write_SPI_Data(unsigned char ucData);
int DisplayDriver(char state);
/**************Time*************/
int UpdateTime(Time *pTime);

/**************Show*************/
void ShowLineDate(char y);
void ShowLineTime(char y);
int ButtonSet(char button,  Time *pTime);

/**************Button*************/
char KeyState();
char ScanKey();


/***********************Initial code*********************/
void Initial_LY096BG30()
{
	Write_SPI_Command(0xAE);//display off
	Write_SPI_Command(0x20);//Set Memory Addressing Mode	
	Write_SPI_Command(0x10);//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	Write_SPI_Command(0xb0);//Set Page Start Address for Page Addressing Mode,0-7
	Write_SPI_Command(0xc8);//Set COM Output Scan Direction
	Write_SPI_Command(0x00);//---set low column address
	Write_SPI_Command(0x10);//---set high column address
	Write_SPI_Command(0x40);//--set start line address
	Write_SPI_Command(0x81);//--set contrast control register
	Write_SPI_Command(0x7f);
	Write_SPI_Command(0xa1);//--set segment re-map 0 to 127
	Write_SPI_Command(0xa6);//--set normal display
	Write_SPI_Command(0xa8);//--set multiplex ratio(1 to 64)
	Write_SPI_Command(0x3F);//
	Write_SPI_Command(0xa4);//0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	Write_SPI_Command(0xd3);//-set display offset
	Write_SPI_Command(0x00);//-not offset
	Write_SPI_Command(0xd5);//--set display clock divide ratio/oscillator frequency
	Write_SPI_Command(0xf0);//--set divide ratio
	Write_SPI_Command(0xd9);//--set pre-charge period
	Write_SPI_Command(0x22); //
	Write_SPI_Command(0xda);//--set com pins hardware configuration
	Write_SPI_Command(0x12);
	Write_SPI_Command(0xdb);//--set vcomh
	Write_SPI_Command(0x20);//0x20,0.77xVcc
	Write_SPI_Command(0x8d);//--set DC-DC enable
	Write_SPI_Command(0x14);//
	Write_SPI_Command(0xaf);//--turn on oled panel 
}
/*********************************************************/

/***********************Picture Constant******************/



const unsigned char SmallShow[2][11][8]={
0x00, 0xF0, 0x18, 0x0C, 0x0C, 0x18, 0xF0, 0x00,
0x00, 0x00, 0x00, 0xC0, 0x7E, 0x00, 0x00, 0x00,
0x00, 0x30, 0x18, 0x0C, 0x04, 0x8C, 0xF8, 0x60,
0x00, 0x0C, 0x86, 0x82, 0xC6, 0x64, 0x38, 0x00,
0x00, 0x00, 0xE0, 0x18, 0x00, 0xFC, 0x00, 0x00,
0x00, 0x08, 0xFE, 0x88, 0x88, 0x88, 0x08, 0x00,
0x00, 0x00, 0xC0, 0xF0, 0x98, 0x86, 0x00, 0x00,
0x00, 0x04, 0x04, 0x04, 0x84, 0x7C, 0x0C, 0x00,
0x00, 0xF8, 0x8C, 0x84, 0x84, 0x44, 0x7C, 0x00,
0x00, 0xF8, 0x8C, 0x84, 0x84, 0x44, 0xFC, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x1F, 0x10, 0x20, 0x20, 0x30, 0x0F, 0x00, 
0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00,
0x00, 0x30, 0x38, 0x2C, 0x26, 0x23, 0x21, 0x00,
0x00, 0x20, 0x41, 0x41, 0x61, 0x31, 0x1E, 0x00,
0x00, 0x03, 0x03, 0x02, 0x02, 0x7F, 0x02, 0x00,
0x00, 0x10, 0x21, 0x21, 0x30, 0x18, 0x0F, 0x00,
0x00, 0x1F, 0x23, 0x20, 0x30, 0x18, 0x0F, 0x00,     
0x00, 0x00, 0x00, 0x7C, 0x07, 0x00, 0x00, 0x00,
0x38, 0x4E, 0x43, 0x43, 0x42, 0x46, 0x3C, 0x00,
0x00, 0x00, 0x00, 0x60, 0x38, 0x0E, 0x03, 0x00,
0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00

};

const unsigned char BigShow[3][11][12]={
0x00, 0x80, 0xF0, 0xF8, 0x78, 0x18, 0x18, 0x18, 0xF8, 0xF0, 0xC0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFE, 0xFE, 0x1E, 0x00, 0x00,
0x00, 0x30, 0xF8, 0xFC, 0x7E, 0x1E, 0x1E, 0x1E, 0x3E, 0xFC, 0xFC, 0xF8,
0x00, 0x30, 0x38, 0x3C, 0x1E, 0x0E, 0x06, 0x0E, 0x0E, 0xBC, 0xFC, 0xF8,
0x00, 0x00, 0x80, 0xE0, 0xF8, 0x3C, 0x00, 0xF8, 0xFC, 0x00, 0x00, 0x00,
0x00, 0x60, 0xE0, 0xF8, 0xFC, 0x7C, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70,
0x00, 0x00, 0x80, 0xF0, 0xFE, 0x3E, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x18, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0xF8, 0xF8, 0xF8, 0x78,
0x7C, 0xFE, 0xC6, 0x06, 0x06, 0x06, 0x0E, 0x0E, 0x9E, 0xFE, 0xF8, 0x00,
0xF0, 0xFC, 0x1C, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x1C, 0xFC, 0xF8, 0xF0,
0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00,
0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFF, 0x1F, 0x07, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0x80, 0xE0, 0xF0, 0xF8, 0x7E, 0x1F, 0x07, 0x01,
0x00, 0x00, 0x10, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3E, 0x7F, 0xF7, 0xC1,
0x30, 0x78, 0x7F, 0x7F, 0x70, 0x70, 0x70, 0xFF, 0xFF, 0x70, 0x70, 0x00,
0x00, 0x00, 0x3F, 0x3F, 0x1F, 0x18, 0x18, 0x18, 0x38, 0xF8, 0xE0, 0xC0,
0xC0, 0xFC, 0x7F, 0x1F, 0x19, 0x18, 0x18, 0x18, 0x38, 0xF8, 0xE0, 0xC0,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xFC, 0xFF, 0x0F, 0x03, 0x00, 0x00,
0x80, 0xE1, 0x73, 0x3F, 0x1E, 0x1C, 0x3E, 0x7A, 0xE3, 0xC1, 0x80, 0x00,
0x01, 0x0F, 0x1E, 0x1C, 0x1C, 0x38, 0xB8, 0xB8, 0xFC, 0xFF, 0x3F, 0x07,
0x00, 0x00, 0x00, 0x00, 0x00, 0x81, 0x81, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x01, 0x07, 0x1F, 0x38, 0x38, 0x38, 0x3C, 0x3E, 0x0F, 0x01, 0x00,
0x00, 0x00, 0x00, 0x3E, 0x3F, 0x3F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x1E, 0x3F, 0x3F, 0x3B, 0x3B, 0x39, 0x38, 0x38, 0x38, 0x38, 0x18,
0x00, 0x30, 0x70, 0x60, 0x60, 0x60, 0x70, 0x70, 0x78, 0x3C, 0x1F, 0x07,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, 0x00, 0x00,
0x0E, 0x1E, 0x3C, 0x70, 0x70, 0x70, 0x70, 0x38, 0x38, 0x1C, 0x1F, 0x07,
0x01, 0x1F, 0x3E, 0x70, 0x70, 0x70, 0x70, 0x38, 0x38, 0x1C, 0x1F, 0x07,
0x00, 0x00, 0x00, 0x00, 0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,
0x1F, 0x31, 0x30, 0x60, 0x60, 0x60, 0x30, 0x30, 0x39, 0x1F, 0x0F, 0x07,
0x00, 0x00, 0x40, 0x70, 0x38, 0x1E, 0x0F, 0x07, 0x01, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char weeker[2][8] = {
0xFC, 0x04, 0xA4, 0xF4, 0xF4, 0xA4, 0x04, 0xFC,
0x3F, 0x00, 0x0E, 0x0A, 0x0A, 0x0E, 0x20, 0x7F
};


/******************Gloable Variables**********************/
Time time;
Time *pTime = &time;

char mainState = MainWin;
char current = NONE;
char keyState = NONE;
char key = NONE;
char count = 0;
char keyCurrent =NONE;

char setState = 0;
/*********************************************************/


void interrupt isr(void)
{
    

    if(PIR1bits.TMR1IF == 1){
        PIR1bits.TMR1IF = 0;   
        //UpdateTime(pTime);
        key = ScanKey();
        KeyState();
        ButtonSet(keyState, pTime);
    }
    if(INTCONbits.TMR0IF == 1){
        INTCONbits.TMR0IF = 0;
        DisplayDriver(mainState);
    }
    

}


/*******************************Main*******************************/
void main(void)
{
    OSCCONbits.IRCF = 0b1101;
    LATA = 0x00;
    ANSELA = 0x00;
    TRISA = 0x00;
	RES=0;
	Delay_1ms(10);
	RES=1;
	Delay_1ms(10);				//
	DC=0;			//			//
	Initial_LY096BG30();
	Delay_1ms(5);
	pTime->date = 31;
    pTime->year = 2017;
    pTime->hour = 23;
    pTime->minute = 59;
    pTime->second = 24;
    pTime->month = 12;
    
    
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    
    PIR1bits.TMR1IF = 0;
  
    PIE1bits.TMR1IE = 1;
    
    INTCONbits.IOCIE = 1;
    //Timer0 
    INTCONbits.TMR0IF = 0;
    OPTION_REG  =   0b00000000;

      
    TMR1H = TMRHVALUE;
    TMR1L = TMRLVALUE;
  
    T1CON = 0b00001101;
    T1CONbits.T1CKPS = 0b00;
    TRISAbits.TRISA4 = 1;
    TRISAbits.TRISA5 = 1;
    TRISAbits.TRISA6 = 1;
    WPUAbits.WPUA4 = 1;
    WPUAbits.WPUA5 = 1;
    WPUAbits.WPUA6 = 1;
            
        while(1){

        }
        //fill_picture(0xff);
	
}

int JudgeMinuteEn(Time *pTime) {
    if (pTime -> minute < 0) {
        pTime -> minute = 0;
        return 0;
    } 
    if (pTime -> minute >= 60) {
        pTime -> minute = 0;   
        return 1;    
    }
    return 0;
}

int JudgeHourEn(Time *pTime) {
    if (pTime -> hour < 0) {
        pTime -> hour = 0;
        return 0;
    } 
    if (pTime -> hour >= 24) {
        pTime -> hour = 0;
        return 1;
    }
    return 0;
}

int JudgeDateEn(Time *pTime) {
    if (pTime -> date < 0) {
        pTime -> date = 1;
        return 0;
    } 

    if (pTime -> month == 2) {
        //?????
        if (  (pTime -> year % 100 == 0 && (pTime -> year) % 400 == 0) | (pTime -> year % 100 != 0 && pTime -> year % 4 == 0 )) {
            //???
            if (pTime -> date > 29) {
                pTime -> date = 1;
                return 1;
            } 
        } 
        //???
        else {
            if (pTime -> date > 28) {
                pTime -> date = 1;
                return 1;
            }
        }
    } else {
        //4 6 9 11 ??
        if ( (pTime -> month == 4) | (pTime -> month == 6) | (pTime -> month == 9) | (pTime -> month == 11)) {
            if (pTime -> date > 30) {
                pTime -> date = 1;
                return 1;
            }
        } else {
            //1 3 5 7 9 10 12 ??
            if (pTime -> date > 31) {
                pTime ->date = 1;
                return 1;
            }
        }
    }
    return 0;
}
int JudgeMonthEn(Time *pTime) {
    if (pTime -> month < 0) {
        pTime -> month = 1;
        return 0;
    } 
    if (pTime -> month > 12) {
        pTime -> month = 1;
        return 1;
    } 
    return 0;
} 

int CalWeek(Time *pTime) {
    int date = pTime -> date;
    int month = pTime -> month;
    int year = pTime -> year;
    int week;
    if (month == 1) {
        month = 13;
        year -= 1;
    } else if(month == 2) {
        month = 14;
        year -= 1;
    }
    week = (date + 2 * month + 3 * (month + 1)/5 + year + year / 4 - year/100 + year/400 +1) % 7;
    if (week == 0) {
        week = 7;
    }
    pTime -> week = week;
    return 0;
}


int UpdateTime(Time *pTime){
    pTime -> second += 1;
    //sec ?????
    if (pTime -> second >= 60) {
        pTime -> second = 0;
        //min ? 1
        pTime -> minute += 1;
        //min?????
        if (JudgeMinuteEn (pTime)) {
            pTime -> hour += 1;
            //hour ?????
            if (JudgeHourEn(pTime)) {
                pTime -> date += 1;
                //date ?????
                if (JudgeDateEn(pTime)) {
                    pTime -> month += 1;
                    //month ?????
                    if (JudgeMonthEn(pTime)) {
                        pTime -> year += 1;
                    }
                }  
            }
        }
    }
    CalWeek(pTime);
    return 0;
}


void ShowLineTime(char y){
    y -= 2;
    unsigned char tens = (pTime->hour) / 10;
    unsigned char ones = (pTime->hour) % 10;
    char i = 0;
    //margin
    for (i = 0; i < 13; i++){
        Write_SPI_Data(0x00);
    }
    //show hour
    for(i = 0; i < 12; i++){
        Write_SPI_Data(BigShow[y][tens][i]);
    }
    Write_SPI_Data(0x00);
    Write_SPI_Data(0x00);
    for(i = 0; i < 12; i++){
        Write_SPI_Data(BigShow[y][ones][i]);
    }
    for(i = 0; i < 12; i++){
        Write_SPI_Data(BigShow[y][10][i]);
    }

    //show minutes
    unsigned char tens = (pTime->minute) / 10;
    unsigned char ones = (pTime->minute) % 10;
    for(i = 0; i < 12; i++){
        Write_SPI_Data(BigShow[y][tens][i]);
    }
    Write_SPI_Data(0x00);
    Write_SPI_Data(0x00);
    for(i = 0; i < 12; i++){
        Write_SPI_Data(BigShow[y][ones][i]);
    }
    for(i = 0; i < 12; i++){
        Write_SPI_Data(BigShow[y][10][i]);
    }

    //show seconds
    unsigned char tens = (pTime->second) / 10;
    unsigned char ones = (pTime->second) % 10;
    for(i = 0; i < 12; i++){
        Write_SPI_Data(BigShow[y][tens][i]);
    }
    Write_SPI_Data(0x00);
    Write_SPI_Data(0x00);
    for(i = 0; i < 12; i++){
        Write_SPI_Data(BigShow[y][ones][i]);
    }

    //margin
    for (i = 0; i < 13; i++){
        Write_SPI_Data(0x00);
    }
}

void ShowLineDate(char y){
    y -= 6;
    unsigned char thous = (pTime->year) / 1000;
    unsigned char hunders = ((pTime->year)%1000) / 100;
    unsigned char tens =  ((pTime->year)%100) / 10;
    unsigned char ones = (pTime->year) % 10;
    char i = 0;
    //margin
    for (i = 0; i < 8; i++){
        Write_SPI_Data(0x00);
    }
    //show year
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][thous][i]);
    }
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][hunders][i]);
    }
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][tens][i]);
    }
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][ones][i]);
    }

    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][10][i]);
    }

    //show month
    unsigned char tens = (pTime->month) / 10;
    unsigned char ones = (pTime->month) % 10;
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][tens][i]);
    }

    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][ones][i]);
    }
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][10][i]);
    }

    //show date
    unsigned char tens = (pTime->date) / 10;
    unsigned char ones = (pTime->date) % 10;
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][tens][i]);
    }
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][ones][i]);
    }

    //margin
    for (i = 0; i < 8; i++){
        Write_SPI_Data(0x00);
    }
    for(i = 0; i < 8; i++){
        Write_SPI_Data(weeker[y][i]);
    }
    for (i = 0; i < 8; i++){
        Write_SPI_Data(0x00);
    }
    for(i = 0; i < 8; i++){
        Write_SPI_Data(SmallShow[y][pTime->week][i]);
    }
    for (i = 0; i < 8; i++){
        Write_SPI_Data(0x00);
    }

}

/*********************/
void Write_SPI_Data(unsigned char ucData)
{
    unsigned char i = 8;
    DC   = 1;
    SCL  = 0;
    
    while (i--)
    {
        if (ucData & 0x80)
        {
            SDA = 1;
        }
        else
        {
            SDA = 0;
        }
        
        SCL = 1; 
        asm("NOP");
        SCL = 0;    
        ucData <<= 1;    
    }

    return;
}

/**************************/
void Write_SPI_Command(unsigned char ucCmd)
{
    unsigned char i = 8;
    DC   = 0;
    SCL  = 0;
    while (i--)
    {
        if (ucCmd & 0x80)
        {
            SDA = 1;
        }
        else
        {
            SDA = 0;
        }
        SCL = 1;
        asm("NOP");        
        SCL = 0;    
        ucCmd <<= 1;   
    }  

    return;
}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        Write_SPI_Command(0xb0+m);      //page0-page1
        Write_SPI_Command(0x00);        //low column start address
        Write_SPI_Command(0x10);        //high column start address
        for(n=0;n<128;n++)
            {
                Write_SPI_Data(fill_Data);
            }
    }
}



/******************************************
// picture
******************************************/
void Picture_MainWin()
{
  char x,y;

  for(y=0;y<8;y++)
    {
      Write_SPI_Command(0xb0+y);
      Write_SPI_Command(0x00);
      Write_SPI_Command(0x10);
      switch(y){
            case 0:
                for(x=0;x<128;x++)
                {
                    Write_SPI_Data(0x00);
                }
                break;
            case 1:
                for(x=0;x<128;x++)
                {
                    Write_SPI_Data(0x00);
                }
                break;
            case 2:
                ShowLineTime(y);
                break;
            case 3:
                ShowLineTime(y);
                break;
            case 4:
                ShowLineTime(y);
                break;
            case 5:
                for(x=0;x<128;x++)
                {
                    Write_SPI_Data(0x00);
                }
                break;
            case 6:
                ShowLineDate(y);
                break;
            case 7:
                ShowLineDate(y);
                break;
          default:
                for(x=0;x<128;x++)
                {
                    Write_SPI_Data(0x00);
                }
                break;
                        
      }


    }
  
}


/***********************Delay****************************************/
void Delay_50ms(unsigned int Del_50ms)      //
{
    unsigned int m;
    for(;Del_50ms>0;Del_50ms--)
        for(m=6245;m>0;m--);
}

void Delay_1ms(unsigned int Del_1ms)        //
{
    unsigned char j;
    while(Del_1ms--)
    {   
        for(j=0;j<123;j++);
    }
}

int DisplayDriver(char state){
    switch (state){
        case MainWin:
            Picture_MainWin();
            break;
        case Menu:
            break;
        case SetTime:
            break;
        case Schedule:
            break;

    }


}
/*
	Output: keyState
	Description: State Machine, return button clicked state

*/
char KeyState(){
    switch (current){
        case NONE:
            if(NONE == key){
                keyState = NONE;
                break;
            }else{
                count = 1;
                current = CLICKED;
                keyCurrent = key;
                break;
            }
        case CLICKED:
            if(NONE == key){
                if(count >= MINSCAN && count <= MAXSCAN){
                    count = 0;
                    current = NONE;
                    switch(keyCurrent){
                        case Home:
                            keyState = Home;
                            keyCurrent =NONE;
                            break;
                        case ButtonUP:
                            keyState = ButtonUP;
                            keyCurrent =NONE;
                            break;
                        case ButtonDown:
                            keyState = ButtonDown;
                            keyCurrent =NONE;
                            break;
                    }
                    break;
                }else if(current < MINSCAN){
                    count = 0;
                    current = NONE;
                    keyCurrent =NONE;
                    break;
                }
             }else if(keyCurrent == key){
                ++count;
                if(count > MAXSCAN){
                    current = LCLICKED;
                    switch(keyCurrent){
                        case Home:
                            keyState = HomeL;
                            break;
                        case ButtonUP:
                            keyState = ButtonUPL;
                            break;
                        case ButtonDown:
                            keyState = ButtonDownL;
                            break;
                    }
                }
                break;
             }else{
                count = 0;
                current = NONE;
                keyCurrent =NONE;
                keyState = NONE;
                break;
            }
        case LCLICKED:
            if(NONE == key){
                count = 0;
                current = NONE;
                keyCurrent =NONE;
                keyState = NONE;
                break;
            }
            break;
    }

}

/*
	Output: key
	Description: Scan IO and return which key is clicked

*/
char ScanKey(){
    //tmp test whether two or more buttons are clicked
    //(0 -> three buttons) (1-> two buttons)  (2 -> one buttons) (3 -> no button)
    char tmp = UPBUT + DOWNBUT + HOME;
    if(2 == tmp){
        if(0 == UPBUT){
            return ButtonUP;
        }
        if(0 == HOME){
            return Home;
        }
        if(0 == DOWNBUT){
            return ButtonDown;
        }
        return ERROR;
    }else{
        return NONE;
    }

}

/*
    Input: 
        button: which button is clicked
        pTime: pointer to the Time Struct
        pSetState: the marking state of SetState, change which to set
    Output:
        NEXT: change to next state
        CONTINUE: waiting for other input
        FINISH: change to MainWin
        ERROR: unkonw error
    Description:
        According to which button is cliced and set time
    Author: Yin
*/

int ButtonSet(char button,  Time *pTime){

    if(Home == button || HomeL == button){
        /* State change */
        if(MINUTE == setState || HomeL == button){
            setState = 0;
            return FINISH;
        }
        ++setState;
        return NEXT;
    }
    if(ButtonUPL == button){
        if(YEAR == setState){
            return FINISH;
        }
        else{
            setState -= 1;
            return NEXT;
        }
    }

    /* up button clicked*/
    if(ButtonUP == button){
        /* Set time */
        switch (setState){
            case YEAR:
                pTime->year += 1;
                break;
            case MONTH:
                pTime->month += 1;
                JudgeMonthEn(pTime);
                break;
            case DATE:
                pTime->date += 1;
                JudgeDateEn(pTime);
                break;
            case HOUR:
                pTime->hour += 1;
                JudgeHourEn(pTime);
                break;
            case MINUTE:
                pTime->minute += 1;
                JudgeMinuteEn(pTime);
                break;
        }
        return CONTINUE;
    }
    /* down button clicked */
    if(ButtonDown == button){
        /* Set time */
        switch (setState){
            case YEAR:
                pTime->year -= 1;
                break;
            case MONTH:
                pTime->month -= 1;
                JudgeMonthEn(pTime);
                break;
            case DATE:
                pTime->date -= 1;
                JudgeDateEn(pTime);
                break;
            case HOUR:
                pTime->hour -= 1;
                JudgeHourEn(pTime);
                break;
            case MINUTE:
                pTime->minute -= 1;
                JudgeMinuteEn(pTime);
                break;
        }
        return CONTINUE;
    }
    return ERROR;
}