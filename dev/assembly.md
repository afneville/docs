---
title: Assembly Programming
author: Alexander Neville
date: 2023-10-19
---

# Sections

Assembly code can be broken into three `sections`:

:::{.wrap-table}

| Section | Purpose                                                   |
| :------ | :-------------------------------------------------------- |
| data    | hold constants and program parameters, before compilation |
| bss     | reserve memory to use for new data                        |
| text    | contains progam code                                      |

:::

The syntax for a section looks like this:

```asm

section .data
section .bss
section .text

```

A `text` section must contain a reference to
`_start`, signifying the start of the program, needed for the
_linker_ to create executable machine code. This is a _label_,
identifying a piece of code.

```asm

section .text

    global _start

_start:

```

# Registers

In x86_64 architecture, each register is 64 bits in size. Smaller
registers can be emulated by using part of a 64 bit register. These
registers are referred to with short names, shown in this table.

:::{.wrap-table}

| 8 bit | 16 bit | 32 bit | 64 bit |
| ----: | -----: | -----: | -----: |
|    al |     ax |    eax |    rax |
|    bl |     bx |    ebx |    rbx |
|    cl |     cx |    ecx |    rcx |
|    dl |     dx |    edx |    rdx |
|   sil |     si |    esi |    rsi |
|   dil |     di |    edi |    rdi |
|   bpl |     bp |    ebp |    rbp |
|   spl |     sp |    esp |    rsp |
|   r8b |    r8w |    r8d |     r8 |
|   r9b |    r9w |    r9d |     r9 |
|  r10b |   r10w |   r10d |    r10 |
|  r11b |   r11w |   r11d |    r11 |
|  r12b |   r12w |   r12d |    r12 |
|  r13b |   r13w |   r13d |    r13 |
|  r14b |   r14w |   r14d |    r14 |
|  r15b |   r15w |   r15d |    r15 |

:::

# Pointers

Pointers are another type of on-chip register. They are used to store
memory addresses of instructions. These are the basic pointers:

::: {.wrap-table}

| 16 bit | 32 bit | 64 bit | Purpose                                          |
| ------ | ------ | ------ | ------------------------------------------------ |
| IP     | EIP    | RIP    | holds address of next instruction to fetch       |
| SP     | ESP    | RSP    | holds address of the top of the address stack    |
| BP     | EBP    | RBP    | holds address of the bottom of the address stack |

:::

Many of these registers are used to manage control flow, which is
typically linear from top to bottom. During normal operation, the _RIP_
will be incremented by 1 for every instruction that is run, making the
program move on.

# Flags

Flags - like registers - are a type of on-chip data storage. Unlike
other registers, a flag holds a single bit. Therefore, there are two
possible values for each bit: _true_ or _false_. Each flag is part of a
larger register, the _status register_. Like other registers, flags are
referred to with unique identifiers within assembly programs.

:::{.wrap-table}

| Symbol | Purpose          |
| ------ | ---------------- |
| CF     | carry bit        |
| PF     | parity bit       |
| ZF     | zero             |
| SF     | sign bit         |
| OF     | overflow         |
| AF     | adjust           |
| IF     | interupt enabled |

:::

# Mathematical Operations

Here is a table of available operations. _rXX_ means any 64 bit
register. _b_ could be a register or a hard coded value.

:::{.wrap-table}

| Operation | Parameters | Description      |
| --------- | ---------- | ---------------- |
| add       | rXX, b     | rXX = rXX + b    |
| sub       | rXX, b     | rXX = rXX - b    |
| mul       | rXX        | rax = rax \* rXX |
| div       | rXX        | rax = rax / rXX  |
| neg       | rXX        | rXX = - rXX      |
| inc       | rXX        | rXX = rXX + 1    |
| dec       | rXX        | rXX = rXX -1     |

:::

# Labels & Jumps

