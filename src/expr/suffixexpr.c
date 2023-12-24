//
// Created by 杨钧博 on 2023/12/22.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define EXPR_LEN_MAX    1024

enum _token_type {
    NIL = 0,
    DIGIT = 1,
    ALPHABET = 2,
    OP = 3,
    BAD_TOKEN = 1000
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
const char VALID_OP[] = {'+', '-', '*', '/', '%', '(', ')', '^'};
const int VALID_DIGIT_LEN = 11;
const int VALID_OP_LEN = 8;

token_queue_t token_queue = {};
token_stack_t token_stack = {};
token_t cur_token = {};
token_t bad_token = {"", BAD_TOKEN};

char infix_expr[EXPR_LEN_MAX] = "";
int expr_pos = 0;

void reset() {
    expr_pos = 0;
    memset(&cur_token, 0, sizeof(token_t));
    memset(&token_queue, 0, sizeof(token_queue_t));
    memset(&token_stack, 0, sizeof(token_stack_t));
}

void set_cur_token(token_t * token, const char * lex, int lex_len, token_type_t token_type) {
    memset(token, 0, sizeof(token_t));
    strncpy(token->lexeme, lex, lex_len);
    token->token_type = token_type;
}

void token_enqueue(token_queue_t * q, token_t * t) {
    if (q->tail == EXPR_LEN_MAX && q->head == 0) return;
    q->tail %= EXPR_LEN_MAX;
    memcpy(&q->queue[q->tail ++], t, sizeof(token_t));
}

token_t * token_dequeue(token_queue_t * q) {
    if (q->head == q->tail) return NULL;
    q->head %= EXPR_LEN_MAX;
    return &q->queue[q->head ++];
}

void token_pushstack(token_stack_t * s, token_t * t) {
    if (s->top >= EXPR_LEN_MAX) return;
    if (s->top < 0) s->top = 0;
    memcpy(&s->stack[s->top ++], t, sizeof(token_t));
}

token_t * token_popstack(token_stack_t * s) {
    if (s->top <= 0) return NULL;
    if (s->top > EXPR_LEN_MAX) s->top = EXPR_LEN_MAX;
    return &s->stack[-- s->top];
}

token_t * token_peekstack(token_stack_t * s) {
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
        case '^':
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
    while ((cur_char == ' ' || cur_char == '\t' || cur_char == '\n')
        && expr_pos < EXPR_LEN_MAX) {
        cur_char = infix_expr[++ expr_pos];
    }

    if (expr_pos == EXPR_LEN_MAX || cur_char == NULL) {
        return NULL;
    }

    // If OP then return
    if (OP == get_token_type(cur_char)) {
        if ((cur_char == '-' || cur_char == '+')) {
            if (cur_token.token_type == NIL || !strcmp("(", cur_token.lexeme)) {
                set_cur_token(&cur_token, "0", 1, DIGIT);
                return &cur_token;
            }
            if (!strcmp("+", cur_token.lexeme) || !strcmp("-", cur_token.lexeme)
                || !strcmp("*", cur_token.lexeme) || !strcmp("/", cur_token.lexeme)
                || !strcmp("%", cur_token.lexeme) || !strcmp("^", cur_token.lexeme)) {
                sprintf(bad_token.lexeme, "Bad symbol at pos %d", expr_pos + 1);
                return &bad_token;
            }
        }
        expr_pos ++;
        set_cur_token(&cur_token, &cur_char, 1, OP);
        return &cur_token;
    }

    expr_pre_pos = expr_pos;
    do {
        cur_char = infix_expr[++ expr_pos];
    } while (expr_pos < EXPR_LEN_MAX
        && cur_char != 0 && DIGIT == get_token_type(cur_char)
        && (cur_char != ' ') && (cur_char != '\t') && (cur_char != '\n'));

    set_cur_token(&cur_token, &infix_expr[expr_pre_pos], expr_pos - expr_pre_pos, DIGIT);
    return &cur_token;
}

int parse() {
    token_t * s;
    while ((s = scan()) != NULL) {
        token_t *top;
        if (s->token_type == BAD_TOKEN) {
            return 1;
        }
        if (DIGIT == cur_token.token_type) {
            token_enqueue(&token_queue, &cur_token);
        } else if (OP == cur_token.token_type) {
            if (!strcmp("(", cur_token.lexeme)) {
                token_pushstack(&token_stack, &cur_token);
            } else if (!strcmp(")", cur_token.lexeme)) {
                while (!is_stack_empty(&token_stack)) {
                    top = token_popstack(&token_stack);
                    if (!strcmp("(", top->lexeme)) {
                        break;
                    }
                    token_enqueue(&token_queue, top);
                }
            } else {
                while (!is_stack_empty(&token_stack)) {
                    top = token_peekstack(&token_stack);
                    if (operator_prior_compare(top->lexeme[0],
                                             cur_token.lexeme[0]) >= 0) {
                        token_enqueue(&token_queue, top);
                        token_popstack(&token_stack);
                    } else {
                        break;
                    }
                }
                token_pushstack(&token_stack, &cur_token);
            }
        }
    }
    //Deal remains
    while (!is_stack_empty(&token_stack)) {
        token_enqueue(&token_queue, token_popstack(&token_stack));
    }
    return 0;
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

token_t * calc_power(token_t * left, token_t * right) {
    char res_str[EXPR_LEN_MAX] = {0};
    double left_num = strtod(left->lexeme, NULL);
    double right_num = strtod(right->lexeme, NULL);
    double res = pow(left_num, right_num);
    sprintf(res_str, "%f", res);
    set_cur_token(&cur_token, res_str, strlen(res_str), DIGIT);
    return &cur_token;
}

token_t * calc_oppos(token_t * origin) {
    char oppos_str[EXPR_LEN_MAX] = {0};
    double origin_num = strtod(origin->lexeme, NULL);
    sprintf(oppos_str, "%f", - origin_num);
    set_cur_token(&cur_token, oppos_str, strlen(oppos_str), DIGIT);
    return &cur_token;
}

void do_calc() {
    token_t * t;
    memset(&token_stack, 0, sizeof(token_stack_t));
    while ((t = token_dequeue(&token_queue)) != NULL ) {
        if (t->token_type == DIGIT) {
            token_pushstack(&token_stack, t);
        } else if (t->token_type == OP) {
            token_t * right = token_popstack(&token_stack);
            token_t * left = token_popstack(&token_stack);
            switch (t->lexeme[0]) {
                case '+':
                    token_pushstack(&token_stack, calc_add(left, right));
                    break;
                case '-':
                    token_pushstack(&token_stack, calc_minus(left, right));
                    break;
                case '*':
                    token_pushstack(&token_stack, calc_mult(left, right));
                    break;
                case '/':
                    token_pushstack(&token_stack, calc_div(left, right));
                    break;
                case '%':
                    token_pushstack(&token_stack, calc_remaind(left, right));
                    break;
                case '^':
                    token_pushstack(&token_stack, calc_power(left, right));
                    break;
            }
        }
    }
}

void post_exp() {
    printf("PostExpression: ");
    for (int i = token_queue.head; i < token_queue.tail; i ++) {
        printf("[%s] ", token_queue.queue[i].lexeme);
    }
    printf("\n");
}

void result() {
    token_t * t = token_peekstack(&token_stack);
    if (t != NULL) printf("%s\n", t->lexeme);
}

void error() {
    printf("ERROR: %s\n", bad_token.lexeme);
}

void calc() {
    if (!parse()) {
        post_exp();
        do_calc();
        result();
    } else {
        error();
    }
}

void test() {
    reset();
    strcpy(infix_expr, "-(((1+4)^2+5)-(9)^(-1/2))");
    calc();

    reset();
    strcpy(infix_expr, "(-1+3)/2-4.32-(-1)");
    calc();

    reset();
    strcpy(infix_expr, "5*((-2+5)+21/3+((8-(-2))*2.5))");
    calc();

    reset();
    strcpy(infix_expr, "0-(-1-2)+3*4");
    calc();
}
int main() {
    printf("Expression Evaluator 1.0\nBy YangJunbo(yangjunbo@360.cn) 12/22/23\n");
    while (1) {
        reset();
        printf(">>>");
        scanf("%s", infix_expr);
        if (!strcmp("quit", infix_expr)) break;
        if (!strcmp("test", infix_expr)) {
            test();
            continue;
        }
        calc();
    }

    return 0;
}
