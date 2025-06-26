#ifndef lexer_h
#define lexer_h

#include <circes.h>
#include <mf/memory.h>
#include <mf/string.h>

/*
open => '<'
close => '>'
equals => '='
slash => '/'
comma => ','
int => [1-9].[0-9]*
string => [a-zA-Z].[0-9a-zA-Z]*
*/

typedef enum token_type {
        TTOK_BROKEN, // 0
        TTOK_OPEN,   // 1
        TTOK_CLOSE,  // 2
        TTOK_EQUAL,  // 3
        TTOK_SLASH,  // 4
        TTOK_COMMA,  // 5
        TTOK_INT,    // 6
        TTOK_STRING, // 7

        TTOK_OVER,     // 8
        NTOK_START,    // 9
        NTOK_LOWER,    // 10
        NTOK_UPPER,    // 11
        NTOK_RULES,    // 12
        NTOK_FIZZBUZZ, // 13
} token_type;

void lex_init(arena *a);
token_string lex(arena *a, string input);

#endif
