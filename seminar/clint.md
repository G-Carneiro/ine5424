# Core Local Interruptor (CLINT)

## Exceção, Interrupção, Armadilha (Trap)

### Terminologias Básicas

1. **Exceções** se referem a uma condição incomum no sistema ocorrida em tempo
  de execução em uma instrução.
    - Exemplo: O endereço de um dado que não foi alinhado corretamente em uma instrução *load*, faz com que a CPU entre com o tratamento de exceção do tipo "endereço
    desalinhado", que irá aparecer no registrador **mcause**. 

2. **Interrupção** se refere a um evento externo que ocorre de forma **assíncrona** na 
  thread corrente.
  Quando uma interrupção precisa ser atendida, uma instrução é selecionada para receber
  a exceção de interrupção.
    - Exemplo: Um *timer* de interrupção é utilizado para acionar um evento futuro,
    sendo assim a CPU escreve em seu registrador **mtimecmp** o valor de **mtime** + ticks que se referem a um número de clocks de relógio no futuro. Como **mtime** 
    se incrementa automaticamente independente de qualquer instrução executada pela CPU,
    em algum ponto **mtimecmp** se iguala a **mtime** e dessa forma a CPU entra com o tratador de interrupção.

  
3. **Armadilha ou Trap**, se refere a uma transferência de controle **síncrona** para o
  tratador de armadilha devido a um condição excepcional causada na thread corrente (e.g execução de um programa que teve uma divisão por zero).
    - Exemplo: Seja uma CPU com três modos de operação: Máquina, Supervisor e Usuário.
      Cada um deles possui seus próprios registradores de controle e status (CSRs) para
      tratamento de armadilha e um área de pilhada dedicada a eles. Quando em modo usuário, uma troca de contexto é requerida para tratar de um evento em modo supervisor, o software configura o sistema para uma troca de contexto e
      chama a instrução **ECALL** que troca o controle para o tratador de exceção de ambiente de usuário.  


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