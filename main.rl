#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>

typedef struct {
    int cs;
    bool echo_en;
} at_ctx_t;

struct termios orig_termios;

void set_raw_mode() {
    struct termios new_termios;
    tcgetattr(STDIN_FILENO, &orig_termios);
    new_termios = orig_termios;
    // 关闭系统回显和行缓冲，实现实时字符捕获
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_termios);
}

void restore_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

%%{
    machine at_parser;
    access ctx->;

    eol = "\r\n" | "\n" | "\r";

    action on_heartbeat { printf("OK\r\n"); }
    action on_echo_off  { ctx->echo_en = false; printf("OK\r\n"); }
    action on_echo_on   { ctx->echo_en = true;  printf("OK\r\n"); }

    at_core = "AT" (
        eol @on_heartbeat |
        "E0" eol @on_echo_off |
        "E1" eol @on_echo_on |
        "+TEST" eol @on_heartbeat
    );

    main := ( at_core | eol )*;

    write data;
}%%

int main() {
    at_ctx_t at_ctx = { .echo_en = true };
    at_ctx_t *ctx = &at_ctx;
    bool skipping = false;

    set_raw_mode();
    atexit(restore_mode);

    %% write init;

    printf("Strict Line-Based AT Parser Ready.\r\n");

    int c;
    while ((c = getchar()) != EOF) {
        char input = (char)c;

        if (ctx->echo_en) {
            putchar(input);
            fflush(stdout);
        }

        if (skipping) {
            if (input == '\r' || input == '\n') {
                printf("ERROR\r\n");
                fflush(stdout);
                skipping = false;
                %% write init;
            }
            continue;
        }

        const char *p = &input;
        const char *pe = p + 1;
        const char *eof = NULL;

        %% write exec;

        if (ctx->cs == at_parser_error) {
            if (input == '\r' || input == '\n') {
                printf("ERROR\r\n");
                fflush(stdout);
                %% write init;
            } else {
                skipping = true;
            }
        }
    }
    return 0;
}