Labels are used to store the address of an instruction in memory. Labels
are used in conjunction with jump commands to manipulate the control
flow of a program. **~start~** is a label. When the jump command is
encountered, the address of the instruction associated with the label is
loaded into the **RIP** and hence the control flow is changed. The
syntax of a jump is shown below:

```asm

_start:

    jmp _start

```

This code performs an infinite loop.

# Comparisons & Conditional Jumps

Used with jump commands, comparisons can introduce complex flow control
into assembly programs. Comparisons are always made with at least one
register. (so long as the other argument is a value) eg.

```asm

cmp r15, 37
cmp r15, r14

```

After a comparison is made, flags are set in the status register.
_Conditional jumps_ are made depending on the state of these flags, so a
jump command may directly follow a comparison operation. Here are some
common conditional jump commands:

:::{.wrap-table}

| Symbol | Purpose                |
| ------ | ---------------------- |
| je     | jump if a = b          |
| jne    | jump if a != b         |
| jg     | jump if a \> b         |
| jge    | jump if a \>= b        |
| jl     | jump if a \< b         |
| jle    | jump if a \<= b        |
| jz     | jump if a = 0          |
| jnz    | jump if a != 0         |
| jo     | overflow occurred      |
| jno    | overflow did not occur |

:::

# System Calls

A `syscall` is a request that is made, asking the OS kernel to service
the program. These calls are OS-dependent and each type of call has an
associated ID. In an assembly code program, a `syscall` will also have
parameters. Arguments are passed by filling certain registers in order.

The registers involved in a `syscall` are in this table, where the _ID_
is found in `rax` and all subsequent arguments are found in the
following registers:

:::{.wrap-table}

| Argument | Register |
| -------- | -------- |
| ID       | rax      |
| 1        | rdi      |
| 2        | rsi      |
| 3        | rdx      |
| 4        | r10      |
| 5        | r8       |
| 6        | r9       |

:::

Note that these are the registers for 64 bit architecture. In a 32 bit
machine, registers beginning with an \'e\' will be used. For a full list
of available syscalls, look here:
<https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/>

# Compiling

We will use the nasm compiler we installed earlier

- First step is producing the object code; **nasm** is used to do this:

```shell

nasm -f elf64 -o hello_world.o hello_world.asm

```

- Next, use `ld`, the **GNU Linker** to make this executable:

```shell

ld hello_world.o -o hello_world

```

- Execute newly made file as usual:

```shell

./hello_world

```

# Examples

## Hello World

```{.asm .gnuassembler .s}
section .data

    greeting db "hello world",10
    length_of_greeting equ $ - greeting ; find the length of the string.

section .text

    global _start

_start:

    mov rax, 1                  ; The syscall ID is stored in the rax register
    mov rdi, 1                  ; In rdi, the second register involved, store the first arguement of the syscall
    mov rsi, greeting
    mov rdx, length_of_greeting
    syscall

    mov rax, 60
    mov rdi, 0
    syscall                     ; exit the program
```

## if elif else

```{.asm .gnuassembler .s}
section .data

    equal db "equal",10
    length_equal equ $ - equal ; find the length of the string.
    less db "less",10
    length_less equ $ - less ; find the length of the string.
    more db "more",10
    length_more equ $ - more ; find the length of the string.

section .text

    global _start

_start:

    mov r15, 1
    mov r14, 1
    cmp r15, r14
    jl _less
    jg _more

    mov rax, 1
    mov rdi, 1
    mov rsi, equal
    mov rdx, length_equal
    syscall
    call _exit

_less:

    mov rax, 1
    mov rdi, 1
    mov rsi, less
    mov rdx, length_less
    syscall
    call _exit

_more:

    mov rax, 1
    mov rdi, 1
    mov rsi, more
    mov rdx, length_more
    syscall
    call _exit

_exit:

    mov rax, 60
    mov rdi, 0
    syscall
```

