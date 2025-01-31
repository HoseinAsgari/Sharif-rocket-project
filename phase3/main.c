#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

// ساختار داده‌ها
typedef struct
{
    char name[50];
    int fuel;
    int weight;
    int count;
} Rocket;

typedef struct
{
    char name[50];
    int x;
    int max_force;
} LaunchPad;

typedef struct
{
    int x;
    int y;
} Target;

typedef struct
{
    int x;
    int y;
    int point;
    int score;
    int needed_fuel;
    int best_pad_id;
    int best_rocket_id;
    int id;
} AttackTarget;

Rocket rockets[100];
LaunchPad pads[100];
Target targets[100];
AttackTarget attack_targets[100];

int rocket_count = 0;
int pad_count = 0;
int target_count = 0;
int attack_target_count = 0;

void addRocket(char name[], int fuel, int weight, int count)
{
    strcpy(rockets[rocket_count].name, name);
    rockets[rocket_count].fuel = fuel;
    rockets[rocket_count].weight = weight;
    rockets[rocket_count].count = count;
    rocket_count++;
}

void addLaunchPad(char name[], int x, int max_force)
{
    strcpy(pads[pad_count].name, name);
    pads[pad_count].x = x;
    pads[pad_count].max_force = max_force;
    pad_count++;
}

double getMinVelocity(int x_target, int y_target)
{
    return sqrt(9.8 * y_target + 9.8 * sqrt(pow(x_target, 2) + pow(y_target, 2)));
}

void findBestRocketAndPad(int x_target, int y_target)
{
    double min_velocity, angle = 45;

    min_velocity = getMinVelocity(x_target, y_target);

    int best_rocket_index = -1, best_pad_index = -1;
    double best_fuel = DBL_MAX, best_weight = DBL_MAX;

    // get pads list
    int pad_list[100][2];
    for (int i = 0; i < pad_count; i++)
    {
        double pad_distance = sqrt(pow(pads[i].x - x_target, 2) + pow(y_target, 2));
        pad_list[i][0] = i;
        pad_list[i][1] = pad_distance;
    }

    // sort pads(bubble sort)
    for (int i = 0; i < pad_count; i++)
    {
        for (int j = 0; j < pad_count - i; j++)
        {
            if (pad_list[j][1] > pad_list[j + 1][1])
            {
                pad_list[j][0] += pad_list[j + 1][0];
                pad_list[j + 1][0] = pad_list[j][0] - pad_list[j + 1][0];
                pad_list[j][0] = pad_list[j][0] - pad_list[j + 1][0];

                pad_list[j][1] += pad_list[j + 1][1];
                pad_list[j + 1][1] = pad_list[j][1] - pad_list[j + 1][1];
                pad_list[j][1] = pad_list[j][1] - pad_list[j + 1][1];
            }
        }
    }

    // find best rocket
    for (int j = 0; j < pad_count && best_rocket_index == -1; j++)
    {
        for (int i = 0; i < rocket_count; i++)
        {
            // f=ma=m(v0)
            double required_force = rockets[i].weight * min_velocity;

            // F=3(v0)/10
            double fuel_needed = (3 * min_velocity) / 10;

            if (pads[pad_list[j][0]].max_force >= required_force && rockets[i].count > 0 && rockets[i].fuel >= fuel_needed)
            {
                if (rockets[i].weight < best_weight || (rockets[i].weight == best_weight && rockets[i].fuel < best_fuel))
                {
                    best_fuel = rockets[i].fuel;
                    best_weight = rockets[i].weight;
                    best_rocket_index = i;
                    best_pad_index = pad_list[j][0];
                }
            }
        }
    }

    if (best_rocket_index != -1)
    {
        printf("The target was destroyed by launching %s rocket from %s launch pad\n",
               rockets[best_rocket_index].name, pads[best_pad_index].name);
        rockets[best_rocket_index].count--;

        if (rockets[best_rocket_index].count == 0)
        {
            rockets[best_rocket_index] = rockets[rocket_count - 1];
            rocket_count--;
        }
    }
    else
    {
        printf("Target is unreachable\n");
    }
}

void printRemainingRockets()
{
    for (int i = 0; i < rocket_count; i++)
    {
        printf("%d Rockets of %s\n", rockets[i].count, rockets[i].name);
    }
}

void addTarget(int x, int y, int point)
{
    attack_targets[attack_target_count].x = x;
    attack_targets[attack_target_count].y = y;
    attack_targets[attack_target_count].point = point;
    attack_targets[attack_target_count].id = attack_target_count;
    attack_target_count++;
}

