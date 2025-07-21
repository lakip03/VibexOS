You are building a freestanding C kernel shell for VibexOS. Implement a minimal scanf-like function to read user input and store it in variables.
🧾 Requirements

    Functionality:

        Implement a primitive scanf()-style function called kscanf().

        Must support only basic format specifiers:

            %s → read a word into a char* buffer (null-terminated)

            %d → read an integer into an int*

        Input is read from the keyboard (buffered as usual).

        Parsing should stop at whitespace or Enter.

    Constraints:

        Freestanding kernel — no libc, no stdarg, no heap, no dynamic memory.

        Input is already captured into a fixed-size buffer by your shell's keyboard ISR.

        Parsing and conversion must be manually handled.

        Function signature:

    void kscanf(const char* format, ...);

Modular Code Layout:

    src/lib/kscanf.c – implementation of kscanf.

    src/include/kscanf.h – header file with include guards.

    Reuse existing shell input buffer or allow kscanf() to read line-by-line.

    Provide helper functions (e.g., str_to_int() if needed).

Testing Scenario:

    Inside kernel_main() or your shell command handler, write a small test:

        char name[32];
        int age;
        printf("Enter your name: ");
        kscanf("%s", name);
        printf("Enter your age: ");
        kscanf("%d", &age);
        printf("Hello, %s. You are %d years old.\n", name, age);

    Optional:

        Make sure scanf skips leading whitespace.

        If %s is too long, truncate and null-terminate.

        Can read input by reusing shell’s line buffer or directly hooking into the same keyboard logic.

📦 Output LLM Should Produce:

    src/lib/kscanf.c – full implementation

    src/include/kscanf.h – header with kscanf() declaration

    Update to kernel_main() or shell to test it

    Optional helper: str_to_int() if %d support is isolated