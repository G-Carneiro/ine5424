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

#### Machine Software Interrupt Pending (msip)

- Cada CPU possui seu registrador.
- Interrupt ID #3.
- Em sistemas com várias CPUs, uma CPU pode escrever no `msip` de qualquer outra.
  - Isso permite uma comunicação eficiente entre processadores.

#### Machine Timer (mtime)

- Contém o número de ciclos a partir de `RTCCLK` (CPU real time clock).
- Gera interrupção sempre que `mtime >= mtimecmp`, a qual é indicada em `mip.mtip`.
- Existe um único `mtime`, independente da quantidade de CPUs.
- Quando resetado, vai para 0.
- Interrupções de tempo sempre vão para o modo Machine, a não ser quando delegados ao modo Supervisor com o uso do `mideleg`. O mesmo ocorre com as exceções.

#### Machine Timer Compare (mtimecmp)

- Usado em conjunto com `mtime`.
- Não é resetado, diferente de `mtime`.
- Cada CPU possui seu próprio registrador.

## Modos de Operação

Existem dois modos de operação do CLINT, o **modo direto** e o **modo vetorizado**.
Para configurar os modos do CLINT, escreva no campo ***mtvec.mode***, que é o **bit[0]**
do registrador de status e controle ***mtvec***:

- Para o **modo direto**, ***mtvec.mode*** = 0
- Para o **modo vetorizado**, ***mtvec.mode*** = 1

O **modo direto** é o valor **padrão** de reset. O campo ***mtvec.base*** guarda o endereço base para
interrupções e exceções, e deve ter um valor alinhado a um mínimo de **4 bytes no modo direto**, e
um mínimo de **64 bytes no modo vetorizado**.

### Modo Direto

O **modo direto** significa que todas as interrupções têm armadilha para o mesmo tratador, e não há uma tabela de vetores implementada.
É a responsabilidade do **software** executar código para descobrir qual interrupção ocorreu.

O **tratador em software** no **modo direto**, deve primeiro ler ***mcause.interrupt*** para determinar se uma **interrupção** ou **exceção** ocorreu,
para então decidir oque fazer baseado no valor de ***mcause.code** que contém o código de interrupção ou exceção respectivo.

### Modo Vetorizado

O **modo vetorizado** introduz um método para criar uma **tabela de vetores** que o **hardware** usa para reduzir a latência do tratamento de interrupções.
Quando uma interrupção acontece no **modo vetorizado**, o registrador ***pc*** será atribuído pelo hardware ao **endereço do índice** da **tabela de vetores**
correspondente ao **ID da interrupção**. Do **índice da tabela de vetores**, um ***jump*** subsequente irá ocorrer para atender a interrupção.

Lembre-se de que a **tabela de vetores** contém um ***opcode*** que é uma instrução de ***jump*** para um local específico.

## RV32 vs RV64 (no CLINT)

Colocar oq muda no clint ao mudar de 32 para 64.

- CSRs: Control and Status Registers.
- HART: hardware threads.
- WARL: Write Any Values, Reads Legal Values.
- WLRL: Write/Read Only Legal Values.
- WPRI: Reserved Writes Preserve Values, Reads Ignore Values.
