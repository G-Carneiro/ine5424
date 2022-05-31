
/* Copyright 2018 SiFive, Inc */
/* SPDX-License-Identifier: Apache-2.0 */

/*
 * This example sets up the CPU to service local interrupts using
 * the CLINT mode of operation. SiFive GPIO's are configured as inputs
 * to support a hardware platform like the Arty 100T with buttons
 * that are connected to the local interrupt lines.
 */

// #include <stdio.h>
// #include <stdlib.h>
// #include <cstdint>

/* These includes get created at build time, and are based on the contents
 * in the bsp folder.  They are useful since they allow us
 * to use auto generated symbols and base addresses which may change
 * based on the design, and every design has it's own unique bsp.
 */

//#include <metal/machine.h>
//#include <metal/machine/platform.h>
//#include <metal/machine/inline.h>

/*
 * This test demonstrates how to enable and handle local interrupts,
 * like the software interrupt using Interrupt ID #3, the
 * timer interrupt using Interrupt ID #7, and buttons on the
 * Arty 100T platform, which are typically in the #16-31 range.
 *
 * This example uses the CLINT vectored mode of operation, which
 * uses a vector table and is lower latency than CLINT direct mode,
 * due to the software overhead.
 *
 * CLINT direct mode does not use a vector table, so all
 * interrupts and exceptions trap to mtvec.base address and software
 * is responsible for dispatching interrupts based on the contents
 * in the mcause CSR.  CLINT direct mode of operation is not supported
 * in this example.  In CLINT direct mode (mtvec.mode = 0), all interrupts
 * and exceptions would trap to address in mtvec.base.
 */

// [K] defining some important memory addresses
// [K] I believe this is set to this address in qemu, but the specification has one less 0
#define METAL_RISCV_CLINT0_0_BASE_ADDRESS 0x2000000
#define METAL_RISCV_CLINT0_MSIP_BASE 0x0
#define METAL_RISCV_CLINT0_MTIMECMP_BASE 0x4000
#define METAL_RISCV_CLINT0_MTIME 0xbff8

// [K] we are not using this platform (Arty 100T) so we need some simple definition for the uart for output
// maybe use this approach https://theintobooks.wordpress.com/2019/12/28/hello-world-on-risc-v-with-qemu/
// just store ascii values in the uart address
#define UART_UPPER_ADDR 0x10010

#define DISABLE                 0
#define ENABLE                  1
#define TRUE                    1
#define FALSE                   0

// [K] I dunno what is the right frequency for this 
#define RTC_FREQ                32768

#define __riscv_xlen 64

// IN 32 BITS this register is 32 bit long, in 64 it is 64 bit long
#define MCAUSE_INT_MASK                     0x8000000000000000UL
#define MCAUSE_CAUSE_MASK                   0x7FFFFFFFFFFFFFFFUL
#define MCAUSE_CODE(cause)                  (cause & MCAUSE_CAUSE_MASK)

/* Compile time options to determine which interrupt modules we have */
#define CLINT_PRESENT TRUE

/* Interrupt Specific defines - used for mtvec.mode field, which is bit[0] for
 * designs with CLINT, or [1:0] for designs with a CLIC */
#define MTVEC_MODE_CLINT_DIRECT                 0x00
#define MTVEC_MODE_CLINT_VECTORED               0x01

/* Offsets for multi-core systems */
#define MSIP_PER_HART_OFFSET                             0x4
#define MTIMECMP_PER_HART_OFFSET                         0x8

#if CLINT_PRESENT
#define CLINT_BASE_ADDR                                 METAL_RISCV_CLINT0_0_BASE_ADDRESS
#define MSIP_BASE_ADDR(hartid)                          (CLINT_BASE_ADDR + METAL_RISCV_CLINT0_MSIP_BASE + (hartid * MSIP_PER_HART_OFFSET))
#define MTIMECMP_BASE_ADDR(hartid)                      (CLINT_BASE_ADDR + METAL_RISCV_CLINT0_MTIMECMP_BASE + (hartid * MTIMECMP_PER_HART_OFFSET))
#define MTIME_BASE_ADDR                                 (CLINT_BASE_ADDR + METAL_RISCV_CLINT0_MTIME)
#endif

