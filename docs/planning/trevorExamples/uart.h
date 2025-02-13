#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stddef.h>

#define BUF_SIZE 7

typedef struct {
    char data[BUF_SIZE];
    size_t index;
    bool is_full;
} rxbuf_t;

void init_uart(void);

bool rxbuf_is_full(void);

int get_rxbuf(char *buf);

void send_message(const char *buf);

#endif // UART_H