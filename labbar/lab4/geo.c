#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

struct point
{
  int x;
  int y;
};

typedef struct point point_t;

struct rectangle
{
    point_t p1;
    point_t p2;
};

typedef struct rectangle rectangle_t;

void translate(point_t *p1, point_t *p2)
{
    p1->x += p2->x;
    p1->y += p2->y;
}

void print_point(point_t *p)
{
    printf("(%d,%d)", p->x, p->y);
}

point_t make_point(int a, int b)
{
    point_t p = { .x = a, .y = b};
    return p;
}

void print_rec(rectangle_t *r)
{
    printf("Rectangle(upper_left");
    print_point(&r->p1);
    printf(", lower_right");
    print_point(&r->p2);
    printf(")\n");
}

rectangle_t make_rec(int a, int b, int c, int d)
{
    point_t ul = { .x = a, .y = b};
    point_t lr = { .x = c, .y = d};
    rectangle_t r = { .p1 = ul, .p2 = lr};
    return r;
}

int main(void)
{
    rectangle_t r = (make_rec(1, 5, 5, 2));
    print_rec(&r);
    return 0;
}