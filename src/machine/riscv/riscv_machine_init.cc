// EPOS RISC V Initialization

#include <machine.h>

__BEGIN_SYS

// TODO: talvez precisemos do pre_init()
//void Machine::pre_init(System_Info * si)
//{
//    // Wake up other CPUs
//    // Note that moving this code to SETUP would make RISC-V more similar to PC, but IC::_int_vector is in the BSS, so it's hard to prevent CPU 0 from zeroing it before others have finished booting
//    if(CPU::mhartid() == 0)
//        smp_barrier_init(Traits<Build>::CPUS);
//
//    if(CPU::mhartid() == 0)
//        Display::init();
//
//    db<Init, Machine>(TRC) << "Machine::pre_init()" << endl;
//}

void Machine::init()
{
    db<Init, Machine>(TRC) << "Machine::init()" << endl;

    if(Traits<IC>::enabled)
        IC::init();

    if(Traits<Timer>::enabled)
        Timer::init();
}

__END_SYS
