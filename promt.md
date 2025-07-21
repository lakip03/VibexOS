
✅ Prompt: Build a Basic Kernel Shell (Input + Echo) in C

You are writing a basic shell for your monolithic operating system kernel (VibexOS). Generate C code that provides:
🧾 Functional Requirements

    Input from keyboard:

        Read key presses from keyboard hardware via the keyboard interrupt (IRQ1).

        Decode scancodes into ASCII characters (handle lowercase letters, digits, and basic symbols).

        Store typed characters into a buffer in RAM (use a fixed-size char array).

    Display to screen:

        For every keypress, echo the character to screen via VGA text mode (write to 0xB8000).

        When Enter is pressed:

            Move to a new line.

            Print: You typed: <buffered string> (or similar response).

            Then clear the buffer and wait for next input.

    Environment:

        Written in C, compiled with gcc -m32, running after the bootloader.

        Assume protected mode is active.

        Keyboard interrupt handling should be set up via the IDT (Interrupt Descriptor Table).

    Constraints:

        Do not use libc or any OS features — this is a freestanding kernel.

        Handle only printable ASCII characters and backspace (optional).

        Keep implementation modular (e.g., separate keyboard.c, shell.c, vga.c files).

📦 Output Should Include:

    src/kernel/shell.c – core loop that handles input + echo

    src/drivers/keyboard.c – ISR and buffer management

    src/drivers/vga.c – VGA text-mode display helpers (optional if not already created)

    src/include/*.h – headers for each module (with include guards)

    Example session:

    > hello world
    You typed: hello world
    >

    BONUS (Optional): Implement basic scrolling if VGA reaches bottom of screen.

⚙️ Integration Notes

    Make sure your IDT is initialized and the IRQ1 interrupt handler is correctly set up.

    Remap the PIC if needed (IRQ1 = INT 0x21 or similar).

    The shell loop can run from kernel_main() after enabling interrupts (sti).