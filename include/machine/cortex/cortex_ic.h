// EPOS ARM Cortex IC Mediator Declarations

#ifndef __cortex_ic_h
#define __cortex_ic_h

#include <architecture/cpu.h>
#include <machine/ic.h>
#include __HEADER_MMOD(ic)

__BEGIN_SYS

class IC: private IC_Engine
{
    friend class Machine;

private:
    typedef IC_Engine Engine;
    typedef CPU::Reg32 Reg32;

public:
    using Engine::INTS;
    using Engine::EXCS;
    using Engine::IRQS;

    using Engine::Interrupt_Id;
    using Engine::Interrupt_Handler;

    using Engine::INT_SYS_TIMER;
    using Engine::INT_USR_TIMER;
    using Engine::INT_TIMER0;
    using Engine::INT_TIMER1;
    using Engine::INT_TIMER2;
    using Engine::INT_TIMER3;
    using Engine::INT_GPIOA;
    using Engine::INT_GPIOB;
    using Engine::INT_GPIOC;
    using Engine::INT_GPIOD;
    using Engine::INT_USB0;
    using Engine::INT_NIC0_RX;
    using Engine::INT_NIC0_TX;
    using Engine::INT_NIC0_ERR;
    using Engine::INT_NIC0_TIMER;
    using Engine::INT_RESCHEDULER;

public:
    IC() {}

    static Interrupt_Handler int_vector(Interrupt_Id i) {
        assert(i < INTS);
        return _int_vector[i];
    }

    static void int_vector(Interrupt_Id i, const Interrupt_Handler & h) {
        db<IC>(TRC) << "IC::int_vector(int=" << i << ",h=" << reinterpret_cast<void *>(h) <<")" << endl;
        assert(i < INTS);
        _int_vector[i] = h;
    }

    static void int_vector(Interrupt_Id i, const Interrupt_Handler & ih, const Interrupt_Handler & eh) {
        db<IC>(TRC) << "IC::int_vector(int=" << i << ",ih=" << reinterpret_cast<void *>(ih) << ",ih=" << reinterpret_cast<void *>(eh) << ")" << endl;
        assert(i < INTS);
        _int_vector[i] = ih;
        _eoi_vector[i] = eh;
    }

    static void enable() {
        db<IC>(TRC) << "IC::enable()" << endl;
        Engine::enable();
    }
    static void enable(Interrupt_Id i) {
        db<IC>(TRC) << "IC::enable(int=" << i << ")" << endl;
        assert(i < INTS);
        Engine::enable(i);
    }

    static void disable() {
        db<IC>(TRC) << "IC::disable()" << endl;
        Engine::disable();
    }
    static void disable(Interrupt_Id i) {
        db<IC>(TRC) << "IC::disable(int=" << i << ")" << endl;
        assert(i < INTS);
        Engine::disable(i);
    }

    using Engine::int_id;
    using Engine::irq2int;
    using Engine::int2irq;

    static void ipi(unsigned int cpu, Interrupt_Id i) {
        db<IC>(TRC) << "IC::ipi(cpu=" << cpu << ",int=" << i << ")" << endl;
        assert(i < INTS);
        Engine::ipi(cpu, i);
    }

private:
#ifdef __cortex_m__
    static void dispatch(Interrupt_Id i);
#else
    static void dispatch();
#endif

    // ARMv7 exception handlers are physical, ARMv8 are logical

    // Logical handlers
    static void int_bad(Interrupt_Id i) { int_bad(); };
    static void undefined_instruction(Interrupt_Id i) { undefined_instruction(); };
    static void software_interrupt(Interrupt_Id i) { software_interrupt(); };
    static void prefetch_abort(Interrupt_Id i) { prefetch_abort(); };
    static void data_abort(Interrupt_Id i) { data_abort(); };
    static void int_not(Interrupt_Id i);
    // int_entry is always physical
    static void fiq(Interrupt_Id i) { fiq(); };

    // Physical handlers
    static void int_bad() __attribute__ ((naked));
    static void undefined_instruction() __attribute__ ((naked));
    static void software_interrupt() __attribute__ ((naked));
    static void prefetch_abort() __attribute__ ((naked));
    static void data_abort() __attribute__ ((naked));
    // int_not is always logical
    static void entry() __attribute__ ((naked));
    static void fiq() __attribute__ ((naked));

    static void kill();

    static void init();

private:
    static Interrupt_Handler _int_vector[INTS];
    static Interrupt_Handler _eoi_vector[INTS];
};

__END_SYS

#endif
