#include "OMG.h"

// remove -> only for node delete
// delete -> node delete plus add blokced pos

Stack* top = NULL;

int block_pos[100][2];
int block_cnt = 0;
int node_count = 0;

void add_blocked_pos(int x, int y)
{
	block_pos[block_cnt][0] = x; //2차원 좌표 활용
	block_pos[block_cnt][1] = y;
	block_cnt++;

	char log[128];
	sprintf(log, "!BLOCKED! Position : (%d, %d) blocked by enemy.", x, y); // 로그를 기록하기 위해 snprintf()로 출력 내용을 버퍼에 저장하고 로그에 기록.
	save_log(log);
}

int is_blocked_pos(int x, int y)
{
	for (int i = 0; i < block_cnt; i++)
	{
		if (block_pos[i][0] == x && block_pos[i][1] == y)
		{
			return 1;
		}
	}
	return 0;
}

Node* start_program()
{
	srand((unsigned int)time(NULL)); // 랜덤 함수를 골고루 섞어주는 함수. rand를 썼을 때 다른 결과를 도출하기 위하여 사용.

	Node* head = (Node*)malloc(sizeof(Node));

	if (head == NULL)
	{
		printf("Memory allocation failed.\n");
		return NULL;
	}

	head->x = 0;
	head->y = 0;
	head->link_pos = 0;
	head->is_blocked = 0;
	head->content = "Start";
	head->rlink = NULL;
	head->llink = NULL;

	node_count = 1;

	printf("---<Program started>---\n");
	printf("!!Base Point!! Position : (%d, %d)\n\n", head->x, head->y);

	int temp;

	while (1)
	{
		printf("-----<please enter the number>-----\n");
		printf("Create Node : 1\n");
		printf("Show List : 2\n");
		printf("Move Drone : 3\n");
		printf("Remove Node : 4\n");
		printf("Exit : 0\n");

		if (scanf("%d", &temp) != 1)
		{
			printf("Invalid Input: Please enter a number.\n");
			getchar(); //while 문을 통한 무한 반복의 방지를 위해 항상 버퍼를 비움.
			continue;
		}
		getchar();

		if (temp == 0) return head;

		switch (temp)
		{
		case 1:
			Create_Node(head);
			break;
		case 2:
			show_list(head);
			break;
		case 3:
			move_drone(head);
			break;
		case 4:
		{
			show_list(head);
			int pos;
			printf("Enter node number to remove : ");
			if (scanf("%d", &pos) != 1)
			{
				printf("Invalid input.\n");
				getchar();
				break;
			}
			getchar();
			remove_node(&head, pos);
			break;
		}
		default:
			printf("Please enter again. \n");
			break;
		}
	}
}

void get_node_nums(Node* head) // 노드들의 넘버를 한 번에 관리. 정렬하게 될 때마다 하면 불편해진다.
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

void sort_nodes(Node* head, Node* new_node) // 노드를 정렬하는 함수를 따로 구현. 노드를 삭제, 삽입 할 때마다 계속 구현을 하게 되면 실행 시간이 느려질 수 있음.
{
	Node* cur = head;

	while (cur->rlink != NULL && cur->rlink->x < new_node->x)
	{
		cur = cur->rlink;
	}

	new_node->rlink = cur->rlink;
	new_node->llink = cur;

	if (cur->rlink != NULL)
	{
		cur->rlink->llink = new_node;
	}

	cur->rlink = new_node;
}

