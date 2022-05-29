# Core Local Interruptor (CLINT)

## Exceção, Interrupção, Armadilha (?)

Objetivo aqui é fazer uma breve revisão de conceitos e mostrar exemplos.

## Registradores

### Registradores da Máquina

#### mstatus

- Acompanha e controla o estado operacional atual do hart.

#### mcause

#### mip e mie

#### mtvec

#### medeleg e mideleg

### Comuns ao CLINT e CLIC

#### msip

#### mtime

#### mtimecmp

## RV32 vs RV64 (no CLINT)

Colocar oq muda no clint ao mudar de 32 para 64.


- HART: hardware threads
- WARL: 