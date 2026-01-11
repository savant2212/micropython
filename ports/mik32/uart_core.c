#include <unistd.h>
#include <stdbool.h>
#include "py/mpconfig.h"
#include "uart_lib.h"

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    UART_WaitReceiving(UART_0);
    return UART_ReadByte(UART_0);
}

// Write a character out to the UART.
static inline void uart_write_char(int c) {
    UART_WriteByte(UART_0, c);
    while (UART_IsTransmissionFinished(UART_0) == 0);
}

void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len--) {
        uart_write_char(*str++);
    }
}