void Create_Node(Node* head)
{
	int temp;

	while (1)
	{
		printf("Randomly? : 1 or Directly? : 2 or Read on File : 3\n");
		if (scanf("%d", &temp) != 1)
		{
			printf("Invalid input.\n");
			getchar();
			continue;
		}
		getchar();

		if (temp != 1 && temp != 2 && temp != 3)
		{
			printf("Invalid input.\n");
			continue;
		}
		break;
	}

	int x, y;
	int number = 1;

	if (temp == 1)
	{
		while (1)
		{
			printf("How many nodes do you want to add? : ");
			if (scanf("%d", &number) != 1)
			{
				printf("Invalid input.\n");
				getchar();
				continue;
			}
			getchar();
			if (number < 1) continue;
			break;
		}

		for (int i = 0; i < number; i++)
		{
			while (1)
			{
				x = rand() % Map_size + 1;
				y = rand() % Map_size + 1;

				if (is_blocked_pos(x, y)) continue; // 차단 노드 방지.

				char* N = check_node(head, x, y); // 노드 중복 등 체크
				if (strcmp(N, "Overlap") == 0) continue;

				break;
			}

			Node* new_node = (Node*)malloc(sizeof(Node));
			if (new_node == NULL) return;

			new_node->x = x;
			new_node->y = y;
			new_node->is_blocked = 0;
			new_node->content = "Node";
			new_node->rlink = NULL;
			new_node->llink = NULL;

			sort_nodes(head, new_node);
			get_node_nums(head);

			printf("Node created | Position : ( %d, %d ) | ID : %d\n",
				new_node->x, new_node->y, new_node->link_pos);
		}
	}
	else if (temp == 2)
	{
		while (1)
		{
			printf("Enter x (1~%d) : ", Map_size);

			if (scanf("%d", &x) != 1)
			{
				getchar(); continue;
			}

			printf("Enter y (1~%d) : ", Map_size);

			if (scanf("%d", &y) != 1)
			{ 
				getchar(); continue; 
			}
			getchar();

			if (x < 1 || x > Map_size || y < 1 || y > Map_size)
			{
				printf("Invalid Input.\n");
				continue;
			}

			if (is_blocked_pos(x, y))
			{
				printf("Blocked position.\n");
				continue;
			}

			char* N = check_node(head, x, y);

			if (strcmp(N, "Overlap") == 0)
			{
				printf("Overlap detected.\n");
				continue;
			}
			break;
		}

		Node* new_node = (Node*)malloc(sizeof(Node));
		if (new_node == NULL) return;

		new_node->x = x;
		new_node->y = y;
		new_node->is_blocked = 0;
		new_node->content = "Node";
		new_node->rlink = NULL;
		new_node->llink = NULL;

		sort_nodes(head, new_node);
		get_node_nums(head);

		printf("Node created | Pos : (%d, %d) | ID : %d\n",
			new_node->x, new_node->y, new_node->link_pos);
	}

	else if (temp == 3)
	{
		FILE* fp = fopen("coordinates.txt", "r");
		if (fp == NULL)
		{
			printf("Error: Could not open.\n");
			return;
		}

		int file_id, file_x, file_y;
		int count = 0;

		while (fscanf(fp, "%d ( %d, %d )", &file_id, &file_x, &file_y) == 3) //이건 문자 형태에 따라 달라질 듯? 그걸로 유동적으로 전환
		{
			// 해당 좌표가 유효한지 검사
			if (file_x < 1 || file_x > Map_size || file_y < 1 || file_y > Map_size) continue;
			if (is_blocked_pos(file_x, file_y)) continue;

			// 해당 좌표가 중복인지 검사
			char* N = check_node(head, file_x, file_y);
			if (strcmp(N, "Overlap") == 0) continue;

			// 노드 생성
			Node* new_node = (Node*)malloc(sizeof(Node));
			if (new_node == NULL) break;

			new_node->x = file_x;
			new_node->y = file_y;
			new_node->is_blocked = 0;
			new_node->content = "FileNode"; //노드 이름
			new_node->rlink = NULL;
			new_node->llink = NULL;

			// 정렬
			sort_nodes(head, new_node);

			// 넘버링
			get_node_nums(head);

			printf("Node created | Pos: (%d, %d) | ID: %d\n", new_node->x, new_node->y, new_node->link_pos);
			count++;
		}

		fclose(fp);
		if (count == 0) printf("No valid data loaded from file.\n");
		else printf("Successfully loaded %d nodes.\n", count);
		}
}

