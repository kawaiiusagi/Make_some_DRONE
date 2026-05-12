#include "drone.h"


Node* head = NULL;
int node_count = 0;
FILE* fp = NULL;

void read_list()
{
	fp = fopen("01.txt", "r");
	if (fp == NULL)
	{
		printf("Cannot read the file\n");
		return;
	}
	int x, y;

	while (fscanf(fp, "%d %d", &x, &y) == 2)
	{
		Node* node = (Node*)malloc(sizeof(Node));

		node->x = x;
		node->y = y;
		node->content = "Node";
		node->rlink = NULL;
		node->llink = NULL;

		if (head == NULL)
		{
			head = node;
		}
		else
		{
			sort_nodes(head, node);
			get_node_nums();
		}
	}
	show_nodes(head);
	fclose(fp); //안 닫아두셔서 추가 했습니다
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

void show_nodes(Node* head)
{
	if (head == NULL)
		return;
	Node* temp = head;
	fp = fopen("02.txt", "w");
	if (fp == NULL)
	{
		printf("Cannot read the file\n");
		return;
	}

	fprintf(fp, "link_pos  x  y\n"); //03.c에서 읽어올 때 한 줄 비우고 시작할 것
	int count = 1;
	while (temp->rlink != NULL)
	{
		printf("\n%d번 이동\n", count);
		printf("%d(%d,%d)", temp->link_pos, temp->x, temp->y);
		printf(" ---(x->%d, y->%d)---> ", temp->rlink->x - temp->x, temp->rlink->y - temp->y);
		temp = temp->rlink;
		printf("%d(%d,%d)\n", temp->link_pos, temp->x, temp->y);
		count++;
	}
	temp = head;
	while (temp != NULL)
	{
		fprintf(fp, "%d %d %d\n", temp->link_pos, temp->x, temp->y);
		temp = temp->rlink;
	}
	
	fclose(fp);

	freeList(head);
}

//show_nodes하고 동적 메모리 헤채하는것도 추가하세요 < 완료
void freeList(Node* head)
{
	Node* temp = head->rlink;
	while (temp != NULL)
	{
		head->rlink = temp->rlink;
		free(temp);
		temp = head->rlink;
	}
	free(head);
	printf("해제 완료");
}