void calculateTargetsDetails()
{
    double min_velocity, angle = 45;

    for (int k = 0; k < attack_target_count; k++)
    {
        int x_target = attack_targets[k].x, y_target = attack_targets[k].y;

        min_velocity = getMinVelocity(x_target, y_target);

        int best_rocket_index = -1, best_pad_index = -1;
        double best_fuel = DBL_MAX, best_weight = DBL_MAX;

        // get pads list
        int pad_list[100][2];
        for (int i = 0; i < pad_count; i++)
        {
            double pad_distance = sqrt(pow(pads[i].x - x_target, 2) + pow(y_target, 2));
            pad_list[i][0] = i;
            pad_list[i][1] = pad_distance;
        }

        // sort pads(bubble sort)
        int temp;
        for (int i = 0; i < pad_count; i++)
        {
            for (int j = 0; j < pad_count - i; j++)
            {
                if (pad_list[j][1] > pad_list[j + 1][1])
                {
                    temp = pad_list[j + 1][0];
                    pad_list[j + 1][0] = pad_list[j][0];
                    pad_list[j][0] = temp;

                    temp = pad_list[j + 1][1];
                    pad_list[j + 1][1] = pad_list[j][1];
                    pad_list[j][1] = temp;
                }
            }
        }

        // find best rocket
        for (int j = 0; j < pad_count && best_rocket_index == -1; j++)
        {
            for (int i = 0; i < rocket_count; i++)
            {
                // f=ma=m(v0)
                double required_force = rockets[i].weight * min_velocity;

                // F=3(v0)/10
                double fuel_needed = (3 * min_velocity) / 10;

                printf("\n%d\n", j);
                printf("\n%d\n", pad_list[j][0]);
                printf("\n%d\n", pads[pad_list[j][0]].max_force);

                if (pads[pad_list[j][0]].max_force >= required_force && rockets[i].count > 0 && rockets[i].fuel >= fuel_needed)
                {
                    if (rockets[i].weight < best_weight || (rockets[i].weight == best_weight && rockets[i].fuel < best_fuel))
                    {
                        best_fuel = rockets[i].fuel;
                        best_weight = rockets[i].weight;
                        best_rocket_index = i;
                        best_pad_index = pad_list[j][0];
                    }
                }
            }
        }

        attack_targets[k].needed_fuel = best_fuel;
        attack_targets[k].best_rocket_id = best_rocket_index;
        attack_targets[k].best_pad_id = best_pad_index;
        attack_targets[k].score = best_fuel * attack_targets[k].point;
    }
}

void planWar(int fuel_budget)
{
    // calculate targets best pad, best rocket and score
    calculateTargetsDetails();

    // sort targets (bubble sort)
    AttackTarget target_temp;
    for (int i = 0; i < attack_target_count; i++)
    {
        for (int j = 0; j < attack_target_count - i; j++)
        {
            if (attack_targets[j].score > attack_targets[j + 1].score)
            {
                target_temp = attack_targets[j];
                attack_targets[j] = attack_targets[j + 1];
                attack_targets[j + 1] = target_temp;
            }
        }
    }

    int value = 0;
    AttackTarget attacks[100];

    // calculate attacks fuel and point
    int k = 0;
    for (int i = attack_target_count - 1; i >= 0; i--)
    {
        if (fuel_budget >= attack_targets[i].needed_fuel)
        {
            fuel_budget -= attack_targets[i].needed_fuel;
            value += attack_targets[i].point;
            k++;
        }
    }

    printf("Maximized target value after optimization: %d", value);

    // points rocket
    for (int i = 0; i <= k; i++)
    {
        printf("Target %d -> Rocket %s from Pad %s (Fuel: %d, Value: %d)", attacks[i].id, rockets[attacks[i].best_rocket_id].name,
               pads[attacks[i].best_pad_id].name, attacks[i].needed_fuel, attacks[i].point);
    }
}

void readCommand(char command[])
{
    char cmd[50], type[50], name[50];
    int x, fuel, weight, max_force, count;

    if (sscanf(command, "%s %s %s", cmd, type, name) >= 3)
    {
        if (strcmp(cmd, "Add") == 0)
        {
            // اضافه کردن سکوی پرتاب
            if (strcmp(type, "Rocket") == 0)
            {
                // مثال: Add Rocket Launch Pad Qods in 5 and with max force 200
                if (sscanf(command, "%*s %*s %*s %*s %s %*s %d %*s %*s %*s %*s %d", name, &x, &max_force) == 3)
                {
                    addLaunchPad(name, x, max_force);
                }
                else
                {
                    printf("Invalid input format!\n");
                }
            }
            // اضافه کردن موشک
            else if (strcmp(name, "Rocket") == 0)
            {
                // مثال: Add 10 Rocket Fattah with initiaⅼ fuel amount of 20 and weight of 100
                if (sscanf(command, "%*s %d %*s %s %*s %*s %*s %*s %*s %d %*s %*s %*s %d", &count, name, &fuel, &weight) == 4)
                {
                    addRocket(name, fuel, weight, count);
                }
                else
                {
                    printf("Invalid input format!\n");
                }
            }
            else if (strcmp(type, "Target") == 0)
            {
                int x, y, point;
                // مثال: Add Target x: 120 y: 100 point: 30
                if (sscanf(command, "%*s %*s %*s %d %*s %d %*s %d", &x, &y, &point) == 3)
                {
                    addTarget(x, y, point);
                }
                else
                {
                    printf("Invalid input format!\n");
                }
            }
            else
            {
                printf("Invalid input format!\n");
            }
        }
        // پرتاب پدافند
        else if (strcmp(cmd, "Shoot") == 0)
        {
            int x, y;
            if (sscanf(command, "%*s %*s %d %d", &x, &y) == 2)
            {
                findBestRocketAndPad(x, y);
            }
            else
            {
                printf("Invalid input format!\n");
            }
        }
        else if (strcmp(cmd, "Print") == 0)
        {
            printRemainingRockets();
        }
        else if (strcmp(cmd, "War") == 0)
        {
            int fuel_budget;
            if (sscanf(command, "%*s %*s %*s %*s %d", &fuel_budget) == 1)
            {
                planWar(fuel_budget);
            }
        }
        else if (strcmp(cmd, "END") == 0)
        {
            exit(0);
        }
        else
        {
            printf("Invalid input format!\n");
        }
    }
    else
    {
        printf("Invalid input format!\n");
    }
}

int main()
{
    char command[100];

    while (1)
    {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        readCommand(command);
    }

    return 0;
}