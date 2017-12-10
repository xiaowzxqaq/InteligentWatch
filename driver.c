#include "struct.h"



int DisplayDriver(int state){
    switch (state){

        case MainWin:
            break;
        case Menu:
            break;
        case SetTime:
            break;
        case Schedule:
            break;

    }


}

int UpdateTime(Time *pTime){
    pTime -> second += 1;
    //sec 进位够不够
    if (pTime -> second >= 60) {
        pTime -> second = 0;
        //min 加 1
        pTime -> minute += 1;
        //min进位够不够
        if (JudgeMinuteEn (pTime)) {
            pTime -> hour += 1;
            //hour 进位够不够
            if (JudgeHourEn(pTime)) {
                pTime -> date += 1;
                //date 进位够不够
                if (JudgeDateEn(pTime)) {
                    pTime -> month += 1;
                    //month 进位够不够
                    if (JudgeMonthEn(pTime)) {
                        pTime -> year += 1;
                    }
                }  
            }
        }
    }
    return 0;
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
        //是否为闰年
        if (  (pTime -> year % 100 == 0 && (pTime -> year) % 400 == 0) | (pTime -> year % 100 != 0 && pTime -> year % 4 == 0 )) {
            //为闰年
            if (pTime -> date > 29) {
                pTime -> date = 1;
                return 1;
            } 
        } 
        //非闰年
        else {
            if (pTime -> date > 28) {
                pTime -> date = 1;
                return 1;
            }
        }
    } else {
        //4 6 9 11 小月
        if ( (pTime -> month == 4) | (pTime -> month == 6) | (pTime -> month == 9) | (pTime -> month == 11)) {
            if (pTime -> date > 30) {
                pTime -> date = 1;
                return 1;
            }
        } else {
            //1 3 5 7 9 10 12 大月
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
    pTime -> week = week;
    return 0;
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
int ButtonSet(int button,  Time *pTime,  int* pSetState){
    if(Home == button || HomeL == button){
        /* State change */
        if(MINUTE == *pSetState || HomeL == button){
            return FINISH;
        }
        *pSetState += 1;
        return NEXT;
    }
    if(ButtonUPL == *pSetState){
        if(YEAR == *pSetState){
            return FINISH;
        }
        else{
            *pSetState -= 1;
            return NEXT;
        }
    }

    /* up button clicked*/
    if(ButtonUP == button){
        /* Set time */
        switch (*pSetState){
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
        switch (*pSetState){
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