char* check_node(Node* head, int x, int y)
{
	if (head == NULL)
	{
		return "Empty";
	}

	Node* temp = head;

	while (temp != NULL)
	{
		if (temp->x == x && temp->y == y)
		{
			return "Overlap";
		}
		temp = temp->rlink;
	}
	return "Nothing";
}

void show_list(Node* head)
{
	Node* temp = head;
	printf("\n");

	while (temp != NULL)
	{
		printf("+------------------+");
		if (temp->rlink != NULL)
		{
			printf("--->");
		}
		temp = temp->rlink;
	}
	printf("\n");

	temp = head;
	while (temp != NULL)
	{
		printf("| [%2d] %-12s|", temp->link_pos, temp->content); //시각적으로 %-12s를 이용하여 정렬. 12칸 중 사용한 칸들을 왼쪽으로 정렬한다는 의미이다.
		if (temp->rlink != NULL)
		{
			printf("    ");
		}

		temp = temp->rlink;
	}
	printf("\n");

	temp = head; //위에 있는 선 출력하려면 계속 초기화해줘야함.
	while (temp != NULL)
	{
		printf("| (%2d, %2d)         |", temp->x, temp->y); // 항상 고정되어있는 값이므로 사용 안해도 된다. -12%s 의미하는 것.
		if (temp->rlink != NULL)
		{
			printf("    ");
		}
		temp = temp->rlink;
	}
	printf("\n");

	temp = head;
	while (temp != NULL)
	{
		printf("+------------------+");
		if (temp->rlink != NULL)
		{
			printf("--->");
		}
		temp = temp->rlink;
	}

	printf("\n\n      [ Blocked Positions ]      \n");

	if (block_cnt == 0)
	{
		printf("  -> None\n");
	}
	else
	{
		for (int i = 0; i < block_cnt; i++)
		{
			printf("  | XX BLOCK AT (%2d, %2d) XX |\n", block_pos[i][0], block_pos[i][1]);
		}
	}
	printf("__________________________\n\n");
}

void delete_node(Node** head, int pos)
{
	Node* find = *head;
	while (find != NULL && find->link_pos != pos)
	{
		find = find->rlink;
	}

	if (find == NULL) return;

	add_blocked_pos(find->x, find->y);
	printf("Node [%d] blocked and removed!\n", find->link_pos);

	if (find->llink != NULL)
	{
		find->llink->rlink = find->rlink;
	}

	if (find->rlink != NULL)
	{
		find->rlink->llink = find->llink;
	}

	if (find == *head)
	{
		*head = find->rlink;
	}

	free(find);
	get_node_nums(*head);
}

void remove_node(Node** head, int pos)
{
	if (pos == 0)
	{
		printf("Cannot remove !start node!\n");
		return;
	}

	Node* find = *head;
	while (find != NULL && find->link_pos != pos) //and 연산자라 안걸러질 수 있음
	{
		find = find->rlink;
	}

	if (find == NULL)
	{
		printf("404 Not found.\n");
		return;
	}

	if (find->llink != NULL) //따라서 뒤에 널 검사
	{
		find->llink->rlink = find->rlink;
	}

	if (find->rlink != NULL)
	{
		find->rlink->llink = find->llink;
	}

	free(find);
	get_node_nums(*head); // 지운 후 다시 노드 정렬
}

void push_history(int x, int y, int pos)
{
	Stack* new_node = (Stack*)malloc(sizeof(Stack));
	if (new_node == NULL) return;

	new_node->x = x;
	new_node->y = y;
	new_node->link_pos = pos;
	new_node->next = top;
	top = new_node;
}

Stack* pop_history()
{
	if (top == NULL) return NULL;
	Stack* temp = top;
	Stack* data = (Stack*)malloc(sizeof(Stack));
	*data = *temp;
	top = temp->next;
	free(temp);
	return data;	
}

void clear_history()
{
	while (top != NULL)
	{
		Stack* temp = top;
		top = top->next;
		free(temp);
	}
}

