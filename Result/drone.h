#ifndef NODE_H
#define NODE_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <stdbool.h>
#include <math.h>

#define Map_size 100
#define FAILURE_PROB 10

typedef struct Node
{
	struct Node* rlink;
	struct Node* llink;
	int x, y;
	int link_pos;
	//int is_blocked;
	//char* content;
} Node;

typedef struct Dist_node
{
	struct Dist_node* next;
	int x, y;
	char* stage;
	double distance, battery_use;
	double k;
}Dist_node;

typedef struct report_Node {
	char* stage; //구간
	double dist; //거리
	double K; //상수k
	double battery_use; //배터리 소모량
	int state; //상태
	int event; //이벤트
	struct report_Node* rlink;
}report_Node;

typedef struct reportstack_Node {
	char* stage;
	double used_battery;
	struct reportstack_Node* rlink;
}reportstack_Node;

extern Node* head;

//typedef struct Stack
//{
//	int x, y;
//	int link_pos;
//	struct Stack* next;
//} Stack;

//extern int block_pos[100][2];
//extern int block_cnt;

void start_UI();
void input_sys();
void print_center(const char* str, int width);
void sort_nodes(Node* node);
void get_node_nums();
void do_random();
void do_direct();
void loading_bar();

int check_coordinate(int x, int y);
void freeList();

//파일 입출력 함수
void fprint01(FILE*, Node*);

void show_nodes();
void read_list();
//void make_list(Node* head);
void read_node();
void build_dist_list();
double calc_battery(int x1, int y1, int x2, int y2);
double calc_dist(int x1, int y1, int x2, int y2);

double get_k(double dist);
void reporting();
void push(reportstack_Node* top, int* listlen, double battery_use);
void make_final();

void print_dist_list();

#endif
