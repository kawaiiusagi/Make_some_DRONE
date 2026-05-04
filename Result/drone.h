#ifndef NODE_H
#define NODE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>

#define Battery_per_move
#define Enemy_spotted 8
#define Bomb 1
#define Map_size 100

typedef struct Node
{
	struct Node* rlink;
	struct Node* llink;
	int x, y;
	int link_pos;
	//int is_blocked;
	char* content;
} Node;

typedef struct Stack
{
	int x, y;
	int link_pos;
	struct Stack* next;
} Stack;


extern int block_pos[100][2];
extern int block_cnt;

void start_UI();
void input_sys();
void sort_nodes();
void get_node_nums();
void do_random();
void do_direct();
void do_FILE();

//파일 입출력 함수
void print01(Node* ,FILE* );

#endif
