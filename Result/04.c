#include "drone.h"

reportstack_Node* top = NULL;
report_Node* rhead = NULL;

FILE* fp = NULL;

double Maxbattery, battery;
double used_battery = 0.0;
char* droneState[] = { "NORMAL", "ERROR", "FAILURE", "RECOVERY" };
char* pointName[] = { "A","B","C","D","E","F","G","H","I","J" };
int listlen = 0;

void make_final()
{

	double total_battery = 0;

	FILE* fp_bat = NULL;
	fp_bat = fopen("03.txt", "r");

	if (fp_bat == NULL)
	{
		printf("Cannot read the file\n");
		return;
	}

	char s_stage[10], s_dash[4];
	double s_dist, s_bat;

	while (fscanf(fp_bat, "%s %lf %s %lf", s_stage, &s_dist, s_dash, &s_bat) == 4)
	{
		if (strcmp(s_stage, "TOTAL") == 0)
		{
			total_battery = s_bat;
			break;
		}
	}
	fclose(fp_bat);

	srand((unsigned int)time(NULL));
	int rand_offset = rand() % (int)(total_battery * 1.5 + 1);

	Maxbattery = total_battery + rand_offset;
	battery = Maxbattery;

	fp = fopen("03.txt", "r");
	top = (reportstack_Node*)calloc(1, sizeof(reportstack_Node));
	rhead = (report_Node*)calloc(1, sizeof(report_Node));

	if (fp == NULL)
	{
		printf("Cannot read the file\n");
		return;
	}

	printf("\n\n----------<Next Step>----------\n");
	printf("Step 4 : Running Simulator!\n");

	printf("\n");
	print_center("UPLOADING files...", 33);
	loading_bar();
	printf("\033[A\r                                     \r");
	printf("\033[A\r                                     \r");
	printf("Upload success!\n");

	char stage[6];
	double dist, K, battery_use;
	int state;

	report_Node* pre = rhead;

	while (fscanf(fp, "%s %lf %lf %lf", stage, &dist, &K, &battery_use) == 4)
	{
		report_Node* node = (report_Node*)malloc(sizeof(report_Node));

		node->stage = (char*)malloc(strlen(stage) + 1);
		strcpy(node->stage, stage);
		node->dist = dist;
		node->K = K;
		node->battery_use = battery_use;
		node->rlink = NULL;

		int fail_roll = rand() % 100;

		if (pre->state == 1 || pre->state == 2)
		{
			state = 3;
		}
		else if (fail_roll < FAILURE_PROB)
		{
			state = 2;
		}
		else if (battery < Maxbattery / 2)
		{
			state = 1;
		}
		else
		{
			state = 0;
		}

		node->state = state;
		node->event = 0;
		pre->rlink = node;
		pre = node;
		battery -= battery_use;
		push(top, &listlen, battery_use);

		if (state == 1 || state == 2) break;
	}

	fclose(fp);
	reporting();
}

void reporting()
{
	fp = fopen("04.txt", "w");
	if (fp == NULL)
	{
		printf("Cannot open the file\n");
		return;
	}

	printf("\n----------<FINAL RESULT>----------\n");

	const char* header_format = "%-14s %-10s %-8s %-15s %-15s %-12s %-20s\n";
	const char* row_format = "%-14s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %-20s\n";
	const char* track_format  = "%-14s %-10s %-8s %-15s %-15.1f %-12s %-20s\n";

	fprintf(fp, header_format, "Stage", "Dist(m)", "K", "Battery", "Total", "State", "Return Path");
	fprintf(stdout, header_format, "Stage", "Dist(m)", "K", "Battery", "Total", "State", "Return Path");

	fprintf(fp, "-----------------------------------------------------------------------------------------\n");
	fprintf(stdout, "-----------------------------------------------------------------------------------------\n");

	report_Node* temp = rhead->rlink;
	used_battery = 0.0;

	while (temp != NULL)
	{
		used_battery += temp->battery_use;

		if (temp->state == 2)
		{
			fprintf(fp, row_format, temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], "MISSION FAILED");
			fprintf(stdout, row_format, temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], "MISSION FAILED");
			break;
		}

		if (temp->state == 1)
		{
			char full_path[200] = "";
			char dest_buf[4];
			sprintf(dest_buf, "%c->", temp->stage[1]);
			strcat(full_path, dest_buf);
			reportstack_Node* s = top->rlink;
			while (s != NULL)
			{
				strcat(full_path, s->stage);
				if (s->rlink != NULL) strcat(full_path, "->");
				s = s->rlink;
			}

			fprintf(fp, row_format, temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], full_path);
			fprintf(stdout, row_format, temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], full_path);

			while (top->rlink != NULL)
			{
				reportstack_Node* cur = top->rlink;

				if (cur->rlink != NULL)
				{
					char stage_buf[30];
					char path_buf[30];
					sprintf(stage_buf, "%s%s(Return)", cur->stage, cur->rlink->stage);
					sprintf(path_buf, "%s->%s", cur->stage, cur->rlink->stage);
					used_battery -= cur->used_battery;

					fprintf(fp, track_format, stage_buf, "-", "-", "-", used_battery, "RECOVERY", path_buf);
					fprintf(stdout, track_format, stage_buf, "-", "-", "-", used_battery, "RECOVERY", path_buf);
				}

				top->rlink = cur->rlink;
				free(cur->stage);
				free(cur);
			}
			break;
		}

		fprintf(fp, row_format, temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], "-");
		fprintf(stdout, row_format, temp->stage, temp->dist, temp->K,temp->battery_use, used_battery, droneState[temp->state], "-");

		temp = temp->rlink;
	}
	fclose(fp);

	report_Node* rtemp = rhead->rlink;
	while (rtemp != NULL)
	{
		report_Node* next = rtemp->rlink;
		free(rtemp->stage);
		free(rtemp);
		rtemp = next;
	}
	free(rhead);
	rhead = NULL;

	while (top->rlink != NULL)
	{
		reportstack_Node* cur = top->rlink;
		top->rlink = cur->rlink;
		free(cur->stage);
		free(cur);
	}
	free(top);
	top = NULL;
	system("04.txt");

	printf("Thank you for using.");
	exit(0);
}

void push(reportstack_Node* top, int* listlen, double battery_use)
{
	if (top == NULL || listlen < 0)
	{
		return;
	}

	reportstack_Node* temp;

	temp = (reportstack_Node*)malloc(sizeof(reportstack_Node));

	temp->stage = (char*)malloc(strlen(pointName[*listlen]) + 1);
	strcpy(temp->stage, pointName[*listlen]);

	(*listlen)++;
	temp->used_battery = battery_use;
	temp->rlink = top->rlink;
	top->rlink = temp;
}
