//
// Created by 杨钧博 on 2023/12/22.
//
#include "expreval.h"
#include "token.h"
#include "result.h"
#include <stdio.h>
#include <string.h>

#define VALID_DIGIT_LEN 11
const char VALID_DIGIT[] = {
        '0', '1', '2', '3', '4', '5',
        '6', '7', '8', '9', '.'
};

token_queue_t token_queue = {};
token_stack_t token_stack = {};
token_t cur_token = {};

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

result_t * scan() {
    char cur_char;
    int expr_pre_pos;

    cur_char = infix_expr[expr_pos];
    while ((cur_char == ' ' || cur_char == '\t' || cur_char == '\n')
        && expr_pos < EXPR_LEN_MAX) {
        cur_char = infix_expr[++ expr_pos];
    }

    if (expr_pos == EXPR_LEN_MAX || cur_char == '\0') {
        return success(0);
    }

    // If OP then return
    if (get_token_type(cur_char) & OP) {
        if ((cur_char == '-' || cur_char == '+')) {
            if (cur_token.token_type == NIL || !strcmp("(", cur_token.lexeme)) {
                set_token(&cur_token, "0", 1, DIGIT, OP_NULL, 0);
                return success(&cur_token);
            }
            if (!strcmp("+", cur_token.lexeme) || !strcmp("-", cur_token.lexeme)
                || !strcmp("*", cur_token.lexeme) || !strcmp("/", cur_token.lexeme)
                || !strcmp("%", cur_token.lexeme) || !strcmp("^", cur_token.lexeme)) {
                return error_seq(UNREC_SYMBOL_ERR, cur_token.lexeme, expr_pos + 1);
            }
        }
        expr_pos ++;
        token_t * mapped = map_op_token_tbl(&cur_char);
        if (mapped == NULL) {
            return error_ch(UNREC_SYMBOL_ERR, cur_char, expr_pos + 1);
        }
        set_token(&cur_token, mapped->lexeme, strlen(mapped->lexeme), mapped->token_type,
                  mapped->op_code, mapped->op_prior);
        return success(&cur_token);
    }

    expr_pre_pos = expr_pos;
    cur_char = infix_expr[expr_pos];

    if (get_token_type(cur_char) == ALPHABET) {
        do {
            cur_char = infix_expr[ ++ expr_pos];
        } while (expr_pos < EXPR_LEN_MAX
                 && cur_char != 0 && ALPHABET == get_token_type(cur_char)
                 && (cur_char != ' ') && (cur_char != '\t') && (cur_char != '\n'));

        set_token(&cur_token, &infix_expr[expr_pre_pos], expr_pos - expr_pre_pos,
                  ALPHABET, OP_NULL, 0);
    } else {
        int dp = 0;
        do {
            cur_char = infix_expr[++expr_pos];
            if (cur_char == '.') dp ++;
            if (dp > 1)
                return error_ch(ILLEGAL_NUM_ERR, cur_char, expr_pos + 1);
        } while (expr_pos < EXPR_LEN_MAX
                 && cur_char != 0 && DIGIT == get_token_type(cur_char)
                 && (cur_char != ' ') && (cur_char != '\t') && (cur_char != '\n'));

        set_token(&cur_token, &infix_expr[expr_pre_pos], expr_pos - expr_pre_pos,
                  DIGIT, OP_NULL, 0);
    }

    return success(&cur_token);
}

result_t * parse(int nest) {
    token_t * tk;
    result_t * res;
    int lbrac_cnt = 0;
    int sp_btm = token_stack.top;
    if (nest) {
        res = scan();
        tk = res->data;
        if (res->code != SUCC || tk == NULL || tk->op_code != OP_LBRAC) {
            return error_seq(SYMBOL_NEED_ERR, "(", expr_pos + 1);
        }
        token_pushstack(&token_stack, tk);
        lbrac_cnt ++;
    }
    while ((res = scan(), tk = res->data) != NULL && res->code == SUCC) {
        token_t *top;
        if (tk->token_type == DIGIT) {
            token_enqueue(&token_queue, tk);
        } else if (tk->token_type & OP) {
            if (!strcmp("(", tk->lexeme)) {
                token_pushstack(&token_stack, tk);
                lbrac_cnt ++;
            } else if (!strcmp(")", tk->lexeme)) {
                while (token_stack.top > sp_btm) {
                    top = token_popstack(&token_stack);
                    if (!strcmp("(", top->lexeme)) {
                        break;
                    }
                    token_enqueue(&token_queue, top);
                }
                lbrac_cnt --;
                if (nest && lbrac_cnt == 0) {
                    return success(SUCC);
                }
            } else {
                while (token_stack.top > sp_btm) {
                    top = token_peekstack(&token_stack);
                    if (operator_prior_compare(top->lexeme[0],tk->lexeme[0]) >= 0) {
                        token_enqueue(&token_queue, top);
                        token_popstack(&token_stack);
                    } else {
                        break;
                    }
                }
                token_pushstack(&token_stack, tk);
            }
        } else if (tk->token_type == ALPHABET) {
            token_t * mapped = map_op_token_tbl(tk->lexeme);
            if (mapped != NULL) {
                token_pushstack(&token_stack, mapped);
                parse(1);
                token_enqueue(&token_queue, token_popstack(&token_stack));
            } else {
                return error_seq(SYNTAX_ERR, tk->lexeme, expr_pos + 1);
            }
        }
    }

    //deal remains
    while (/* !is_stack_empty(&token_stack) */ token_stack.top > sp_btm) {
        token_enqueue(&token_queue, token_popstack(&token_stack));
    }
    return success(SUCC);
}

result_t * do_calc() {
    token_t * t;
    memset(&token_stack, 0, sizeof(token_stack_t));
    while ((t = token_dequeue(&token_queue)) != NULL ) {
        if (t->token_type == DIGIT) {
            token_pushstack(&token_stack, t);
        } else if (t->token_type & OP) {
            op_func_t * op_func = op_func_tbl[t->op_code].op_func;
            if (op_func != NULL) {
                if (t->token_type == BIN_OP) {
                    token_t *right = token_popstack(&token_stack);
                    token_t *left = token_popstack(&token_stack);
                    token_pushstack(&token_stack, op_func(&cur_token, left, right));
                } else if (t->token_type == UN_OP) {
                    token_t *op = token_popstack(&token_stack);
                    token_pushstack(&token_stack, op_func(&cur_token, op, NULL));
                }
            } else {
                return error(SYNTAX_UNKNOW_ERR);
            }
        }
    }
    return success(SUCC);
}

void post_exp() {
//    printf("PostExpression: ");
//    for (int i = token_queue.head; i < token_queue.tail; i ++) {
//        printf("[%s] ", token_queue.queue[i].lexeme);
//    }
//    printf("\n");
}

void result() {
    token_t * t = token_peekstack(&token_stack);
    if (t != NULL) printf("%s\n", t->lexeme);
}

void calc() {
    if (parse(0)->code == 0) {
        post_exp();
        do_calc();
        result();
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

    reset();
    strcpy(infix_expr, "-((3+2)*sin(3.141592654/2)+10+cos(3.141592654))");
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
