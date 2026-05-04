#include "drone.h"

Node* head = NULL;
int node_count = 0;
FILE* fw=NULL,* fa=NULL,* fr=NULL;

void start_UI() {} // starting point
void input_sys()
{
	while (1)
	{
		int num;
		printf("\nPlease enter the number you want to do\n");
		printf("1 : random, 2: Directly, 3 : Read on file\n");
		

		if (scanf("%d", &num) != 1 || num != 1 && num != 2 && num != 3)
		{
			printf("Please enter correct value");
			while (getchar() != '\n');
			continue;
		}

		getchar();

		switch (num)
		{
		case 1:
			do_random();
			break;
		case 2:
			do_direct();
			break;
		case 3:
			do_FILE();
			break;
		default:
			printf("please try again.");
			break;
		}
	}
}

int check_node(Node* head, int x, int y)
{
	if (head == NULL)
	{
		return 1;
	}
	else {
		Node* temp = head;
		while (temp != NULL)
		{
			if (temp->x == x && temp->y == y)
			{
				return 2; // 2가 반환되면 중복되는게 있다는 의미
			}
			temp = temp->rlink;
		}
		return 1;
	}
}

void sort_nodes(Node* head, Node* node)
{
	Node* temp = head;

	if (head == node) return; // 자기 자신 순회 예외처리

	while (temp->rlink != NULL && temp->rlink->x < node->x)
	{
		temp = temp->rlink;
	}
	node->rlink = temp->rlink;
	node->llink = temp;

	if (temp->rlink != NULL) //두 번 하는 이유 -> 양방향 연결리스트이기 때문이다.
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

void do_random() {
	int temp_num = 0;

	while (1)
	{
		printf("How many nodes do you want to add? : ");
		if (scanf("%d", &temp_num) != 1)
		{

			printf("Please add correct value\n");

			while (getchar() != '\n');

			continue;
		}
		else break;
	}
	
	getchar();

	int x, y;

	srand((unsigned int)time(NULL));

	for (int i = 0; i < temp_num; i++)
	{
		while (1)
		{
			x = rand() % Map_size + 1;
			y = rand() % Map_size + 1;

			int state = check_node(head, x, y);

			if (state == 2)
			{
				fprint01(FILE* fw, Node* head)
				continue;
			}
			else
			{
				break;
			}
		}

		// Node* node = (Node*)malloc(sizeof(Node));
		// if (node == NULL) {
		// 	printf("Error. Node cannot created");
		// }
		// else
		// {
		// 	if (head == NULL) head = node;

		// 	node->x = x;
		// 	node->y = y;
		// 	node->content = "Node";
		// 	node->rlink = NULL;
		// 	node->llink = NULL;

		// 	sort_nodes(head, node);
		// 	get_node_nums();

		// 	printf("Node created at (%d, %d)\n", node->x, node->y);

			
		}

	}

}
void do_direct() {}
void do_FILE() {}
