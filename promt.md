You are extending your basic kernel shell in VibexOS with its first command: clear.
🧾 Functional Requirements

    Command Behavior:

        When the user types clear and presses Enter, the screen should:

            Be completely cleared (overwrite all VGA cells with spaces).

            Cursor should reset to the top-left corner (row 0, column 0).

    Environment:

        You are operating in VGA text mode (0xB8000).

        All shell display output uses VGA memory directly (no framebuffer, no BIOS).

        Input is captured from keyboard interrupts and stored in a buffer.

    Design Constraints:

        Modular architecture:

            Implement vga_clear() inside src/drivers/vga.c.

            Expose it via vga.h.

            In the shell command handler (shell.c), detect "clear" and call vga_clear().

        Do not reset the input buffer unless Enter is pressed.

        Should not affect interrupt handling or IDT state.

    Optional UX Polish:

        After clearing, redisplay the shell prompt like > at the top-left corner.

📦 LLM Output Should Include:

    vga_clear() implementation:

        Wipes 80x25 VGA cells (usually 2000 characters) by writing spaces and default color.

        Resets row/column counters if used in your VGA abstraction.

    Updated shell.c:

        In your shell loop or command handler, detect if buffer equals "clear" on Enter.

        If so, call vga_clear() and reprint the prompt.

    Header update:

        src/include/vga.h should declare vga_clear() with proper include guard.