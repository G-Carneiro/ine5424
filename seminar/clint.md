# Core Local Interruptor (CLINT)

## Exceção, Interrupção, Armadilha (?)

Objetivo aqui é fazer uma breve revisão de conceitos e mostrar exemplos.

## Registradores

### Registradores da Máquina

#### Machine Status (mstatus)

- Acompanha e controla o estado operacional atual do hart.
- `x = [M | S | U]`
- O bit xIE indica se as interrupções estão habilitadas no modo x.
- Bit xPIE mantém o valor do bit xIE anterior, ou seja, salva o contexto da interrupção anterior.
    ```
    xPIE <- xIE
    xIE  <- 0
    ```
- Bit xPP recebe y, quando um trap é levado do modo de privilégio y para o x.
  - `xPP = y`
- xRET instruction:
  ```
  mstatus.xIE <- mstatus.xPIE
  Priv <- xPP
  ```

#### mcause

#### Machine Interrupt Pending (mip)

- Indica quais interrupções estão pendentes.

#### Machine Interrupt Enable (mie)

- Indica quais interrupções estão habilitadas.

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