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
	0, 1, 0, 1, 1, 1, 2, 0
};

static const signed char _at_parser_key_offsets[] = {
	0, 0, 5, 6, 10, 11, 12, 13,
	14, 16, 18, 20, 22, 27, 32, 37,
	0
};

static const char _at_parser_trans_keys[] = {
	9, 10, 13, 32, 65, 84, 10, 13,
	43, 69, 84, 69, 83, 84, 10, 13,
	48, 49, 10, 13, 10, 13, 9, 10,
	13, 32, 65, 9, 10, 13, 32, 65,
	9, 10, 13, 32, 65, 9, 10, 13,
	32, 65, 0
};

static const signed char _at_parser_single_lengths[] = {
	0, 5, 1, 4, 1, 1, 1, 1,
	2, 2, 2, 2, 5, 5, 5, 5,
	0
};

static const signed char _at_parser_range_lengths[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0
};

static const signed char _at_parser_index_offsets[] = {
	0, 0, 6, 8, 13, 15, 17, 19,
	21, 24, 27, 30, 33, 39, 45, 51,
	0
};

static const signed char _at_parser_cond_targs[] = {
	1, 12, 12, 1, 2, 0, 3, 0,
	12, 13, 4, 9, 0, 5, 0, 6,
	0, 7, 0, 8, 0, 12, 13, 0,
	10, 11, 0, 12, 14, 0, 12, 15,
	0, 1, 12, 12, 1, 2, 0, 1,
	12, 12, 1, 2, 0, 1, 12, 12,
	1, 2, 0, 1, 12, 12, 1, 2,
	0, 0, 1, 2, 3, 4, 5, 6,
	7, 8, 9, 10, 11, 12, 13, 14,
	15, 0
};

static const signed char _at_parser_cond_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 1, 1, 0,
	0, 0, 0, 3, 3, 0, 5, 5,
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 3, 0,
	0, 0, 0, 0, 5, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0
};

static const int at_parser_start = 12;
static const int at_parser_first_final = 12;
static const int at_parser_error = 0;

static const int at_parser_en_main = 12;


#line 48 "main.rl"


int main() {
	at_ctx_t at_ctx = { .echo_en = true };
	at_ctx_t *ctx = &at_ctx;
	bool skipping = false;
	
	set_raw_mode();
	atexit(restore_mode);
	

#line 108 "main.c"
	{
		ctx->cs = (int)at_parser_start;
	}
	
#line 58 "main.rl"

	
	printf("Strict Line-Based AT Parser Ready.\r\n");
	
	int c;
	while ((c = getchar()) != EOF) {
		char input = (char)c;
		
		if (ctx->echo_en) {
			if (input == '\r' || input == '\n') {
				printf("\r\n");
				fflush(stdout);
			} else if (input >= 32) {
				putchar(input);
				fflush(stdout);
			}
		}
		
		if (skipping) {
			if (input == '\r' || input == '\n') {
				printf("ERROR\r\n");
				fflush(stdout);
				skipping = false;

#line 135 "main.c"
	{
					ctx->cs = (int)at_parser_start;
				}
				
#line 81 "main.rl"

			}
			continue;
		}
		
		const char *p = &input;
		const char *pe = p + 1;
		const char *eof = NULL;
		

#line 148 "main.c"
	{
			int _klen;
			unsigned int _trans = 0;
			const char * _keys;
			const signed char * _acts;
			unsigned int _nacts;
			_resume: {}
			if ( p == pe )
				goto _out;
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
#line 34 "main.rl"
								printf("OK\r\n"); }
							
#line 223 "main.c"

							break; 
						}
						case 1:  {
								{
#line 35 "main.rl"
								ctx->echo_en = false; printf("OK\r\n"); }
							
#line 231 "main.c"

							break; 
						}
						case 2:  {
								{
#line 36 "main.rl"
								ctx->echo_en = true;  printf("OK\r\n"); }
							
#line 239 "main.c"

							break; 
						}
					}
					_nacts -= 1;
					_acts += 1;
				}
				
			}
			
			if ( ctx->cs != 0 ) {
				p += 1;
				goto _resume;
			}
			_out: {}
		}
		
#line 90 "main.rl"

		
		if (ctx->cs == at_parser_error) {
			if (input == '\r' || input == '\n') {
				printf("ERROR\r\n");
				fflush(stdout);

#line 262 "main.c"
	{
					ctx->cs = (int)at_parser_start;
				}
				
#line 96 "main.rl"

			} else {
				skipping = true;
			}
		}
	}
	return 0;
}