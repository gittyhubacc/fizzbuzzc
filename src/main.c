#include "lexer.h"
#include "parser.h"
#include "selector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct program_input {
        string program;
} program_input;

typedef struct program_output {
        int bytes_used;
        string program;
} program_output;

program_input read_input(int argc, char **argv)
{
        program_input input;
        input.program = (string){.addr = argv[1], .len = strlen(argv[1])};
        return input;
}

int write_output(int argc, char **argv, program_output output)
{
        fprintf(stdout, "%.*s\n", output.program.len, output.program.addr);
        return 0;
}

int main(int argc, char **argv)
{
        if (argc < 2) {
                printf("USAGE: %s [fizzbuzz program]\n", argv[0]);
                return 1;
        }

        char *mem = malloc(4 * mebibyte);
        arena root = make_arena_ptr(mem, mebibyte);
        lex_init(&root);

        program_input input = read_input(argc, argv);
        token_string tokens = lex(&root, input.program);
        parse_tree *tree = parse(&root, tokens);
        string program = selection(&root, tree);
        unsigned int used = bytes_used(root);
        program_output output = {
            .program = program,
            .bytes_used = used,
        };

        return write_output(argc, argv, output);
}
