//
// Created by 杨钧博 on 2023/12/22.
//
#include "expreval.h"
#include "operator.h"
#include "token.h"
#include <stdio.h>
#include <string.h>

#define VALID_DIGIT_LEN 11
const char VALID_DIGIT[] = {
        '0', '1', '2', '3', '4', '5',
        '6', '7', '8', '9', '.'
};

const func_tbl_entry_t op_func_tbl[FUNC_TBL_SIZE] = {
        {OP_NULL, NULL}, //PADDING
        {OP_ADD, calc_add},
        {OP_MINUS, calc_minus},
        {OP_MULT, calc_mult},
        {OP_DIV, calc_div},
        {OP_REMMAIND, calc_remaind},
        {OP_LBRAC, NULL},
        {OP_RBRAC, NULL},
        {OP_POWER, calc_power},
        /** 9-20 reserved for extending **/
        {OP_SIN, calc_sin},
        {OP_COS, calc_cos},
        {OP_TAN, calc_tan},
        {OP_CTAN, calc_ctan},
        /** 9-20 end **/
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_NULL, NULL}, //PADDING
        {OP_LOG, calc_log}
};

token_queue_t token_queue = {};
token_stack_t token_stack = {};
token_t cur_token = {};
token_t bad_token = {.token_type = OP_NULL, .op_code = BAD_TOKEN};

char infix_expr[EXPR_LEN_MAX] = "";
int expr_pos = 0;

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
    if (op_token_tbl[ch].op_code != 0) {
        return OP;
    } else {
        return ALPHABET;
    }
}

int operator_prior_compare(char op1, char op2) {
    return op_token_tbl[op1].op_prior > op_token_tbl[op2].op_prior ? 1
        : (op_token_tbl[op1].op_prior == op_token_tbl[op2].op_prior ? 0 : -1);
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
                set_token(&cur_token, "0", 1, DIGIT, OP_NULL, 0, 0);
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
        token_t * mapped = map_op_token_tbl(&cur_char);
        if (mapped == NULL) {
            sprintf(bad_token.lexeme, "Unknown operator '%c' at pos %d", cur_char, expr_pos + 1);
            return &bad_token;
        }
        set_token(&cur_token, mapped->lexeme, strlen(mapped->lexeme), OP,
                  mapped->op_code, mapped->op_prior, mapped->value);
        return &cur_token;
    }

    int alphabet = 0;
    expr_pre_pos = expr_pos;
    cur_char = infix_expr[expr_pos];

    if (get_token_type(cur_char) == ALPHABET) {
        alphabet = 1;
        do {
            cur_char = infix_expr[ ++ expr_pos];
        } while (expr_pos < EXPR_LEN_MAX
                 && cur_char != 0 && ALPHABET == get_token_type(cur_char)
                 && (cur_char != ' ') && (cur_char != '\t') && (cur_char != '\n'));
    } else {
        do {
            cur_char = infix_expr[++expr_pos];
        } while (expr_pos < EXPR_LEN_MAX
                 && cur_char != 0 && DIGIT == get_token_type(cur_char)
                 && (cur_char != ' ') && (cur_char != '\t') && (cur_char != '\n'));
    }
    set_token(&cur_token, &infix_expr[expr_pre_pos], expr_pos - expr_pre_pos,
                  alphabet == 1 ? ALPHABET : DIGIT, OP_NULL, 0, 0);

    return &cur_token;
}

int parse(int sp_bottom) {
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
                while (token_stack.top > sp_bottom) {
                    top = token_popstack(&token_stack);
                    if (!strcmp("(", top->lexeme)) {
                        break;
                    }
                    token_enqueue(&token_queue, top);
                }
            } else {
                while (token_stack.top > sp_bottom) {
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
        } else if (ALPHABET == cur_token.token_type) {
            token_t * mapped = map_op_token_tbl(cur_token.lexeme);
            if (mapped != NULL) {
                parse(token_stack.top);
                token_pushstack(&token_stack, mapped);
            }
        }
    }
    //deal remains
    while (/* !is_stack_empty(&token_stack) */ token_stack.top > sp_bottom) {
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
            op_func_t * op_func = op_func_tbl[t->op_code].op_func;
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
    init_op_token_tbl();
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
