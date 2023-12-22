//
// Created by 杨钧博 on 2023/12/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EXPR_LEN_MAX    1024

enum _token_type
{
    NIL = 0,
    DIGIT = 1,
    ALPHABET = 2,
    OP = 3
};
typedef enum _token_type token_type_t;

struct _token {
    char lexeme[EXPR_LEN_MAX];
    token_type_t token_type;
};
typedef struct _token token_t;

struct _token_queue {
    token_t queue[EXPR_LEN_MAX];
    int head;
    int tail;
};
typedef struct _token_queue token_queue_t;

struct _token_stack {
    token_t stack[EXPR_LEN_MAX];
    int top;
};
typedef struct _token_stack token_stack_t;

const char VALID_DIGIT[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.'};
const char VALID_OP[] = {'+', '-', '*', '/', '%', '(', ')'};
const int VALID_DIGIT_LEN = 11;
const int VALID_OP_LEN = 7;

token_queue_t token_queue = {};
token_stack_t token_stack = {};
token_t cur_token = {};
token_t pre_token = {};

char infix_expr[EXPR_LEN_MAX] = "";
int expr_pos = 0;

void reset() {
    expr_pos = 0;
    memset(&cur_token, 0, sizeof(token_t));
    memset(&pre_token, 0, sizeof(token_t));
    memset(&token_queue, 0, sizeof(token_queue_t));
    memset(&token_stack, 0, sizeof(token_stack_t));
}

void set_cur_token(token_t * token, const char * lex, int lex_len, token_type_t token_type) {
    memset(token, 0, sizeof(token_t));
    strncpy(token->lexeme, lex, lex_len);
    token->token_type = token_type;
}

void en_token_queue(token_queue_t * q, token_t * t) {
    if (q->tail == EXPR_LEN_MAX && q->head == 0) return;
    q->tail %= EXPR_LEN_MAX;
    memcpy(&q->queue[q->tail ++], t, sizeof(token_t));
}

token_t * de_token_queue(token_queue_t * q) {
    if (q->head == q->tail) return NULL;
    q->head %= EXPR_LEN_MAX;
    return &q->queue[q->head ++];
}

void push_token_stack(token_stack_t * s, token_t * t) {
    if (s->top >= EXPR_LEN_MAX) return;
    if (s->top < 0) s->top = 0;
    memcpy(&s->stack[s->top ++], t, sizeof(token_t));
}

token_t * pop_token_stack(token_stack_t * s) {
    if (s->top <= 0) return NULL;
    if (s->top > EXPR_LEN_MAX) s->top = EXPR_LEN_MAX;
    return &s->stack[-- s->top];
}

token_t * peek_token_stack(token_stack_t * s) {
    if (s->top <= 0) return NULL;
    if (s->top > EXPR_LEN_MAX) s->top = EXPR_LEN_MAX;
    return &s->stack[s->top - 1];
}

int is_stack_empty(token_stack_t * s) {
    return s->top <= 0 ? 1 : 0;
}

token_type_t get_token_type(char ch) {
    int i;
    for (i = 0; i < VALID_DIGIT_LEN; i ++) {
        if (ch == VALID_DIGIT[i]) {
            return DIGIT;
        }
    }
    for (i = 0; i < VALID_OP_LEN; i ++) {
        if (ch == VALID_OP[i]) {
            return OP;
        }
    }
    return ALPHABET;
}

int operator_prior(char op) {
    switch (op) {
        case '*':
        case '/':
        case '%':
            return 2;
        case '+':
        case '-':
            return 1;
        case '(':
            return 0;
        default:
            return -1;
    }
}

int operator_prior_compare(char op1, char op2) {
    return operator_prior(op1) > operator_prior(op2) ? 1
        : (operator_prior(op1) == operator_prior(op2) ? 0 : -1);
}

token_t * scan() {
    char cur_char;
    int expr_pre_pos;

    cur_char = infix_expr[expr_pos];
    while ((cur_char == ' ' || cur_char == '\t' || cur_char == '\n') && expr_pos < EXPR_LEN_MAX) {
        cur_char = infix_expr[++ expr_pos];
    }

    if (expr_pos == EXPR_LEN_MAX || cur_char == NULL) {
        return NULL;
    }
    // Reserved current token to prevToken
    memcpy(&pre_token, &cur_token, sizeof(token_t));
    // If OP then return
    if (OP == get_token_type(cur_char)) {
        if ((cur_char == '-' || cur_char == '+')
            && (pre_token.token_type == NIL
                || !strcmp("(", pre_token.lexeme)
                || !strcmp("+", pre_token.lexeme)
                || !strcmp("-", pre_token.lexeme)
                || !strcmp("*", pre_token.lexeme)
                || !strcmp("/", pre_token.lexeme)
                || !strcmp("%", pre_token.lexeme))
        ) {
        } else {
            expr_pos ++;
            set_cur_token(&cur_token, &cur_char, 1, OP);
            return &cur_token;
        }
    }

    expr_pre_pos = expr_pos;
    do {
        cur_char = infix_expr[++ expr_pos];
    } while ((expr_pos < EXPR_LEN_MAX) && (cur_char != 0) && (DIGIT == get_token_type(cur_char))
           && (cur_char != ' ') && (cur_char != '\t') && (cur_char != '\n'));

    set_cur_token(&cur_token, &infix_expr[expr_pre_pos], expr_pos - expr_pre_pos, DIGIT);
    return &cur_token;
}

void parse() {
    while (scan() != NULL) {
        token_t *top;
        if (DIGIT == cur_token.token_type) {
            en_token_queue(&token_queue, &cur_token);
        } else if (OP == cur_token.token_type) {
            if (!strcmp("(", cur_token.lexeme)) {
                push_token_stack(&token_stack, &cur_token);
            } else if (!strcmp(")", cur_token.lexeme)) {
                while (!is_stack_empty(&token_stack)) {
                    top = pop_token_stack(&token_stack);
                    if (!strcmp("(", top->lexeme)) {
                        break;
                    }
                    en_token_queue(&token_queue, top);
                }
            } else {
                while (!is_stack_empty(&token_stack)) {
                    top = peek_token_stack(&token_stack);
                    if (operator_prior_compare(top->lexeme[0],
                                             cur_token.lexeme[0]) >= 0) {
                        en_token_queue(&token_queue, top);
                        pop_token_stack(&token_stack);
                    } else {
                        break;
                    }
                }
                push_token_stack(&token_stack, &cur_token);
            }
        }
    }
    //Deal remains
    while (!is_stack_empty(&token_stack)) {
        en_token_queue(&token_queue, pop_token_stack(&token_stack));
    }
}

token_t * calc_add(token_t * left, token_t * right) {
    char sum_str[EXPR_LEN_MAX] = {0};
    double left_num = strtod(left->lexeme, NULL);
    double right_num = strtod(right->lexeme, NULL);
    double sum = left_num + right_num;
    sprintf(sum_str, "%f", sum);
    set_cur_token(&cur_token, sum_str, strlen(sum_str), DIGIT);
    return &cur_token;
}

token_t * calc_minus(token_t * left, token_t * right) {
    char minus_str[EXPR_LEN_MAX] = {0};
    double left_num = strtod(left->lexeme, NULL);
    double right_num = strtod(right->lexeme, NULL);
    double minus = left_num - right_num;
    sprintf(minus_str, "%f", minus);
    set_cur_token(&cur_token, minus_str, strlen(minus_str), DIGIT);
    return &cur_token;
}

token_t * calc_mult(token_t * left, token_t * right) {
    char mult_str[EXPR_LEN_MAX] = {0};
    double left_num = strtod(left->lexeme, NULL);
    double right_num = strtod(right->lexeme, NULL);
    double mult = left_num * right_num;
    sprintf(mult_str, "%f", mult);
    set_cur_token(&cur_token, mult_str, strlen(mult_str), DIGIT);
    return &cur_token;
}

token_t * calc_div(token_t * left, token_t * right) {
    char div_str[EXPR_LEN_MAX] = {0};
    double left_num = strtod(left->lexeme, NULL);
    double right_num = strtod(right->lexeme, NULL);
    double div = left_num / right_num;
    sprintf(div_str, "%f", div);
    set_cur_token(&cur_token, div_str, strlen(div_str), DIGIT);
    return &cur_token;
}

token_t * calc_remaind(token_t * left, token_t * right) {
    char remaind_str[EXPR_LEN_MAX] = {0};
    double left_num = strtod(left->lexeme, NULL);
    double right_num = strtod(right->lexeme, NULL);
    double remaind = fmod(left_num, right_num);
    sprintf(remaind_str, "%f", remaind);
    set_cur_token(&cur_token, remaind_str, strlen(remaind_str), DIGIT);
    return &cur_token;
}

void do_calc() {
    token_t * t;
    memset(&token_stack, 0, sizeof(token_stack_t));
    while ((t = de_token_queue(&token_queue)) != NULL ) {
        if (t->token_type == DIGIT) {
            push_token_stack(&token_stack, t);
        } else if (t->token_type == OP) {
            token_t * right = pop_token_stack(&token_stack);
            token_t * left = pop_token_stack(&token_stack);
            switch (t->lexeme[0]) {
                case '+':
                    push_token_stack(&token_stack, calc_add(left, right));
                    break;
                case '-':
                    push_token_stack(&token_stack, calc_minus(left, right));
                    break;
                case '*':
                    push_token_stack(&token_stack, calc_mult(left, right));
                    break;
                case '/':
                    push_token_stack(&token_stack, calc_div(left, right));
                    break;
                case '%':
                    push_token_stack(&token_stack, calc_remaind(left, right));
                    break;
            }
        }
    }
}

void calc() {
    parse();
    do_calc();
}

void result() {
    token_t * t = peek_token_stack(&token_stack);
    if (t != NULL) printf("%s\n", t->lexeme);
}

int main() {
    while (strcmp("quit", infix_expr)) {
        reset();
        scanf("%s", infix_expr);
        printf("InfixExpress: %s\n", infix_expr);
        calc();
        result();
    }

    return 0;
}
