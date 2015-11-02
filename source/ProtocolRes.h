#ifndef PROTOCOLRES_H
#define	PROTOCOLRES_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "ComandosP.h"

unsigned char charD = 0;

typedef union {
    long valDL;
    float valD;
    struct {
        unsigned char valDa : 8;//LessSifnificant
        unsigned char valDb : 8;
        unsigned char valDc : 8;
        unsigned char valDd : 8;//MostSignificant
    };
}DATA_ITEM;

typedef union {
    unsigned short theGet           : 9;
    struct {
        unsigned getGraphData       : 1;
        unsigned getDesiredState    : 1;
        unsigned getTempPID         : 1;
        unsigned getQ1PID           : 1;
        unsigned getQ2PID           : 1;
        unsigned getOnOff           : 1;
        unsigned getActualState     : 1;
        unsigned getVol             : 1;
        unsigned getPass            : 1;
    };
}TO_SEND;

typedef union {
    unsigned long processDetails      : 32;
    struct {
        unsigned char err0          : 8;
        unsigned char err1          : 8;
        unsigned char n1            : 8;
        unsigned char n             : 8;
    };
    struct {
        unsigned fProcessInterrupted    : 1;
        unsigned eF1                    : 1;
        unsigned eF2                    : 1;
        unsigned eF3                    : 1;
        unsigned eF4                    : 1;
        unsigned eF5                    : 1;
        unsigned eF6                    : 1;
        unsigned eF7                    : 1;
        unsigned eF8                    : 1;
        unsigned eF9                    : 1;
        unsigned eF10                   : 1;
        unsigned eF11                   : 1;
        unsigned eF12                   : 1;
        unsigned eF13                   : 1;
        unsigned eF14                   : 1;
        unsigned eF15                   : 1;
//        unsigned char n1            : 8;
//        unsigned char n             : 8;
    };
}PROCESS_DETAILS;

typedef union {
    unsigned long theTime               : 32;
    struct {
        unsigned char segundos          : 8;
        unsigned char minutos           : 8;
        unsigned char horas             : 8;
        unsigned char fecha             : 8;
    };
}THE_TIME;

typedef struct {
        char syncTime;
        char n;
        char n1;
        char n2;
        char fecha;//
        char horas;//
        char minutos;//
        char segundos;//
        float temp;
        float q1;
        float q2;
        float tempkP;
        float tempkI;
        float tempkD;
        float q1kP;
        float q1kI;
        float q1kD;
        float q2kP;
        float q2kI;
        float q2kD;
        unsigned char process;
        unsigned char pump1;
        unsigned char pump2;
        unsigned long pass;
        long checkSum;
}SYS_PARAMETERS;

typedef struct {
    float temp;
    float q1;
    float q2;
}SYS_STATE;

#ifdef	__cplusplus
}
#endif

#endif	/* PROTOCOLRES_H */