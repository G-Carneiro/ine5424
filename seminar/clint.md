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

#### Machine Cause (mcause)

- Indica qual evento que causou o trap.
- Caso a causa seja uma interrupção o bit `Interrupt` é setado.
- Se for gerada mais de uma exceção síncrona, a tabela de prioridades é utilizada.
- 

#### Machine Interrupt Pending (mip)

- Indica quais interrupções estão pendentes.

#### Machine Interrupt Enable (mie)

- Indica quais interrupções estão habilitadas.

#### mtvec

- Contém o endereço base da tabela de vetores de interrupção e a configuração do modo de interrupção.
- Todas as exceções síncronas usam para tratamento de exceções.
- Sempre deve ser implementado, mas se poderá ser escrito varia com a implementação. 

#### Machine Exception Delegation (medeleg)

- Delega exceções ao modo supervisor.

#### Machine Interrupt Delegation (mideleg)

- Delega interrupções ao modo supervisor.

### Comuns ao CLINT e CLIC

#### msip

#### mtime

#### mtimecmp

## RV32 vs RV64 (no CLINT)

Colocar oq muda no clint ao mudar de 32 para 64.


- HART: hardware threads
- WARL: 