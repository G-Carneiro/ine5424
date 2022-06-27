// EPOS Alarm Initialization

#include <time.h>
#include <system.h>

__BEGIN_SYS

void Alarm::init()
{
    db<Init, Alarm>(TRC) << "Alarm::init()" << endl;
    db<Init, Alarm>(TRC) << "0 b4 lock" << endl;
    Thread::lock();
    db<Init, Alarm>(TRC) << "0 between lock" << endl;
    _timer = new (SYSTEM) Alarm_Timer(handler);
    Thread::unlock();
    db<Init, Alarm>(TRC) << "0 after unlock" << endl;


}

__END_SYS
