#include "SYSconfig.h"
#include "../../Comunic.h"
#include "Protocolo.h"
#include "InitialSysPar.h"
#include "UART1Config.h"
#include "Timer1Config.h"//send actual state
#include "Timer2Config.h"//"clock"'n save item to eeprom

int main(void) {
    config_clock();
    initVars();
    config_uart1();
    config_timer1();
    config_timer2();
    
    while(1) {
        if(final_send.getGraphData)
            sendGraphData();
        sendDatax();
    }
    return 1;
}