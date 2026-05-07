#ifndef NODE_H
#define NODE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define Battery_per_move 1
#define Enemy_spotted 8
#define Bomb 1
#define Map_size 10

typedef struct Node
{
	struct Node* rlink;
	struct Node* llink;
	int x, y;
	int link_pos;
	int is_blocked;
	char* content;
} Node;

typedef struct Stack
{
	int x, y;
	int link_pos;
	struct Stack* next;
} Stack;

extern int node_count; //다른 파일에서 이 변수를 쓸거야라고 선언.
// 함수는 자동 공유가 되지만 일반 변수는 자동으로 공유되지 않음.
// 여기서 가져다 쓰라는 의미이다.

extern int block_pos[100][2];
extern int block_cnt;

void add_blocked_pos(int x, int y);
int is_blocked_pos(int x, int y);
void remove_node(Node** head, int pos);

void push_history(int x, int y, int pos);
Stack* pop_history();
void clear_history();

Node* start_program();
void Create_Node(Node* head);
char* check_node(Node* head, int x, int y);
void show_list(Node* head);
void delete_node(Node** head, int pos);
void get_node_nums(Node* head);
void sort_nodes(Node* head, Node* new_node);
void move_drone(Node* head);
int calc_distance(int x1, int y1, int x2, int y2);
int check_event();
int check_explode();
void save_log(char* msg);
void mission_log(int success, int fail_pos, char* reason);
void free_all(Node* head);

#endif
