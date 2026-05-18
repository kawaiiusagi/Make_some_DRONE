#include "drone.h"

FILE* fp = NULL;
char buf[100];
char bbuf[50];

double Maxbattery, battery;
double used_battery = 0.0;

char* droneState[] = { "NORMAL", "ERROR", "FAILURE", "RECOVERY" };
char* pointName[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };

reportstack_Node* top = NULL;
report_Node* rhead = NULL;
int listlen = 0;

void push(reportstack_Node* top, int* listlen, double battery_use)
{
    if (top == NULL || listlen == NULL || *listlen < 0)
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

void drone_Explosion(report_Node* temp)
{
    if (temp == NULL)
    {
        printf("The drone exploded on its own.\nMission completed\n");
        return;
    }

    if (temp->state == 2)
    {
        printf("\n[FAILURE] Aircraft malfunction. The drone has crashed.\n");
        printf("Mission failed.\n");
    }
    else if (temp->state == 1)
    {
        printf("\n[ERROR] Battery depleted. The drone is returning to base.\n");
        printf("Mission failed.\n");
    }
}

void reporting()
{
    fp = fopen(bbuf, "a");
    if (fp == NULL)
    {
        printf("  Cannot open the file.\n");

        report_Node* temp = rhead->rlink;
        while (temp != NULL)
        {
            report_Node* next = temp->rlink;
            free(temp->stage);
            free(temp);
            temp = next;
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

        return;
    }

    fprintf(fp, "%-14s %-10s %-8s %-15s %-15s %-12s %-20s\n", "Stage", "Dist(m)", "K", "Battery", "Total", "State", "Return Path");
    fprintf(stdout, "%-14s %-10s %-8s %-15s %-15s %-12s %-20s\n", "Stage", "Dist(m)", "K", "Battery", "Total", "State", "Return Path");
    fprintf(fp, "-----------------------------------------------------------------------------------------\n");
    fprintf(stdout, "-----------------------------------------------------------------------------------------\n");

    report_Node* temp = rhead->rlink;
    used_battery = 0.0;

    while (temp != NULL)
    {
        used_battery += temp->battery_use;

        if (temp->state == 2)
        {
            fprintf(fp, "%-14s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %-20s\n", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], "MISSION FAILED");
            fprintf(stdout, "%-14s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %-20s\n", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], "MISSION FAILED");
            break;
        }

        if (temp->state == 1)
        {
            fprintf(fp, "%-14s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %c->", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], temp->stage[1]);
            printf("%-14s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %c->", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], temp->stage[1]);

            reportstack_Node* s = top->rlink;
            while (s != NULL)
            {
                fprintf(fp, "%s", s->stage);
                printf("%s", s->stage);
                if (s->rlink != NULL)
                {
                    fprintf(fp, "->");
                    printf("->");
                }
                s = s->rlink;
            }
            fprintf(fp, "\n");
            printf("\n");

            while (top->rlink != NULL)
            {
                reportstack_Node* cur = top->rlink;

                if (cur->rlink != NULL)
                {
                    used_battery -= cur->used_battery;

                    fprintf(fp, "%c%c%-12s %-10s %-8s %-15s %-15.1f %-12s %c->%c\n", cur->stage[0], cur->rlink->stage[0], "(Return)", "-", "-", "-", used_battery, "RECOVERY", cur->stage[0], cur->rlink->stage[0]);
                    printf("%c%c%-12s %-10s %-8s %-15s %-15.1f %-12s %c->%c\n", cur->stage[0], cur->rlink->stage[0], "(Return)", "-", "-", "-", used_battery, "RECOVERY", cur->stage[0], cur->rlink->stage[0]);
                }

                top->rlink = cur->rlink;
                free(cur->stage);
                free(cur);
            }
            break;
        }

        fprintf(fp, "%-14s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %-20s\n", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], "-");
        fprintf(stdout, "%-14s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %-20s\n", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], "-");

        temp = temp->rlink;
    }

    drone_Explosion(temp);
    fclose(fp);

    report_Node* del_temp = rhead->rlink;
    while (del_temp != NULL)
    {
        report_Node* next = del_temp->rlink;
        free(del_temp->stage);
        free(del_temp);
        del_temp = next;
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
}