void move_drone(Node* head)
{
	if (head == NULL) return;

	system("cls");
	printf("\nMISSION START\n");

	int drone_battery = rand() % 26 + 50;
	Node* last = head;
	while (last->rlink != NULL) { last = last->rlink; }

	int total_dist = 0;
	Node* info = head;
	while (info->rlink != NULL)
	{
		total_dist += calc_distance(info->x, info->y, info->rlink->x, info->rlink->y);
		info = info->rlink;
	}

	printf("Battery: %d | Distance: %d\n", drone_battery, total_dist);
	show_list(head);

	Node* cur = head;
	int is_start = 1;

	clear_history();

	while (cur != NULL)
	{
		Sleep(700);
		printf("Node [%2d] (%2d,%2d) | Battery: %d\n", cur->link_pos, cur->x, cur->y, drone_battery);

		push_history(cur->x, cur->y, cur->link_pos);

		if (is_start) is_start = 0;
		else
		{
			if (check_explode())
			{
				mission_log(0, cur->link_pos, "Battery Explosion during advance");
				return;
			}

			if (check_event())
			{
				printf("\n[!] Enemy attack! Drone damaged. Return to Home.\n");

				int cur_x = cur->x, cur_y = cur->y, old_pos = cur->link_pos;
				Node* next_target = cur->rlink;

				delete_node(&head, old_pos);
				// 현재 위치가 삭제되었으므로 UI 갱신
				show_list(head);

				int x, y;
				int detour_choice;

				while (1)
				{
					printf("New node | 1: Randomly? or 2: Directly? : ");
					if (scanf("%d", &detour_choice) != 1) 
					{
						getchar();
						continue; 
					}
					getchar();

					if (detour_choice != 1 && detour_choice != 2) continue;
					break;
				}

				if (detour_choice == 1)
				{
					int next_x, next_y;
					if (next_target != NULL) {
						next_x = next_target->x;
						next_y = next_target->y;
					}
					else {
						next_x = cur_x + (rand() % 3 - 1); //마지막 노드이므로 임의로 추가.
						next_y = cur_y + (rand() % 3 - 1);
					}

					int min_x = (cur_x < next_x) ? cur_x : next_x;
					int max_x = (cur_x > next_x) ? cur_x : next_x; // 최댓값과 최소값을 찾는 알고리즘 ?연산자로. 
					x = (min_x == max_x) ? min_x : (rand() % (max_x - min_x + 1) + min_x);

					int min_y = (cur_y < next_y) ? cur_y : next_y;
					int max_y = (cur_y > next_y) ? cur_y : next_y;
					y = (min_y == max_y) ? min_y : (rand() % (max_y - min_y + 1) + min_y);
				}
				else
				{
					while (1)
					{
						printf("Enter x (1~%d): ", Map_size);

						if (scanf("%d", &x) != 1) 
						{
							getchar();
							continue; 
						}
						printf("Enter y (1~%d): ", Map_size);

						if (scanf("%d", &y) != 1) 
						{ 
							getchar(); 
							continue; 
						}

						getchar();
						break;
					}
				}

				// 새로 지정한 노드를 다시 리스트에 추가.
				Node* detour_node = (Node*)malloc(sizeof(Node));
				if (detour_node != NULL) {
					detour_node->x = x;
					detour_node->y = y;
					detour_node->is_blocked = 0;
					detour_node->content = "Detour";
					detour_node->rlink = NULL;
					detour_node->llink = NULL;

					sort_nodes(head, detour_node); // 리스트에 삽입
					get_node_nums(head);           // 넘버링
					show_list(head);               // 보여주기
				}

				// 우회 지점으로 이동 처리
				int b_cost = calc_distance(cur_x, cur_y, x, y) * Battery_per_move;
				if (drone_battery < b_cost)
				{
					mission_log(0, old_pos, "Battery exhausted during detour");
					return;
				}
				drone_battery -= b_cost;
				printf("Moved to Detour Point (%d, %d). Battery: %d\n", x, y, drone_battery);

				// 방금 추가한 우회 노드 위치도 스택에 저장 (복귀 경로에 포함)
				if (detour_node != NULL) {
					push_history(detour_node->x, detour_node->y, detour_node->link_pos);
				}

				printf("!!! EMERGENCY RETURN INITIATED !!!\n");

				// 현재 스택의 가장 위(방금 넣은 우회 지점)는 리턴 시작점이므로 제외
				pop_history();

				// 스택을 이용하여 복귀
				int curr_x = x, curr_y = y;

				while (1)
				{
					Stack* prev = pop_history();
					if (prev == NULL) break;

					Sleep(500);

					if (check_explode())
					{
						printf("\n[!!!!!] CRITICAL ERROR: BATTERY EXPLODED DURING RETURN! [!!!!!]\n");
						printf("Coordinates : (%d, %d)\n", prev->x, prev->y);
						mission_log(0, prev->link_pos, "Self-destructed during return");
						free(prev);
						clear_history();
						return;
					}

					int b_cost = calc_distance(curr_x, curr_y, prev->x, prev->y) * Battery_per_move;
					if (drone_battery < b_cost)
					{
						printf("Battery exhausted during return!\n");
						mission_log(0, prev->link_pos, "Battery exhausted during return");
						free(prev);
						clear_history();
						return;
					}
					drone_battery -= b_cost;
					curr_x = prev->x;
					curr_y = prev->y;

					printf("<- Returning to: Node [%d] (%d, %d) | Battery: %d\n", prev->link_pos, prev->x, prev->y, drone_battery);
					free(prev);
				}

				printf("\n[SAFE] Drone returned to Start Point successfully.\n");
				mission_log(0, 0, "Mission Aborted (Safe Return)");
				return;
			}
		}

		if (cur->rlink != NULL)
		{
			int b_need = calc_distance(cur->x, cur->y, cur->rlink->x, cur->rlink->y) * Battery_per_move;
			if (drone_battery < b_need)
			{
				mission_log(0, cur->link_pos, "Insufficient battery");
				return;
			}
			drone_battery -= b_need;
			cur = cur->rlink;
		}
		else
		{
			printf("SUCCESS!\n");
			mission_log(1, cur->link_pos, "None");
			return;
		}
	}
}

