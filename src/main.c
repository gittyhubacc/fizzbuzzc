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
        string path = (string){.addr = argv[2], .len = strlen(argv[2])};
        FILE *f = fopen(path.addr, "w");
        fwrite(output.program.addr, 1, output.program.len, f);
        fclose(f);
        printf("bytes used: %ikib (%i)\n", output.bytes_used / kibibyte,
               output.bytes_used);
        return 0;
}

int main(int argc, char **argv)
{
        if (argc < 2) {
                printf("USAGE: %s [fzzbzz program]\n", argv[0]);
                return 1;
        }

        int root_sz = 64 * mebibyte;
        char *mem = malloc(root_sz);
        arena root = make_arena_ptr(mem, root_sz);
        lex_init(&root);

        program_input input = read_input(argc, argv);
        token_string tokens = lex(&root, input.program);
        parse_tree *tree = parse(&root, tokens);
        if (tree == NULL) {
                fprintf(stderr, "failed to parse\n");
                return 1;
        }
        string program = selection(&root, tree);
        unsigned int used = bytes_used(root);
        program_output output = {
            .program = program,
            .bytes_used = used,
        };

        return write_output(argc, argv, output);
}
