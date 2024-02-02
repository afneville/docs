---
title: Assembly Programming
author: Alexander Neville
date: 2024-01-23
path: dev/unix/assembly
description: An introduction to programming in Intel x86 assembly.
image-meta: nasm-logo.png
image-meta-credit: https://nasm.us/
---

Assembly languages include the panoply of low-level,
architecture-specific systems languages. There is no single assembly
language specification, though such languages generally have in common a
sequential structure and an immediate correspondence between their
mnemonic or abbreviated assembly instructions and the architecture's
machine code instructions.

# Sections

Assembly programs are divided into sections, which provide information
to the linker and ensure that data in a process is arranged sensibly in
memory. Three common sections are:

- `.data`: Store initialised constants and program parameters.
- `.bss`: Reserve memory to use for dynamic and uninitialised data.
- `.text`: Contains executable program code.

These sections would be introduced with the statements:

```asm
section .data
section .bss
section .text
```

A `.text` section must contain a reference to `_start`, signifying the
start of the program, required by the _linker_ if it is to create
executable machine code. This is an example of a _label_.

```asm
section .text

    global _start

_start:

```

# Registers

Assembly languages allow the system registers to be addressed. In x86_64
architecture, each register is 64 bits in size. Smaller registers can be
emulated by using part of a 64 bit register. These registers have the
names:

:::{.wrap-table}

| 8 bit | 16 bit | 32 bit | 64 bit |
| ----- | ------ | ------ | ------ |
| al    | ax     | eax    | rax    |
| bl    | bx     | ebx    | rbx    |
| cl    | cx     | ecx    | rcx    |
| dl    | dx     | edx    | rdx    |
| sil   | si     | esi    | rsi    |
| dil   | di     | edi    | rdi    |
| bpl   | bp     | ebp    | rbp    |
| spl   | sp     | esp    | rsp    |
| r8b   | r8w    | r8d    | r8     |
| r9b   | r9w    | r9d    | r9     |
| r10b  | r10w   | r10d   | r10    |
| r11b  | r11w   | r11d   | r11    |
| r12b  | r12w   | r12d   | r12    |
| r13b  | r13w   | r13d   | r13    |
| r14b  | r14w   | r14d   | r14    |
| r15b  | r15w   | r15d   | r15    |

:::

# Pointers

Pointers are another type of on-chip register. They are used to store
memory addresses of instructions. These are the most fundamental
pointers:

::: {.wrap-table}

| 16 bit | 32 bit | 64 bit | Purpose                                               |
| ------ | ------ | ------ | ----------------------------------------------------- |
| IP     | EIP    | RIP    | Holds the address of next instruction to be fetched.  |
| SP     | ESP    | RSP    | Holds the address of the top of the address stack.    |
| BP     | EBP    | RBP    | Holds the address of the bottom of the address stack. |

:::

Many of these registers are used to manage control flow, which is
typically sequential. During normal operation, the `RIP` will be
incremented by 1 for every instruction that is executed, making the
program progress.

# Flags

Flags, like registers, are a type of on-chip data storage. Unlike other
registers, a flag holds a single bit. Each flag is part of a larger
register, the _status register_. Like other registers, flags are
referred to with unique mnemonic identifiers within assembly programs.

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

Here is a table detailing some of the available arithmetic operations.
_rXX_ means any 64 bit register. _b_ could be a register or a hard coded
value.

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
flow of a program. `_start` is an example of a label. When the jump
command is encountered, the address of the instruction associated with
the label is loaded into the `RIP` and hence the control flow is
changed. The syntax of a jump is illustrated below:

```asm
_start:

    jmp _start

```

This program performs an infinite loop.

# Comparisons & Conditional Jumps

Used with jump commands, comparisons introduce complex flow control into
assembly programs. Comparisons are always drawn between one register and
either a literal operand or another register.

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

