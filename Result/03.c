#include "drone.h"

Node* head = NULL;

void read_node()
{
	FILE* fp = fopen("02.txt", "r");
	if (fp == NULL)
	{
		printf("Cannot read the flle\n");
		return;
	}

	int x, y, ID;

	while (fscanf(fp, "%d %d %d", &x, &y, &ID) == 3)
	{
		Node* node = (Node*)malloc(sizeof(Node));

		node->x = x;
		node->y = y;
		node->rlink = NULL;
		node->llink = NULL;

		if (head == NULL) head = node;
		else
		{
			sort_nodes(head, node);
			get_node_nums();
		}
	}
}

int battery_use(int x, int y)
{

} 

int get_k(dist)
{
}

/*
#include <math.h>

dist = sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1))
dist 크기에 따라서 k 값을 리턴하는 get_k 함수
*/
