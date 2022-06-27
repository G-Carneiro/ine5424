// EPOS System Initializer

#include <utility/random.h>
#include <machine.h>
#include <memory.h>
#include <system.h>
#include <process.h>

__BEGIN_SYS

class Init_System
{
private:
    static const unsigned int HEAP_SIZE = Traits<System>::HEAP_SIZE;

public:
    Init_System() {
        db<Init>(TRC) << "Init_System()" << endl;

        // garante que todas CPUs estejam no mesmo ponto
        db<Init>(TRC) << "after barrier" << endl;

        db<Init>(INF) << "Init:si=" << *System::info() << endl;

        // somente a hart 0 precisa inicializar o sistema completamente.
        if (CPU::mhartid() == 0) {
            db<Init>(INF) << "Initializing the architecture: " << endl;
            CPU::init();

            db<Init>(TRC) << "Initializing system's heap: " << endl;
            if (Traits<System>::multiheap) {
                System::_heap_segment = new(&System::_preheap[0]) Segment(HEAP_SIZE, Segment::Flags::SYS);
                char *heap;
                if (Memory_Map::SYS_HEAP == Traits<Machine>::NOT_USED)
                    heap = Address_Space(MMU::current()).attach(System::_heap_segment);
                else
                    heap = Address_Space(MMU::current()).attach(System::_heap_segment, Memory_Map::SYS_HEAP);
                if (!heap)
                    db<Init>(ERR) << "Failed to initialize the system's heap!" << endl;
                System::_heap = new(&System::_preheap[sizeof(Segment)]) Heap(heap, System::_heap_segment->size());
            } else
                System::_heap = new(&System::_preheap[0]) Heap(MMU::alloc(MMU::pages(HEAP_SIZE)), HEAP_SIZE);

            db<Init>(TRC) << "Initializing the machine: " << endl;
            Machine::init();
            
        } else {
            CPU::init();
            Timer::init();
        }
        CPU::smp_barrier();
//        CPU::smp_barrier();

        db<Init>(TRC) << "Initializing system abstractions: " << endl;
        System::init();
        db<Init>(TRC) << "HART 2 SAFE" << endl;

        // somente hart 0 precisa fazer isso
        if (CPU::mhartid() == 0) {
            db<Init>(TRC) << "making progress " << endl;

            // Randomize the Random Numbers Generator's seed
            if (Traits<Random>::enabled) {
                db<Init>(TRC) << "Randomizing the Random Numbers Generator's seed." << endl;
                if (Traits<TSC>::enabled)
                    Random::seed(TSC::time_stamp());

                if (!Traits<TSC>::enabled)
                    db<Init>(WRN) << "Due to lack of entropy, Random is a pseudo random numbers generator!" << endl;
            }
        }

        // Initialization continues at init_end
    }
};

// Global object "init_system" must be constructed first.
Init_System init_system;

__END_SYS
