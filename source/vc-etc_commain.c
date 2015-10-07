#include "SYSconfig.h"
#include "../../Comunic.h"
#include "Protocolo.h"
#include "UART1Config.h"
#include "Timer1Config.h"

int main(void) {
    initVars();
    config_clock();
    config_uart1();
    config_timer1();
//    initVars();
    
    while(1) {
        sendDatax();
    }
    return 1;
}