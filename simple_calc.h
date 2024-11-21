#pragma once

#ifndef SIMPLE_CALCULATOR_H_
#define SIMPLE_CALCULATOR_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#ifndef NUM_TYPE
    #define NUM_TYPE double
#endif // NUM_TYPE

NUM_TYPE scalc_calculate(const char *text);

#endif // SIMPLE_CALCULATOR_H_

#ifdef SIMPLE_CALC_IMPLEMENTATION

#define DEF(return_type, func_name, ...) return_type simple_calculator_##func_name(__VA_ARGS__)
#define CAL(func_name, ...) simple_calculator_##func_name(__VA_ARGS__)
#define TOK(name) simple_calculator_token_type_##name
#define T(name) simple_calculator_##name
#define UNUSED(item) (void)(item)
    
#define list_define(name, type) \
    typedef struct {\
        T(type) *items;\
        size_t count;\
        size_t capacity;\
    } T(name)\

#define list_append(list, item)\
    do {\
        if ((list).count >= (list).capacity) {\
            (list).capacity = (list).capacity < 64 ? 64 : (list).capacity * 2;\
            (list).items = realloc((list).items, sizeof(*(list).items) * (list).capacity);\
        }\
        (list).items[(list).count] = item;\
        (list).count += 1;\
    } while (0)

#define list_delete(list) \
    do {\
        free((list).items);\
        (list).items = NULL;\
        (list).count = 0;\
        (list).capacity = 0;\
    } while (0)

typedef enum {
    TOK(NUM),
    TOK(PLUS),
    TOK(HYPHEN),
    TOK(STAR),
    TOK(SLASH),
    TOK(CARRET),
    TOK(LPAREN),
    TOK(RPAREN),
    TOK(SYMBOL),
    TOK(END),
    TOK(ERROR),
    TOK(COUNT),
} T(token_type);

typedef struct {
    const char *begin;
    const char *end;
    T(token_type) type;
} T(token);

#define TOKEN_LEN(token) ((int)((token).end - (token).begin + 1))

list_define(token_list, token);

typedef struct {
    const char *text;
    size_t current;
} T(lexer);

DEF(T(lexer), lexer_new, const char *text) 
{
    return (T(lexer)) {
        .text = text,
        .current = 0,
    };
}

#define is_alpha(c) (\
    (c >= 'a' && c <= 'z') || \
    (c >= 'A' && c <= 'Z') || \
    (c == '_')\
)
#define is_num(c) (c >= '0' && c <= '9')
#define is_alnum(c) (is_alpha(c) || is_num(c))
#define is_space(c) (c == ' ' || c == '\r' || c == '\t' || c == '\n')

DEF(char, lexer_peek, T(lexer) *lexer)
{
    return lexer->text[lexer->current];
}

DEF(char, lexer_consume, T(lexer) *lexer) 
{
    return lexer->text[lexer->current++];
}

DEF(T(token), tokenize_num, T(lexer) *lexer)
{
    T(token) token = {0};
    token.begin = &lexer->text[lexer->current];

    while (is_num((CAL(lexer_peek, lexer))) && CAL(lexer_peek, lexer) != '\0') {
        CAL(lexer_consume, lexer);
    }

    if (CAL(lexer_peek, lexer) == '.') {
        CAL(lexer_consume, lexer);
        while (is_num((CAL(lexer_peek, lexer))) && CAL(lexer_peek, lexer) != '\0') {
            CAL(lexer_consume, lexer);
        }
    }

    token.end = &lexer->text[lexer->current - 1];
    token.type = TOK(NUM);

    return token;
}

DEF(T(token), tokenize_symbol, T(lexer) *lexer)
{
    T(token) token = {0};
    token.begin = &lexer->text[lexer->current];

    while (is_alnum(CAL(lexer_peek, lexer))) {
        CAL(lexer_consume, lexer);
    }

    token.end = &lexer->text[lexer->current - 1];
    token.type = TOK(SYMBOL);

    return token;
}

