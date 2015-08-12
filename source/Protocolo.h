#include "ComandosP.h"

typedef union {
    struct {
        float valD;
        char charD;
    };
    struct {
        char valDa;//LessSifnificant
        char valDb;
        char valDc;
        char valDd;//MostSignificant
        char charD;
    };
    struct {  
        long valDL;
        char charD;
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
        char process;
        char pump1;
        char pump2;
        unsigned char myget;
        long pass;
        long checkSum;
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
    sendItemL(cChecking,checksum);
    putch(PKG_F);
}

void sendData(void) {
    if(sendDataEN && sendCont > 0) {
        long checksumVal = 0;
        switch(applyGet[sendCont]) {
            case(cgetSysState): {
                putch(PKG_I);
                checksumVal += sendItem(ctemp, sysState.temp);
                checksumVal += sendItem(cq1, sysState.q1);
                checksumVal += sendItem(cq2, sysState.q2);
                sendItemL(cChecking, checksumVal);
                putch(PKG_F);
                break;
            }
            case(cgetSysParameters): {
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
                putch(PKG_I);
                sendOnOff(sysParameters.process, sysParameters.pump1, sysParameters.pump2);
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
    newSysParameters.myget = 0;
    switch(newSysParameters.myget) {
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
        case(cgetSysState): {
            newSysParameters.myget = item.charD;
            break;
        }
        case(cgetSysParameters): {
            newSysParameters.myget = item.charD;
            break;
        }
        case(cgetTempPID): {
            newSysParameters.myget = item.charD;
            break;
        }
        case(cgetQ1PID): {
            newSysParameters.myget = item.charD;
            break;
        }
        case(cgetQ2PID): {
            newSysParameters.myget = item.charD;
            break;
        }
        case(cgetPass): {
            newSysParameters.myget = item.charD;
            break;
        }
        case(cgetVol): {
            newSysParameters.myget = item.charD;
            break;
        }
        case(cgetOnOff): {
            newSysParameters.myget = item.charD;
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
            dataItem.charD = plec;
            if(dataItem.charD == PKG_F) {
                dataInit = 0;
                checkSum -= checkSumM;
                if(checkSum == newSysParameters.checkSum) {
                    sysParameters = newSysParameters;//updateVars();
                    sendError(0);
                    if(sysParameters.myget != 0) {
                        sendCont++;
                        applyGet[sendCont] = sysParameters.myget;
                        sendDataEN = 1;
                    }
                }else {
                    newSysParameters = sysParameters;
                    sendError(1);
                }
            }
        }else if(itemCont == 1)
            dataItem.valDd = plec; //Mayor significancia
        else if(itemCont == 2)
            dataItem.valDc = plec;
        else if(itemCont == 3)
            dataItem.valDb = plec;
        else if(itemCont == 4)
            dataItem.valDa = plec; //Menor significancia
        if(itemCont > 3) {
            char cd = interprete(dataItem);
            if(!cd) {
                dataInit = 0;
                newSysParameters = sysParameters;
                sendError(1);
            }else
                itemCont = 0;
        }else
            itemCont++;
        checkSum += plec;
        dataCont++;
    }
}