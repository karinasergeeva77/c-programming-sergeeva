#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Структуры

typedef enum {
    EXPR_NUMBER,
    EXPR_VARIABLE,
    EXPR_ADD,
    EXPR_SUB,
    EXPR_MUL,
    EXPR_DIV,
    EXPR_NEG,
    EXPR_SQRT,
    EXPR_POW
} ExprType;

typedef struct Expr Expr;

struct Expr {
    ExprType type;
    union {
        double number;
        char variable;
        struct {
            Expr *left;
            Expr *right;
        } binary;
        Expr *operand;
    } data;
};

// Контекст для переменных

typedef struct {
    char name;
    double value;
} Variable;

typedef struct {
    Variable vars[26];
    int count;
} Context;

// Конструкторы

Expr* expr_number(double value) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_NUMBER;
    e->data.number = value;
    return e;
}

Expr* expr_variable(char name) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_VARIABLE;
    e->data.variable = name;
    return e;
}

Expr* expr_add(Expr *left, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_ADD;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_sub(Expr *left, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_SUB;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_mul(Expr *left, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_MUL;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr* expr_div(Expr *left, Expr *right) {
    Expr *e = malloc(sizeof(Expr));
    e->type = EXPR_DIV;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

// Вычисление

double expr_eval(const Expr *e, const Context *ctx) {
    if (e == NULL) return 0;
    
    switch (e->type) {
        case EXPR_NUMBER:
            return e->data.number;
            
        case EXPR_VARIABLE: {
            char var = e->data.variable;
            for (int i = 0; i < ctx->count; i++) {
                if (ctx->vars[i].name == var) {
                    return ctx->vars[i].value;
                }
            }
            return 0;
        }
            
        case EXPR_ADD:
            return expr_eval(e->data.binary.left, ctx) + expr_eval(e->data.binary.right, ctx);
            
        case EXPR_SUB:
            return expr_eval(e->data.binary.left, ctx) - expr_eval(e->data.binary.right, ctx);
            
        case EXPR_MUL:
            return expr_eval(e->data.binary.left, ctx) * expr_eval(e->data.binary.right, ctx);
            
        case EXPR_DIV:
            return expr_eval(e->data.binary.left, ctx) / expr_eval(e->data.binary.right, ctx);
            
        default:
            return 0;
    }
}

// Вывод в строку

void expr_print(const Expr *e) {
    if (e == NULL) return;
    
    switch (e->type) {
        case EXPR_NUMBER:
            printf("%g", e->data.number);
            break;
            
        case EXPR_VARIABLE:
            printf("%c", e->data.variable);
            break;
            
        case EXPR_ADD:
            printf("(");
            expr_print(e->data.binary.left);
            printf(" + ");
            expr_print(e->data.binary.right);
            printf(")");
            break;
            
        case EXPR_SUB:
            printf("(");
            expr_print(e->data.binary.left);
            printf(" - ");
            expr_print(e->data.binary.right);
            printf(")");
            break;
            
        case EXPR_MUL:
            printf("(");
            expr_print(e->data.binary.left);
            printf(" * ");
            expr_print(e->data.binary.right);
            printf(")");
            break;
            
        case EXPR_DIV:
            printf("(");
            expr_print(e->data.binary.left);
            printf(" / ");
            expr_print(e->data.binary.right);
            printf(")");
            break;
            
        default:
            printf("?");
            break;
    }
}

// Освобождение памяти

void expr_free(Expr *e) {
    if (e == NULL) return;
    
    switch (e->type) {
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MUL:
        case EXPR_DIV:
            expr_free(e->data.binary.left);
            expr_free(e->data.binary.right);
            break;
            
        default:
            break;
    }
    free(e);
}

// Контекст

void context_init(Context *ctx) {
    if (ctx == NULL) return;
    ctx->count = 0;
    memset(ctx->vars, 0, sizeof(ctx->vars));
}

void context_set(Context *ctx, char name, double value) {
    if (ctx == NULL) return;
    
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->vars[i].name == name) {
            ctx->vars[i].value = value;
            return;
        }
    }
    
    if (ctx->count < 26) {
        ctx->vars[ctx->count].name = name;
        ctx->vars[ctx->count].value = value;
        ctx->count++;
    }
}

double context_get(const Context *ctx, char name) {
    if (ctx == NULL) return 0;
    
    for (int i = 0; i < ctx->count; i++) {
        if (ctx->vars[i].name == name) {
            return ctx->vars[i].value;
        }
    }
    return 0;
}

// Функция MAIN 

int main(void) {
    // Создаём выражение: (x + 3) * (x - 2)
    Expr *x = expr_variable('x');
    Expr *three = expr_number(3);
    Expr *two = expr_number(2);
    
    Expr *left = expr_add(x, three);
    Expr *right = expr_sub(x, two);
    Expr *expr = expr_mul(left, right);
    
    printf("Выражение: ");
    expr_print(expr);
    printf("\n\n");
    
    // Контекст с переменной x
    Context ctx;
    context_init(&ctx);
    context_set(&ctx, 'x', 5);
    
    double result = expr_eval(expr, &ctx);
    printf("При x = 5: %g\n", result);
    
    return 0;
}
