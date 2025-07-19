You are building a bare-metal operating system kernel (monolithic, C-based) without any standard C library. Write a primitive C library that includes:
🧾 Requirements

    printf() implementation:

        Self-contained — no libc, stdlib, or stdio.

        Supports a minimal subset of format specifiers:

            %s – strings

            %c – characters

            %d or %i – signed integers (base 10)

            %x – hexadecimal integers (lowercase)

        Writes directly to the VGA text buffer at 0xB8000 in text mode.

        Supports newline (\n) handling (i.e., advances to the next row properly).

    Modular design:

        Implement printf() on top of a simpler function like putchar() or print_char() that writes a single character to the VGA buffer.

        Include helper functions for:

            Integer to ASCII conversion (decimal and hexadecimal)

            Cursor tracking (row/column)

            Basic screen scrolling (optional)

    Usage context:

        This library will be used within the kernel (kernel.c) after bootloading completes and protected mode is active.

        Written in C, compiled with gcc -m32.

    Output:

        File: /src/lib/printf.c (implementations)

        Header: /src/include/printf.h (declarations)

        Ensure header guards are present

        The implementation must be freestanding and suitable for a kernel.

    Example usage:

        Demonstrate calling printf("Welcome to VibexOS version %d\n", 1); from within your kernel_main().

📦 Output Should Include:

    src/lib/printf.c – full implementation

    src/include/printf.h – header with printf declaration

    Example call inside kernel.c

    Updated Makefile (optional): include -I src/include and compile printf.c into kernel binary.