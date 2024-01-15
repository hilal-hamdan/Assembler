Assembly Language Pre-Assembler

The Assembly Language Pre-Assembler (assembler.c) is a tool designed to simplify the development process of assembly language projects. It processes assembly code files, providing support for macros and handling various data types. The goal is to enhance code organization, reusability, and overall workflow.

Key Features:

Macro Support: Define and use macros in assembly code for code reuse and improved readability.

Data Handling: Supports different data types, including integers, strings, and structs, making it versatile for various assembly programming needs.

Intermediate Machine Code: Generates intermediate machine code as output, providing a bridge between the high-level assembly code and the final executable.

Getting Started:

To use the pre-assembler (assembler.c), clone the repository and compile the code using a C compiler. The tool processes assembly code files, generating intermediate machine code files.

Usage:

Run the pre-assembler on an assembly code file:

./assembler input.asm


This command generates an intermediate machine code file (output.am) in the same directory.

Examples:

Here are some examples illustrating basic usage and leveraging macros in assembly code.

Basic Usage:

./assembler example.asm

Using Macros:

; example.asm

macro add_two_numbers(a, b)
    mov ax, a
    add ax, b
endm

section .text
    add_two_numbers(5, 10)

Run the pre-assembler:

./assembler example.asm

Contributing:

Contributions are welcome! Feel free to open issues or submit pull requests for improvements or bug fixes.

Fork the project.
Create your feature branch (git checkout -b feature/my-feature).
Commit your changes (git commit -am 'Add my feature').
Push to the branch (git push origin feature/my-feature).
Open a pull request.

Author:

@hilal-hamdan