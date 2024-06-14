#include <stdio.h>
#include "include/lexer.h"
#include "include/parser.h"
int main(int agrc, char *agrv[])
{
    lexer_T *lexer = init_lexer(
        "var name = \" Joao \";\n"
        "print(name);");

    parser_T *parser = init_parser(lexer);
    AST_T *root = parser_parse(parser);

    printf("%d\n", root->type);
    printf("%ld\n", root->compound_size);
    return 0;
}