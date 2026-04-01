#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Структуры

typedef enum {
    SHAPE_POINT,
    SHAPE_CIRCLE,
    SHAPE_RECTANGLE,
    SHAPE_TRIANGLE,
    SHAPE_POLYGON
} ShapeType;

typedef struct {
    double x, y;
} Point;

typedef struct {
    Point center;
    double radius;
} Circle;

typedef struct {
    Point top_left;
    double width, height;
} Rectangle;

typedef struct {
    Point a, b, c;
} Triangle;

typedef struct {
    Point *vertices;
    int count;
} Polygon;

typedef struct {
    ShapeType type;
    union {
        Point point;
        Circle circle;
        Rectangle rectangle;
        Triangle triangle;
        Polygon polygon;
    } data;
} Shape;

// Конструкторы

Shape shape_point(double x, double y) {
    Shape s;
    s.type = SHAPE_POINT;
    s.data.point.x = x;
    s.data.point.y = y;
    return s;
}

Shape shape_circle(double cx, double cy, double radius) {
    Shape s;
    s.type = SHAPE_CIRCLE;
    s.data.circle.center.x = cx;
    s.data.circle.center.y = cy;
    s.data.circle.radius = radius;
    return s;
}

Shape shape_rectangle(double x, double y, double w, double h) {
    Shape s;
    s.type = SHAPE_RECTANGLE;
    s.data.rectangle.top_left.x = x;
    s.data.rectangle.top_left.y = y;
    s.data.rectangle.width = w;
    s.data.rectangle.height = h;
    return s;
}

Shape shape_triangle(Point a, Point b, Point c) {
    Shape s;
    s.type = SHAPE_TRIANGLE;
    s.data.triangle.a = a;
    s.data.triangle.b = b;
    s.data.triangle.c = c;
    return s;
}

// Вычисления

double shape_area(const Shape *s) {
    if (s == NULL) return 0.0;
    
    switch (s->type) {
        case SHAPE_POINT:
            return 0.0;
            
        case SHAPE_CIRCLE:
            return M_PI * s->data.circle.radius * s->data.circle.radius;
            
        case SHAPE_RECTANGLE:
            return s->data.rectangle.width * s->data.rectangle.height;
            
        case SHAPE_TRIANGLE: {
            Triangle t = s->data.triangle;
            // Формула Герона
            double a = hypot(t.b.x - t.a.x, t.b.y - t.a.y);
            double b = hypot(t.c.x - t.b.x, t.c.y - t.b.y);
            double c = hypot(t.a.x - t.c.x, t.a.y - t.c.y);
            double p = (a + b + c) / 2.0;
            return sqrt(p * (p - a) * (p - b) * (p - c));
        }
            
        case SHAPE_POLYGON:
            // Упрощённая версия для многоугольника
            return 0.0;
            
        default:
            return 0.0;
    }
}

double shape_perimeter(const Shape *s) {
    if (s == NULL) return 0.0;
    
    switch (s->type) {
        case SHAPE_POINT:
            return 0.0;
            
        case SHAPE_CIRCLE:
            return 2 * M_PI * s->data.circle.radius;
            
        case SHAPE_RECTANGLE:
            return 2 * (s->data.rectangle.width + s->data.rectangle.height);
            
        case SHAPE_TRIANGLE: {
            Triangle t = s->data.triangle;
            double a = hypot(t.b.x - t.a.x, t.b.y - t.a.y);
            double b = hypot(t.c.x - t.b.x, t.c.y - t.b.y);
            double c = hypot(t.a.x - t.c.x, t.a.y - t.c.y);
            return a + b + c;
        }
            
        default:
            return 0.0;
    }
}

