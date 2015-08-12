#ifndef COMUNIC_H
#define	COMUNIC_H

#ifdef	__cplusplus
extern "C" {
#endif


typedef struct floatData {
    union {
        struct {
            char floata : 8;
            char floatb : 8;
            char floatc : 8;
            char floatd : 8;
        };
        struct {
            float floatdat;
        };
  };
}FLOATDATA;
    
void putch(char val) {
    while(U1STAbits.UTXBF);
    U1TXREG = val;
}

void send_float(float valFloat){//funcion pensada para enviar los diferentes elementos de una variable tipo flotante de 3 bytes
    FLOATDATA floatdata;
    floatdata.floatdat = valFloat;
    putch(floatdata.floata);
    putch(floatdata.floatb);
    putch(floatdata.floatc);
    putch(floatdata.floatd);
//    signed char i;
//    for(i = 3; i >= 0; i--){
//        char byte = *((unsigned char *) & valFloat + i);
//        putch(byte);
//    }
}

void send_int32(long valLong){
    signed char i;
    for(i = 3; i >= 0; i--){
        char byte = *((unsigned char *) & valLong + i);
        putch(byte);
    }
}

void send_int16(int valInt){////funcion pensada para enviar los diferentes elementos de una variable int (2 bytes)
    signed char i;
    for(i = 1; i >= 0; i--){
        char byte = *((unsigned char *) & valInt + i);
        putch(byte);
    }
}

float genFloat(char byte3, char byte2, char byte1, char byte0) { //a lo java!!!
    FLOATDATA floatdata;
    floatdata.floata = byte3;
    floatdata.floatb = byte2;
    floatdata.floatc = byte1;
    floatdata.floatd = byte0;
//    float floatRes = floatdata.floatdat;
//    unsigned char * val3 = (unsigned char *) & floatRes + 3; //Mayor significancia
//    unsigned char * val2 = (unsigned char *) & floatRes + 2; //Media mayor
//    unsigned char * val1 = (unsigned char *) & floatRes + 1; //Media menor
//    unsigned char * val0 = (unsigned char *) & floatRes + 0; //Menor significancia
//    *val3 = byte3;
//    *val2 = byte2;
//    *val1 = byte1;
//    *val0 = byte0;
    return floatdata.floatdat;
}
    

#ifdef	__cplusplus
}
#endif

#endif	/* COMUNIC_H */