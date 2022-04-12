#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "tables.h"
#define SIZE 1000

int main(int argc, char *argv[])
{
    int sts = 0;
    for (;;)
    {
        printf("\033[0;32m"); // green colour
        printf("dbman$ ");
        printf("\033[0m"); // default colour
        char cmd_str[SIZE];
        fgets(cmd_str, SIZE, stdin);
        cmd_str[strlen(cmd_str) - 1] = '\0';
        char *cmd_arr[SIZE];
        int idx = split_string(cmd_str, " ", cmd_arr);
        if (strcmp(cmd_arr[0], "create") == 0)
        {
            if (idx == 1)
            {
                printf("command not found\n");
            }
            else if (strcmp(cmd_arr[1], "db") == 0)
            {
                // create db logic
            }
            else if (strcmp(cmd_arr[1], "table") == 0)
            {
                // create table logic
            }
            else
            {
                printf("command not found\n");
            }
        }
        else if (strcmp(cmd_arr[0], "insert") == 0)
        {
            if (idx == 1)
            {
                printf("command not found\n");
            }
            else if (strcmp(cmd_arr[1], "fields") == 0)
            {
                // insert field logic
            }
            else if (strcmp(cmd_arr[1], "values") == 0)
            {
                // insert value logic
            }
            else
            {
                printf("command not found\n");
            }
        }
        else if (strcmp(cmd_arr[0], "fetch") == 0)
        {
            // fetch logic
        }
        else if (strcmp(cmd_arr[0], "update") == 0)
        {
            if (idx == 1)
            {
                printf("command not found\n");
            }
            else if (strcmp(cmd_arr[1], "db") == 0)
            {
                // update db logic
            }
            else if (strcmp(cmd_arr[1], "table") == 0)
            {
                // update table logic
            }
            else if (strcmp(cmd_arr[1], "values") == 0)
            {
                // update values logic
            }
            else
            {
                printf("command not found\n");
            }
        }
        else if (strcmp(cmd_arr[0], "delete") == 0)
        {
            if (idx == 1)
            {
                printf("command not found\n");
            }
            else if (strcmp(cmd_arr[1], "db") == 0)
            {
                // delete db logic
            }
            else if (strcmp(cmd_arr[1], "table") == 0)
            {
                // delete table logic
            }
            else if (strcmp(cmd_arr[1], "values") == 0)
            {
                // delete values logic
            }
            else
            {
                printf("command not found\n");
            }
        }
        else if (strcmp(cmd_arr[0], "exit") == 0)
        {
            sts = 1;
        }
        else
        {
            printf("command not found\n");
        }

        free_str_arr(cmd_arr, idx);
        if (sts)
        {
            break;
        }
    }
    return 0;
}
