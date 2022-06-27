// EPOS Thread Initialization

#include <machine/timer.h>
#include <machine/ic.h>
#include <system.h>
#include <process.h>

__BEGIN_SYS

extern "C" { void __epos_app_entry(); }

void Thread::init()
{
    db<Init, Thread>(TRC) << "Thread::init(" << CPU::mhartid() << ")" << endl;
    Thread::lock();
    db<Init, Thread>(TRC) << "between lock(" << CPU::mhartid() << ")" << endl;
    if (CPU::mhartid() == 0) {
        Criterion::init();

        typedef int (Main)();

        // If EPOS is a library, then adjust the application entry point to __epos_app_entry, which will directly call main().
        // In this case, _init will have already been called, before Init_Application to construct MAIN's global objects.
        Main *main = reinterpret_cast<Main *>(__epos_app_entry);
        
        new(SYSTEM) Thread(Thread::Configuration(Thread::RUNNING, Thread::MAIN), main);

        // Idle thread creation does not cause rescheduling (see Thread::constructor_epilogue)
        new(SYSTEM) Thread(Thread::Configuration(Thread::READY, Thread::IDLE), &Thread::idle);
        
    } else {
        new (SYSTEM) Thread(Thread::Configuration(Thread::READY, Thread::IDLE), &Thread::idle);
    }


    // The installation of the scheduler timer handler does not need to be done after the
    // creation of threads, since the constructor won't call reschedule() which won't call
    // dispatch that could call timer->reset()
    // Letting reschedule() happen during thread creation is also harmless, since MAIN is
    // created first and dispatch won't replace it nor by itself neither by IDLE (which
    // has a lower priority)
    if(Criterion::timed && (CPU::mhartid() == 0))
        _timer = new (SYSTEM) Scheduler_Timer(QUANTUM, time_slicer);

    // No more interrupts until we reach init_end
    // CPU::int_disable();
    // Install an interrupt handler to receive forced reschedules
    if (smp) {
        if(CPU::mhartid() == 0)
            IC::int_vector(IC::INT_RESCHEDULER, rescheduler);
        CPU::int_disable();
        IC::enable(IC::INT_RESCHEDULER);
    }

    // Transition from CPU-based locking to thread-based locking
//    This_Thread::not_booting();
    db<Init, Thread>(TRC) << "before unlock(" << CPU::mhartid() << ")" << endl;

    Thread::unlock();
    db<Init, Thread>(TRC) << "after unlock(" << CPU::mhartid() << ")" << endl;

}

__END_SYS
