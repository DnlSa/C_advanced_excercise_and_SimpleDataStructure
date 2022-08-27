#include <stdlib.h>

struct node {
	int key;
	struct node *next;
	struct node *prev;
};

void sort(struct node **head)
{
	// ...
}

struct node n = {0, NULL, NULL};
struct node *head = &n;

int main(void)
{
	sort(&head);

	return 0;
}
