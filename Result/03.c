#include "drone.h"

Dist_node* dist_head = NULL;
double x_total = 0.0;
double dist_mst = 0.0;
int use_mst = 0;

void dfs_order(TreeNode* node, int* path_order, int* path_cnt)
{
	path_order[(*path_cnt)++] = node->idx;
	TreeNode* temp = node->llink;
	while (temp != NULL)
	{
		dfs_order(temp, path_order, path_cnt);
		temp = temp->rlink;
	}
}

void read_node()
{
	system("cls");
	Sleep(500);

	printf("\n");
	printf("  +================================================+\n");
	printf("  |         DRONE MISSION SETUP                    |\n");
	printf("  |           [ STEP 3 of 4 ]                      |\n");
	printf("  +================================================+\n");
	printf("\n");
	printf("  +------------------------------------------------+\n");
	printf("  |  Mission : Distance & Battery Calculation      |\n");
	printf("  |  Input   : Sorted waypoint list from Step 2    |\n");
	printf("  +------------------------------------------------+\n");
	printf("\n");

	Sleep(500);
	printf("  +------------------------------------------------+\n");
	printf("  |  NOTE: File format must be as follows          |\n");
	printf("  |                                                |\n");
	printf("  |    ID x y  (one waypoint per line)             |\n");
	printf("  |                                                |\n");
	printf("  |  Example:                                      |\n");
	printf("  |    link_pos  x  y                              |\n");
	printf("  |    0  0  0                                     |\n");
	printf("  |    1  10 20                                    |\n");
	printf("  +------------------------------------------------+\n");
	printf("\n");

	FILE* fp = NULL;

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

	print_center("Uploading...", 33);
	loading_bar();
	printf("\033[A\r                                     \r");
	printf("\033[A\r                                     \r");
	printf("  Upload success!\n\n");

	char header[50];
	fgets(header, sizeof(header), fp);

	int wpx[MAX_waypoint], wpy[MAX_waypoint];
	int node_cnt = 0;
	int link_pos, x, y;

	while (fscanf(fp, "%d %d %d", &link_pos, &x, &y) == 3)
	{
		wpx[node_cnt] = x;
		wpy[node_cnt] = y;
		node_cnt++;
	}
	fclose(fp);

	// sum distance of sorted by x
	x_total = 0.0;
	for (int i = 0; i < node_cnt - 1; i++)
		x_total += calc_dist(wpx[i], wpy[i], wpx[i + 1], wpy[i + 1]);

	// MST
	double dist_min[MAX_waypoint];
	int parent_idx[MAX_waypoint];
	bool is_visited[MAX_waypoint];

	for (int i = 0; i < node_cnt; i++)
	{
		dist_min[i] = 1000; //never get 1000
		parent_idx[i] = -1; //to know now initate 
		is_visited[i] = false;
	}
	dist_min[0] = 0.0;

	for (int step = 0; step < node_cnt; step++)
	{
		int now = -1;
		for (int i = 0; i < node_cnt; i++)
		{
			if (!is_visited[i] && (now == -1 || dist_min[i] < dist_min[now]))
				now = i;
		}

		is_visited[now] = true;

		for (int who = 0; who < node_cnt; who++)
		{
			if (!is_visited[who])
			{
				double d = calc_dist(wpx[now], wpy[now], wpx[who], wpy[who]);
				if (d < dist_min[who])
				{
					dist_min[who] = d;
					parent_idx[who] = now;
				}
			}
		}
	}

	TreeNode nodes[MAX_waypoint];
	for (int i = 0; i < node_cnt; i++)
	{
		nodes[i].x = wpx[i];
		nodes[i].y = wpy[i];
		nodes[i].idx = i;
		nodes[i].llink = NULL;
		nodes[i].rlink = NULL;
	}

	for (int i = 1; i < node_cnt; i++)
	{
		int p = parent_idx[i];
		double d_new = calc_dist(nodes[p].x, nodes[p].y, nodes[i].x, nodes[i].y);

		if (nodes[p].llink == NULL)
		{
			nodes[p].llink = &nodes[i];
		}
		else if (d_new < calc_dist(nodes[p].x, nodes[p].y, nodes[p].llink->x, nodes[p].llink->y))
		{
			nodes[i].rlink = nodes[p].llink;
			nodes[p].llink = &nodes[i];
		}
		else
		{
			TreeNode* temp = nodes[p].llink;
			while (temp->rlink != NULL && calc_dist(nodes[p].x, nodes[p].y, temp->rlink->x, temp->rlink->y) <= d_new)
				temp = temp->rlink;
			nodes[i].rlink = temp->rlink;
			temp->rlink = &nodes[i];
		}
	}

	// to decide sequence
	int path_order[MAX_waypoint];
	int path_cnt = 0;
	dfs_order(&nodes[0], path_order, &path_cnt);

	// get toatal dist
	dist_mst = 0.0;
	for (int i = 0; i < path_cnt - 1; i++)
	{
		int a = path_order[i], b = path_order[i + 1];
		dist_mst += calc_dist(wpx[a], wpy[a], wpx[b], wpy[b]);
	}

	use_mst = (dist_mst <= x_total);


	head = NULL;
	for (int i = 0; i < node_cnt; i++)
	{
		int idx = use_mst ? path_order[i] : i;
		Node* node = (Node*)malloc(sizeof(Node));
		node->x = wpx[idx];
		node->y = wpy[idx];
		node->link_pos = i;
		node->llink = NULL;
		node->rlink = NULL;

		if (head == NULL)
		{
			head = node;
		}
		else
		{
			Node* temp = head;
			while (temp->rlink != NULL)
				temp = temp->rlink;
			temp->rlink = node;
		}
	}

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
	printf("  Success!\n\n");

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
	FILE* fw = NULL;

	char bbuf[50];

	printf("  Please enter the name of the file to save the linked list result.\n");
	printf("  (include the file extension)\n");
	printf("  >>>");
	scanf("%s", bbuf);
	getchar();

	fw = fopen(bbuf, "w");

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
		fprintf(fw, "%s %.1f %.1f %.1f\n", temp->stage, temp->distance, temp->k, temp->battery_use);
		printf("%-10s %-10.1f %-10.1f %-10.1f\n", temp->stage, temp->distance, temp->k, temp->battery_use);
		temp = temp->next;
	}

	fprintf(fw, "TOTAL %.1f - %.1f\n", total_dist, total_battery);

	printf("------------------------------------------\n");
	printf("%-10s %-10.1f %-10s %-10.1f\n", "TOTAL", total_dist, "-", total_battery);

	double dif = x_total - dist_mst;
	double how = (x_total > 0.0) ? dif / x_total * 100.0 : 0.0;

	fprintf(fw, "------------------------------------------\n");
	fprintf(fw, "  [Path Comparison]\n");
	fprintf(fw, "  MST Distance   : %.1f\n", dist_mst);
	fprintf(fw, "  X-Sort Distance: %.1f\n", x_total);
	if (use_mst)
		fprintf(fw, "  Used           : MST  (shorter by %.1f, %.1f%%)\n", dif, how);
	else
		fprintf(fw, "  Used           : X-Sort  (shorter by %.1f, %.1f%%)\n", -dif, -how);
	fprintf(fw, "------------------------------------------\n");

	printf("------------------------------------------\n");
	printf("  [Path Comparison]\n");
	printf("  MST Distance   : %.1f\n", dist_mst);
	printf("  X-Sort Distance: %.1f\n", x_total);
	if (use_mst)
		printf("  Used           : MST  (shorter by %.1f, %.1f%%)\n", dif, how);
	else
		printf("  Used           : X-Sort  (shorter by %.1f, %.1f%%)\n", -dif, -how);
	printf("------------------------------------------\n");

	fclose(fw);
	freeList();
	system(bbuf);
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
