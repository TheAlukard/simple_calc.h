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

NUM_TYPE sc_calculate(const char *text, int len); // -1 for null terminated string

#endif // SIMPLE_CALCULATOR_H_

#ifdef SIMPLE_CALC_IMPLEMENTATION

#define FUN(return_type, func_name, ...) return_type simple_calculator_##func_name(__VA_ARGS__)
#define CAL(func_name, ...) simple_calculator_##func_name(__VA_ARGS__)
#define ENUM(name) simple_calculator_enum_type_##name
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
    ENUM(NUM),
    ENUM(PLUS),
    ENUM(HYPHEN),
    ENUM(STAR),
    ENUM(SLASH),
    ENUM(CARRET),
    ENUM(LPAREN),
    ENUM(RPAREN),
    ENUM(SYMBOL),
    ENUM(END),
    ENUM(ERROR),
    ENUM(COUNT),
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
    size_t text_len;
    size_t current;
} T(lexer);

FUN(T(lexer), lexer_new, const char *text, size_t len) 
{
    return (T(lexer)) {
        .text = text,
        .text_len = len,
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

FUN(char, lexer_peek, T(lexer) *lexer)
{
    return lexer->text[lexer->current];
}

FUN(char, lexer_consume, T(lexer) *lexer) 
{
    return lexer->text[lexer->current++];
}

FUN(T(token), tokenize_num, T(lexer) *lexer)
{
    T(token) token = {0};
    token.begin = &lexer->text[lexer->current];

    while (lexer->current < lexer->text_len && is_num((CAL(lexer_peek, lexer)))) {
        CAL(lexer_consume, lexer);
    }

    if (lexer->current < lexer->text_len && CAL(lexer_peek, lexer) == '.') {
        CAL(lexer_consume, lexer);
        while (lexer->current < lexer->text_len && is_num((CAL(lexer_peek, lexer)))) {
            CAL(lexer_consume, lexer);
        }
    }

    token.end = &lexer->text[lexer->current - 1];
    token.type = ENUM(NUM);

    return token;
}

FUN(T(token), tokenize_symbol, T(lexer) *lexer)
{
    T(token) token = {0};
    token.begin = &lexer->text[lexer->current];

    while (lexer->current < lexer->text_len && is_alnum(CAL(lexer_peek, lexer))) {
        CAL(lexer_consume, lexer);
    }

    token.end = &lexer->text[lexer->current - 1];
    token.type = ENUM(SYMBOL);

    return token;
}

FUN(T(token), tokenize_operator, T(lexer) *lexer)
{
    T(token) token = {0};
    token.type = ENUM(END);

    if (lexer->current >= lexer->text_len) return token;

    token.begin = &lexer->text[lexer->current];

    switch (CAL(lexer_peek, lexer)) {
        case '+':
            CAL(lexer_consume, lexer);
            token.type = ENUM(PLUS);
            break;
        case '-':
            CAL(lexer_consume, lexer);
            token.type = ENUM(HYPHEN);
            break;
        case '*':
            CAL(lexer_consume, lexer);
            token.type = ENUM(STAR);
            break;
        case '/':
            CAL(lexer_consume, lexer);
            token.type = ENUM(SLASH);
            break;
        case '^':
            CAL(lexer_consume, lexer);
            token.type = ENUM(CARRET);
            break;
        case '(':
            CAL(lexer_consume, lexer);
            token.type = ENUM(LPAREN);
            break;
        case ')':
            CAL(lexer_consume, lexer);
            token.type = ENUM(RPAREN);
            break;
        default:
            token.type = ENUM(ERROR);
    }

    token.end = &lexer->text[lexer->current - 1];

    return token;
}

FUN(void, trim_left, T(lexer) *lexer)
{
    while (lexer->current < lexer->text_len && is_space(CAL(lexer_peek, lexer))) {
        CAL(lexer_consume, lexer);
    }
}

FUN(T(token), lexer_next, T(lexer) *lexer)
{
    CAL(trim_left, lexer);

    if (lexer->current >= lexer->text_len) {
        return (T(token)) {
            .begin = NULL,
            .end = NULL,
            .type = ENUM(END),
        };
    }

    char c = CAL(lexer_peek, lexer);

    if (is_num(c)) {
        return CAL(tokenize_num, lexer);
    }
    else if (is_alpha(c)) {
        return CAL(tokenize_symbol, lexer);
    }
    else {
        return CAL(tokenize_operator, lexer);
    }
}

FUN(bool, tokenize, T(lexer) *lexer, T(token_list) *token_list)
{
    T(token) token = {0};

    while ((token = CAL(lexer_next, lexer)).type != ENUM(END)) {
        list_append(*token_list, token);

        if (token.type == ENUM(ERROR)) return false;
    }

    list_append(*token_list, token);

    return true;
}

typedef struct {
    T(token_list) *tokens;
    size_t current;
    NUM_TYPE ans;
    bool error;
} T(parser);

FUN(T(parser), parser_new, T(token_list) *token_list)
{
    return (T(parser)) {
        .tokens = token_list,
        .current = 0,
        .ans = 0,
        .error = false,
    };
}

FUN(T(token), parser_consume, T(parser) *parser)
{
    return parser->tokens->items[parser->current++];
}

FUN(T(token), parser_peek, T(parser) *parser)
{
    return parser->tokens->items[parser->current];
}

FUN(T(token), parser_prev, T(parser) *parser)
{
    return parser->tokens->items[parser->current - 1];
}

typedef enum {
    ENUM(PREC_NONE),
    ENUM(PREC_ADDSUB),
    ENUM(PREC_MULDIV),
    ENUM(PREC_POW),
    ENUM(PREC_UNARY),
} T(precedence);

typedef NUM_TYPE (*T(parse_fn))(T(parser)*);

typedef struct {
    T(parse_fn) prefix;
    T(parse_fn) infix;
    int lbp;
} T(parse_rule);

FUN(NUM_TYPE, num, T(parser) *parser);
FUN(NUM_TYPE, binary, T(parser) *parser);
FUN(NUM_TYPE, unary, T(parser) *parser);
FUN(NUM_TYPE, grouping, T(parser) *parser);
FUN(NUM_TYPE, identifier, T(parser) *parser);

static const T(parse_rule) T(rules)[ENUM(COUNT)] = {
    {T(num), NULL, ENUM(PREC_NONE)},
    {NULL, T(binary), ENUM(PREC_ADDSUB)},
    {T(unary), T(binary), ENUM(PREC_ADDSUB)},
    {NULL, T(binary), ENUM(PREC_MULDIV)},
    {NULL, T(binary), ENUM(PREC_MULDIV)},
    {NULL, T(binary), ENUM(PREC_POW)},
    {T(grouping), NULL, ENUM(PREC_NONE)},
    {NULL, NULL, ENUM(PREC_NONE)},
    {T(identifier), NULL, ENUM(PREC_NONE)},
    {NULL, NULL, ENUM(PREC_NONE)},
    {NULL, NULL, ENUM(PREC_NONE)},
};

FUN(T(parse_rule), get_rule, T(token) token)
{
    return T(rules)[token.type];
}

FUN(NUM_TYPE, expression, T(parser) *parser, T(precedence) prec)
{
    if (parser->error) return 0;

    T(token) token = CAL(parser_consume, parser);

    if (token.type == ENUM(END)) {
        fprintf(stderr, "ERROR: Expression isn't complete\n");
        parser->error = true;
        return 0;
    }

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
            case ENUM(PLUS) :  left = left + right; break;
            case ENUM(HYPHEN): left = left - right; break;
            case ENUM(STAR)  : left = left * right; break;
            case ENUM(SLASH) : left = left / right; break;
            case ENUM(CARRET): left = pow(left, right); break;
            default:
                fprintf(stderr, "ERROR: Unknown token type: '%.*s'\n", TOKEN_LEN(token), token.begin);
                parser->error = true;
                return 0;
        }
    }

    return left;
}

