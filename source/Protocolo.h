#include "ProtocolRes.h"

TO_SEND temp_send;
TO_SEND final_send;

DATA_ITEM dataItem;
SYS_PARAMETERS newSysParameters =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SYS_PARAMETERS sysParameters =      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
SYS_STATE sysState = {0,0,0};
THE_TIME ActualTime =   {0};
THE_TIME valDeltaT =    {0};//jejeje
THE_TIME deltaTdes =    {0};
THE_TIME ProcessInit =  {0};
THE_TIME ProcessEnd =   {0};

//char sendDataEN = 0;
//int sendCont = 0;
//int sendInit = 0;
int dataCont = 0;
unsigned char tempGet = 0;
unsigned char betaGet = 0;
char itemCont = 0;
char floatCont = 0;
char dataInit = 0;
char itemInit = 0;
long checkSum = 0;
long checkSumM = 0;
int valN = 0;

short sendItem(char comando, float floatVal) {
    short checksum = 0;
    checksum += comando; 
    putch(comando);
    checksum += send_float(floatVal);
    return checksum;
}

short sendItemL(char comando, long longVal) {
    short checksum = 0;
    checksum += comando; 
    putch(comando);
    checksum += send_int32(longVal);
    return checksum;
}

void sendError(char err) {
    short checksum = 0;
    checksum += cError;
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

void saveGraphItem2EEPROM(long item) {
    valN++;
}

void getEEPROMGraphdata(float * aPointer, int numDat) {
    int i;
    for(i = 0; i < numDat; i++)
        *(aPointer + i) = 19.2f;
}

PROCESS_DETAILS getProcessDetails() {
    PROCESS_DETAILS res;
    res.n = valN;//baaaa
    res.n1 = 2;//baaa
    res.err1 = 0;
    res.err0 = 0;
    res.fProcessInterrupted = 1;//err1:err0 short, pancho da
    return res;
}

void sendGraphData() {
    long checksumVal = 0;
    putch(PKG_I);
    checksumVal += sendItemL(cSummarySendInit, ProcessInit.theTime);
    PROCESS_DETAILS processD = getProcessDetails();
    checksumVal += sendItemL(cSummarySendDetails, processD.processDetails);
    float graphItems[processD.n];
    float * arrayPointer = (float *) & graphItems;
    getEEPROMGraphdata(arrayPointer, processD.n);
    int i;
    for(i = 0; i < processD.n; i++)
        checksumVal += sendItem(cSummarySendItem, graphItems[i]);
    if(sysParameters.process) {
        THE_TIME LastSample;
        LastSample.fecha = ActualTime.fecha - valDeltaT.fecha;
//        LastSample.horas = ActualTime.horas - valDeltaT.horas;
//        LastSample.minutos = ActualTime.minutos - valDeltaT.minutos;
        LastSample.segundos = ActualTime.segundos - valDeltaT.segundos;
        checksumVal += sendItemL(cSummarySendEnd, LastSample.theTime);
    }else
        checksumVal += sendItemL(cSummarySendEnd, ProcessEnd.theTime);
    sendItemL(cChecking, checksumVal);
    final_send.getGraphData = 0;
    putch(PKG_F);
}

void sendDatax(void) {
    if(final_send.theGet != 0) {
        long checksumVal = 0;
//        send_int32_vt(checksumVal);
        putch(PKG_I);
        if(final_send.getActualState) {
            checksumVal += sendItem(ctempA, sysState.temp);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(cq1A, sysState.q1);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(cq2A, sysState.q2);
//            send_int32_vt(checksumVal);
            final_send.getActualState = 0;
        }
        if(final_send.getDesiredState) {
//            checksumVal = 0;
            checksumVal += sendItem(ctemp, sysParameters.temp);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(cq1, sysParameters.q1);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(cq2, sysParameters.q2);
//            send_int32_vt(checksumVal);
            final_send.getDesiredState = 0;
        }
        if(final_send.getTempPID) {
//            checksumVal = 0;
            checksumVal += sendItem(ctempkP, sysParameters.tempkP);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(ctempkI, sysParameters.tempkI);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(ctempkD, sysParameters.tempkD);
//            send_int32_vt(checksumVal);
            final_send.getTempPID = 0;
        }
        if(final_send.getQ1PID) {
//            checksumVal = 0;
            checksumVal += sendItem(cq1kP, sysParameters.q1kP);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(cq1kI, sysParameters.q1kI);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(cq1kD, sysParameters.q1kD);
//            send_int32_vt(checksumVal);
            final_send.getQ1PID = 0;
        }
        if(final_send.getQ2PID) {
//            checksumVal = 0;
            checksumVal += sendItem(cq2kP, sysParameters.q2kP);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(cq2kI, sysParameters.q2kI);
//            send_int32_vt(checksumVal);
//            checksumVal = 0;
            checksumVal += sendItem(cq2kD, sysParameters.q2kD);
//            send_int32_vt(checksumVal);
            final_send.getQ2PID = 0;
        }
        if(final_send.getPass) {
//            checksumVal = 0;
            checksumVal += sendItemL(cPass, sysParameters.pass);
//            send_int32_vt(checksumVal);
            final_send.getPass  = 0;
        }
        if(final_send.getVol) {

            final_send.getVol = 0;
        }
        if(final_send.getOnOff) {
//            checksumVal = 0;
            DATA_ITEM sysOnOff;
            sysOnOff.valDd = 0;
            sysOnOff.valDc = sysParameters.pump1;
            sysOnOff.valDb = sysParameters.pump2;
            sysOnOff.valDa = sysParameters.process;
            checksumVal += sendItemL(cOnOff, sysOnOff.valDL);
//            send_int32_vt(checksumVal);
            /*putch(cOnOff); //sin usar estructura
            checksumVal += cOnOff;
            putch(0);
            putch(sysParameters.pump1);
            checksumVal += sysParameters.pump1;
            putch(sysParameters.pump2);
            checksumVal += sysParameters.pump2;
            putch(sysParameters.process);
            checksumVal += sysParameters.process;*/
            final_send.getOnOff = 0;
        }
//        send_int32_vt(checksumVal);
        sendItemL(cChecking, checksumVal);
        putch(PKG_F);
    }
}

#include "Timer2Config.h"

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
        case(cHourA): {
            newSysParameters.fecha = item.valDd;
            newSysParameters.secC = item.valDc;
            newSysParameters.secB = item.valDb;
            newSysParameters.secA = item.valDa;
            newSysParameters.syncTime = 1;
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
            if(item.valDa == errSendGraph)
                final_send.getGraphData = 1;//>>auto reintentar<< ó preguntar usuario?
            else if(item.valDa != errNone) {
                temp_send.getDesiredState = 1;
                temp_send.getTempPID = 1;
                temp_send.getQ1PID = 1;
                temp_send.getQ2PID = 1;
                temp_send.getOnOff = 1;
            }
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
        case(cgetSummary): {
            temp_send.getGraphData = 1;
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
                        final_send = temp_send;
                        temp_send.theGet = 0;
                    }else {
                        if(!sysParameters.process && newSysParameters.process) {
                            valDeltaT.theTime = 0;
                            valDeltaT.segundos = 3600;
                            ProcessInit.fecha = ActualTime.fecha;
//                            ProcessInit.horas = ActualTime.horas;
//                            ProcessInit.minutos = ActualTime.minutos;
                            ProcessInit.segundos = ActualTime.segundos;
                            //startProcess (if updating 'process' flag is not enought)
                        }else if(sysParameters.process && !newSysParameters.process) {
                            ProcessEnd.fecha = ActualTime.fecha - valDeltaT.fecha;
//                            ProcessEnd.horas = ActualTime.horas - valDeltaT.horas;
//                            ProcessEnd.minutos = ActualTime.minutos - valDeltaT.minutos;
                            ProcessEnd.segundos = ActualTime.segundos - valDeltaT.segundos;
                            //endProcess (if updating 'process' flag is not enought)
                        }
                        sysParameters = newSysParameters;
                        if(sysParameters.syncTime) {
                            sysParameters.syncTime = 0;
                            updateTime();
                        }
                        sendError(errNone);
                    }
                }else {
                    newSysParameters = sysParameters;
                    sendError(errCheckSum);
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
                sendError(errCommunic);
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