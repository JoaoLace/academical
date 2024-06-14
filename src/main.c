#include <stdio.h>
#include "include/lexer.h"
int main(int agrc, char *agrv[])
{
    lexer_T *lexer = init_lexer(
        "var name = \" Joao \";\n"
        "print(name);");

    token_T *token = (void *)0;

    while ((token = lexer_get_next_token(lexer)) != (void *)0)
    {
        printf("TOKEN(%d, %s )", token->type, token->value);
        printf("\n");
    }

    return 0;
}