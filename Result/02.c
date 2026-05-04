#include "drone.h"

Node* head = NULL;
int node_count = 0;

void read_list()
{
	FILE* fp = fopen("01.txt", "r");
	if (fp == NULL)
	{
		printf("Cannot read the file");
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

		if (head == NULL) head = node;
		else sort_nodes(head, node);
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

//void show_nodes(Node* head)
//{
//	Node* temp = head;
//	while (temp != NULL) <- 여기 래욱님이 UI 예쁘게 만들어주세요. + 여기서 명보님 파일 로그로 남겨주는것도 작성해주시면 좋을 것 같아요.
//	{
//		printf("%d")
//	}
//}