void make_final()
{
    system("cls");
    Sleep(500);

    printf("\n");
    printf("  +================================================+\n");
    printf("  |         DRONE MISSION SETUP                    |\n");
    printf("  |           [ STEP 4 of 4 ]                      |\n");
    printf("  +================================================+\n");
    printf("\n");
    printf("  +------------------------------------------------+\n");
    printf("  |  Mission : Flight Simulation&Report Generation |\n");
    printf("  |  Input   : Per-segment data from Step 3        |\n");
    printf("  +------------------------------------------------+\n");
    printf("\n");

    Sleep(800);
    printf("  +------------------------------------------------+\n");
    printf("  |  NOTE: File format must be as follows          |\n");
    printf("  |                                                |\n");
    printf("  |     Stage Dist K Battery                       |\n");
    printf("  |                                                |\n");
    printf("  |  Example:                                      |\n");
    printf("  |    AB  30.0  1.5  45.0                         |\n");
    printf("  |    BC  38.8  1.5  58.2                         |\n");
    printf("  |    ...                                         |\n");
    printf("  |    TOTAL  68.8  -  103.2                       |\n");
    printf("  +------------------------------------------------+\n");
    printf("\n");

    Sleep(500);

    FILE* fp = NULL;

    Sleep(500);
    while (1)
    {
        printf("  Please enter the file name(include the file extension)\n");
        printf("  >>> ");

        scanf("%s", buf);

        fp = fopen(buf, "r");

        if (fp == NULL)
        {
            printf("  Cannot read the file.\n");
            continue;
        }
        else break;

    }
    fclose(fp);
    fp = NULL;

    printf("\n");
    print_center("Reading file...", 33);
    loading_bar();
    printf("\033[A\r                                     \r");
    printf("\033[A\r                                     \r");
    printf("  Upload success!\n\n");

    printf("  Please enter the output file name(include the file extension)\n");
    printf("  >>> ");
    scanf("%s", bbuf);
    getchar();

    fp = fopen(bbuf, "w");
    if (fp == NULL)
    {
        printf("  Cannot open the file.\n");
        return;
    }
    fclose(fp);
    fp = NULL;

    int run_count = 0;
    while (1)
    {
        printf("\n  How many simulations? (MAX : 50)\n");
        printf("  >>> ");
        if (scanf("%d", &run_count) != 1 || run_count <= 0 || run_count > 50)
        {
            printf("Please enter a valid value.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar();
        break;
    }

    print_center("SIMULATING...", 33);
    loading_bar();
    printf("\033[A\r                                     \r");
    printf("\033[A\r                                     \r");
    printf("  Success!\n\n");

    srand((unsigned int)time(NULL));

    for (int i = 0; i < run_count; i++)
    {
        Sleep(500);
        printf("\n========== [ Simulation %d / %d ] ==========\n", i + 1, run_count);

        double total_battery = 0;

        FILE* fp_bat = fopen(buf, "r");
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

        int rand_offset = rand() % (int)(total_battery * 1.5 + 1);
        Maxbattery = total_battery + rand_offset;
        battery = Maxbattery;

        listlen = 0;
        top = (reportstack_Node*)malloc(sizeof(reportstack_Node));
        rhead = (report_Node*)malloc(sizeof(report_Node));

        top->rlink = NULL;
        rhead->rlink = NULL;
        rhead->state = 0;

        fp = fopen(buf, "r");
        if (fp == NULL)
        {
            printf("Cannot read the file\n");
            free(top);
            free(rhead);
            top = NULL;
            rhead = NULL;
            return;
        }

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

        FILE* f_sep = fopen(bbuf, "a");
        if (f_sep != NULL)
        {
            fprintf(f_sep, "\n========== [ Simulation %d / %d ] ==========\n", i + 1, run_count);
            fclose(f_sep);
        }

        reporting();
    }


    system(bbuf);
    printf("\nThank you for using.");
    exit(0);
}
