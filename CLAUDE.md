# AT Command Parser Project

## Project Overview
This project implements a strict, line-based AT command parser using the **Ragel State Machine Compiler** and **C**. It emulates traditional AT command interfaces used in telecommunications and embedded systems, featuring real-time character processing and manual echo control.

### Key Technologies
- **C Language**: Core implementation language for system calls and runtime logic.
- **Ragel**: State machine compiler used to define and generate the AT command parsing logic.
- **Termios**: Configures the terminal in raw mode for character-by-character input capture.

### Architecture
- **`main.rl`**: The source of truth — contains the Ragel state machine definition, terminal configuration logic, and the main event loop.
- **`main.c`**: Generated output from Ragel. Do not edit directly.
- **State Management**: Uses an `at_ctx_t` structure to track parser state and configuration (e.g. echo settings).

## Build Commands

```bash
# Generate the C source
ragel main.rl -o main.c

# Compile the binary
gcc main.c -o a.out

# Run
./a.out
```

The program puts the terminal into raw mode. Use `Ctrl+C` or `EOF` to exit; terminal state is restored via `atexit`.

## Development Conventions

### Source Integrity
- **Always edit `main.rl`** — `main.c` is derivative and will be overwritten on regeneration.

### Parser Logic
- Grammar is defined in the `%%{ ... }%%` block in `main.rl`.
- Actions (e.g. printing `OK` or `ERROR`) are defined as Ragel `action` blocks.
- Line endings handled: `\r\n`, `\n`, and `\r`.

### Terminal Handling
- Raw mode disables `ICANON` and `ECHO`.
- Echo is handled manually in the main loop:
  ```c
  if (ctx->echo_en) {
      if (input == '\r' || input == '\n') {
          printf("\r\n");
      } else if (input >= 32) {
          putchar(input);
      }
  }
  ```

### Supported Commands
- `AT` — Returns `OK` (heartbeat).
- `ATE0` — Disables local echo.
- `ATE1` — Enables local echo.
- `AT+TEST` — Returns `OK` (test command).
- Any unrecognized input followed by a line ending returns `ERROR`.
