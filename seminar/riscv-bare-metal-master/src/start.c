#include "main.h"
#include "riscv.h"
#include "stdint.h"
#include "timer.h"

extern void trap_entry();

void start() {
    // set M Previous Privilege mode to Supervisor, for mret.
    unsigned long x = r_mstatus();
    x &= ~MSTATUS_MPP_MASK;
    x |= MSTATUS_MPP_S;
    w_mstatus(x);

    // set M Exception Program Counter to main, for mret.
    // requires gcc -mcmodel=medany
    w_mepc((uint64_t)main);

    // setup trap_entry
    w_mtvec((uint64_t)trap_entry);

    timer_init();

    // switch to supervisor mode and jump to main().
    asm volatile("mret");
}