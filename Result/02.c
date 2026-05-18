#include "drone.h"

Node* head = NULL;

int node_count = 0;

void read_list()
{
	FILE* fp = NULL;

	system("cls");
	Sleep(500);

	printf("\n");
	printf("  +================================================+\n");
	printf("  |         DRONE MISSION SETUP                    |\n");
	printf("  |           [ STEP 2 of 4 ]                      |\n");
	printf("  +================================================+\n");
	printf("\n");
	printf("  +------------------------------------------------+\n");
	printf("  |  Mission : Optimal Path Calculation            |\n");
	printf("  |  Input   : Waypoint coordinates from Step 1    |\n");
	printf("  +------------------------------------------------+\n");
	printf("\n");

	Sleep(500);
	printf("  +------------------------------------------------+\n");
	printf("  |  NOTE: File format must be as follows          |\n");
	printf("  |                                                |\n");
	printf("  |    x y  (one coordinate per line)              |\n");
	printf("  |                                                |\n");
	printf("  |  Example:                                      |\n");
	printf("  |    10 20                                       |\n");
	printf("  |    35 47                                       |\n");
	printf("  |    60 80                                       |\n");
	printf("  +------------------------------------------------+\n");
	printf("\n");

	Sleep(500);
	while (1)
	{
		printf("  Please enter the file name(include the file extension)\n");
		printf("  >>> ");

		char buf[100];
		scanf("%s", buf);

		fp = fopen(buf, "r");

		if (fp == NULL)
		{
			printf("Cannot read the file.\n");
			continue;
		}
		else break;

	}

	printf("\n");

	print_center("  Reading FILE...", 33);
	loading_bar();
	printf("  \033[A\r                                     \r");
	printf("  \033[A\r                                     \r");
	printf("  Upload success!\n");


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
	print_center("  Making List...", 33);
	loading_bar();
	printf("  \033[A\r                                    \r");
	printf("  \033[A\r                                    \r");
	printf("  Success!\n\n");

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

	while (temp->rlink != NULL && temp->rlink->x < node->x) //because of pre node
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
	char bbuf[50];

	printf("  Please enter the name of the file to save the linked list result.\n");
	printf("  (include the file extension)\n");
	printf("  >>>");
	scanf("%s", bbuf);
	getchar();


	printf("\n  ----------<Result>----------\n");

	if (head == NULL) return;

	Node* temp = head;

	FILE* fw = NULL;

	fw = fopen(bbuf, "w");

	if (fw == NULL)
	{
		printf("Cannot read the file\n");
		freeList();
		return;
	}

	fprintf(fw, "link_pos  x  y\n");

	while (temp->rlink != NULL)
	{

		printf("  [%d](%d,%d)\n", temp->link_pos, temp->x, temp->y);
		printf("     |--> dx:%d, dy:%d ---> ", temp->rlink->x - temp->x, temp->rlink->y - temp->y);
		fprintf(fw, "%d %d %d\n", temp->link_pos, temp->x, temp->y);
		temp = temp->rlink;
		printf("  [%d](%d,%d)\n", temp->link_pos, temp->x, temp->y);
	}
	printf("  [%d](%d,%d)\n", temp->link_pos, temp->x, temp->y);
	fprintf(fw, "%d %d %d\n", temp->link_pos, temp->x, temp->y);
	fclose(fw);

	system("02.txt");
	freeList();
}
