#include "SYSconfig.h"
#include "../../Comunic.h"
#include "Protocolo.h"
#include "UART1Config.h"
#include "Timer1Config.h"

int main(void) {
    config_clock();
    initVars();
    config_uart1();
//    config_timer1();
    
    while(1) {
        sendData();
    }
    return 1;
}