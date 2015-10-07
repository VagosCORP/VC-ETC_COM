#include "ComandosP.h"

unsigned char charD = 0;

typedef union {
    long valDL;
    float valD;
    struct {
//        unsigned char charD : 8;
        unsigned char valDa : 8;//LessSifnificant
        unsigned char valDb : 8;
        unsigned char valDc : 8;
        unsigned char valDd : 8;//MostSignificant
    };
//    struct { 
//        unsigned char       : 8;
        
//    };
//    struct {
//        unsigned char       : 8;
        
//    };
}DATA_ITEM;

typedef union {
    unsigned char theGet            : 8;
    struct {
    //    unsigned getAllData        : 1;
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

typedef struct {
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

TO_SEND temp_send;
TO_SEND final_send;

DATA_ITEM dataItem;
SYS_PARAMETERS newSysParameters = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SYS_PARAMETERS sysParameters = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SYS_STATE sysState = {0,0,0};

char sendDataEN = 0;
int sendCont = 0;
int sendInit = 0;
int dataCont = 0;
unsigned char tempGet = 0;
unsigned char betaGet = 0;
//unsigned char applyGet[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char itemCont = 0;
char floatCont = 0;
char dataInit = 0;
char itemInit = 0;
long checkSum = 0;
long checkSumM = 0;

void initVars() {
    sysState.temp = 0.0f;//temperatura actual
    sysState.q1 = 0.0f;//caudal actual 1
    sysState.q2 = 0.0f;//caudal actual 2
    sysParameters.pass = 1234567890;
    sysParameters.temp = 35.1f;//temperatura deseada
    sysParameters.q1 = 5.2f;//caudal deseado 1
    sysParameters.q2 = 5.1f;//caudal deseado 2
    sysParameters.tempkP = 2000.00f;//constante proporcional
    sysParameters.tempkI = 8.0f;//constante integral
    sysParameters.tempkD = 2.00f;//constante derivativa
    sysParameters.q1kP = 120.00f;//constante proporcional
    sysParameters.q1kI = 12.0f;//constante integral
    sysParameters.q1kD = 0.40f;//constante derivativa
    sysParameters.q2kP = 120.00f;//constante proporcional
    sysParameters.q2kI = 12.0f;//constante integral
    sysParameters.q2kD = 0.46f;//constante derivativa
    sysParameters.process = 0;
    sysParameters.pump1 = 0;
    sysParameters.pump2 = 0;
    temp_send.theGet = 0;
    final_send.theGet = 0;
    newSysParameters = sysParameters;
}

short sendItem(char comando, float floatVal) {
    short checksum = comando; 
    putch(comando);
    checksum += send_float(floatVal);
    return checksum;
}

short sendItemL(char comando, long longVal) {
    short checksum = comando; 
    putch(comando);
    checksum += send_int32(longVal);
    return checksum;
}

void sendOnOff(char process, char pump1, char pump2) {
    short checksum = cOnOff;
    putch(PKG_I);
    putch(cOnOff);
    putch(0);
    putch(pump1);
    checksum += pump1;
    putch(pump2);
    checksum += pump2;
    putch(process);
    checksum += process;
    sendItemL(cChecking,checksum);
    putch(PKG_F);
}

void sendError(char err) {
    short checksum = cError;
    putch(PKG_I);
    putch(cError);
    putch(0);
    putch(0);
    putch(0);
    putch(err);
    checksum += err;
    sendItemL(cChecking, (long)checksum);
    putch(PKG_F);
}

//void sendData(void) {
//    if(sendDataEN && sendCont > 0) {
//        long checksumVal = 0;
//        switch(applyGet[sendCont]) {
//            case(cgetActualState): {
//                putch(PKG_I);
//                checksumVal += sendItem(ctempA, sysState.temp);
//                checksumVal += sendItem(cq1A, sysState.q1);
//                checksumVal += sendItem(cq2A, sysState.q2);
//                sendItemL(cChecking, checksumVal);
//                putch(PKG_F);
//                break;
//            }
//            case(cgetDesiredState): {
//                putch(PKG_I);
//                checksumVal += sendItem(ctemp, sysParameters.temp);
//                checksumVal += sendItem(cq1, sysParameters.q1);
//                checksumVal += sendItem(cq2, sysParameters.q2);
//                sendItemL(cChecking, checksumVal);
//                putch(PKG_F);
//                break;
//            }
//            case(cgetTempPID): {
//                putch(PKG_I);
//                checksumVal += sendItem(ctempkP, sysParameters.tempkP);
//                checksumVal += sendItem(ctempkI, sysParameters.tempkI);
//                checksumVal += sendItem(ctempkD, sysParameters.tempkD);
//                sendItemL(cChecking, checksumVal);
//                putch(PKG_F);
//                break;
//            }
//            case(cgetQ1PID): {
//                putch(PKG_I);
//                checksumVal += sendItem(cq1kP, sysParameters.q1kP);
//                checksumVal += sendItem(cq1kI, sysParameters.q1kI);
//                checksumVal += sendItem(cq1kD, sysParameters.q1kD);
//                sendItemL(cChecking, checksumVal);
//                putch(PKG_F);
//                break;
//            }
//            case(cgetQ2PID): {
//                putch(PKG_I);
//                checksumVal += sendItem(cq2kP, sysParameters.q2kP);
//                checksumVal += sendItem(cq2kI, sysParameters.q2kI);
//                checksumVal += sendItem(cq2kD, sysParameters.q2kD);
//                sendItemL(cChecking, checksumVal);
//                putch(PKG_F);
//                break;
//            }
//            case(cgetPass): {
//                putch(PKG_I);
//                checksumVal += sendItemL(cPass, sysParameters.pass);
//                sendItemL(cChecking, checksumVal);
//                putch(PKG_F);
//                break;
//            }
//            case(cgetVol): {
//                putch(PKG_I);
//
//                putch(PKG_F);
//                break;
//            }
//            case(cgetOnOff): {
//                sendOnOff(sysParameters.process, sysParameters.pump1, sysParameters.pump2);
//                break;
//            }
//            case(cgetAllData): {
//                putch(PKG_I);
//                checksumVal += sendItem(ctempA, sysState.temp);
//                checksumVal += sendItem(cq1A, sysState.q1);
//                checksumVal += sendItem(cq2A, sysState.q2);
//                checksumVal += sendItem(ctemp, sysParameters.temp);
//                checksumVal += sendItem(cq1, sysParameters.q1);
//                checksumVal += sendItem(cq2, sysParameters.q2);
//                checksumVal += sendItem(ctempkP, sysParameters.tempkP);
//                checksumVal += sendItem(ctempkI, sysParameters.tempkI);
//                checksumVal += sendItem(ctempkD, sysParameters.tempkD);
//                checksumVal += sendItem(cq1kP, sysParameters.q1kP);
//                checksumVal += sendItem(cq1kI, sysParameters.q1kI);
//                checksumVal += sendItem(cq1kD, sysParameters.q1kD);
//                checksumVal += sendItem(cq2kP, sysParameters.q2kP);
//                checksumVal += sendItem(cq2kI, sysParameters.q2kI);
//                checksumVal += sendItem(cq2kD, sysParameters.q2kD);
//                putch(cOnOff);
//                checksumVal += cOnOff;
//                putch(0);
//                putch(sysParameters.pump1);
//                checksumVal += sysParameters.pump1;
//                putch(sysParameters.pump2);
//                checksumVal += sysParameters.pump2;
//                putch(sysParameters.process);
//                checksumVal += sysParameters.process;
//                sendItemL(cChecking, checksumVal);
//                putch(PKG_F);
//                break;
//            }
//            default: {
//                sendCont--;
//            }
//        }
//        sendDataEN = 0;
//    }
//}

void sendDatax(void) {
    if(final_send.theGet != 0) {
        long checksumVal = 0;
        putch(PKG_I);
        if(final_send.getActualState) {
            checksumVal += sendItem(ctempA, sysState.temp);
            checksumVal += sendItem(cq1A, sysState.q1);
            checksumVal += sendItem(cq2A, sysState.q2);
            final_send.getActualState = 0;
        }
        if(final_send.getDesiredState) {
            checksumVal += sendItem(ctemp, sysParameters.temp);
            checksumVal += sendItem(cq1, sysParameters.q1);
            checksumVal += sendItem(cq2, sysParameters.q2);
            final_send.getDesiredState = 0;
        }
        if(final_send.getTempPID) {
            checksumVal += sendItem(ctempkP, sysParameters.tempkP);
            checksumVal += sendItem(ctempkI, sysParameters.tempkI);
            checksumVal += sendItem(ctempkD, sysParameters.tempkD);
            final_send.getTempPID = 0;
        }
        if(final_send.getQ1PID) {
            checksumVal += sendItem(cq1kP, sysParameters.q1kP);
            checksumVal += sendItem(cq1kI, sysParameters.q1kI);
            checksumVal += sendItem(cq1kD, sysParameters.q1kD);
            final_send.getQ1PID = 0;
        }
        if(final_send.getQ2PID) {
            checksumVal += sendItem(cq2kP, sysParameters.q2kP);
            checksumVal += sendItem(cq2kI, sysParameters.q2kI);
            checksumVal += sendItem(cq2kD, sysParameters.q2kD);
            final_send.getQ2PID = 0;
        }
        if(final_send.getPass) {
            checksumVal += sendItemL(cPass, sysParameters.pass);
            final_send.getPass  = 0;
        }
        if(final_send.getVol) {

            final_send.getVol = 0;
        }
        if(final_send.getOnOff) { //sendOnOff(sysParameters.process, sysParameters.pump1, sysParameters.pump2);
            putch(cOnOff);
            checksumVal += cOnOff;
            putch(0);
            putch(sysParameters.pump1);
            checksumVal += sysParameters.pump1;
            putch(sysParameters.pump2);
            checksumVal += sysParameters.pump2;
            putch(sysParameters.process);
            checksumVal += sysParameters.process;
            final_send.getOnOff = 0;
        }
        sendItemL(cChecking, checksumVal);
        putch(PKG_F);
    }
}

char interprete(unsigned char charDx, DATA_ITEM item) {
    char res = 1;
    tempGet = 0;
    switch(charDx) {
        case(ctemp): {
            newSysParameters.temp = item.valD;
            break;
        }
        case(cq1): {
            newSysParameters.q1 = item.valD;
            break;
        }
        case(cq2): {
            newSysParameters.q2 = item.valD;
            break;
        }
        case(ctempkP): {
            newSysParameters.tempkP = item.valD;
            break;
        }
        case(ctempkI): {
            newSysParameters.tempkI = item.valD;
            break;
        }
        case(ctempkD): {
            newSysParameters.tempkD = item.valD;
            break;
        }
        case(cq1kP): {
            newSysParameters.q1kP = item.valD;
            break;
        }
        case(cq1kI): {
            newSysParameters.q1kI = item.valD;
            break;
        }
        case(cq1kD): {
            newSysParameters.q1kD = item.valD;
            break;
        }
        case(cq2kP): {
            newSysParameters.q2kP = item.valD;
            break;
        }
        case(cq2kI): {
            newSysParameters.q2kI = item.valD;
            break;
        }
        case(cq2kD): {
            newSysParameters.q2kD = item.valD;
            break;
        }
        case(cPass): {
            newSysParameters.pass = item.valDL;
            break;
        }
        case(cOnOff): {
            newSysParameters.pump1 = item.valDc;
            newSysParameters.pump2 = item.valDb;
            newSysParameters.process = item.valDa;
            break;
        }
        case(cChecking): {
            newSysParameters.checkSum = item.valDL;
            checkSumM = (short)charDx + item.valDa + item.valDb + item.valDc + item.valDd;
            break;
        }
        case(cError): {
            if(item.valDa != 0) {
                temp_send.getDesiredState = 1;
                temp_send.getTempPID = 1;
                temp_send.getQ1PID = 1;
                temp_send.getQ2PID = 1;
                temp_send.getOnOff = 1;
//                sendCont--;
            }        
//            sendDataEN = 1;
            break;
        }
        case(cgetAllData): {
            temp_send.getDesiredState = 1;
            temp_send.getTempPID = 1;
            temp_send.getQ1PID = 1;
            temp_send.getQ2PID = 1;
            temp_send.getOnOff = 1;
            break;
        }
//        case(cgetActualState): {
//            temp_send.getActualState = 1;
//            break;
//        }
        case(cgetDesiredState): {
            temp_send.getDesiredState = 1;
            break;
        }
        case(cgetTempPID): {
            temp_send.getTempPID = 1;
            break;
        }
        case(cgetQ1PID): {
            temp_send.getQ1PID = 1;//tempGet = charDx;
            break;
        }
        case(cgetQ2PID): {
            temp_send.getQ2PID = 1;
            break;
        }
        case(cgetPass): {
            temp_send.getPass = 1;
            break;
        }
        case(cgetVol): {
            temp_send.getVol = 1;
            break;
        }
        case(cgetOnOff): {
            temp_send.getOnOff = 1;
            break;
        }
        default: {
            res = 0;
        }
    }
    return res;
}

void rcvProtocol(unsigned char plec) {
    if(dataInit == 0) {
        if(plec == PKG_I) {
            dataInit = 1;
            dataCont = 0;
            itemCont = 0;
            checkSum = 0;
            checkSumM = 0;
        }
    }else {
        if(itemCont == 0) {
            if(plec == PKG_F) {
                dataInit = 0;
                checkSum -= checkSumM;
                if(checkSum == newSysParameters.checkSum) {
                    if(temp_send.theGet != 0) {
//                        sendCont++;
//                        applyGet[sendCont] = betaGet;
//                        sendDataEN = 1;
                        final_send = temp_send;
                        temp_send.theGet = 0;
                    }else {
                        sysParameters = newSysParameters;
                        sendError(0);
                    }
                }else {
                    newSysParameters = sysParameters;
                    sendError(2);
                }
                betaGet = 0;
            }else {
                charD = plec;
//                putch(charD);
            }
        }else if(itemCont == 1) {
            dataItem.valDd = plec; //Mayor significancia
        }else if(itemCont == 2) {
            dataItem.valDc = plec;
        }else if(itemCont == 3) {
            dataItem.valDb = plec;
        }else if(itemCont == 4) {
            dataItem.valDa = plec; //Menor significancia
        }
        if(itemCont > 3) {
            char cd = interprete(charD, dataItem);
            if(cd == 0) {
                dataInit = 0;
                newSysParameters = sysParameters;
                sendError(1);
            }else {
                itemCont = 0;
                if(betaGet == 0)
                    betaGet = tempGet;
            }
        }else {
            itemCont++;
        }
        checkSum += plec;
        dataCont++;
    }
}