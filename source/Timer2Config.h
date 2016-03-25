#ifndef TIMER2_H
#define	TIMER2_H

#include "ProtocolRes.h"

#ifdef	__cplusplus
extern "C" {
#endif

char contTim2 = 0;
    
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    contTim2++;
    if(contTim2 > 4) {//1s = 5*200_ms
        contTim2 = 0;
        ActualTime.segundos++;
        if(ActualTime.segundos >= 86400) {
            ActualTime.segundos = 0;
            ActualTime.fecha++;
        }
        if(sysParameters.process) {
            valDeltaT.segundos++;
            if(valDeltaT.segundos >= deltaTdes.segundos) {
                saveGraphItem2EEPROM(sysState.temp);
                valDeltaT.segundos = 0;
            }
        }
    }
    IFS0bits.T2IF = 0; // Clear Timer2 Interrupt Flag
}

void updateTime() {
    IEC0bits.T2IE = 0;
    ActualTime.secA = sysParameters.secA;
    ActualTime.secB = sysParameters.secB;
    ActualTime.secC = sysParameters.secC;
    ActualTime.fecha = sysParameters.fecha;
    IEC0bits.T2IE = 1;
}
    
void config_timer2(void) {
    /////////Configuración timer 1/////////
    T2CONbits.TSIDL = 1;
    T2CONbits.TCS = 0;
    T2CONbits.TGATE = 0;
    T2CONbits.TCKPS = 0b11; //1:256 PreScaller 6.4us per ++
//    PR2 = 62499; //periodo del timer 1 400ms
    PR2 = 31249; //periodo del timer 1 200ms
    IPC1bits.T2IP = 2; // Prioridad 1 para inttimer1
    IFS0bits.T2IF = 0; // limpiar flag de interrupcion 1
    IEC0bits.T2IE = 1; // habilitar interrupcion del timer1
    TMR2 = 0;
    T2CONbits.TON = 1; // iniciar timer 1
}

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER2_H */