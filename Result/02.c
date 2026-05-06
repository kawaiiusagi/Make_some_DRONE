#include "drone.h"

Node* head = NULL;

int node_count = 0;

void read_list()
{
	FILE* fp = NULL;

	printf("\n\n----------<Next Step>----------\n");
	printf("Step 2 : Make Coordinate Linked-List!\n");

	fp = fopen("01.txt", "r");

	printf("\n");
	print_center("Reading 01.txt...", 33);
	loading_bar();
	printf("\033[A\r                                     \r");
	printf("\033[A\r                                     \r");
	printf("Upload success!\n");


	int x, y;

	while (fscanf(fp, "%d %d", &x, &y) == 2)
	{
		Node* node = (Node*)malloc(sizeof(Node));

		node->x = x;
		node->y = y;
		node->rlink = NULL;
		node->llink = NULL;

		if (head == NULL) head = node;
		
		else
		{
			sort_nodes(node);
		}

	}

	get_node_nums();
	fclose(fp);

	printf("\n");
	print_center("Making List...", 33);
	loading_bar();
	printf("\033[A\r                                    \r");
	printf("\033[A\r                                    \r");
	printf("Success!\n");

	show_nodes();
}

void sort_nodes(Node* node)
{
	if (node->x < head->x)
	{
		node->rlink = head;
		head->llink = node;
		head = node;
		return;
	}

	Node* temp = head;

	while (temp->rlink != NULL && temp->rlink->x < node->x)
	{
		temp = temp->rlink;
	}
	node->rlink = temp->rlink;
	node->llink = temp;

	if (temp->rlink != NULL)
	{
		temp->rlink->llink = node;
	}
	temp->rlink = node;
}

void get_node_nums()
{
	Node* temp = head;
	int cnt = 0;

	while (temp != NULL)
	{
		temp->link_pos = cnt++;
		temp = temp->rlink;
	}

	node_count = cnt;
}

void show_nodes()
{
	printf("\n----------<Result>----------\n");

	if (head == NULL) return;

	Node* temp = head;

	FILE* fw = NULL;

	fw = fopen("02.txt", "w");

	if (fw == NULL)
	{
		printf("Cannot read the file\n");
		freeList();
		return;
	}

	fprintf(fw, "link_pos  x  y\n");

	while (temp->rlink != NULL)
	{

		printf("[%d](%d,%d)\n", temp->link_pos, temp->x, temp->y);
		printf("   |--> dx:%d, dy:%d ---> ", temp->rlink->x - temp->x, temp->rlink->y - temp->y);
		fprintf(fw, "%d %d %d\n", temp->link_pos, temp->x, temp->y);
		temp = temp->rlink;
		printf("[%d](%d,%d)\n", temp->link_pos, temp->x, temp->y);
	}
	printf("[%d](%d,%d)\n", temp->link_pos, temp->x, temp->y);
	fprintf(fw, "%d %d %d\n", temp->link_pos, temp->x, temp->y);
	fclose(fw);

	system("02.txt");
	freeList();
}
