#include "uart_lib.h"

void SystemInit(void) {
    UART_Init(UART_0, 276, UART_CONTROL1_RE_M | UART_CONTROL1_TE_M, 0, 0);
}