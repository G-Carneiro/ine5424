// EPOS RISC-V 32 PMU Mediator Declarations

#ifndef __rv32_pmu_h
#define __rv32_pmu_h

#include <architecture/cpu.h>
#define __common_only__
#include <architecture/pmu.h>
#undef __common_only__

__BEGIN_SYS

class RV32_A_PMU: public PMU_Common
{
private:
    typedef CPU::Reg32 Reg32;
    typedef CPU::Reg64 Reg64;

protected:
    static const unsigned int CHANNELS = 32;
    static const unsigned int FIXED = 0;
    static const unsigned int EVENTS = 23;  // 25 if consider DATA_CACHE and TLB_MISS

public:
    // Instruction Commit Events, mhpmeventX[7:0] = 0
    enum {
        EXCEPTION_TAKEN                    = 1 << 8,
        INT_LOAD_INSTRUCTION_RETIRED       = 1 << 9,
        INT_STORE_INSTRUCTION_RETIRED      = 1 << 10,
        ATOMIC_MEM_OPERATION_RETIRED       = 1 << 11,
        SYSTEM_INSTRUCTION_RETIRED         = 1 << 12,
        INT_ARITHMETIC_INSTRUCTION_RETIRED = 1 << 13,
        CONDITIONAL_BRANCH_RETIRED         = 1 << 14,
        JAL_INSTRUCTION_RETIRED            = 1 << 15,
        JALR_INSTRUCTION_RETIRED           = 1 << 16,
        INT_MUL_INSTRUCTION_RETIRED        = 1 << 17,
        INT_DIV_INSTRUCTION_RETIRED        = 1 << 18,
    };

    // Microarchitectural Events, mhpmeventX[7:0] = 1
    enum {
        LOAD_USE_INTERLOCK                 = 1 << 8,
        LONG_LATENCY_INTERLOCK             = 1 << 9,
        CSR_READ_INTERLOCK                 = 1 << 10,
        INSTRUCTION_CACHE_ITIM_BUSY        = 1 << 11,
        DATA_CACHE_DTIM_BUSY               = 1 << 12,
        BRANCH_DIRECTION_MISPREDICTION     = 1 << 13,
        BRANCH_JUMP_TARGET_MISPREDICTION   = 1 << 14,
        PIPELINE_FLUSH_FROM_CSR_WRITE      = 1 << 15,
        PIPELINE_FLUSH_FROM_OTHER_EVENT    = 1 << 16,
        INTEGER_MULTIPLICATION_INTERLOCK   = 1 << 17,
    };

    // Memory System Events, mhpmeventX[7:0] = 2
    enum {
        INSTRUCTION_CACHE_MISS             = 1 << 8,
        MEMORY_MAPPED_IO_ACCESS            = 1 << 9,
//        DATA_CACHE_WRITEBACK               = 1 << 10,
//        INSTRUCTION_TLB_MISS               = 1 << 11,
//        DATA_TLB_MISS                      = 1 << 12,
    };

public:
    RV32_A_PMU() {}

    static void config(Channel channel, const Event event, Flags flags = NONE) {}

    static Count read(Channel channel) {
        db<PMU>(TRC) << "PMU::read(c=" << channel << ")" << endl;
        return csrr(channel);
    }

    static void write(Channel channel, Count count) {
        db<PMU>(TRC) << "PMU::write(ch=" << channel << ",ct=" << count << ")" << endl;
        csrw(channel);
    }

    static void start(Channel channel) {}

    static void stop(Channel channel) {}

    static void reset(Channel channel) {}

    static void init();

private:
    static Reg64 csrr(Reg32 reg) {
        Reg32 low;
        Reg32 high;

        if (reg == 0) {         // cycle
            ASM("csrr %0, cycle\n\t" : "=r"(low) : );
            ASM("csrr %0, cycleh\n\t" : "=r"(high) : );
        } else if (reg == 1) {  // time
            ASM("csrr %0, time\n\t" : "=r"(low) : );
            ASM("csrr %0, timeh\n\t" : "=r"(high) : );
        } else if (reg == 2) {  // instret
            ASM("csrr %0, instret\n\t" : "=r"(low) : );
            ASM("csrr %0, instreth\n\t" : "=r"(high) : );
        } else {                // mhpmcounter3
            ASM("csrr %0, mhpmcounter3\n\t" : "=r"(low) : );
            ASM("csrr %0, mhpmcounter3h\n\t" : "=r"(high) : );
        }

        Reg64 r;
        r = Reg64(high);
        r = r << 32;
        return Reg64(r | low);
    }

    static void csrw(Reg64 reg) {
        Reg32 low = Reg32((reg << 32) >> 32);
        Reg32 high = Reg32(reg >> 32);

        if (reg == 0) {         // cycle
            ASM("csrw %0, cycle\n\t" : : "r"(low));
            ASM("csrw %0, cycleh\n\t" : : "r"(high));
        } else if (reg == 1) {  // time
            ASM("csrw %0, time\n\t" : : "r"(low));
            ASM("csrw %0, timeh\n\t" : : "r"(high));
        } else if (reg == 2) {  // instret
            ASM("csrw %0, instret\n\t" : : "r"(low));
            ASM("csrw %0, instreth\n\t" : : "r"(high));
        } else {                // mhpmcounter3
            ASM("csrw %0, mhpmcounter3\n\t" : : "r"(low));
            ASM("csrw %0, mhpmcounter3h\n\t" : : "r"(high));
        }
    }

private:
    static const Event _events[EVENTS];
};


class PMU: private RV32_A_PMU
{
    friend class CPU;

private:
    typedef RV32_A_PMU Engine;

public:
    using Engine::CHANNELS;
    using Engine::FIXED;
    using Engine::EVENTS;

    using Engine::Event;
    using Engine::Count;
    using Engine::Channel;

public:
    PMU() {}

    using Engine::config;
    using Engine::read;
    using Engine::write;
    using Engine::start;
    using Engine::stop;
    using Engine::reset;

private:
    static void init() { Engine::init(); }
};

__END_SYS

#endif
