#line 1 "main.rl"
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

#line 28 "main.c"
static const signed char _at_parser_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1,
	3, 2, 0, 3, 2, 1, 3, 2,
	2, 3, 0
};

static const signed char _at_parser_key_offsets[] = {
	0, 2, 8, 11, 15, 18, 21, 24,
	27, 29, 33, 35, 37, 43, 49, 55,
	61, 67, 0
};

static const char _at_parser_trans_keys[] = {
	10, 13, 10, 13, 32, 65, 9, 12,
	10, 13, 84, 10, 13, 43, 69, 10,
	13, 84, 10, 13, 69, 10, 13, 83,
	10, 13, 84, 10, 13, 10, 13, 48,
	49, 10, 13, 10, 13, 10, 13, 32,
	65, 9, 12, 10, 13, 32, 65, 9,
	12, 10, 13, 32, 65, 9, 12, 10,
	13, 32, 65, 9, 12, 10, 13, 32,
	65, 9, 12, 10, 13, 32, 65, 9,
	12, 0
};

static const signed char _at_parser_single_lengths[] = {
	2, 4, 3, 4, 3, 3, 3, 3,
	2, 4, 2, 2, 4, 4, 4, 4,
	4, 4, 0
};

static const signed char _at_parser_range_lengths[] = {
	0, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 1, 1, 1,
	1, 1, 0
};

static const signed char _at_parser_index_offsets[] = {
	0, 3, 9, 13, 18, 22, 26, 30,
	34, 37, 42, 45, 48, 54, 60, 66,
	72, 78, 0
};

static const signed char _at_parser_cond_targs[] = {
	13, 14, 0, 13, 14, 1, 2, 1,
	0, 13, 14, 3, 0, 13, 15, 4,
	9, 0, 13, 14, 5, 0, 13, 14,
	6, 0, 13, 14, 7, 0, 13, 14,
	8, 0, 13, 15, 0, 13, 14, 10,
	11, 0, 13, 16, 0, 13, 17, 0,
	12, 12, 1, 2, 1, 0, 12, 12,
	1, 2, 1, 0, 13, 12, 1, 2,
	1, 0, 13, 12, 1, 2, 1, 0,
	13, 12, 1, 2, 1, 0, 13, 12,
	1, 2, 1, 0, 0, 1, 2, 3,
	4, 5, 6, 7, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17, 0
};

static const signed char _at_parser_cond_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 0, 0,
	0, 0, 3, 3, 0, 5, 5, 0,
	0, 0, 0, 0, 0, 0, 7, 7,
	7, 7, 7, 7, 7, 7, 7, 7,
	7, 7, 9, 7, 7, 7, 7, 7,
	12, 7, 7, 7, 7, 7, 15, 7,
	7, 7, 7, 7, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 7, 7, 7, 7, 7, 0
};

static const signed char _at_parser_eof_trans[] = {
	85, 86, 87, 88, 89, 90, 91, 92,
	93, 94, 95, 96, 97, 98, 99, 100,
	101, 102, 0
};

static const int at_parser_start = 12;
static const int at_parser_first_final = 12;
static const int at_parser_error = -1;

static const int at_parser_en_main = 12;


#line 60 "main.rl"


int main() {
	at_ctx_t at_ctx = { .echo_en = true };
	at_ctx_t *ctx = &at_ctx;
	
	set_raw_mode();
	atexit(restore_mode);
	

#line 125 "main.c"
	{
		ctx->cs = (int)at_parser_start;
	}
	
#line 69 "main.rl"

	
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
		

#line 153 "main.c"
	{
			int _klen;
			unsigned int _trans = 0;
			const char * _keys;
			const signed char * _acts;
			unsigned int _nacts;
			_resume: {}
			if ( p == pe && p != eof )
				goto _out;
			if ( p == eof ) {
				if ( _at_parser_eof_trans[ctx->cs] > 0 ) {
					_trans = (unsigned int)_at_parser_eof_trans[ctx->cs] - 1;
				}
			}
			else {
				_keys = ( _at_parser_trans_keys + (_at_parser_key_offsets[ctx->cs]));
				_trans = (unsigned int)_at_parser_index_offsets[ctx->cs];
				
				_klen = (int)_at_parser_single_lengths[ctx->cs];
				if ( _klen > 0 ) {
					const char *_lower = _keys;
					const char *_upper = _keys + _klen - 1;
					const char *_mid;
					while ( 1 ) {
						if ( _upper < _lower ) {
							_keys += _klen;
							_trans += (unsigned int)_klen;
							break;
						}
						
						_mid = _lower + ((_upper-_lower) >> 1);
						if ( ( (*( p))) < (*( _mid)) )
							_upper = _mid - 1;
						else if ( ( (*( p))) > (*( _mid)) )
							_lower = _mid + 1;
						else {
							_trans += (unsigned int)(_mid - _keys);
							goto _match;
						}
					}
				}
				
				_klen = (int)_at_parser_range_lengths[ctx->cs];
				if ( _klen > 0 ) {
					const char *_lower = _keys;
					const char *_upper = _keys + (_klen<<1) - 2;
					const char *_mid;
					while ( 1 ) {
						if ( _upper < _lower ) {
							_trans += (unsigned int)_klen;
							break;
						}
						
						_mid = _lower + (((_upper-_lower) >> 1) & ~1);
						if ( ( (*( p))) < (*( _mid)) )
							_upper = _mid - 2;
						else if ( ( (*( p))) > (*( _mid + 1)) )
							_lower = _mid + 2;
						else {
							_trans += (unsigned int)((_mid - _keys)>>1);
							break;
						}
					}
				}
				
				_match: {}
			}
			ctx->cs = (int)_at_parser_cond_targs[_trans];
			
			if ( _at_parser_cond_actions[_trans] != 0 ) {
				
				_acts = ( _at_parser_actions + (_at_parser_cond_actions[_trans]));
				_nacts = (unsigned int)(*( _acts));
				_acts += 1;
				while ( _nacts > 0 ) {
					switch ( (*( _acts)) )
					{
						case 0:  {
								{
#line 36 "main.rl"
								printf("OK\r\n"); }
							
#line 235 "main.c"

							break; 
						}
						case 1:  {
								{
#line 37 "main.rl"
								ctx->echo_en = false; printf("OK\r\n"); }
							
#line 243 "main.c"

							break; 
						}
						case 2:  {
								{
#line 38 "main.rl"
								ctx->echo_en = true;  printf("OK\r\n"); }
							
#line 251 "main.c"

							break; 
						}
						case 3:  {
								{
#line 39 "main.rl"
								printf("ERROR\r\n"); }
							
#line 259 "main.c"

							break; 
						}
					}
					_nacts -= 1;
					_acts += 1;
				}
				
			}
			
			if ( p == eof ) {
				if ( ctx->cs >= 12 )
					goto _out;
			}
			else {
				p += 1;
				goto _resume;
			}
			_out: {}
		}
		
#line 93 "main.rl"

		
		if (ctx->cs == at_parser_error) {

#line 283 "main.c"
	{
				ctx->cs = (int)at_parser_start;
			}
			
#line 96 "main.rl"

		}
	}
	return 0;
}