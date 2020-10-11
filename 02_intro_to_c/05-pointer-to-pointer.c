#include <stdio.h>
#include <stdlib.h>

struct point_t {int x; int y;};
typedef struct point_t point_t;

struct rectangle {
	int width;
	int height;
	point_t top_left;
};

void new_rect(struct rectangle **p)
{
	// struct rectangle *r = calloc(sizeof(struct rectangle));
	struct rectangle *r = malloc(sizeof(struct rectangle));
	*r = (struct rectangle){
		.width = 1,
		.height= 2,
		.top_left = (point_t) {
			.x = 3, .y = 4
		},
	};

	*p = r;
}

void print_rect(struct rectangle *p) {
	printf("<w: %d, h: %d, x: %x, y: %x>\n",
			p->width, p->height,
			p->top_left.x, p->top_left.y);
}

int main(void)
{
	struct rectangle *r1 = NULL;
	new_rect(&r1);
	print_rect(r1);

	return 0;
}