Point shape_center(const Shape *s) {
    Point center = {0, 0};
    if (s == NULL) return center;
    
    switch (s->type) {
        case SHAPE_POINT:
            center = s->data.point;
            break;
            
        case SHAPE_CIRCLE:
            center = s->data.circle.center;
            break;
            
        case SHAPE_RECTANGLE:
            center.x = s->data.rectangle.top_left.x + s->data.rectangle.width / 2;
            center.y = s->data.rectangle.top_left.y + s->data.rectangle.height / 2;
            break;
            
        case SHAPE_TRIANGLE: {
            Triangle t = s->data.triangle;
            center.x = (t.a.x + t.b.x + t.c.x) / 3;
            center.y = (t.a.y + t.b.y + t.c.y) / 3;
            break;
        }
            
        default:
            break;
    }
    return center;
}

// Трансформации

void shape_move(Shape *s, double dx, double dy) {
    if (s == NULL) return;
    
    switch (s->type) {
        case SHAPE_POINT:
            s->data.point.x += dx;
            s->data.point.y += dy;
            break;
            
        case SHAPE_CIRCLE:
            s->data.circle.center.x += dx;
            s->data.circle.center.y += dy;
            break;
            
        case SHAPE_RECTANGLE:
            s->data.rectangle.top_left.x += dx;
            s->data.rectangle.top_left.y += dy;
            break;
            
        case SHAPE_TRIANGLE:
            s->data.triangle.a.x += dx;
            s->data.triangle.a.y += dy;
            s->data.triangle.b.x += dx;
            s->data.triangle.b.y += dy;
            s->data.triangle.c.x += dx;
            s->data.triangle.c.y += dy;
            break;
            
        default:
            break;
    }
}

void shape_scale(Shape *s, double factor) {
    if (s == NULL || factor <= 0) return;
    
    switch (s->type) {
        case SHAPE_CIRCLE:
            s->data.circle.radius *= factor;
            break;
            
        case SHAPE_RECTANGLE:
            s->data.rectangle.width *= factor;
            s->data.rectangle.height *= factor;
            break;
            
        default:
            break;
    }
}

// Вывод

void shape_print(const Shape *s) {
    if (s == NULL) return;
    
    switch (s->type) {
        case SHAPE_POINT:
            printf("Точка: (%.2f, %.2f)\n", s->data.point.x, s->data.point.y);
            break;
            
        case SHAPE_CIRCLE:
            printf("Круг: центр (%.2f, %.2f), радиус %.2f\n", 
                   s->data.circle.center.x, s->data.circle.center.y, s->data.circle.radius);
            printf("  Площадь: %.2f, Периметр: %.2f\n", shape_area(s), shape_perimeter(s));
            break;
            
        case SHAPE_RECTANGLE:
            printf("Прямоугольник: (%.2f, %.2f), %.2f x %.2f\n",
                   s->data.rectangle.top_left.x, s->data.rectangle.top_left.y,
                   s->data.rectangle.width, s->data.rectangle.height);
            printf("  Площадь: %.2f, Периметр: %.2f\n", shape_area(s), shape_perimeter(s));
            break;
            
        case SHAPE_TRIANGLE:
            printf("Треугольник: (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\n",
                   s->data.triangle.a.x, s->data.triangle.a.y,
                   s->data.triangle.b.x, s->data.triangle.b.y,
                   s->data.triangle.c.x, s->data.triangle.c.y);
            printf("  Площадь: %.2f, Периметр: %.2f\n", shape_area(s), shape_perimeter(s));
            break;
            
        default:
            printf("Неизвестная фигура\n");
            break;
    }
}

// Функция MAIN 

int main(void) {
    Shape circle = shape_circle(0, 0, 5);
    Shape rect = shape_rectangle(0, 0, 4, 3);
    
    Point a = {0, 0};
    Point b = {4, 0};
    Point c = {2, 3};
    Shape triangle = shape_triangle(a, b, c);
    
    printf("=== Геометрические фигуры ===\n\n");
    
    shape_print(&circle);
    printf("\n");
    shape_print(&rect);
    printf("\n");
    shape_print(&triangle);
    
    // Трансформации
    shape_move(&circle, 2, 3);
    shape_scale(&rect, 2);
    
    printf("\n=== После трансформаций ===\n\n");
    printf("Круг после перемещения:\n");
    shape_print(&circle);
    printf("\nПрямоугольник после масштабирования:\n");
    shape_print(&rect);
    
    return 0;
}
