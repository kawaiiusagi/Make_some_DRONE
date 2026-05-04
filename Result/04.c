#include "drone.h"

reportstack_Node* top = NULL;
report_Node* rhead = NULL;
FILE* fp1 = NULL;
double Maxbattery=800,battery = 800; 
double used_battery = 0.0;
char* droneState[] = { "NORMAL", "ERROR", "FAILURE", "RECOVERY"};//state : 0 정상, 1 배터리 부족, 2 기체고장, 3 역추척
char* droneEvent[] = {"NORMAL", "Mission fail", "Recovery tracking"}; //event : 0 정상, 1 미션실패, 2 역추적
char* pointName[] = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
int listlen = 0;
int box;

void fread03() //03.txt읽어와서 리스트 만들기
{
	fp1 = fopen("03.txt","r");
	top=(reportstack_Node*)malloc(sizeof(reportstack_Node));
	rhead=(report_Node*)malloc(sizeof(report_Node));
	top->rlink=NULL;
	rhead->rlink=NULL;
	
	if (fp1 == NULL)
	{
		printf("Cannot read the file\n");
		return;
	}
	char stage[5];
	double dist;
	double K;
	double battery_use;
	int state;
	int event;

	report_Node* pre = rhead;
	while (pre != NULL)
	{
		pre = pre->rlink;
		listlen++;
	}
	box = listlen;
	pre = rhead;
	while ((fscanf(fp1, "%s %lf %lf %lf ", stage, &dist, &K, &battery_use)) == 4)
	{
		report_Node* node = (report_Node*)malloc(sizeof(report_Node));

		if (pre == NULL)
		{
			strcpy(node->stage,stage);
			node->dist = dist;
			node->K = K;
			node->battery_use = battery_use;
			node->state = 0;
			node->event = 0;
			node->rlink = NULL;

			if (rhead->rlink == NULL)
			{
				rhead->rlink = node;
			}
			pre = node;
			battery = battery - battery_use;
			push(top, &listlen,battery_use);
		}

		else
		{
			strcpy(node->stage, stage);
			node->dist = dist;
			node->K = K;
			node->battery_use = battery_use;

			if (battery < Maxbattery / 2)
			{
				if (pre->state == 1 || pre->state == 2)
				{
					state = 3;
				}
				else
				{
					state = 1;
				}
			}

			else
			{
				state = 0;
			}



			if (state==1)
			{
				event = state;
			}
			else if (state == 3)
			{
				event = 2;
			}
			else
			{
				event = 0;
			}

			node->state = state;
			node->event = event;
			node->rlink = NULL;
			pre->rlink = node;
			pre = node;
			battery = battery - battery_use;
			push(top, &listlen,battery_use);
		}
	}
	fclose(fp1);
	
}

void reporting()
{
	fp1 = fopen("04.txt", "w");
	if (fp1 == NULL)
	{
		printf("Cannot read the file\n");
		return;
	}
	reportstack_Node* pre = top->rlink;
	report_Node* temp = rhead->rlink;
	temp = rhead->rlink;
	while (temp != NULL)
	{
		
		if (temp->state != 0&&temp->state!=1)
		{
			used_battery = used_battery + (pre->used_battery);
			fprintf(fp1, "%s%s(tracking)							%.1f %s %s %s -> %s\n",pre->stage,pre->rlink->stage,used_battery, droneState[temp->state], droneEvent[temp->event], pre->rlink->stage, pre->stage);
			fprintf(stdout, "%s%s(tracking)							   %.1f %s %s %s -> %s\n", pre->stage, pre->rlink->stage, used_battery, droneState[temp->state], droneEvent[temp->event], pre->rlink->stage, pre->stage);
			top = pre->rlink;
			free(pre);
		}
		else if (temp->state == 1)
		{
			used_battery = used_battery + (temp->battery_use);
			fprintf(fp1, "%s %.1f %.1f %.1f %.1f %s %s",temp->stage,temp->dist,temp->K,temp->battery_use,used_battery,droneState[temp->state],droneEvent[temp->event]);
			fprintf(stdout, "%s %.1f %.1f %.1f %.1f %s %s",temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], droneEvent[temp->event]);
			int i = 0;
			do
			{
				fprintf(fp1, "%s ", pre->stage);
				fprintf(stdout, "%s ", pre->stage);
				i++;
				if (i < box)
				{
					fprintf(fp1, "-> ");
					fprintf(stdout, "-> ");
				}
				pre=pre->rlink;
			}while (i < box);
		}

		if(temp->state==0)
		{
			used_battery = used_battery + (temp->battery_use);
			fprintf(fp1, "%s %.1f %.1f %.1f %.1f %s %s", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], droneEvent[temp->event]);
			fprintf(stdout, "%s %.1f %.1f %.1f %.1f %s %s", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], droneEvent[temp->event]);
			fprintf(fp1, "		\n");
			fprintf(stdout, "		\n");
		}
		temp = temp->rlink;
		pre = top->rlink;
	}
}

void push(reportstack_Node* top,int* listlen,double battery_use)
{
	if (top == NULL||listlen<0)
	{
		return;
	}

	reportstack_Node* temp;
	temp = (reportstack_Node*)malloc(sizeof(reportstack_Node));
	strcpy(temp->stage,pointName[*listlen]);
	(*listlen)--;
	temp->used_battery = battery_use;
	temp->rlink = top->rlink;
	top->rlink = temp;
}
