char cont = 0;
    
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void) {
    cont++;
    if(cont > 4) {
//        led++; //LATBbits.LATB5++;
        cont = 0;
    }
    IFS0bits.T1IF = 0; // Clear Timer1 Interrupt Flag}
}
    
void config_timer1(void) {
    /////////Configuración timer 1/////////
    T1CONbits.TSIDL = 1;
    T1CONbits.TCS = 0;
    T1CONbits.TGATE = 0;
    T1CONbits.TCKPS = 0b10; //1:64 PreScaller 4us per ++
    PR1 = 25000; //periodo del timer 1 100ms
    IPC0bits.T1IP = 1; // Prioridad 1 para inttimer1
    IFS0bits.T1IF = 0; // limpiar flag de interrupcion 1
    IEC0bits.T1IE = 1; // habilitar interrupcion del timer1
    TMR1 = 0;
    T1CONbits.TON = 1; // iniciar timer 1
}