#include "py/obj.h"
#include <mik32_memory_map.h>
#include <mik32_pinmap.h>
#include <mik32_ll_gpio.h>

#define MP_HAL_PIN_ALT(function, unit)          (MP_HAL_PIN_ALT_MAKE((MP_HAL_PIN_ALT_##function), (unit)))
#define MP_HAL_PIN_ALT_MAKE(function, unit)     ((function) | ((unit) << 8))

#define mp_hal_pin_obj_t const machine_pin_obj_t *

enum {
    MP_HAL_PIN_ALT_NONE = 0,
    MP_HAL_PIN_ALT_GPIO,
    MP_HAL_PIN_ALT_ADC,
    MP_HAL_PIN_ALT_DAC,
    MP_HAL_PIN_ALT_TIM32A_0,
    MP_HAL_PIN_ALT_TIM32A_1,
    MP_HAL_PIN_ALT_TIM32A_2,
    MP_HAL_PIN_ALT_TIM32A_3,
    MP_HAL_PIN_ALT_TIM32A_4,
    MP_HAL_PIN_ALT_TIM32A_5,
    MP_HAL_PIN_ALT_TIM32A_IO1,
    MP_HAL_PIN_ALT_TIM32A_IO2,
    MP_HAL_PIN_ALT_TIM32A_IO3,
    MP_HAL_PIN_ALT_TIM32A_IO4,
    MP_HAL_PIN_ALT_TIM32A_CLK,
    MP_HAL_PIN_ALT_TIM16A_0,
    MP_HAL_PIN_ALT_TIM16A_OUT,
    MP_HAL_PIN_ALT_TIM16A_IN_1,
    MP_HAL_PIN_ALT_TIM16A_IN_2,
    MP_HAL_PIN_ALT_TIM16A_1,
    MP_HAL_PIN_ALT_TIM16A_2,
    MP_HAL_PIN_ALT_TIM16A_3,
    MP_HAL_PIN_ALT_TIM16A_4,
    MP_HAL_PIN_ALT_SPI_MISO,
    MP_HAL_PIN_ALT_SPI_CLK,
    MP_HAL_PIN_ALT_SPI_MOSI,
    MP_HAL_PIN_ALT_SPI_CSOUT_0,
    MP_HAL_PIN_ALT_SPI_CSOUT_1,
    MP_HAL_PIN_ALT_SPI_CSOUT_2,
    MP_HAL_PIN_ALT_SPI_CSOUT_3,
    MP_HAL_PIN_ALT_SPI_CSIN,
    MP_HAL_PIN_ALT_I2C_SDA,
    MP_HAL_PIN_ALT_I2C_SCL,
    MP_HAL_PIN_ALT_ADC_0,
    MP_HAL_PIN_ALT_ADC_1,
    MP_HAL_PIN_ALT_ADC_2,
    MP_HAL_PIN_ALT_ADC_3,
    MP_HAL_PIN_ALT_ADC_4,
    MP_HAL_PIN_ALT_ADC_5,
    MP_HAL_PIN_ALT_ADC_6,
    MP_HAL_PIN_ALT_ADC_7,
    MP_HAL_PIN_ALT_DAC_0,
    MP_HAL_PIN_ALT_DAC_1,
    MP_HAL_PIN_ALT_DAC_2,
    MP_HAL_PIN_ALT_DAC_3,
    MP_HAL_PIN_ALT_DAC_4,
    MP_HAL_PIN_ALT_SPIFI_CLK,
    MP_HAL_PIN_ALT_SPIFI_CS,
    MP_HAL_PIN_ALT_SPIFI_DATA_0,
    MP_HAL_PIN_ALT_SPIFI_DATA_1,
    MP_HAL_PIN_ALT_SPIFI_DATA_2,
    MP_HAL_PIN_ALT_SPIFI_DATA_3,
    MP_HAL_PIN_ALT_UART_RX,
    MP_HAL_PIN_ALT_UART_TX,
    MP_HAL_PIN_ALT_UART_RTS,
    MP_HAL_PIN_ALT_UART_CTS,
    MP_HAL_PIN_ALT_UART_DCD,
    MP_HAL_PIN_ALT_UART_DTR,
    MP_HAL_PIN_ALT_UART_DSR,
    MP_HAL_PIN_ALT_UART_RI,
    MP_HAL_PIN_ALT_UART_CK,
    MP_HAL_PIN_ALT_JTAG_TDI,
    MP_HAL_PIN_ALT_JTAG_TDO,
    MP_HAL_PIN_ALT_JTAG_TMS,
    MP_HAL_PIN_ALT_JTAG_TCK,
    MP_HAL_PIN_ALT_JTAG_NTRST,
};

typedef struct _machine_pin_obj_t {
    mp_obj_base_t base;
    uint32_t gpio;
    uint8_t port;
    uint8_t pin;
    qstr name;
    const uint16_t alt[4]; // holds result of MP_HAL_PIN_ALT_MAKE(function, unit)
} machine_pin_obj_t;

// Include all the pin definitions.
#include "genhdr/pins_board.h"

static inline void mp_hal_set_interrupt_char(char c) {}
mp_hal_pin_obj_t mp_hal_get_pin_obj(mp_obj_t pin_in);

static inline qstr mp_hal_pin_name(mp_hal_pin_obj_t pin) {
    return pin->name;
}

static inline void mp_hal_pin_input(mp_hal_pin_obj_t pin) {
    ll_gpio_set_input(pin->gpio);
}

static inline void mp_hal_pin_output(mp_hal_pin_obj_t pin) {
    ll_gpio_set_output(pin->gpio);
}

static inline void mp_hal_pin_open_drain(mp_hal_pin_obj_t pin) {
}

static inline void mp_hal_pin_low(mp_hal_pin_obj_t pin) {
    ll_gpio_out_write(pin->gpio, 0);
}

static inline void mp_hal_pin_high(mp_hal_pin_obj_t pin) {
    ll_gpio_out_write(pin->gpio, 1);
}

static inline int mp_hal_pin_read(mp_hal_pin_obj_t pin) {
    return ll_gpio_read(pin->gpio);
}

static inline void mp_hal_pin_write(mp_hal_pin_obj_t pin, int v) {
    ll_gpio_out_write(pin->gpio, v ? true : false);
}

static inline void mp_hal_pin_od_low(mp_hal_pin_obj_t pin) {
    ll_gpio_set_pull_down(pin->gpio);
}

static inline void mp_hal_pin_od_high(mp_hal_pin_obj_t pin) {
    ll_gpio_set_pull_up(pin->gpio);
}
