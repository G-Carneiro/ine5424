// EPOS RV64 PMU Events Declaration

#include <architecture/pmu.h>

#ifdef __sifive_u__

__BEGIN_SYS

const RV64_A_PMU::Event RV64_A_PMU::_events[RV64_A_PMU::EVENTS] = {
    RV64_A_PMU::EXCEPTION_TAKEN,                        // 0
    RV64_A_PMU::INT_LOAD_INSTRUCTION_RETIRED,           // 1
    RV64_A_PMU::INT_STORE_INSTRUCTION_RETIRED,          // 2
    RV64_A_PMU::ATOMIC_MEM_OPERATION_RETIRED,           // 3
    RV64_A_PMU::SYSTEM_INSTRUCTION_RETIRED,             // 4
    RV64_A_PMU::INT_ARITHMETIC_INSTRUCTION_RETIRED,     // 5
    RV64_A_PMU::CONDITIONAL_BRANCH_RETIRED,             // 6
    RV64_A_PMU::JAL_INSTRUCTION_RETIRED,                // 7
    RV64_A_PMU::JALR_INSTRUCTION_RETIRED,               // 8
    RV64_A_PMU::INT_MUL_INSTRUCTION_RETIRED,            // 9
    RV64_A_PMU::INT_DIV_INSTRUCTION_RETIRED,            // 10
    RV64_A_PMU::LOAD_USE_INTERLOCK,                     // 11
    RV64_A_PMU::LONG_LATENCY_INTERLOCK,                 // 12
    RV64_A_PMU::CSR_READ_INTERLOCK,                     // 13
    RV64_A_PMU::INSTRUCTION_CACHE_ITIM_BUSY,            // 14
    RV64_A_PMU::DATA_CACHE_DTIM_BUSY,                   // 15
    RV64_A_PMU::BRANCH_DIRECTION_MISPREDICTION,         // 16
    RV64_A_PMU::BRANCH_JUMP_TARGET_MISPREDICTION,       // 17
    RV64_A_PMU::PIPELINE_FLUSH_FROM_CSR_WRITE,          // 18
    RV64_A_PMU::PIPELINE_FLUSH_FROM_OTHER_EVENT,        // 19
    RV64_A_PMU::INTEGER_MULTIPLICATION_INTERLOCK,       // 20
    RV64_A_PMU::INSTRUCTION_CACHE_MISS,                 // 21
    RV64_A_PMU::MEMORY_MAPPED_IO_ACCESS,                // 22
//    RV64_A_PMU::DATA_CACHE_WRITEBACK,                   // 23
//    RV64_A_PMU::INSTRUCTION_TLB_MISS,                   // 24
//    RV64_A_PMU::DATA_TLB_MISS                           // 25
};

__END_SYS

#endif
