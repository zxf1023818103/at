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
    padding = space*;

    # --- Actions ---
    # 重点：在响应内容前面加上 \r\n，确保与输入的指令行分开
    action on_heartbeat { printf("OK\r\n"); }
    action on_echo_off  { ctx->echo_en = false; printf("OK\r\n"); }
    action on_echo_on   { ctx->echo_en = true;  printf("OK\r\n"); }
    action on_error     { printf("ERROR\r\n"); }

    # --- Logic ---
    
    at_core = "AT" (
        eol @on_heartbeat |
        "E0" eol @on_echo_off |
        "E1" eol @on_echo_on |
        "+TEST" eol @on_heartbeat
    );

    error_line = ( (any - eol)+ eol ) %on_error;

    # 这里的 $do_echo 会捕捉每一个字符，包括最后的换行符
    main := (
        (padding at_core) | 
        (padding eol)     | 
        error_line
    )*;

    write data;
}%%

int main() {
    at_ctx_t at_ctx = { .echo_en = true };
    at_ctx_t *ctx = &at_ctx;

    set_raw_mode();
    atexit(restore_mode);

    %% write init;

    printf("Strict Line-Based AT Parser Ready.\r\n");

    int c;
    while ((c = getchar()) != EOF) {
        char input = (char)c;
        const char *p = &input;
        const char *pe = p + 1;
        const char *eof = NULL;

        // 方案：不要在 Ragel 内部处理 EOL 的回显，在外部处理更可控
        // 删掉 Ragel 里的 action do_echo 对 \r \n 的判断，改用以下逻辑：
        if (ctx->echo_en) {
            if (input == '\r' || input == '\n') {
                // 用户按下回车，我们手动给一个换行回显
                printf("\r\n");
                fflush(stdout);
            } else if (input >= 32) {
                putchar(input);
                fflush(stdout);
            }
        }

        %% write exec;

        if (ctx->cs == at_parser_error) {
            %% write init;
        }
    }
    return 0;
}