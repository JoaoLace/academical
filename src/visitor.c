#include "include/visitor.h"
#include <stdio.h>
#include <string.h>

static AST_T *print_new_line()
{
    printf("\n");

    return init_ast(AST_NOOP);
}

static AST_T *builtin_function_print(visitor_T *visitor, AST_T **args, int args_size)
{
    for (int i = 0; i < args_size; i++)
    {
        AST_T *visited_ast = visitor_visit(visitor, args[i]);

        switch (visited_ast->type)
        {
        case AST_STRING:
            printf("%s", visited_ast->string_value);
            break;
        default:
            printf("%p\n", visited_ast);
            break;
        }
    }
    return init_ast(AST_NOOP);
}

visitor_T *init_visitor()
{
    visitor_T *visitor = calloc(1, sizeof(struct VISITOR_STRUCT));
    visitor->variable_definitions = (void *)0;
    visitor->variable_definitions_size = 0;

    return visitor;
}

AST_T *visitor_visit(visitor_T *visitor, AST_T *node)
{
    switch (node->type)
    {
    case AST_VARIABLE_DEFINITION:
        return visitor_visit_variable_definition(visitor, node);
        break;
    case AST_VARIABLE:
        return visitor_visit_variable(visitor, node);
        break;
    case AST_FUNCTION_CALL:
        return visitor_visit_function_call(visitor, node);
        break;
    case AST_STRING:
        return visitor_visit_string(visitor, node);
        break;
    case AST_COMPOUND:
        return visitor_visit_compound(visitor, node);
        break;
    case AST_NOOP:
        return node;
        break;
    }
    printf("ERROR: Uncaught statement of type: %d\n", node->type);
    exit(1);
}

AST_T *visitor_visit_variable_definition(visitor_T *visitor, AST_T *node)
{
    if (visitor->variable_definitions == (void *)0)
    {
        visitor->variable_definitions = calloc(1, sizeof(struct AST_STRUCT *));
        visitor->variable_definitions[0] = node;
        visitor->variable_definitions_size++;
    }
    else
    {
        visitor->variable_definitions_size++;
        visitor->variable_definitions = realloc(
            visitor->variable_definitions,
            visitor->variable_definitions_size * sizeof(struct AST_STRUCT *));
        visitor->variable_definitions[visitor->variable_definitions_size - 1] = node;
    }

    return node;
}

AST_T *visitor_visit_variable(visitor_T *visitor, AST_T *node)
{
    for (int i = 0; i < visitor->variable_definitions_size; i++)
    {
        AST_T *vardef = visitor->variable_definitions[i];

        if (strcmp(vardef->variable_definition_variable_name, node->variable_name) == 0)
        {
            return visitor_visit(visitor, vardef->variable_definition_value);
        }
    }
    printf("ERROR: Undefined variable '%s'\n ", node->variable_name);
    exit(1);
}

AST_T *visitor_visit_string(visitor_T *visitor, AST_T *node)
{
    return node;
}

AST_T *visitor_visit_function_call(visitor_T *visitor, AST_T *node)
{
    if (strcmp(node->function_call_name, "print") == 0)
    {
        return builtin_function_print(visitor, node->function_call_arguments, node->function_call_arguments_size);
    }
    else if (strcmp(node->function_call_name, "printnl") == 0)
    {
        return print_new_line();
    }
    printf("ERROR: Undefined method '%s' \n", node->function_call_name);
    exit(1);
}

AST_T *visitor_visit_compound(visitor_T *visitor, AST_T *node) // all source code
{
    for (int i = 0; i < node->compound_size; i++) // loop through every children of compound
    {
        visitor_visit(visitor, node->compound_value[i]);
    }

    return init_ast(AST_NOOP);
}
