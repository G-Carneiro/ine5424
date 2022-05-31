
/*
 * handlers.S
 *
 * This file supports the population of the mtvec vector table,
 * which is populated with opcodes containing a jump instruction
 * to a specific handler used for CLINT vectored mode.
 */

.weak default_handler
.balign 4, 0
.global default_handler
