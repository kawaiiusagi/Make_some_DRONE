#include "drone.h"

reportstack_Node* top = NULL;
report_Node* rhead = NULL;
FILE* fp1 = NULL;
double Maxbattery = 250, battery = 250;
double used_battery = 0.0;
char* droneState[] = { "NORMAL", "ERROR", "FAILURE", "RECOVERY" };//state : 0 정상, 1 배터리 부족, 2 기체고장, 3 역추척
char* droneEvent[] = { "NORMAL", "Mission fail", "Recovery tracking" }; //event : 0 정상, 1 미션실패, 2 역추적
char* pointName[] = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
int listlen = 0;
int box;

void fread03() //03.txt읽어와서 리스트 만들기
{
	fp1 = fopen("03.txt", "r");
	top = (reportstack_Node*)malloc(sizeof(reportstack_Node));
	rhead = (report_Node*)malloc(sizeof(report_Node));
	top->rlink = NULL;
	rhead->rlink = NULL;

	if (fp1 == NULL)
	{
		printf("Cannot read the file\n");
		return;
	}
	char stage[6];
	double dist;
	double K;
	double battery_use;
	int state;
	int event;

	report_Node* pre = rhead;
	while (pre->rlink != NULL)
	{
		pre = pre->rlink;
		listlen++;
	}
	box = listlen;
	pre = rhead;
	for (int i = 0; i < 4; i++)
	{
		while ((fscanf(fp1, "%s %lf %lf %lf ", stage, &dist, &K, &battery_use)) == 4)
		{
			report_Node* node = (report_Node*)malloc(sizeof(report_Node));

			if (pre == NULL)
			{
				node->stage = (char*)malloc(strlen(stage) + 1);
				strcpy(node->stage, stage);
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
				push(top, &listlen, battery_use);
			}

			else
			{
				node->stage = (char*)malloc(strlen(stage) + 1);
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



				if (state == 1)
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
				push(top, &listlen, battery_use);
			}
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

	const char* header_format = "%-12s %-10s %-8s %-15s %-15s %-12s %-20s %-20s\n";
	const char* row_format = "%-12s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %-20s %-20s\n";
	const char* track_format = "%-12s %-10s %-8s %-15s %-15.1f %-12s %-20s %-20s\n";

	fprintf(fp1, header_format, "구간", "거리(m)", "K", "배터리 소모량", "누적 소모량", "상태", "이벤트", "안전회귀 경로");
	fprintf(stdout, header_format, "구간", "거리(m)", "K", "배터리 소모량", "누적 소모량", "상태", "이벤트", "안전회귀 경로");

	fprintf(fp1, "---------------------------------------------------------------------------------------------------------\n");
	fprintf(stdout, "---------------------------------------------------------------------------------------------------------\n");

	report_Node* temp = rhead->rlink;
	used_battery = 0.0;

	while (temp != NULL)
	{
		// 1. 현재 노드는 (정상이든 에러든) 일단 한 줄 무조건 출력합니다.
		used_battery += temp->battery_use;
		fprintf(fp1, row_format, temp->stage, temp->dist, temp->K, temp->battery_use,
			used_battery, droneState[temp->state], droneEvent[temp->event], "-");
		fprintf(stdout, row_format, temp->stage, temp->dist, temp->K, temp->battery_use,
			used_battery, droneState[temp->state], droneEvent[temp->event], "-");

		// 2. 출력 후, 방금 출력한 노드의 상태가 1(ERROR)이거나 2(FAILURE)라면 바로 스택을 꺼내며 회귀(역추적) 시작
		if (temp->state == 1 || temp->state == 2) 
		{
			while (top->rlink != NULL) 
			{
				reportstack_Node* current_pos = top->rlink;
				
				// 다음 노드(rlink)가 있을 때만 회귀 경로를 출력 (마지막 A 도착 시 빈 줄 출력 방지)
				if (current_pos->rlink != NULL) {
					char stage_buf[30];
					char path_buf[30];
					
					sprintf(stage_buf, "%s%s(회귀)", current_pos->stage, current_pos->rlink->stage);
					sprintf(path_buf, "%s -> %s", current_pos->stage, current_pos->rlink->stage);

					// 회귀 시 배터리 누적 (스택에 저장된 해당 구간의 소모량 가산)
					used_battery += current_pos->used_battery; 

					fprintf(fp1, track_format, stage_buf, "-", "-", "-", used_battery,
						"RECOVERY", "Recovery tracking", path_buf);
					fprintf(stdout, track_format, stage_buf, "-", "-", "-", used_battery,
						"RECOVERY", "Recovery tracking", path_buf);
				}

				// 스택에서 꺼내기 (Pop) 및 메모리 해제
				reportstack_Node* del = current_pos;
				top->rlink = current_pos->rlink;
				free(del->stage); // 동적 할당된 stage 문자열 먼저 반환[cite: 1]
				free(del);
			}
			break; // 회귀를 모두 마쳤으므로 전체 반복문 완전히 종료
		}

		temp = temp->rlink;
	}
	fclose(fp1);
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
