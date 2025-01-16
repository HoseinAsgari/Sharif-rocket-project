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

Rocket rockets[100];
LaunchPad pads[100];
Target targets[100];

int rocket_count = 0;
int pad_count = 0;
int target_count = 0;

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