#define NUM_TICKS_ONE_S                         RTC_FREQ            // it takes this many ticks of mtime for 1s to elapse
#define NUM_TICKS_ONE_MS                        (RTC_FREQ/1000)     // it takes this many ticks of mtime for 1ms to elapse
#define DEMO_TIMER_INTERVAL                     5000                // 5s timer interval
#define SET_TIMER_INTERVAL_MS(ms_ticks)         write_dword(MTIMECMP_BASE_ADDR(read_csr(mhartid)), (read_dword(MTIME_BASE_ADDR) + (ms_ticks * NUM_TICKS_ONE_MS))) //*

/* Setup prototypes */
void interrupt_global_enable (void);
void interrupt_global_disable (void);
void interrupt_software_enable (void);
void interrupt_software_disable (void);
void interrupt_timer_enable (void);
void interrupt_timer_disable (void);
void interrupt_external_enable (void);
void interrupt_external_disable (void);
void interrupt_local_enable (int id);
void interrupt_local_disable (int id);

/* Defines to access CSR registers within C code */
#define read_csr(reg) ({ unsigned long __tmp; \
  asm volatile ("csrr %0, " #reg : "=r"(__tmp)); \
  __tmp; })

#define write_csr(reg, val) ({ \
  asm volatile ("csrw " #reg ", %0" :: "rK"(val)); })

#define write_dword(addr, data)                 ((*(unsigned long long *)(addr)) = data)
#define read_dword(addr)                        (*(unsigned long long *)(addr))

void __attribute__((weak)) default_handler(void);

// setup handler vectors
void* async_handler[] = {
    (void*) 0,          // User Software Interrupt
    (void*) 0,          // Supervisor Software Interrupt
    (void*) 0,          // Reserved
    (void*) 0,          // Machine Software Interrupt
    (void*) 0,          // User Timer Interrupt
    (void*) 0,          // Supervisor Timer Interrupt
    (void*) 0,          // Reserved
    (void*) 0, //&timer_handler,     // Machine Timer Interrupt
    (void*) 0,          // User External Interrupt
    (void*) 0,          // Supervisor External Interrupt
    (void*) 0,          // Reserved
    (void*) 0,          // Machine External Interrupt
    (void*) 0,          // Reserved
    (void*) 0,          // Reserved
    (void*) 0,          // Reserved
    (void*) 0,          // Reserved
    
    (void*) 0,          // Local Interrupt 1
    (void*) 0           // Local Interrupt 2
};
/*
struct uart {
    void put(// char* in) {
        int pos = 0;
        char tmp = *in;
        while(tmp != '\0'){
            asm volatile ("lui t0, %0 \n"
                          "andi t1, t1, 0 \n" 
                          "addi t1, t1, %1 \n"
                          "sw t1, 0(t0) \n"
                          :
                          : "r"(UART_UPPER_ADDR), "r"(tmp));
            tmp = in[pos];
        }
    }
} uart;
*/
void default_exception_handler(void) {

    /* Read mcause to understand the exception type */
    unsigned long long mcause = read_csr(mcause);
    unsigned long long mepc = read_csr(mepc);
    unsigned long long mtval = read_csr(mtval);
    unsigned long long code = MCAUSE_CODE(mcause);

    // char* s = "Exception Hit!\n";
    // uart.put(s);
    // char* s2 = "Now Exiting...\n";
    // uart.put(s2);

    /* Exit here using non-zero return code */
}

// define direct mode handler
void __attribute__((weak)) default_handler(void) {
    unsigned long mcause_value = read_csr(mcause);

    if (mcause_value & MCAUSE_INT_MASK) {
        // branch to interrupt handler
        async_handler[MCAUSE_CODE(mcause_value)];
    } else {
        // branch to exception handler
        default_exception_handler();
    }
}

// timer handler
void timer_handler() {
    unsigned long long code = MCAUSE_CODE(read_csr(mcause));
    unsigned long long mtime, mip;
    unsigned long long int_bit = read_csr(mip);

    // char* s = "Timer Handler!\n"; 
    // uart.put(s);

    /* set our next interval */
    SET_TIMER_INTERVAL_MS(DEMO_TIMER_INTERVAL);
}

// test variable
unsigned int timer_isr_counter = 0;

/* Main - Setup CLINT interrupt handling and describe how to trigger interrupt */
int main() {

    // define direct mode
    unsigned int i, mode = MTVEC_MODE_CLINT_DIRECT;
    unsigned long long mtvec_base;
    // struct metal_gpio *ggpio;

    /* Write mstatus.mie = 0 to disable all machine interrupts prior to setup */
    interrupt_global_disable();

    /* Setup mtvec to point to our exception handler table using mtvec.base,
     * and assign mtvec.mode = 1 for CLINT vectored mode of operation. The
     * mtvec.mode field is bit[0] for designs with CLINT, or [1:0] using CLIC */
    mtvec_base = (unsigned long long) &default_handler;
    write_csr (mtvec, (mtvec_base | mode));

    /* enable software interrupts */
    interrupt_software_enable();

    /* enable timer interrupts */
    SET_TIMER_INTERVAL_MS(DEMO_TIMER_INTERVAL);
    interrupt_timer_enable();

    /* Write mstatus.mie = 1 to enable all machine interrupts */
    interrupt_global_enable();

    /* Allow timer interrupt to fire before we continue, running at ~5s intervals */
    // char* s = "Waiting for 5s Timer interrupt to fire...\n";
    // uart.put(s);
    while (!timer_isr_counter);
    interrupt_timer_disable();

    /* write msip and display message that s/w handler was hit */
    //fflush(stdout);

    // char* s2 = "\nSetting software interrupt...\n"; 
    // uart.put(s2);
    write_dword(MSIP_BASE_ADDR(read_csr(mhartid)), 0x1);

    // char* s3 = "Thanks!  Now exiting...\n";
    // uart.put(s3);

    return 1;
}
/* External Interrupt ID #11 - handles all global interrupts */
void __attribute__((weak, interrupt)) external_handler (void) {

    /* The external interrupt is usually used for a PLIC, which handles global
     * interrupt dispatching.  If no PLIC is connected, then custom IP can connect
     * to this interrupt line, and this is where interrupt handling
     * support would reside.  This demo does not use the PLIC.
     */
}

void __attribute__((weak, interrupt)) software_handler (void) {

    unsigned long long mip, code = MCAUSE_CODE(read_csr(mcause));
    unsigned long long int_bit = read_csr(mip);

    // uart.put("Software Handler!\n");

    /* Clear Software Pending Bit which clears mip.msip bit */
    write_dword(MSIP_BASE_ADDR(read_csr(mhartid)), 0x0);
}

// unused
void __attribute__((weak, interrupt)) button_handler (void) {

    unsigned long long mip, code = MCAUSE_CODE(read_csr(mcause));
    unsigned long long int_bit = ((1 << code) & read_csr(mip));

    // char* s = "Button Handler!\n"; 
    // uart.put(s);

    /* wait for user to release button */
    while ((read_csr(mip) & int_bit));

    /* increment counter */
    // button_isr_counter++;
}

#define METAL_LOCAL_INTERRUPT_TMR 0x10
#define METAL_MIE_INTERRUPT 0x8
#define METAL_LOCAL_INTERRUPT_SW 0x8
#define METAL_LOCAL_INTERRUPT_EXT 0x800

void interrupt_global_enable (void) {
    unsigned long long m;
    __asm__ volatile ("csrrs %0, mstatus, %1" : "=r"(m) : "r"(METAL_MIE_INTERRUPT));
}

void interrupt_global_disable (void) {
    unsigned long long m;
    __asm__ volatile ("csrrc %0, mstatus, %1" : "=r"(m) : "r"(METAL_MIE_INTERRUPT));
}

void interrupt_software_enable (void) {
    unsigned long long m;
    __asm__ volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_SW));
}

void interrupt_software_disable (void) {
    unsigned long long m;
    __asm__ volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_SW));
}

void interrupt_timer_enable (void) {
    unsigned long long m;
    __asm__ volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_TMR));
}

void interrupt_timer_disable (void) {
    unsigned long long m;
    __asm__ volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_TMR));
}

void interrupt_external_enable (void) {
    unsigned long long m;
    __asm__ volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_EXT));
}

void interrupt_external_disable (void) {
    unsigned long m;
    __asm__ volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(METAL_LOCAL_INTERRUPT_EXT));
}

void interrupt_local_enable (int id) {
    unsigned long long b = 1 << id;
    unsigned long long m;
    __asm__ volatile ("csrrs %0, mie, %1" : "=r"(m) : "r"(b));
}

void interrupt_local_disable (int id) {
    unsigned long long b = 1 << id;
    unsigned long long m;
    __asm__ volatile ("csrrc %0, mie, %1" : "=r"(m) : "r"(b));
}