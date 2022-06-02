#include "mbed.h"
#include "Motor.h"
#include <cstdio>
#include "RC.h"
#include "Wheel.h"
#include "HydraHead.h"
#include "HydraWheel.h"

int main() {

    HydraHead hydra_head(0x33, "Roue_3"); int pos = 3;

    // Define UART and CAN communication for STM32L432KC
    hydra_head.add_hydra_UART(new HydraUART(USBTX, USBRX, 115200));
    hydra_head.add_hydra_CAN(new HydraCAN(D10, D2));
    hydra_head.initialize();

    // Initialize of wheel
    hydra_head.add_hydra_component(new myHydraWheel());

    while(1){
        hydra_head.loop();
    }
}
