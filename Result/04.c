#include "drone.h"

FILE* fp = NULL;
char buf[100];
char bbuf[50];

double Maxbattery, battery;
double used_battery = 0.0;

double battery_total_low = 1500.0;

double dist_total = 0.0;
double total_charge_time = 0.0;
int error_cnt = 0;
int mission_count = 0;

double battery_older_state = 0.015;
double charge_coeff = 0.5;

char* droneState[] = { "NORMAL", "ERROR", "FAILURE", "RECOVERY" };
char* pointName[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J" };

reportstack_Node* top = NULL;
report_Node* rhead = NULL;
int listlen = 0;

void print_both(FILE* fp, const char* f, ...)
{
    va_list args;
    va_start(args, f);
    vfprintf(fp, f, args);
    va_end(args);

    va_start(args, f);
    vfprintf(stdout, f, args);
    va_end(args);
}

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

    (*listlen)++; //to save info after func end
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

    if (temp->state == Failure)
    {
        printf("\n[FAILURE] Aircraft malfunction. The drone has crashed.\n");
        printf("Mission failed.\n");
    }
    else if (temp->state == Error)
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
        // report node = what & reportstack_Node = where
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

    print_both(fp, "%-14s %-10s %-8s %-15s %-15s %-12s %-20s\n", "Stage", "Dist(m)", "K", "Battery", "Total", "State", "Return Path");
    print_both(fp, "-----------------------------------------------------------------------------------------\n");

    report_Node* temp = rhead->rlink;
    used_battery = 0.0;

    while (temp != NULL)
    {
        used_battery += temp->battery_use;

        if (temp->state == Error)
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

        print_both(fp, "%-14s %-10.1f %-8.1f %-15.1f %-15.1f %-12s %-20s\n", temp->stage, temp->dist, temp->K, temp->battery_use, used_battery, droneState[temp->state], "-");

        temp = temp->rlink;
    }

    if (temp == NULL)
    {
        if (top->rlink != NULL)
        {
            fprintf(fp, "Return Path : ");
            printf("Return Path : ");

            reportstack_Node* s = top->rlink;
            while (s != NULL)
            {
                fprintf(fp, "%s", s->stage);
                printf("%s", s->stage);
                if (s->rlink != NULL) { fprintf(fp, "->"); printf("->"); }
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
        }
        printf("\nMission completed. Drone returned to base.\n");
    }
    else
    {
        drone_Explosion(temp);
    }
    fclose(fp);

    report_Node* temp_del = rhead->rlink;
    while (temp_del != NULL)
    {
        report_Node* next = temp_del->rlink;
        free(temp_del->stage);
        free(temp_del);
        temp_del = next;
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

    int cnt = 100;

    {
        FILE* pre = fopen(buf, "r");
        if (pre != NULL)
        {
            char line[200];
            double dist_tmp, battery;
            while (fgets(line, sizeof(line), pre))
            {
                if (sscanf(line, "TOTAL %lf - %lf", &dist_tmp, &battery) == 2)
                {
                    if (battery > 0.0)
                        battery_total_low = battery * 3.0;
                    break;
                }
            }
            fclose(pre);
        }
    }

    print_center("SIMULATING...", 33);
    loading_bar();
    printf("\033[A\r                                     \r");
    printf("\033[A\r                                     \r");
    printf("  Success!\n\n");

    srand((unsigned int)time(NULL));

    int cycle = 0;

    while (cycle < cnt && battery_total_low > 50.0)
    {
        cycle++;
        Sleep(500);
        printf("\n========== [ Simulation %d / %d ] ==========\n", cycle, cnt);

        Maxbattery = battery_total_low;
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

            battery -= battery_use;
            push(top, &listlen, battery_use);

            if (pre->state == Error)
            {
                state = Recovery;
            }
            else if (battery < Maxbattery / 2)
            {
                state = Error;
            }
            else
            {
                state = Normal;
            }

            node->state = state;
            node->event = 0;
            pre->rlink = node;
            pre = node;

            if (state == Error) break;
        }

        fclose(fp);

        FILE* sep = fopen(bbuf, "a");
        if (sep != NULL)
        {
            fprintf(sep, "\n========== [ Simulation %d / %d ] ==========\n", cycle, cnt);
            fclose(sep);
        }

        report_Node* last = rhead->rlink;
        while (last != NULL && last->rlink != NULL) last = last->rlink;

        double dist_cycle = 0.0;
        report_Node* r = rhead->rlink;
        while (r != NULL) { dist_cycle += r->dist; r = r->rlink; }

        mission_count++;

        if (last != NULL && last->state == Error)
        {
            error_cnt++;
            total_charge_time += (Maxbattery - battery) * charge_coeff;
            battery_total_low -= dist_cycle * battery_older_state;
        }
        else
        {
            dist_total += dist_cycle;
            battery_total_low -= dist_cycle * 2 * battery_older_state;
        }

        reporting();
    }

    double MTBF = (error_cnt > 0) ? dist_total / error_cnt : 0.0;
    double MTTR = (error_cnt > 0) ? total_charge_time / error_cnt : 0.0;
    double availability = (MTBF + MTTR > 0) ? MTBF / (MTBF + MTTR) * 100.0 : 100.0;

    FILE* fa = fopen(bbuf, "a");
    if (fa != NULL)
    {
        fprintf(fa, "\n========== [ Availability Report ] ==========\n");
        fprintf(fa, "Total Missions    : %d\n", mission_count);
        fprintf(fa, "Error Count       : %d\n", error_cnt);
        fprintf(fa, "MTBF              : %.2f\n", MTBF);
        fprintf(fa, "MTTR              : %.2f\n", MTTR);
        fprintf(fa, "Availability      : %.2f%%\n", availability);
        fclose(fa);
    }
    printf("\n========== [ Availability Report ] ==========\n");
    printf("Total Missions    : %d\n", mission_count);
    printf("Error Count       : %d\n", error_cnt);
    printf("MTBF              : %.2f\n", MTBF);
    printf("MTTR              : %.2f\n", MTTR);
    printf("Availability      : %.2f%%\n", availability);

    system(bbuf);
    printf("\nThank you for using.");
    exit(0);
}
