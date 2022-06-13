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

    cout<<"main b4 delay" << endl;
    cout << Thread::self()->criterion() << endl;
    
    thread = new Thread(&some_io);
    semaphore->p();
//    Delay waiting(500000);
//    semaphore->v();
//            Thread::self()->criterion().award();//.criterion().award();
    cout<<"main after delay" << endl;
    cout << Thread::self()->criterion() << endl;    
    
    semaphore->v();

    cout << "Bye world!" << endl;
    thread->join();

    return 0;
}

int some_io() {
    cout << "Hello world! (new thread)" << endl;
    cout << "thread priority b4 io" << endl;
    cout << Thread::self()->criterion() << endl;
    semaphore->v();
    semaphore->p();
    Delay waiting(200000);
    cout << "thread priority after io" << endl;
    cout << Thread::self()->criterion() << endl;
    return 1;
}
