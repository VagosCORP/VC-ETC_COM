#ifndef INITIALSYSPAR_H
#define	INITIALSYSPAR_H

#include "ProtocolRes.h"


#ifdef	__cplusplus
extern "C" {
#endif

void initVars() {
    sysState.temp = 0.0f;//temperatura actual
    sysState.q1 = 0.0f;//caudal actual 1
    sysState.q2 = 0.0f;//caudal actual 2
    
    valDeltaT.theTime = 0;//todo en 0
    deltaTdes.theTime = 0;//hr, sec, fecha = 0
    deltaTdes.segundos = 900;//hr, sec, fecha = 0
    
    ActualTime.fecha = 99;
//    ActualTime.horas = 12;
//    ActualTime.minutos = 30;
    ActualTime.segundos = 45025;
    sysParameters.syncTime = 0;
    
    ProcessInit.fecha = 99;
//    ProcessInit.horas = 12;
//    ProcessInit.minutos = 30;
    ProcessInit.segundos = 45025;
    
    ProcessEnd.fecha = 99;
//    ProcessEnd.horas = 12;
//    ProcessEnd.minutos = 30;
    ProcessEnd.segundos = 45025;
    
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

#ifdef	__cplusplus
}
#endif

#endif	/* INITIALSYSPAR */