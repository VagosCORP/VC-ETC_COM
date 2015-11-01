char contTim2 = 0;
char deltatDes = 30;
char deltat = 0;
    
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void) {
    contTim2++;
    if(contTim2 > 5) {//1s = 5*200_ms
        contTim2 = 0;
        sysParameters.segundos++;
        if(sysParameters.segundos >= 60) {
            sysParameters.segundos = 0;
            sysParameters.minutos++;
            deltat++;
            if(deltat >= deltatDes) {
                deltat = 0;
                saveGItemEEPROM(sysState.temp);
            }
            if(sysParameters.minutos >= 60) {
                sysParameters.minutos = 0;
                sysParameters.horas++;
                if(sysParameters.horas >= 24) {
                    sysParameters.horas = 0;
                    sysParameters.fecha++;
                }
            }
        }
    }
    IFS0bits.T2IF = 0; // Clear Timer1 Interrupt Flag}
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