```{.asm .gnuassembler .s}
;; 'Constants' are declared within the .data section
section.data
    ;; db is 'define butes'. it is used for assigning data to memory, with identifier: greeting
    greeting db "hello world",10
    ;; find the length of the stored string
    length_of_greeting equ $ - greeting

;; The code is wrapped within the .text section
section.text
    ;; This declaration is required to determine where the program begins
    global _start

    ;; This is the entry point for the program and is pointed to by the 'text' section
_start:
    ;; Set-up a syscall that can write our greeting to the standard out channel
    mov rax, 1                  ; The syscall ID is stored in the rax register
    mov rdi, 1                  ; In rdi, the second register involved, store the first arguement of the syscall
    mov rsi, greeting
    mov rdx, length_of_greeting
    syscall
    jmp _start                  ; Jump back to the start of the program.

    mov rax, 60
    mov rdi, 0
    syscall
```

## Loop

```{.asm .gnuassembler .s}
section .bss

    string resb 100             ; hold the string itself
    position resb 8             ; hold the current position along the string

section .text

    global _start

_start:

    mov rax, 237                ; put the number to print in rax, used for division
    call _print                 ; call the print subroutine
    mov rax, 43                 ; put the number to print in rax
    call _print                 ; call the print subroutine
    call _exit                  ; call the exit subroutine

_print:                         ; define a subroutine which prints the value in rax.

    mov rcx, string
    mov rbx, 10                 ; newline character
    mov [rcx], rbx              ; put the newline at the beginning of the string
    inc rcx                     ; increment the position along the string
    mov [position], rcx         ; distance along string

_reverse_number:

    mov rdx, 0                  ; zero the rdx register before div/mod operation
    mov rbx, 10                 ; converting from base 10, so divide by 10 each iteration.
    div rbx                     ; divide value in rax by rbx (10) remainder is stored in rdx
    add rdx, 48                 ; to get the ascii value of the character add 48

    mov rcx, [position]
    mov [rcx], dl               ; move least significant bytes of of rdx to address held by rcx
    inc rcx                     ; increment the position along the string.
    mov [position], rcx         ; store the position back in memory

    cmp rax, 0                  ; if there are whole parts left after division, call the function again.
    jne _reverse_number

_display:

    mov rcx, [position]

    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 1
    syscall

    mov rcx, [position]
    dec rcx                     ; starting with the end of the address, iterate backward.
    mov [position], rcx

    cmp rcx, string
    jge _display                ; if the position is not yet back at the start, print the next character.

    ret                         ; end of subroutine, value of rax has been printed.

_exit:

    mov rax, 60
    mov rdi, 0
    syscall                     ; exit the program with sys_exit
```

## Print integer

```{.asm .gnuassembler .s}
section .bss

    string resb 100             ; hold the string itself
    position resb 8             ; hold the current position along the string

section .text

    global _start

_start:

    mov rax, 237                ; put the number to print in rax, used for division
    call _print                 ; call the print subroutine
    mov rax, 43                 ; put the number to print in rax
    call _print                 ; call the print subroutine
    call _exit                  ; call the exit subroutine

_print:                         ; define a subroutine which prints the value in rax.

    mov rcx, string
    mov rbx, 10                 ; newline character
    mov [rcx], rbx              ; put the newline at the beginning of the string
    inc rcx                     ; increment the position along the string
    mov [position], rcx         ; distance along string

_reverse_number:

    mov rdx, 0                  ; zero the rdx register before div/mod operation
    mov rbx, 10                 ; converting from base 10, so divide by 10 each iteration.
    div rbx                     ; divide value in rax by rbx (10) remainder is stored in rdx
    add rdx, 48                 ; to get the ascii value of the character add 48

    mov rcx, [position]
    mov [rcx], dl               ; move least significant bytes of of rdx to address held by rcx
    inc rcx                     ; increment the position along the string.
    mov [position], rcx         ; store the position back in memory

    cmp rax, 0                  ; if there are whole parts left after division, call the function again.
    jne _reverse_number

_display:

    mov rcx, [position]

    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 1
    syscall

    mov rcx, [position]
    dec rcx                     ; starting with the end of the address, iterate backward.
    mov [position], rcx

    cmp rcx, string
    jge _display                ; if the position is not yet back at the start, print the next character.

    ret                         ; end of subroutine, value of rax has been printed.

_exit:

    mov rax, 60
    mov rdi, 0
    syscall                     ; exit the program with sys_exit
```

