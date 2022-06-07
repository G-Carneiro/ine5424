// EPOS RISC-V 64 CPU Mediator Implementation

#include <architecture/rv64/rv64_cpu.h>
#include <system.h>

__BEGIN_SYS

unsigned int CPU::_cpu_clock;
unsigned int CPU::_bus_clock;

void CPU::Context::save() volatile
{
    ASM("       sd       x1,    0(a0)           \n"     // push RA as PC
        "       csrr     x3,  mstatus           \n"
        "       sd       x3,    4(a0)           \n"     // push ST
        "       sd       x1,    8(a0)           \n"     // push RA
        "       sd       x5,   12(a0)           \n"     // push x5-x31
        "       sd       x6,   16(a0)           \n"
        "       sd       x7,   20(a0)           \n"
        "       sd       x8,   24(a0)           \n"
        "       sd       x9,   28(a0)           \n"
        "       sd      x10,   32(a0)           \n"
        "       sd      x11,   36(a0)           \n"
        "       sd      x12,   40(a0)           \n"
        "       sd      x13,   44(a0)           \n"
        "       sd      x14,   48(a0)           \n"
        "       sd      x15,   52(a0)           \n"
        "       sd      x16,   56(a0)           \n"
        "       sd      x17,   60(a0)           \n"
        "       sd      x18,   64(a0)           \n"
        "       sd      x19,   68(a0)           \n"
        "       sd      x20,   72(a0)           \n"
        "       sd      x21,   76(a0)           \n"
        "       sd      x22,   80(a0)           \n"
        "       sd      x23,   84(a0)           \n"
        "       sd      x24,   88(a0)           \n"
        "       sd      x25,   92(a0)           \n"
        "       sd      x26,   96(a0)           \n"
        "       sd      x27,  100(a0)           \n"
        "       sd      x28,  104(a0)           \n"
        "       sd      x29,  108(a0)           \n"
        "       sd      x30,  112(a0)           \n"
        "       sd      x31,  116(a0)           \n"
        "       ret                             \n");
}

// Context load does not verify if interrupts were previously enabled by the Context's constructor
// We are setting mstatus to MPP | MPIE, therefore, interrupts will be enabled only after mret
void CPU::Context::load() const volatile
{
    sp(Log_Addr(this));
    pop();
    iret();
}

void CPU::switch_context(Context ** o, Context * n)     // "o" is in a0 and "n" is in a1
{   
    // Push the context into the stack and update "o"
    Context::push();
    ASM("sd sp, 0(a0)");   // update Context * volatile * o, which is in a0

    // Set the stack pointer to "n" and pop the context from the stack
    ASM("mv sp, a1");   // "n" is in a1
    Context::pop();
    iret();
}

__END_SYS