DEF(T(token), tokenize_operator, T(lexer) *lexer)
{
    T(token) token = {0};
    token.begin = &lexer->text[lexer->current];

    switch (CAL(lexer_peek, lexer)) {
        case '+':
            CAL(lexer_consume, lexer);
            token.type = TOK(PLUS);
            break;
        case '-':
            CAL(lexer_consume, lexer);
            token.type = TOK(HYPHEN);
            break;
        case '*':
            CAL(lexer_consume, lexer);
            token.type = TOK(STAR);
            break;
        case '/':
            CAL(lexer_consume, lexer);
            token.type = TOK(SLASH);
            break;
        case '^':
            CAL(lexer_consume, lexer);
            token.type = TOK(CARRET);
            break;
        case '(':
            CAL(lexer_consume, lexer);
            token.type = TOK(LPAREN);
            break;
        case ')':
            CAL(lexer_consume, lexer);
            token.type = TOK(RPAREN);
            break;
        default:
            token.type = TOK(ERROR);
    }

    token.end = &lexer->text[lexer->current - 1];

    return token;
}

DEF(void, trim_left, T(lexer) *lexer)
{
    while (is_space(CAL(lexer_peek, lexer))) {
        CAL(lexer_consume, lexer);
    }
}

DEF(T(token), lexer_next, T(lexer) *lexer)
{
    CAL(trim_left, lexer);
    char c = CAL(lexer_peek, lexer);

    if (c == '\0') {
        CAL(lexer_consume, lexer);
        return (T(token)) {
            .begin = &lexer->text[lexer->current - 1],
            .end = &lexer->text[lexer->current - 1],
            .type = TOK(END),
        };
    }
    else if (is_num(c)) {
        return CAL(tokenize_num, lexer);
    }
    else if (is_alpha(c)) {
        return CAL(tokenize_symbol, lexer);
    }
    else {
        return CAL(tokenize_operator, lexer);
    }
}

DEF(bool, tokenize, const char* text, T(token_list) *token_list) 
{
    T(lexer) lexer = CAL(lexer_new, text);
    T(token) token = {0};

    while ((token = CAL(lexer_next, &lexer)).type != TOK(END)) {
        list_append(*token_list, token);

        if (token.type == TOK(ERROR)) return false;
    }

    return true;
}

typedef struct {
    T(token_list) *tokens;
    size_t current;
    NUM_TYPE ans;
    bool error;
} T(parser);

DEF(T(parser), parser_new, T(token_list) *token_list)
{
    return (T(parser)) {
        .tokens = token_list,
        .current = 0,
        .ans = 0,
        .error = false,
    };
}

DEF(T(token), parser_consume, T(parser) *parser)
{
    return parser->tokens->items[parser->current++];
}

DEF(T(token), parser_peek, T(parser) *parser)
{
    return parser->tokens->items[parser->current];
}

DEF(T(token), parser_prev, T(parser) *parser)
{
    return parser->tokens->items[parser->current - 1];
}

typedef enum {
    TOK(PREC_NONE),
    TOK(PREC_ADDSUB),
    TOK(PREC_MULDIV),
    TOK(PREC_POW),
    TOK(PREC_UNARY),
} T(precedence);

typedef NUM_TYPE (*T(parse_fn))(T(parser)*);

typedef struct {
    T(parse_fn) prefix;
    T(parse_fn) infix;
    int lbp;
} T(parse_rule);

DEF(NUM_TYPE, num, T(parser) *parser);
DEF(NUM_TYPE, binary, T(parser) *parser);
DEF(NUM_TYPE, unary, T(parser) *parser);
DEF(NUM_TYPE, grouping, T(parser) *parser);
DEF(NUM_TYPE, identifier, T(parser) *parser);

static const T(parse_rule) T(rules)[TOK(COUNT)] = {
    {T(num), NULL, TOK(PREC_NONE)},
    {NULL, T(binary), TOK(PREC_ADDSUB)},
    {T(unary), T(binary), TOK(PREC_ADDSUB)},
    {NULL, T(binary), TOK(PREC_MULDIV)},
    {NULL, T(binary), TOK(PREC_MULDIV)},
    {NULL, T(binary), TOK(PREC_POW)},
    {T(grouping), NULL, TOK(PREC_NONE)},
    {NULL, NULL, TOK(PREC_NONE)},
    {T(identifier), NULL, TOK(PREC_NONE)},
    {NULL, NULL, TOK(PREC_NONE)},
    {NULL, NULL, TOK(PREC_NONE)},
};

