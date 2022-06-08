- UL? unsigned long? 
  - `riscv_ic.h` linha 34
- hysterically_debugged? 
  - `*_traits.h`
  - jogar todas as informações sobre interrupções
- philosophers_dinner com problema nos prints (somente rv64)
- "cc" dentro do `ASM`.
  - `rv64_cpu.h` linha 332.
  - operação em asm que altera flags -> avisa compilador (?) q as informações q ele tem podem estar erradas.
- como saber que thread esta IO bound
- o que eh a propriedade timed de um scheduler
  - indica se a politica precisa de timer

## Garantir que de fato está funcionando

- Olhar operações atômicas.
- tsc
- pmu
- crts

## Escalonador
 
- fifo, shortest job first

## Possíveis Problemas 

- Expansão de sinal de 32 bits para 64.