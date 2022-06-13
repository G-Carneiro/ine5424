#include <utility/ostream.h>
#include <time.h>
#include <synchronizer.h>
#include <process.h>

using namespace EPOS;

OStream cout;

Semaphore * semaphore = new Semaphore(0);

int some_io();

int main()
{
    Thread * thread;
    
    thread = new Thread(&some_io);
    cout<<"main b4 delay" << endl;
    cout << Thread::self()->criterion() << endl;

    Delay waiting(500000);
    semaphore->v();
    cout << "thread prio after io" << endl;
    cout << thread->criterion() << endl;
//            Thread::self()->criterion().award();//.criterion().award();
    cout << "Bye world!" << endl;
    thread->join();


    cout<<"main after delay" << endl;
    cout << Thread::self()->criterion() << endl;

    return 0;
}

int some_io() {
    cout << "Hello world!" << endl;
//    while(1) {
    cout << "thread prio b4 io" << endl;
    cout << Thread::self()->criterion() << endl;
    semaphore->p();
//    break;
//    }
    return 1;
}
