#include <stdio.h>



int DisplayDriver(int state){



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
    if (pTime -> minute >= 60) {
        pTime -> minute = 0;   
        return 1;    
    }
    return 0;
}

int JudgeHourEn(Time *pTime) {
    if (pTime -> hour >= 24) {
        pTime -> hour = 0;
        return 1;
    }
    return 0;
}

int JudgeDateEn(Time *pTime) {
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
    if (pTime -> month > 12) {
        pTime -> month = 1;
        return 1;
    }
    return 0;
} 


int ButtonClicked(int state, int button){


}