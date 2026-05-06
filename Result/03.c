#include "drone.h"

Dist_node* dist_head = NULL;

void read_node()
{
	printf("\n\n----------<Next Step>----------\n");
	printf("Step 3 : Make Simulator Dataset!\n");

	printf("\n");
	print_center("Reading 02.txt...", 33);
	loading_bar();
	printf("\033[A\r                                     \r");
	printf("\033[A\r                                     \r");
	printf("Upload success!\n");

	head = NULL;

	FILE* fp = fopen("02.txt", "r");
	if (fp == NULL)
	{
		printf("Cannot read the flle\n");
		return;
	}

	char buf[50];
	fgets(buf, sizeof(buf), fp);

	int x, y, ID;

	while (fscanf(fp, "%d %d %d", &ID, &x, &y) == 3)
	{
		Node* node = (Node*)malloc(sizeof(Node));

		node->x = x;
		node->y = y;
		node->llink = NULL;
		node->rlink = NULL;
		node->link_pos = ID;

		if (head == NULL)
		{
			head = node;
		}
		else {
			Node* temp = head;

			while (temp->rlink != NULL)
			{
				temp = temp->rlink;
			}
			temp->rlink = node;
		}
	}

	fclose(fp);

	build_dist_list();
}

double calc_dist(int x1, int y1, int x2, int y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

double get_k(double dist)
{
	if (dist > 50) return 1.7;
	else return 1.5;
}

double calc_battery(int x1, int y1, int x2, int y2)
{
	double dist = calc_dist(x1, y1, x2, y2);
	double k = get_k(dist);
	return k * dist;
}

void build_dist_list()
{
	print_center("Caculating...", 33);
	loading_bar();
	printf("\033[A\r                                     \r");
	printf("\033[A\r                                     \r");
	printf("Success!\n");

	if (head == NULL) return;

	Node* temp = head;
	int idx = 0;

	char* stages[] = { "AB","BC","CD","DE","EF","FG","GH","HI","IJ" };

	while (temp->rlink != NULL)
	{
		int x1 = temp->x, y1 = temp->y;
		int x2 = temp->rlink->x, y2 = temp->rlink->y;

		double dist = calc_dist(x1, y1, x2, y2);
		double k = get_k(dist);
		double battery = calc_battery(x1, y1, x2, y2);

		Dist_node* node = (Dist_node*)malloc(sizeof(Dist_node));
		node->next = NULL;

		node->x = x1;
		node->y = y1;
		node->distance = dist;
		node->k = k;
		node->battery_use = battery;
		node->stage = stages[idx];

		if (dist_head == NULL)
		{
			dist_head = node;
		}
		else {
			Dist_node* t = dist_head;

			while (t->next != NULL)
			{
				t = t->next;
			}
			t->next = node;
		}

		idx++;
		temp = temp->rlink;

	}

	print_dist_list();

}

void print_dist_list()
{
	FILE* fp = fopen("03.txt", "w");
	if (fp == NULL)
	{
		printf("Cannot open 03.txt\n");
		freeList();
		return;
	}

	Dist_node* temp = dist_head;
	double total_dist = 0;
	double total_battery = 0;

	printf("\n----------<Result>----------\n");
	printf("%-10s %-10s %-10s %-10s\n", "Stage", "Distance", "K", "Battery");
	printf("------------------------------------------\n");

	while (temp != NULL)
	{
		total_dist += temp->distance;
		total_battery += temp->battery_use;
		fprintf(fp, "%s %.1f %.1f %.1f\n", temp->stage, temp->distance, temp->k, temp->battery_use);
		printf("%-10s %-10.1f %-10.1f %-10.1f\n", temp->stage, temp->distance, temp->k, temp->battery_use);
		temp = temp->next;
	}

	fprintf(fp, "TOTAL %.1f - %.1f\n", total_dist, total_battery);

	printf("------------------------------------------\n");
	printf("%-10s %-10.1f %-10s %-10.1f\n", "TOTAL", total_dist, "-", total_battery);

	fclose(fp);
	freeList();
	system("03.txt");
}

void freeList()
{

	Node* temp = head;
	while (temp != NULL)
	{
		Node* next = temp->rlink;
		free(temp);
		temp = next;
	}
	head = NULL;


	Dist_node* dtemp = dist_head;
	while (dtemp != NULL)
	{
		Dist_node* next = dtemp->next;
		free(dtemp);
		dtemp = next;
	}
	dist_head = NULL;
}
