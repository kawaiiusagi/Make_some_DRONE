#include "drone.h"

int for_check_overlap[Map_size + 1][Map_size + 1] = { 0, };

void start_UI()
{
	printf("-----Program is Starting-----\n");

	char* dots[] = { "",".","..","..." };

	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 4; i++)
		{
			printf("\rLoading%-3s", dots[i]);
			fflush(stdout);
			Sleep(1000);
		}
	}

	printf("\nLoading Complete!\n");
	Sleep(1000);

	int pw_cnt = 0;
	int pw = 1111;
	int pw_buf;

	while (1)
	{
		if (pw_cnt == 5)
		{
			printf("Terminating the program for security reasons.\n");
			exit(0);
		}

		printf("\nPlease enter the password. Access is restricted");
		printf("\n>>> ");

		if (scanf("%d", &pw_buf) != 1 || pw_buf != pw)
		{
			printf("Invalid password. Please try again\n");
			while (getchar() != '\n');
			pw_cnt++;
			continue;
		}
		else {
			break;
		}

		Sleep(500);
		printf("\nVerified.");

	}

	info_UI();

	choose_UI();
}

void print_center(const char* buf, int width)
{
	int len = strlen(buf);
	int pad = (width - len) / 2;

	printf("%*s%s%*s\n", pad, "", buf, pad, "");
}


void input_sys()
{
	system("cls");
	Sleep(500);

	printf("\n");
	printf("  +======================================+\n");
	printf("  |         DRONE MISSION SETUP          |\n");
	printf("  |           [ STEP 1 of 4 ]            |\n");
	printf("  +======================================+\n");

	Sleep(500);

	printf("\n");
	printf("  +--------------------------------------+\n");
	printf("  |  Mission : Waypoint Generation       |\n");
	printf("  |  Origin  : (0, 0) auto-included      |\n");
	printf("  +--------------------------------------+\n");
	printf("\n");

	Sleep(500);

	while (1)
	{

		printf("  How would you like to generate waypoints?\n");
		printf("\n");
		printf("  +-------------------------------------+\n");
		printf("  |  [1]  Random Generation             |\n");
		printf("  |  [2]  Manual Input                  |\n");
		printf("  +-------------------------------------+\n");
		printf("\n");

		int num;
		printf("  >>> ");


		if (scanf("%d", &num) != 1 || (num != 1 && num != 2))
		{
			printf("\n  Please enter a valid value.\n");
			while (getchar() != '\n');
			continue;
		}

		getchar();

		switch (num)
		{
		case 1:
			do_random();
			break;
		case 2:
			do_direct();
			break;
		}
		break;
	}
}

void do_random() {
	int temp_num = 0;

	FILE* fw = NULL;

	while (1)
	{
		printf("\n  How many nodes do you want to add? (Max : 10)\n");
		printf("  >>> ");
		if (scanf("%d", &temp_num) != 1 || temp_num <= 0 || temp_num > 10)
		{

			printf("Please enter a valid value.\n");

			while (getchar() != '\n');

			continue;
		}
		else break;
	}

	printf("\n");

	print_center("Making Coordinates...", 33);
	loading_bar();
	printf("\033[A\r                                      \r");
	printf("\033[A\r                                      \r");
	printf("----------<Result>----------\n");

	getchar();

	int x, y;

	srand((unsigned int)time(NULL));

	fw = fopen("01.txt", "w");
	if (fw == NULL)
	{
		return;
	}

	fprintf(fw, "0 0\n");
	fprintf(stdout, "Coordinate created successfully. Added coordinate => (0, 0)\n");
	for_check_overlap[0][0] = 1;

	for (int i = 0; i < temp_num - 1; i++)
	{
		while (1)
		{
			x = rand() % Map_size + 1;
			y = rand() % Map_size + 1;

			int state = check_coordinate(x, y);

			if (state == 2)
			{
				continue;
			}

			else
			{
				fprintf(fw, "%d %d\n", x, y);
				fprintf(stdout, "Coordinate created successfully. Added coordinate => (%d, %d)\n", x, y);
				break;
			}
		}

	}
	fclose(fw);
	system("01.txt");
}

int check_coordinate(int x, int y)
{
	if (for_check_overlap[x][y] == 1) return 2;
	else
	{
		for_check_overlap[x][y] = 1;
	}
	return 1;

}

void do_direct()
{
	int cnt = 0;

	FILE* fp = NULL;
	fp = fopen("01.txt", "w");

	if (fp == NULL)
	{
		printf("Cannot open the file\n");
		return;
	}

	int x, y;

	int num = 0;

	while (1)
	{

		printf("How many coordinates do you want to add? (MAX : 10)\n");
		printf(">>> ");

		if (scanf("%d", &num) != 1 || num <= 0 || num > 10)
		{
			printf("Please enter a valid value.");
			while (getchar() != '\n');
			continue;
		}
		else break;
	}

	fprintf(fp, "0 0\n");
	fprintf(stdout, "Coordinate created successfully. Added coordinate => (0, 0)\n");
	for_check_overlap[0][0] = 1;

	while (cnt < num - 1)
	{
		printf("Enter X Y [ 1 ~ 100 ] (ex.10 20) >>> ");

		if (scanf("%d %d", &x, &y) != 2 || x <= 0 || y <= 0 || x > 100 || x > 100 || y > 100)
		{
			printf("Please enter a valid value.\n");
			while (getchar() != '\n');
			continue;
		}

		else if (check_coordinate(x, y) == 2)
		{
			printf("Duplicate coordinate detected. Please enter again.\n");
			while (getchar() != '\n');
			continue;
		}

		else
		{
			fprintf(fp, "%d %d\n", x, y);
			fprintf(stdout, "Coordinate created successfully. Added coordinate => (%d, %d)\n", x, y);
			cnt++;
		}

	}
	fclose(fp);
	system("01.txt");

}