DEF(T(parse_rule), get_rule, T(token) token)
{
    return T(rules)[token.type];
}

DEF(NUM_TYPE, expression, T(parser) *parser, T(precedence) prec)
{
    if (parser->error) return 0;

    T(token) token = CAL(parser_consume, parser);
    T(parse_rule) rule = CAL(get_rule, token);

    if (rule.prefix == NULL) {
        fprintf(stderr, "ERROR: token '%.*s' shouldn't be here\n", TOKEN_LEN(token), token.begin);
        parser->error = true;
        return 0;
    }

    NUM_TYPE left = rule.prefix(parser);

    while (parser->current < parser->tokens->count && (int)prec < CAL(get_rule, CAL(parser_peek, parser)).lbp) {
        if (parser->current >= parser->tokens->count) {
            return left;
        }

        token = CAL(parser_consume, parser);

        rule = CAL(get_rule, token);
        NUM_TYPE right = rule.infix(parser);

        switch (token.type) {
            case TOK(PLUS) :  left = left + right; break;
            case TOK(HYPHEN): left = left - right; break;
            case TOK(STAR)  : left = left * right; break;
            case TOK(SLASH) : left = left / right; break;
            case TOK(CARRET): left = pow(left, right); break;
            default:
                fprintf(stderr, "ERROR: Unknown token type: '%.*s'\n", TOKEN_LEN(token), token.begin);
                parser->error = true;
                return 0;
        }
    }

    return left;
}

DEF(NUM_TYPE, num, T(parser) *parser)
{
    T(token) token = CAL(parser_prev, parser);
    char *endptr;
    static char temp[100];
    sprintf(temp, "%.*s", TOKEN_LEN(token), token.begin);

    return strtod(temp, &endptr);
}

DEF(NUM_TYPE, binary, T(parser) *parser)
{
    T(token) token = CAL(parser_prev, parser);
    T(parse_rule) rule = CAL(get_rule, token);

    return CAL(expression, parser, (T(precedence))(rule.lbp));
}

DEF(NUM_TYPE, unary, T(parser) *parser)
{
    T(token) token = CAL(parser_prev, parser);
    T(parse_rule) rule = CAL(get_rule, token);

    return -CAL(expression, parser, (T(precedence))(rule.lbp));
}

DEF(NUM_TYPE, grouping, T(parser) *parser)
{
    T(token) token = CAL(parser_prev, parser);
    T(parse_rule) rule = CAL(get_rule, token);
    NUM_TYPE num = CAL(expression, parser, (T(precedence))(rule.lbp));

    if (CAL(parser_consume, parser).type != TOK(RPAREN)) {
        if (parser->error) return 0;

        token = CAL(parser_prev, parser);
        fprintf(stderr, "ERROR: expected ')' but got '%.*s'\n", TOKEN_LEN(token), token.begin); 
        parser->error = true;
        return 0;
    }

    return num;
}

DEF(NUM_TYPE, identifier, T(parser) *parser)
{
    UNUSED(parser);
    return 0;
}

DEF(NUM_TYPE, parse, T(token_list) *token_list)
{
    T(parser) parser = CAL(parser_new, token_list);
    NUM_TYPE result = CAL(expression, &parser, TOK(PREC_NONE));

    if (parser.error) {
        fprintf(stderr, "ERROR: Parsing failed at '%s'\n", token_list->items[parser.current].begin);
        return 0;
    }

    return result;
}

NUM_TYPE scalc_calculate(const char *text)
{
    if (text == NULL) return 0;

    T(token_list) token_list = {0};
    if (!CAL(tokenize, text, &token_list)) {
        fprintf(stderr, "ERROR: Tokenization failed at '%s'\n", token_list.items[token_list.count - 1].begin);
        list_delete(token_list);
        return 0;
    }
    NUM_TYPE result = CAL(parse, &token_list);
    list_delete(token_list);

    return result;
}

#undef DEF
#undef CAL
#undef TOK
#undef T
#undef UNUSED
#undef TOKEN_LEN
#undef list_define
#undef list_append
#undef list_delete
#undef is_alpha
#undef is_num
#undef is_alnum
#undef is_space

#endif // SIMPLE_CALC_IMPLEMENTATION
