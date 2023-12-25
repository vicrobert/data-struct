//
// Created by 杨钧博 on 2023/12/22.
//
#include "expreval.h"
#include "operator.h"
#include <stdio.h>
#include <string.h>

const char VALID_DIGIT[] = {
        '0', '1', '2', '3', '4', '5',
        '6', '7', '8', '9', '.'
};

#define FUNC_TBL_LEN 9
const func_tbl_entry_t func_tbl[FUNC_TBL_LEN] = {
        {0, NULL}, //PADDING
        {1, calc_add},
        {2, calc_minus},
        {3, calc_mult},
        {4, calc_div},
        {5, calc_remaind},
        {6, NULL},
        {7, NULL},
        {8, calc_power}
};

const char * VALID_FUNC[] = {
        "sin", "cos", "tan", "ctan", "log"
};
const int VALID_DIGIT_LEN = 11;

token_queue_t token_queue = {};
token_stack_t token_stack = {};
token_t cur_token = {};
token_t bad_token = {"", BAD_TOKEN};

char infix_expr[EXPR_LEN_MAX] = "";
int expr_pos = 0;

char op_code_tbl[256] = {};
void init_op_code_tbl() {
    op_code_tbl['+'] = 1;
    op_code_tbl['-'] = 2;
    op_code_tbl['*'] = 3;
    op_code_tbl['/'] = 4;
    op_code_tbl['%'] = 5;
    op_code_tbl['('] = 6;
    op_code_tbl[')'] = 7;
    op_code_tbl['^'] = 8;
}

void reset() {
    expr_pos = 0;
    memset(&cur_token, 0, sizeof(token_t));
    memset(&token_queue, 0, sizeof(token_queue_t));
    memset(&token_stack, 0, sizeof(token_stack_t));
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
    if (op_code_tbl[ch] != 0) {
        return OP;
    } else {
        return ALPHABET;
    }
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

int parse(int stack_top) {
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
    //deal remains
    while (/* !is_stack_empty(&token_stack) */ token_stack.top > stack_top) {
        token_enqueue(&token_queue, token_popstack(&token_stack));
    }
    return 0;
}

int do_calc() {
    token_t * t;
    memset(&token_stack, 0, sizeof(token_stack_t));
    while ((t = token_dequeue(&token_queue)) != NULL ) {
        if (t->token_type == DIGIT) {
            token_pushstack(&token_stack, t);
        } else if (t->token_type == OP) {
            token_t * right = token_popstack(&token_stack);
            token_t * left = token_popstack(&token_stack);
            op_func_t * op_func = func_tbl[op_code_tbl[t->lexeme[0]]].op_func;
            if (op_func != NULL) {
                token_pushstack(&token_stack, op_func(&cur_token, left, right));
            } else {
                sprintf(bad_token.lexeme, "Unknown operator '%c'", t->lexeme[0]);
                return 1;
            }
        }
    }
    return 0;
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
    if (!parse(0)) {
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
    init_op_code_tbl();
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
