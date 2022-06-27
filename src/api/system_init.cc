// EPOS System Initialization

#include <system.h>
#include <time.h>
#include <process.h>

__BEGIN_SYS

void System::init()
{
    // somente hart 0 inicializa Alarm
    if (CPU::mhartid() == 0) {
        if (Traits<Alarm>::enabled)
            Alarm::init();
    }
    // todas CPUs precisam inicializar Thread
    if(Traits<Thread>::enabled)
        Thread::init();
}

__END_SYS
