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


	input_sys();
}

void print_center(const char* buf, int width)
{
	int len = strlen(buf);
	int pad = (width - len) / 2;

	printf("%*s%s%*s\n", pad, "", buf, pad, "");
}


void input_sys()
{
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

	}
	while (1)
	{
		Sleep(500);
		printf("\nVerified.");

		Sleep(500);

		info_UI();

		int num;
		printf("\n----------<First Step>----------\n");
		printf("Step 1 : Make Coordinate!\n");
		printf("\nPlease enter the number of the option you want to select\n");
		printf("Starting point(0,0) is also created when generating waypoints.\n\n");
		printf("=========================\n");
		print_center("MAIN MENU", 25);
		printf("=========================\n");


		printf("How to create waypoints?\n");
		printf("  1 : random\n  2 : Directly\n");
		printf("=========================\n");
		printf(">>> ");


		if (scanf("%d", &num) != 1 || (num != 1 && num != 2))
		{
			printf("Please enter a valid value.\n");
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
		printf("\nHow many nodes do you want to add? (Max : 10)\n");
		printf(">>> ");
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
	printf("   Version 1.0\n");
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
	scanf("%c", &bbuf);
	fflush(stdout);

	while (getchar() != '\n' && !feof(stdin));

}