A system call is a request made of the operating system kernel to
service the program. The nature and identity of these calls are
operating system dependent. In an assembly language program, a system
call will also have parameters. Operands are passed to the kernel by
placing them in a number of registers, specified in the table below. The
ID of the system call is placed in the register `rax` and the list of
operands should be placed in order in the subsequent registers.

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

This table is accurate of 64 bit architecture.
[Here](https://blog.rchapman.org/posts/Linux_System_Call_Table_for_x86_64/)
is a full list of system calls supported by Linux. With the operands
placed in the correct registers, the `syscall` instruction is issued to
trap into the kernel.

# Assembling a Program

Oft-encountered x86 assemblers for the Linux platform include GNU AS and
NASM. The first step in assembling an executable is producing the object
code. Here, the `-f` flag and `elf64` option set the format of the
output executable to the 64 bit _Executable and Linkable Format_.

```shell
nasm -f elf64 -o example.o example.asm
```

As with the compilation of program written in a more abstract language,
linking is required to generate the executable.

```shell
ld example.o -o example
```

# Examples

The examples presented here are written in Intel x86 syntax as
understood by NASM.

## Hello World

This example prints a string of known length, created and initialised in
the `.data` section.

```{.asm .gnuassembler .s}
section .data

    greeting db "Hello, world!",10
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

Assembled and executed, this program outputs:

```language-plaintext
Hello, World!
```

## Jumps & Loops

This program features an example of the `jmp` instruction, which is not
conditional. Consequently, this program does not exit.

```{.asm .gnuassembler .s}
section .data
    greeting db "Hello, World!",10
    length_of_greeting equ $ - greeting

section .text
    global _start

_start:
    mov rax, 1
    mov rdi, 1
    mov rsi, greeting
    mov rdx, length_of_greeting
    syscall
    jmp _start

    mov rax, 60
    mov rdi, 0
    syscall
```

Assembled and executed, this program prints the same message as before
indefinitely:

```language-plaintext
Hello, World!
Hello, World!
Hello, World!
...
```

## Conditional Statements

Here is an example of a conditional statement. The contents of `r15` and
`r14` are set to 1 and 3 respectively. The contents of the two registers
are compared and the program will jump _conditionally_.

```{.asm .gnuassembler .s}
section .data

    equal db "equal to",10
    length_equal equ $ - equal
    less db "less than",10
    length_less equ $ - less
    more db "more than",10
    length_more equ $ - more

section .text

    global _start

_start:

    mov r15, 1
    mov r14, 3
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

In this case, the value in `r15` is less than that in `r14`, so the
program follows the `jl` instruction and begins executing from the label
`_less`, outputting:

```language-plaintext
less than
```

## Print an Integer

In the previous examples, the message printed by the assembly program
was of known-length and intialised in the `.data` section. This program
will print an integer in decimal format, using as many characters as
necessary. The code is commented and divided into labelled subroutines.

```{.asm .gnuassembler .s}
section .bss

    string resb 100             ; hold the string itself
    position resb 8             ; hold the current position along the string

section .text

    global _start

_start:

    ; use rax register as the value will be divided repeatedly
    mov rax, 43                 ; put a number to print in rax
    call _print                 ; call the print subroutine
    mov rax, 1037                ; put a number to print in rax
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

Assembled and executed, this program outputs:

```language-plaintext
43
1037
```

## Print a String

Along the same lines as the last example, this example prints a string -
the length of which is not determined at compile time.

```{.asm .gnuassembler .s}
section .data

    test_string_1 db "Hello, world! My name is Alexander.",10,0 ; define a test string
    test_string_2 db "Goodbye!",10,0 ; define a test string

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

Assembled and executed, this program outputs:

```language-plaintext
Hello, world! My name is Alexander.
Goodbye!
```

## Fibonacci Numbers

Using many of the routines defined above, this example calculates and
prints some Fibonacci numbers.

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

This program prints the first ten numbers of the Fibonacci sequence
(starting with 1 and 2):

```language-plaintext
1
2
3
5
8
13
21
34
55
89
```
