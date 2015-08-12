void putch(char val) {
    while(U1STAbits.UTXBF);
    U1TXREG = val;
}
    
void __attribute__((__interrupt__, no_auto_psv)) _U1RXInterrupt(void) {
    rcvProtocol(U1RXREG);
    IFS0bits.U1RXIF = 0; // Clear U1RX Interrupt Flag
}
    
void config_uart1(void) {
    /////////Configuración UART 1/////////
//    U1BRG = 25; //BaudRate = 38400;//24F
//    U1BRG = 8; //BaudRate = 115200;//24F
    //    U1BRG = 259; //BaudRate = 9600;//24H
    U1BRG = 64; //BaudRate = 38400;//24H
    //    U1BRG = 21; //BaudRate = 115200;//24H
    U1MODEbits.UARTEN = 1;
    U1MODEbits.USIDL = 0;
    U1MODEbits.IREN = 0;
    U1MODEbits.RTSMD = 1;
    U1MODEbits.UEN = 0b00;
    U1MODEbits.WAKE = 0;
    U1MODEbits.LPBACK = 0;
    U1MODEbits.ABAUD = 0;
    U1MODEbits.URXINV = 0;//24F ==> borrar U
    U1MODEbits.BRGH = 0;
    U1MODEbits.PDSEL = 0b00;
    U1MODEbits.STSEL = 0;
    U1STAbits.UTXISEL1 = 0;
    U1STAbits.UTXISEL0 = 1;
    U1STAbits.UTXINV = 0;
    U1STAbits.UTXBRK = 0;
    U1STAbits.UTXEN = 1;
    U1STAbits.URXISEL = 0b01;
    U1STAbits.ADDEN = 0;
    U1STAbits.RIDLE = 0;
    IPC2bits.U1RXIP = 1; // Prioridad 1 para inttimer1
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
    //    U1STAbits.UTXBF //recurso, buffer vacio
    //    U1STAbits.URXDA //recurso, datos recibidos
}