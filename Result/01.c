#include "drone.h"

int for_check_overlap[Map_size + 1][Map_size + 1] = { 0, };

void start_UI()
{
	printf("-----------------\n");
	printf("*****WELCOME*****\n");
	printf("START THE MISSION\n");
	printf("-----------------\n");
} // starting point


void input_sys()
{
	while (1)
	{
		int num;
		printf("\nPlease enter the number you want to do\n");
		printf("1 : random, 2: Directly, 3 : Read on file\n");


		if (scanf("%d", &num) != 1 || num != 1 && num != 2 && num != 3)
		{
			printf("Please enter correct value");
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
		case 3:
			do_FILE();
			break;
		default:
			printf("please try again.");
			break;
		}
	}
}

void do_random() {
	int temp_num = 0;
	FILE* fw=NULL;
	while (1)
	{
		printf("How many nodes do you want to add? : ");
		if (scanf("%d", &temp_num) != 1)
		{

			printf("Please add correct value\n");

			while (getchar() != '\n');

			continue;
		}
		else break;
	}

	getchar();

	int x, y;

	srand((unsigned int)time(NULL));
	
	fw=fopen("01.txt","w");
	if(fw==NULL)
	{
		return 0;
	}
	
	for (int i = 0; i < temp_num; i++)
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
				//여기서 이제 파일 읽어오셔서 해주시면 됩니다.
				//좌표생성 ~~~가 되었습니다.
				fprintf(fw,"%d %d\n",x,y);
				fprintf(stdout,"%d %d\n",x,y);
				break;
			}
		}

	}
	fclose(fw);
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

void do_direct() {}
void do_FILE() {}
