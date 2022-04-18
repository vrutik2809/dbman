#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "tables.h"

int main(int argc, char *argv[])
{
    int sts = 0;
    printf("\n");
    printf("To see the available features type help in command line\n");
    printf("\n");
    for (;;)
    {
        printf("\033[0;32m"); // green colour
        printf("dbman$ ");
        printf("\033[0m"); // default colour
        char cmd_str[SIZE];
        fgets(cmd_str, SIZE, stdin);
        cmd_str[strlen(cmd_str) - 1] = '\0';
        char *cmd_arr[SIZE];
        int cmd_length = split_string(cmd_str, " ", cmd_arr);

        // help to display the availbale features
        if (strcmp(cmd_arr[0], "help") == 0)
        {

            help();
        }
        else if (strcmp(cmd_arr[0], "create") == 0)
        {
            if (cmd_length == 1)
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
        else if(strcmp(cmd_arr[0], "display") == 0)
        {
            display_table(cmd_arr,cmd_length);
        }
        else if (strcmp(cmd_arr[0], "insert") == 0)
        {
            if (cmd_length == 1)
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
            if (cmd_length == 1)
            {
                printf("command not found\n");
            }
            else if (strcmp(cmd_arr[1], "db") == 0)
            {
                if (update_db(cmd_arr, cmd_length))
                {
                    printf("database updated successfully\n");
                }
            }
            else if (strcmp(cmd_arr[1], "table") == 0)
            {
                if (update_table(cmd_arr, cmd_length))
                {
                    printf("table updated successfully\n");
                }
            }
            else if (strcmp(cmd_arr[1], "values") == 0)
            {
                if (update_values(cmd_arr, cmd_length))
                {
                    printf("row updated successfully\n");
                }
            }
            else
            {
                printf("command not found\n");
            }
        }
        else if (strcmp(cmd_arr[0], "delete") == 0)
        {
            if (cmd_length == 1)
            {
                printf("command not found\n");
            }
            else if (strcmp(cmd_arr[1], "db") == 0)
            {
                // delete db 

                if(delete_db(cmd_arr,cmd_length))
                {
                    printf("Database deleted successfully\n");
                }
            }
            else if (strcmp(cmd_arr[1], "table") == 0)
            {
                // delete table 

                if(delete_table(cmd_arr,cmd_length))
                {
                    printf("Table deleted successfully\n");
                }
            }
            else if (strcmp(cmd_arr[1], "values") == 0)
            {
                // delete values 


                if(delete_values(cmd_arr,cmd_length))
                {
                    printf("Successfully Deleted all values specified by ids\n");
                }
            }
            else
            {
                printf("command not found\n");
            }
        }
        else if (strcmp(cmd_arr[0], "clear") == 0)
        {
            system("clear");
        }
        else if (strcmp(cmd_arr[0], "exit") == 0)
        {
            sts = 1;
        }
        else
        {
            printf("command not found\n");
        }

        free_str_arr(cmd_arr, cmd_length);
        if (sts)
        {
            break;
        }
    }
    return 0;
}
