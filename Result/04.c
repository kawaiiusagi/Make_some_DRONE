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

	// 표 형식 출력을 위한 포맷 지정 (이 부분이 빠져서 C2065 에러가 났던 겁니다)
	const char* header_format = "%-12s %-10s %-8s %-15s %-15s %-12s %-20s %-20s\n";
	const char* row_format = "%-12s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %-20s %-20s\n";
	const char* track_format = "%-12s %-10s %-8s %-15s %-15.1f %-12s %-20s %-20s\n";

	// 1. 테이블 헤더(머리글) 출력
	fprintf(fp1, header_format, "구간", "거리(m)", "K", "배터리 소모량", "누적 소모량", "상태", "이벤트", "안전회귀 경로");
	fprintf(stdout, header_format, "구간", "거리(m)", "K", "배터리 소모량", "누적 소모량", "상태", "이벤트", "안전회귀 경로");

	fprintf(fp1, "---------------------------------------------------------------------------------------------------------\n");
	fprintf(stdout, "---------------------------------------------------------------------------------------------------------\n");

	report_Node* temp = rhead->rlink;
	used_battery = 0.0; // 누적 소모량 초기화

	while (temp != NULL)
	{
		// 고장(FAILURE)이나 역추적(RECOVERY) 상태를 만나면 회귀 시작
		if (temp->state == 2 || temp->state == 3)
		{
			// 스택(top)에 쌓인 경로를 역순으로 모두 꺼내서(Pop) 출력
			while (top->rlink != NULL)
			{
				reportstack_Node* current_pos = top->rlink;

				char stage_buf[30];
				char path_buf[30];

				// 다음 노드가 있으면 경로 표시 (예: D -> C)
				if (current_pos->rlink != NULL) {
					sprintf(stage_buf, "%s%s(회귀)", current_pos->stage, current_pos->rlink->stage);
					sprintf(path_buf, "%s -> %s", current_pos->stage, current_pos->rlink->stage);
				}
				else {
					// 마지막 노드면 출발지에 도착한 것
					sprintf(stage_buf, "%s(도착)", current_pos->stage);
					strcpy(path_buf, "Home");
				}

				// 역추적 시에도 배터리는 계속 소모되므로 스택에 있던 값 누적
				used_battery += current_pos->used_battery;

				fprintf(fp1, track_format, stage_buf, "-", "-", "-", used_battery,
					"RECOVERY", "Recovery tracking", path_buf);
				fprintf(stdout, track_format, stage_buf, "-", "-", "-", used_battery,
					"RECOVERY", "Recovery tracking", path_buf);

				// 스택에서 노드를 꺼냈으니 메모리 해제 (Pop)
				reportstack_Node* del = current_pos;
				top->rlink = current_pos->rlink;
				free(del->stage); // push할 때 동적 할당했으므로 먼저 free
				free(del);
			}
			break; // 스택을 다 비우고 회귀를 끝냈으므로 반복문 완전 종료
		}
		else
		{
			// 일반 전진 기록 출력 (NORMAL, ERROR)
			used_battery += temp->battery_use;
			fprintf(fp1, row_format, temp->stage, temp->dist, temp->K, temp->battery_use,
				used_battery, droneState[temp->state], droneEvent[temp->event], "-");
			fprintf(stdout, row_format, temp->stage, temp->dist, temp->K, temp->battery_use,
				used_battery, droneState[temp->state], droneEvent[temp->event], "-");
		}

		temp = temp->rlink;
	}
	fclose(fp1);
	//system("notepad.exe 04.txt");
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