FUN(NUM_TYPE, num, T(parser) *parser)
{
    T(token) token = CAL(parser_prev, parser);
    char *endptr;
    static char temp[100];
    sprintf(temp, "%.*s", TOKEN_LEN(token), token.begin);

    return strtod(temp, &endptr);
}

FUN(NUM_TYPE, binary, T(parser) *parser)
{
    T(token) token = CAL(parser_prev, parser);
    T(parse_rule) rule = CAL(get_rule, token);

    return CAL(expression, parser, (T(precedence))(rule.lbp));
}

FUN(NUM_TYPE, unary, T(parser) *parser)
{
    T(token) token = CAL(parser_prev, parser);
    T(parse_rule) rule = CAL(get_rule, token);

    return -CAL(expression, parser, (T(precedence))(rule.lbp));
}

FUN(NUM_TYPE, grouping, T(parser) *parser)
{
    T(token) token = CAL(parser_prev, parser);
    T(parse_rule) rule = CAL(get_rule, token);
    NUM_TYPE num = CAL(expression, parser, (T(precedence))(rule.lbp));

    if (CAL(parser_consume, parser).type != ENUM(RPAREN)) {
        if (parser->error) return 0;

        token = CAL(parser_prev, parser);
        fprintf(stderr, "ERROR: expected ')' but got '%.*s'\n", TOKEN_LEN(token), token.begin); 
        parser->error = true;
        return 0;
    }

    return num;
}

FUN(NUM_TYPE, identifier, T(parser) *parser)
{
    UNUSED(parser);
    return 0;
}

FUN(NUM_TYPE, parse, T(token_list) *token_list)
{
    if (token_list->count <= 1) return 0;

    T(parser) parser = CAL(parser_new, token_list);
    NUM_TYPE result = CAL(expression, &parser, ENUM(PREC_NONE));

    if (parser.error) {
        fprintf(stderr, "ERROR: Parsing failed at '%s'\n", token_list->items[parser.current].begin);
        return 0;
    }

    return result;
}

NUM_TYPE sc_calculate(const char *text, int len)
{
    if (text == NULL || len == 0) return 0;

    T(token_list) token_list = {0};
    T(lexer) lexer = CAL(lexer_new, text, len < 0 ? strlen(text) : len);

    if (!CAL(tokenize, &lexer, &token_list)) {
        fprintf(stderr, "ERROR: Tokenization failed\n");
        list_delete(token_list);
        return 0;
    }

    NUM_TYPE result = CAL(parse, &token_list);
    list_delete(token_list);

    return result;
}

#undef FUN
#undef CAL
#undef ENUM
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
