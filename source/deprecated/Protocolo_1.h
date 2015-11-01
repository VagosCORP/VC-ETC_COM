#include "ComandosP.h"

typedef union {
    struct {
        unsigned char charD : 8;
        float valD;
    };
    struct {
        unsigned char       : 8;
        unsigned char valDa : 8;//LessSifnificant
        unsigned char valDb : 8;
        unsigned char valDc : 8;
        unsigned char valDd : 8;//MostSignificant
//        float valD;
//        long valDL;
    };
    struct { 
        unsigned char       : 8;
        long valDL;
    };
}DATA_ITEM;

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
        unsigned long checkSum;
}SYS_PARAMETERS;

typedef struct {
    float temp;
    float q1;
    float q2;
}SYS_STATE;

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
unsigned char applyGet[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char itemCont = 0;
char floatCont = 0;
char dataInit = 0;
char itemInit = 0;
long checkSum = 0;
short checkSumM = 0;

void initVars() {
    sysState.temp = 0.0;//temperatura actual
    sysState.q1 = 0.0;//caudal actual 1
    sysState.q2 = 0.0;//caudal actual 2
    sysParameters.pass = 1234567890;
    sysParameters.temp = 35.1;//temperatura deseada
    sysParameters.q1 = 5.2;//caudal deseado 1
    sysParameters.q2 = 6.1;//caudal deseado 2
    sysParameters.tempkP = 2000.00;//constante proporcional
    sysParameters.tempkI = 8.0;//constante integral
    sysParameters.tempkD = 2.00;//constante derivativa
    sysParameters.q1kP = 120.00;//constante proporcional
    sysParameters.q1kI = 12.0;//constante integral
    sysParameters.q1kD = 0.40;//constante derivativa
    sysParameters.q2kP = 120.00;//constante proporcional
    sysParameters.q2kI = 12.0;//constante integral
    sysParameters.q2kD = 0.46;//constante derivativa
    sysParameters.process = 0;
    sysParameters.pump1 = 0;
    sysParameters.pump2 = 0;
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

void sendData(void) {
    if(sendDataEN && sendCont > 0) {
        long checksumVal = 0;
        switch(applyGet[sendCont]) {
            case(cgetActualState): {
                putch(PKG_I);
                checksumVal += sendItem(ctempA, sysState.temp);
                checksumVal += sendItem(cq1A, sysState.q1);
                checksumVal += sendItem(cq2A, sysState.q2);
                sendItemL(cChecking, checksumVal);
                putch(PKG_F);
                break;
            }
            case(cgetDesiredState): {
                putch(PKG_I);
                checksumVal += sendItem(ctemp, sysParameters.temp);
                checksumVal += sendItem(cq1, sysParameters.q1);
                checksumVal += sendItem(cq2, sysParameters.q2);
                sendItemL(cChecking, checksumVal);
                putch(PKG_F);
                break;
            }
            case(cgetTempPID): {
                putch(PKG_I);
                checksumVal += sendItem(ctempkP, sysParameters.tempkP);
                checksumVal += sendItem(ctempkI, sysParameters.tempkI);
                checksumVal += sendItem(ctempkD, sysParameters.tempkD);
                sendItemL(cChecking, checksumVal);
                putch(PKG_F);
                break;
            }
            case(cgetQ1PID): {
                putch(PKG_I);
                checksumVal += sendItem(cq1kP, sysParameters.q1kP);
                checksumVal += sendItem(cq1kI, sysParameters.q1kI);
                checksumVal += sendItem(cq1kD, sysParameters.q1kD);
                sendItemL(cChecking, checksumVal);
                putch(PKG_F);
                break;
            }
            case(cgetQ2PID): {
                putch(PKG_I);
                checksumVal += sendItem(cq2kP, sysParameters.q2kP);
                checksumVal += sendItem(cq2kI, sysParameters.q2kI);
                checksumVal += sendItem(cq2kD, sysParameters.q2kD);
                sendItemL(cChecking, checksumVal);
                putch(PKG_F);
                break;
            }
            case(cgetPass): {
                putch(PKG_I);
                checksumVal += sendItemL(cPass, sysParameters.pass);
                sendItemL(cChecking, checksumVal);
                putch(PKG_F);
                break;
            }
            case(cgetVol): {
                putch(PKG_I);

                putch(PKG_F);
                break;
            }
            case(cgetOnOff): {
                sendOnOff(sysParameters.process, sysParameters.pump1, sysParameters.pump2);
                break;
            }
            case(cgetAllData): {
                putch(PKG_I);
                checksumVal += sendItem(ctempA, sysState.temp);
                checksumVal += sendItem(cq1A, sysState.q1);
                checksumVal += sendItem(cq2A, sysState.q2);
                checksumVal += sendItem(ctemp, sysParameters.temp);
                checksumVal += sendItem(cq1, sysParameters.q1);
                checksumVal += sendItem(cq2, sysParameters.q2);
                checksumVal += sendItem(ctempkP, sysParameters.tempkP);
                checksumVal += sendItem(ctempkI, sysParameters.tempkI);
                checksumVal += sendItem(ctempkD, sysParameters.tempkD);
                checksumVal += sendItem(cq1kP, sysParameters.q1kP);
                checksumVal += sendItem(cq1kI, sysParameters.q1kI);
                checksumVal += sendItem(cq1kD, sysParameters.q1kD);
                checksumVal += sendItem(cq2kP, sysParameters.q2kP);
                checksumVal += sendItem(cq2kI, sysParameters.q2kI);
                checksumVal += sendItem(cq2kD, sysParameters.q2kD);
                putch(cOnOff);
                checksumVal += cOnOff;
                putch(0);
                putch(sysParameters.pump1);
                checksumVal += sysParameters.pump1;
                putch(sysParameters.pump2);
                checksumVal += sysParameters.pump2;
                putch(sysParameters.process);
                checksumVal += sysParameters.process;
                sendItemL(cChecking, checksumVal);
                putch(PKG_F);
                break;
            }
            default: {
                sendCont--;
            }
        }
        sendDataEN = 0;
    }
}

char interprete(DATA_ITEM item) {
    char res = 1;
    tempGet = 0;
    switch(item.charD) {
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
            checkSumM = (short)item.charD + item.valDa + item.valDb + item.valDc + item.valDd;
            break;
        }
        case(cError): {
            if(!item.valDa)
                sendCont--;
            sendDataEN = 1;
            break;
        }
        case(cgetAllData): {
            tempGet = item.charD;
            break;
        }
        case(cgetActualState): {
            tempGet = item.charD;
            break;
        }
        case(cgetDesiredState): {
            tempGet = item.charD;
            break;
        }
        case(cgetTempPID): {
            tempGet = item.charD;
            break;
        }
        case(cgetQ1PID): {
            tempGet = item.charD;
            break;
        }
        case(cgetQ2PID): {
            tempGet = item.charD;
            break;
        }
        case(cgetPass): {
            tempGet = item.charD;
            break;
        }
        case(cgetVol): {
            tempGet = item.charD;
            break;
        }
        case(cgetOnOff): {
            tempGet = item.charD;
            break;
        }
        default: {
            res = 0;
        }
    }
    return res;
}

void rcvProtocol(char plec) {
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
//                send_int32(checkSum);
//                send_int32(newSysParameters.checkSum);
                if(checkSum == newSysParameters.checkSum) {
                    sysParameters = newSysParameters;//updateVars();
                    sendError(0);
                    if(betaGet != 0) {
                        sendCont++;
                        applyGet[sendCont] = betaGet;
                        sendDataEN = 1;
                    }
                }else {
                    newSysParameters = sysParameters;
//                    putch('B');
                    sendError(1);
                }
                betaGet = 0;
            }else {
                dataItem.charD = plec;
                putch(dataItem.charD);
            }
        }else if(itemCont == 1) {
            dataItem.valDd = plec; //Mayor significancia
            putch(dataItem.valDd);
        }else if(itemCont == 2) {
            dataItem.valDc = plec;
            putch(dataItem.valDc);
        }else if(itemCont == 3) {
            dataItem.valDb = plec;
            putch(dataItem.valDb);
        }else if(itemCont == 4) {
            dataItem.valDa = plec; //Menor significancia
            putch(dataItem.valDa);
        }
        if(itemCont > 3) {
            char cd = interprete(dataItem);
            if(cd == 0) {
                dataInit = 0;
                newSysParameters = sysParameters;
//                putch('A');
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
    
}