void loading_bar() {
	int bar_size = 5;
	int total = 30;

	for (int r = 0; r < 2; r++) {
		for (int i = 0; i <= total - bar_size; i++) {

			printf("\r[");

			for (int j = 0; j < i; j++) printf(" ");
			for (int j = 0; j < bar_size; j++) printf("=");
			for (int j = i + bar_size; j < total; j++) printf(" ");

			printf("]");
			fflush(stdout);
			Sleep(50);
		}
		for (int i = total - bar_size; i >= 0; i--) {

			printf("\r[");

			for (int j = 0; j < i; j++) printf(" ");
			for (int j = 0; j < bar_size; j++) printf("=");
			for (int j = i + bar_size; j < total; j++) printf(" ");

			printf("]");
			fflush(stdout);
			Sleep(50);
		}
	}
	printf("\n");
}

void info_UI()
{
	system("cls");
	Sleep(200);

	printf("============================================================\n");
	printf("|                                                           |\n");
	printf("|                                                           |\n");
	printf("|                                                           |\n");
	printf("|                ___    ____   ____  __  _  ____            |\n");
	printf("|               |  _ \\ |  _ \\ |    ||  \\| ||  __|           |\n");
	printf("|               | | | || |_) || || || . ` || |__            |\n");
	printf("|               | |_/ ||  _ < | || || |\\  || |__            |\n");
	printf("|               |____/ |_| \\_\\|____||_| \\_||____|           |\n");
	printf("|                                                           |\n");
	printf("|                     S I M U L A T O R                     |\n");
	printf("|                                                           |\n");
	printf("============================================================\n");
	Sleep(500);

	printf("\n");
	printf("   Autonomous Drone Path Planning & Battery Simulator\n");
	printf("   Version 1.1.0\n");
	printf("\n");

	Sleep(500);

	printf("   [>] Waypoint generation ( random / manual )\n");
	Sleep(150);
	printf("   [>] Optimal path calculation\n");
	Sleep(150);
	printf("   [>] Battery consumption simulation\n");
	Sleep(150);
	printf("   [>] State monitoring --->  NORMAL / ERROR / FAILURE / RECOVERY\n");
	Sleep(150);

	printf("\n");
	Sleep(300);

	char bbuf[10];

	printf("   Press Enter to continue...");
	while (getchar() != '\n');
	getchar();	
	fflush(stdout);

	//to ask how to solve buffer problem using std.
	//fflush(stdout);



}

void choose_UI()
{
	char buf[30];

	system("cls");
	Sleep(500);

	printf("  +================================================+\n");
	printf("  |        DRONE SIMULATION SYSTEM v1.0           |\n");
	printf("  +================================================+\n");

	Sleep(500);
	printf("\n");
	printf("  +------------------------------------------------+\n");
	printf("  |  [1]  Waypoint Generation                      |\n");
	printf("  |  [2]  Optimal Path Calculation                 |\n");
	printf("  |  [3]  Battery Consumption Simulation           |\n");
	printf("  |  [4]  State Monitoring                         |\n");
	printf("  +------------------------------------------------+\n");
	printf("\n");

	Sleep(500);

	while (1)
	{
		printf("  Select a stage to begin  [1-4]\n");
		printf("  Type 'help' for detailed descriptions\n");
		printf("  If you want to quit, please enter 'q'\n");

		printf("\n");
		printf("  >>> ");
		scanf("%s", buf);

		if (strcmp(buf, "help") == 0)
		{
			printf("\n");
			printf("  +================================================+\n");
			printf("  |              DETAILED DESCRIPTION              |\n");
			printf("  +================================================+\n");
			printf("\n");
			printf("  [1] Waypoint Generation\n");
			printf("      Define mission destinations either randomly or\n");
			printf("      manually to establish the drone's flight targets.\n");
			printf("\n");
			printf("  [2] Optimal Path Calculation\n");
			printf("      Compute the most efficient route connecting all\n");
			printf("      waypoints to minimize travel distance and time.\n");
			printf("\n");
			printf("  [3] Battery Consumption Simulation\n");
			printf("      Model and track the drone's energy usage\n");
			printf("      throughout the mission based on calculated path.\n");
			printf("\n");
			printf("  [4] State Monitoring\n");
			printf("      Continuously observe and report the drone's\n");
			printf("      status : NORMAL / ERROR / FAILURE / RECOVERY\n");
			printf("\n");
			continue;
		}
		else if (strcmp(buf, "1") == 0)
		{
			input_sys();
			break;
		}
		else if (strcmp(buf, "2") == 0)
		{
			read_list();
			break;
		}
		else if (strcmp(buf, "3") == 0)
		{
			read_node();
			break;
		}
		else if (strcmp(buf, "4") == 0)
		{
			make_final();
			break;
		}
		else if (strcmp(buf, "q") == 0)
		{
			printf("\n===<Thank you for using>===\n");
			exit(0);
		}
		else {
			printf("Invalid input. Please enter 1 - 4 or help.\n\n");
			while (getchar() != '\n');
			continue;

		}
	}

}
