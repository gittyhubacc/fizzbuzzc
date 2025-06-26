#include "parser.h"
#include "lexer.h"

#define T(t) ((token){.type = t, .sdata = S_NULL, .idata = 0})

// declare the strings
static token lower_rule[] = {T(TTOK_OPEN), T(TTOK_INT)};
static token upper_rule[] = {T(TTOK_INT), T(TTOK_CLOSE)};
static token rules_rule_a[] = {T(TTOK_INT), T(TTOK_EQUAL), T(TTOK_STRING)};
static token rules_rule_b[] = {
    T(NTOK_RULES), T(TTOK_COMMA), T(TTOK_INT), T(TTOK_EQUAL), T(TTOK_STRING),
};
static token fizzbuzz_rule[] = {
    T(NTOK_LOWER), T(TTOK_SLASH), T(NTOK_RULES), T(TTOK_SLASH), T(NTOK_UPPER),
};
static token augmented_rule[] = {T(NTOK_FIZZBUZZ), T(TTOK_OVER)};

// build the rules
static pda_rule fizzbuzz_rules[] = {
    {
        .head = T(NTOK_LOWER),
        .body = TS(lower_rule),
    },
    {
        .head = T(NTOK_UPPER),
        .body = TS(upper_rule),
    },
    {
        .head = T(NTOK_RULES),
        .body = TS(rules_rule_a),
    },
    {
        .head = T(NTOK_RULES),
        .body = TS(rules_rule_b),
    },
    {
        .head = T(NTOK_FIZZBUZZ),
        .body = TS(fizzbuzz_rule),
    },
    {
        .head = T(NTOK_START),
        .body = TS(augmented_rule),
    },
};
static const int fizzbuzz_rules_cnt =
    sizeof(fizzbuzz_rules) / sizeof(*fizzbuzz_rules);
static grammar fizzbuzz_grammar = {
    .rules = fizzbuzz_rules,
    .rule_cnt = fizzbuzz_rules_cnt,
    .start = fizzbuzz_rules + (fizzbuzz_rules_cnt - 1),
};

parse_tree *parse(arena *a, token_string tokens)
{
        return circes_parse(a, &fizzbuzz_grammar, tokens);
}
