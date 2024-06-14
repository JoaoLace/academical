#include <stdio.h>
#include <string.h>
#include "include/lexer.h"
#include "include/parser.h"
#include "include/visitor.h"
char *read_file(char *file_path)
{
    FILE *file = fopen(file_path, "r");
    if (file == NULL)
    {
        printf("ERROR: Could not open file '%s'\n", file_path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *file_content = (char *)malloc(file_size + 1);
    if (file_content == NULL)
    {
        printf("ERROR: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    fread(file_content, 1, file_size, file);
    fclose(file);

    file_content[file_size] = '\0';

    return file_content;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        printf("ERROR: No file provided\n");
        return 1;
    }

    lexer_T *lexer = init_lexer(read_file(argv[1]));

    parser_T *parser = init_parser(lexer);
    AST_T *root = parser_parse(parser);

    visitor_T *visitor = init_visitor();
    visitor_visit(visitor, root);

    return 0;
}