#include "RS485.h"

#if defined(HAVE_RS485)

#if defined(HAVE_RS485_HARD)

#include <driver/uart.h>

static void IRAM_ATTR rs485_isr(void* arg) {
	RS485Class* rs485 = (RS485Class*) arg;
	if (rs485->isTxMode() && (rs485->availableForWrite() == UART_FIFO_LEN)) {
		rs485->disableTx();
	}
}

RS485Class::RS485Class(int numSerial): Uart(numSerial), txMode(false) {
}

void RS485Class::begin(unsigned long baud, uint8_t duplex, uint32_t config) {
	pinMode(RS485_DE_RE, OUTPUT);
	disableTx();
	Uart::begin(baud, config);

	uart_isr_free(RS485_HWSERIAL);
	uart_isr_register(RS485_HWSERIAL, rs485_isr, this, ESP_INTR_FLAG_IRAM, nullptr);
}

size_t RS485Class::write(uint8_t value) {
	enableTx();
	return Uart::write(value);
}

void RS485Class::enableTx() {
	txMode = true;
	digitalWrite(RS485_DE_RE, HIGH);
}

void RS485Class::disableTx() {
	digitalWrite(RS485_DE_RE, LOW);
	txMode = false;
}

bool RS485Class::isTxMode() const {
	return txMode;
}

RS485Class RS485(RS485_HWSERIAL);

#endif // HAVE_RS485_HARD

void RS485Event() __attribute__((weak));

void RS485EventRun() {
	if (RS485Event && RS485.available()) {
		RS485Event();
	}
}

#endif
