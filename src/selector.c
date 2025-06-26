#include "selector.h"
#include "lexer.h"
#include <mf/string.h>
#include <stdio.h>

static string concat(arena *a, string left, string right)
{
        string res;
        res.len = left.len + right.len;
        res.addr = make(a, char, res.len);
        mfmemcpy(res.addr, left.addr, left.len);
        mfmemcpy(res.addr + left.len, right.addr, right.len);
        return res;
}

static void fizzbuzz_target()
{
        for (int i = 1; i <= 100; i++) {
                int state = 0;
                int checks = 0;
                int checked = 0;

                state |= (i % 3 == 0) << checks;
                if (state & (1 << (checks++))) {
                        printf("fizz");
                        checked = 1;
                }

                state |= (i % 5 == 0) << checks;
                if (state & (1 << (checks++))) {
                        printf("buzz");
                        checked = 1;
                }

                if (!checked) {
                        printf("%i", i);
                }

                printf("\n");
        }
}

static void debug_pt(parse_tree *pt, int generation)
{
        printf("%i: [%i] (%.*s)\n", generation, pt->label.type,
               pt->label.sdata.len, pt->label.sdata.addr);
        for (int i = 0; i < pt->child_cnt; i++) {
                debug_pt(pt->children + i, generation + 1);
        }
}

static string selection_string(arena *a, parse_tree *node)
{
        return concat(a, S("\""), concat(a, node->label.sdata, S("\"")));
}

static string selection_rules(arena *a, parse_tree *node)
{
        if (node->child_cnt == 3) {
                string res = concat(a, S("\nstate |= (i % "),
                                    selection(a, node->children + 0));
                res =
                    concat(a, res,
                           S(" == 0) << checks;"
                             "                if (state & (1 << (checks++))) {"
                             "                        printf("));
                res = concat(a, res, selection(a, node->children + 2));
                res = concat(a, res,
                             S(");"
                               "                        checked = 1;"
                               "                }"));
                return res;
        } else if (node->child_cnt == 5) {
                string res = concat(a, S("state |= (i % "),
                                    selection(a, node->children + 2));
                res =
                    concat(a, res,
                           S(" == 0) << checks;"
                             "                if (state & (1 << (checks++))) {"
                             "                        printf("));
                res = concat(a, res, selection(a, node->children + 4));
                res = concat(a, res,
                             S(");"
                               "                        checked = 1;"
                               "                }"));
                return concat(a, selection(a, node->children + 0), res);
        }

        return epsilon;
}

static string selection_fizzbuzz(arena *a, parse_tree *node)
{
        string res = S("#include <stdio.h>\nint main(int argc, char **argv)"
                       "{"
                       "        for (int i =");
        res = concat(a, res, selection(a, node->children + 4));
        res = concat(a, res, S("; i <= "));
        res = concat(a, res, selection(a, node->children + 0));
        res = concat(a, res,
                     S("; i++) {"
                       "                int state = 0;"
                       "                int checks = 0;"
                       "                int checked = 0;"));
        res = concat(a, res, selection(a, node->children + 2));
        res = concat(a, res,
                     S("                if (!checked) {"
                       "                        printf(\"%i\", i);"
                       "                }"
                       "                printf(\"\\n\");"
                       "        }"
                       "        return 0;"
                       "}"));
        return res;
}

string selection(arena *a, parse_tree *node)
{
        switch (node->label.type) {
        case TTOK_INT:
                return node->label.sdata;
        case TTOK_STRING:
                return selection_string(a, node);

        case NTOK_LOWER:
                return selection(a, node->children + 1);
        case NTOK_UPPER:
                return selection(a, node->children + 0);
        case NTOK_RULES:
                return selection_rules(a, node);
        case NTOK_FIZZBUZZ:
                return selection_fizzbuzz(a, node);
        }
        printf("selection: missing!\n");
        return epsilon;
}