## Print String

```{.asm .gnuassembler .s}
section .data

    test_string_1 db "test_string_1",10,0 ; define a test string
    test_string_2 db "test_string_2",10,0 ; define a test string

section .text

    global _start

_start:

    mov r15, test_string_1      ; load the address of a string into r15
    call _print                 ; call the _print subroutine
    mov r15, test_string_2      ; load the address of a string into r15
    call _print                 ; call the _print subroutine
    jmp _exit                   ; exit the program

_print:

    push r15                    ; put the beginning of the string on the stack
    mov rbx, 0                  ; keep track of the length

_iteration:

    inc r15
    inc rbx
    mov cl, [r15]               ; copy the character at r15 into cl
    cmp cl, 0
    jne _iteration              ; if the current character != 0, increment again.

    mov rax, 1                  ; put together a sys_write call
    mov rdi, 1
    pop rsi                     ; retrieve the start of the string from the stack
    mov rdx, rbx                ; copy the final length of the string into the rdx register
    syscall

    ret                         ; return to the function call

_exit:

    mov rax, 60
    mov rdi, 0
    syscall                     ; exit the program with sys_exit
```

## Fibonacci Numbers

```{.asm .gnuassembler .s}
section .bss

    string resb 100             ; hold the string itself
    position resb 8             ; hold the current position along the string

section .text

    global _start

_start:

    mov r13, 1                  ; the current number
    mov r14, 0                  ; the last number
    mov r15, 0                  ; initialise a counter

_loop:

    mov r12, r13                ; backup the value in r13
    add r13, r14                ; add the previous number to this number
    mov r14, r12                     ; pop the value on the stack into r14, the previous number
    mov rax, r13                     ; move the current number into rax
    call _print                 ; call the print subroutine
    inc r15                     ; increment the counter
    cmp r15, 10
    jl _loop                    ; loop if iterations < 10
    call _exit                  ; call the exit subroutine

_print:                         ; define a subroutine which prints the value in rax.

    mov rcx, string
    mov rbx, 10                 ; newline character
    mov [rcx], rbx              ; put the newline at the beginning of the string
    inc rcx                     ; increment the position along the string
    mov [position], rcx         ; distance along string

_reverse_number:

    mov rdx, 0                  ; zero the rdx register before div/mod operation
    mov rbx, 10                 ; converting from base 10, so divide by 10 each iteration.
    div rbx                     ; divide value in rax by rbx (10) remainder is stored in rdx
    add rdx, 48                 ; to get the ascii value of the character add 48

    mov rcx, [position]
    mov [rcx], dl               ; move least significant bytes of of rdx to address held by rcx
    inc rcx                     ; increment the position along the string.
    mov [position], rcx         ; store the position back in memory

    cmp rax, 0                  ; if there are whole parts left after division, call the function again.
    jne _reverse_number

_display:

    mov rcx, [position]

    mov rax, 1
    mov rdi, 1
    mov rsi, rcx
    mov rdx, 1
    syscall

    mov rcx, [position]
    dec rcx                     ; starting with the end of the address, iterate backward.
    mov [position], rcx

    cmp rcx, string
    jge _display                ; if the position is not yet back at the start, print the next character.

    ret                         ; end of subroutine, value of rax has been printed.

_exit:

    mov rax, 60
    mov rdi, 0
    syscall                     ; exit the program with sys_exit
```