int calc_distance(int x1, int y1, int x2, int y2)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1); // 절댓값 반환하는 함수
	return dx + dy;
}

int check_event() 
{ 
	return (rand() % 100) < Enemy_spotted; 
}
int check_explode() 
{
	return (rand() % 100) < Bomb; 
}

/*
mission_log(1, 4, NULL)
	│
	├─ snprintf로 "[MISSION SUCCESS]...5...4" 조립
	│
	└─ save_log(log) 호출
			│
			├─ time()      → 현재 시각을 숫자로
			├─ localtime() → 숫자를 년/월/일/시/분/초로 분해
			└─ fprintf()   → [시간] + 메시지 → 파일에 기록
*/

void save_log(char* msg)
{
	FILE* fp = fopen("mission_log.txt", "a");
	if (fp == NULL) return;

	time_t now = time(NULL); //현재 시간을 기록. NULL을 넣는 이유는 반환값만 넣어달라는 의미이다.
	struct tm* t = localtime(&now); // 숫자들을 구조체로 변환.

	fprintf(fp, "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
		t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec, msg); //메인 부분
	fclose(fp);
}

void mission_log(int success, int node_id, char* reason)
{
	char log[512];
	if (success)
	{
		sprintf(log, "[MISSION SUCCESS] Total Nodes in Path: %d | Target Reached at Node [%d]", node_count, node_id);
	}
	else
	{
		sprintf(log, "[MISSION FAILED] Total Nodes in Path: %d | Failed at Node [%d] | Reason: %s", node_count, node_id, reason);
		// 문자열을 버퍼에 저장 + 오버플로우 방지 + 출력 하느랴고
	}

	printf("\n--- FINAL LOG ---\n%s\n-----------------\n", log);
	save_log(log);
}

void free_all(Node* head)
{
	Node* temp = head;
	while (temp != NULL)
	{
		Node* next = temp->rlink;
		free(temp);	
		temp = next;
	}
}
