#ifndef H_FIO_APP_IO
#define H_FIO_APP_IO

#include <stdint.h>
#include <stdbool.h>
#include <os_io_seproxyhal.h>
#include <ux.h>

enum  {
	P1_UNUSED = 0,
	P2_UNUSED = 0
};

// helper function for sending response APDUs.
void io_send_buf(uint16_t code, uint8_t* buffer, size_t bufferSize);

// Asserts that the response fits into response buffer
void CHECK_RESPONSE_SIZE(unsigned int tx);

// This was added for sanity checking -- our program should always be awaiting on something
// and it should be exactly the expected handler
typedef enum {
	// We are doing IO, display handlers should not fire
	IO_EXPECT_IO = 42, // Note: random constants
	// We are displaying things, IO handlers should not fire
	IO_EXPECT_UI = 47,
	// We should not be handling events
	IO_EXPECT_NONE = 49,
} io_state_t;

extern io_state_t io_state;

// Everything below this point is Ledger magic

void io_seproxyhal_display(const bagl_element_t *element);
unsigned char io_event(unsigned char channel);

bool device_is_unlocked();

#if defined(TARGET_NANOS)
typedef void timeout_callback_fn_t(bool ux_allowed);
void nanos_set_timer(int ms, timeout_callback_fn_t* cb);
void nanos_clear_timer();
#elif defined(TARGET_NANOX) || defined(TARGET_NANOS2)
// we had to disable set_timer for Nano X, since in the new SDK UX_STEP_CB/UX_STEP_NOCB macros
// automatically push a confirm callback to G_ux.stack[].ticker_callback with timeout zero
// which causes other callbacks (i.e. ours) to be ignored in UX_TICKER_EVENT, so set_timer
// does not actually work anymore in Nano X
#endif

#endif // H_FIO_APP_IO
