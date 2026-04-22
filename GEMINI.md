# AT Command Parser Project

## Project Overview
This project implements a strict, line-based AT command parser using the **Ragel State Machine Compiler** and **C**. It is designed to emulate the behavior of traditional AT command interfaces used in telecommunications and embedded systems, featuring real-time character processing and manual echo control.

### Key Technologies
- **C Language**: The core implementation language for system calls and runtime logic.
- **Ragel**: A state machine compiler used to define and generate the AT command parsing logic.
- **Termios**: Utilized to configure the terminal in raw mode, enabling immediate character-by-character input capture.

### Architecture
- **`main.rl`**: The source of truth. It contains the Ragel state machine definition, terminal configuration logic, and the main event loop.
- **`main.c`**: The generated output from Ragel. This file should not be edited directly.
- **State Management**: Uses an `at_ctx_t` structure to track the parser state and configuration (like echo settings).

## Building and Running

### Prerequisites
- `gcc` (or another C compiler)
- `ragel`

### Build Commands
1. **Generate the C source**:
   ```bash
   ragel main.rl -o main.c
   ```
2. **Compile the binary**:
   ```bash
   gcc main.c -o a.out
   ```

### Running
Execute the compiled program:
```bash
./a.out
```
*Note: The program will put your terminal into raw mode. Use `Ctrl+C` or `EOF` to exit, and the terminal state will be restored via `atexit`.*

## Development Conventions

### Source Integrity
- **Always edit `main.rl`**: The `main.c` file is derivative. Any changes made directly to `main.c` will be lost when the state machine is regenerated.

### Parser Logic
- **Grammar**: The AT command grammar is defined within the `%%{ ... }%%` block in `main.rl`.
- **Actions**: logic triggered by state transitions (like printing "OK" or "ERROR") are defined as Ragel `action` blocks.
- **Line Endings**: The parser explicitly handles `\r\n`, `\n`, and `\r`.

### Terminal Handling
- **Raw Mode**: The program disables `ICANON` and `ECHO` to process characters as they are typed.
- **Manual Echo**: Echoing is handled in the main loop logic:
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
- `AT`: Returns `OK` (Heartbeat).
- `ATE0`: Disables local echo.
- `ATE1`: Enables local echo.
- `AT+TEST`: Returns `OK` (Test command).
- Any unrecognized input followed by a line ending returns `ERROR`.
