#include "lexer.h"
#include <circes.h>
#include <mf/list.h>
#include <stdio.h>

static nfa int_nfa;
static nfa string_nfa;

void lex_init(arena *a)
{
        nfa_init_builtins(a);

        nfa nat_digits = *nfa_builtin(S("1"));
        nat_digits = nfa_union(a, nat_digits, *nfa_builtin(S("2")));
        nat_digits = nfa_union(a, nat_digits, *nfa_builtin(S("3")));
        nat_digits = nfa_union(a, nat_digits, *nfa_builtin(S("4")));
        nat_digits = nfa_union(a, nat_digits, *nfa_builtin(S("5")));
        nat_digits = nfa_union(a, nat_digits, *nfa_builtin(S("6")));
        nat_digits = nfa_union(a, nat_digits, *nfa_builtin(S("7")));
        nat_digits = nfa_union(a, nat_digits, *nfa_builtin(S("8")));
        nat_digits = nfa_union(a, nat_digits, *nfa_builtin(S("9")));

        nfa digits = nfa_union(a, nat_digits, *nfa_builtin(S("0")));

        nfa lowercase =
            nfa_union(a, *nfa_builtin(S("a")), *nfa_builtin(S("b")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("c")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("d")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("e")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("f")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("g")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("h")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("i")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("j")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("k")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("l")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("m")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("n")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("o")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("p")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("q")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("r")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("s")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("t")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("u")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("v")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("w")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("x")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("y")));
        lowercase = nfa_union(a, lowercase, *nfa_builtin(S("z")));

        nfa uppercase = *nfa_builtin(S("A"));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("B")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("C")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("D")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("E")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("F")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("G")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("H")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("I")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("J")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("K")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("L")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("M")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("N")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("O")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("P")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("Q")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("R")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("S")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("T")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("U")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("V")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("W")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("X")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("Y")));
        uppercase = nfa_union(a, uppercase, *nfa_builtin(S("Z")));

        nfa space = *nfa_builtin(S(" "));
        nfa alpha = nfa_union(a, space, nfa_union(a, lowercase, uppercase));
        nfa alphanumeric = nfa_union(a, alpha, digits);

        int_nfa = nfa_close(a, digits);
        string_nfa = nfa_close(a, alphanumeric);
}

static token *emit_nfa_token(arena *a, nfa n, string input, int *i)
{
        token *t = make(a, token, 1);
        string substr = {.len = 1, .addr = input.addr + (*i)};
        while (nfa_test(a, n, substr)) {
                // printf("passed '%.*s', next '%c'\n", substr.len, substr.addr,
                // substr.addr[substr.len]);
                substr.len++;
                (*i)++;
        }
        t->sdata.len = --substr.len;
        t->sdata.addr = make(a, char, t->sdata.len);
        mfmemcpy(t->sdata.addr, substr.addr, t->sdata.len);
        // printf("%.*s\n", t->sdata.len, t->sdata.addr);
        return t;
}

static token *emit_token(arena *a, string input, int *i)
{
        token *t;
        switch (input.addr[(*i)++]) {
        case '<':
                t = make(a, token, 1);
                t->type = TTOK_OPEN;
                t->idata = '<';
                return t;
        case '>':
                t = make(a, token, 1);
                t->type = TTOK_CLOSE;
                t->idata = '>';
                return t;
        case '=':
                t = make(a, token, 1);
                t->type = TTOK_EQUAL;
                t->idata = '=';
                return t;
        case '/':
                t = make(a, token, 1);
                t->type = TTOK_SLASH;
                t->idata = '/';
                return t;
        case ',':
                t = make(a, token, 1);
                t->type = TTOK_COMMA;
                t->idata = ',';
                return t;
        }

        string sample;
        sample.len = 1;
        sample.addr = input.addr + (--*i);
        if (nfa_test(a, int_nfa, sample)) {
                // printf("hit int nfa: '%.*s'\n", sample.len, sample.addr);
                t = emit_nfa_token(a, int_nfa, input, i);
                t->type = TTOK_INT;
                return t;
        } else if (nfa_test(a, string_nfa, sample)) {
                // printf("hit string nfa: '%.*s'\n", sample.len, sample.addr);
                t = emit_nfa_token(a, string_nfa, input, i);
                t->type = TTOK_STRING;
                return t;
        }

        t = make(a, token, 1);
        t->type = TTOK_BROKEN;
        (*i)++;

        return t;
}

token_string lex(arena *a, string input)
{
        int i = 0;
        list *tok_node;
        list *tok_list = 0;
        token_string tokens;

        tokens.len = 0;
        while (i < input.len) {
                tok_node = make(a, list, 1);
                tok_node->data = emit_token(a, input, &i);
                list_append(&tok_list, tok_node);
                tokens.len++;
        }

        i = 0;
        list *cursor = tok_list;
        tokens.addr = make(a, token, ++tokens.len);
        while (cursor) {
                tokens.addr[i++] = *(token *)cursor->data;
                cursor = cursor->next;
        }
        tokens.addr[tokens.len - 1] = (token){
            .type = TTOK_OVER,
            .sdata = S_NULL,
            .idata = 0,
        };
        return tokens;
}
