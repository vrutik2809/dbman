#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "tables.h"

#define SIZE 1000
const int siz = 100000;

int create_table_struct(char *db_name, char *table_name, char *fields[], int num_of_fields, Table *table)
{
    char path[SIZE] = "";
    make_table_path(db_name, table_name, path);
    strcpy(table->db_name, db_name);
    strcpy(table->name, table_name);
    strcpy(table->path, path);
    table->num_of_fields = num_of_fields;
    for (int i = 0; i < num_of_fields; i++)
    {
        table->fields[i] = (char *)malloc(sizeof(char) * strlen(fields[i]));
        strcpy(table->fields[i], fields[i]);
    }
    return 1;
}

void free_table_struct(Table *table)
{
    for (int i = 0; i < table->num_of_fields; i++)
    {
        free(table->fields[i]);
        table->fields[i] = NULL;
    }
    free(table);
    table = NULL;
}

void print_table_struct(Table *table)
{
    printf("db_name: %s\n", table->db_name);
    printf("table_name: %s\n", table->name);
    printf("table_path: %s\n", table->path);
    printf("no_of_fields: %d\n", table->num_of_fields);
    printf("fields: ");
    for (int i = 0; i < table->num_of_fields; i++)
    {
        printf("%s ", table->fields[i]);
    }
    printf("\n");
}

int get_all_fields_table(char *db_name, char *table_name, char *fields[])
{
    char path[SIZE];
    make_table_path(db_name, table_name, path);
    FILE *src = fopen(path, "r");
    char *file_content = (char *)malloc(SIZE * sizeof(char *));
    fgets(file_content, SIZE, src);
    int idx = split_string(file_content, ",", fields);
    fields[idx - 1][strlen(fields[idx - 1]) - 1] = '\0';
    free(file_content);
    fclose(src);
    return idx;
}

void reflect_table(char *db_name, char *table_name, Table *table, int *st)
{
    if (!is_table_exists_and_valid(db_name, table_name))
    {
        *st = 0;
        return;
    }
    char *fields[SIZE];
    int no_of_fields = get_all_fields_table(db_name, table_name, fields);
    create_table_struct(db_name, table_name, fields, no_of_fields, table);
    free_str_arr(fields, no_of_fields);
    *st = 1;
}

int is_id_exists(Table *table, int id)
{
    FILE *src = fopen(table->path, "r");
    char *file_content = (char *)malloc(SIZE * sizeof(char *));
    int sts = 0;
    while (fgets(file_content, SIZE, src))
    {
        char *row[SIZE];
        int idx = split_string(file_content, ",", row);
        if (atoi(row[0]) == id)
        {
            sts = 1;
            break;
        }
        free_str_arr(row, idx);
    }
    fclose(src);
    free(file_content);
    return sts;
}

int update_values_helper(Table *table, int id, int no_of_vals, char *vals[])
{
    char temp_table[SIZE];
    make_table_path(table->db_name, "temp", temp_table);
    rename(table->path, temp_table);
    FILE *src = fopen(temp_table, "r");
    FILE *dest = fopen(table->path, "a");
    char *file_content = (char *)malloc(SIZE * sizeof(char));
    int sts = 0, is_updating = 1;
    while (fgets(file_content, SIZE, src))
    {
        if (is_updating)
        {
            char *row[SIZE];
            int idx = split_string(file_content, ",", row);
            if (atoi(row[0]) == id)
            {
                char join_str[SIZE] = "";
                join_string(vals, no_of_vals, ",", join_str);
                fputs(join_str, dest);
                sts = 1;
                is_updating = 0;
            }
            else
            {
                fputs(file_content, dest);
            }
            free_str_arr(row, idx);
        }
        else
        {
            fputs(file_content, dest);
        }
    }
    free(file_content);
    fclose(src);
    fclose(dest);
    remove(temp_table);
    return sts;
}

int update_values(char *cmd_arr[], int cmd_length)
{
    /*
    - db_name = cmd_arr[2]
    - table_name = cmd_arr[3]
    - id = cmd_arr[4]
    - vals = &cmd_arr[4]
    */

    if (cmd_length == 2)
    {
        printf("provide database name\n");
        return 0;
    }
    else if (cmd_length == 3)
    {
        printf("provide table name\n");
        return 0;
    }
    else if (cmd_length == 4)
    {
        printf("provide row id to update\n");
        return 0;
    }

    Table *table = (Table *)malloc(sizeof(Table));
    int st;
    reflect_table(cmd_arr[2], cmd_arr[3], table, &st);
    if (!st)
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[2], cmd_arr[3]);
        return 0;
    }
    else if (table->num_of_fields != cmd_length - 4)
    {
        printf("no of values is not same is no of fields\n");
        return 0;
    }

    int no_of_vals = cmd_length - 4;
    int id = atoi(cmd_arr[4]);
    int sts = update_values_helper(table, id, no_of_vals, &cmd_arr[4]);
    if (!sts)
    {
        printf("id %d doesn't exist in table %s.%s\n", id, table->db_name, table->name);
        return 0;
    }
    free_table_struct(table);
    return 1;
}

int update_table(char *cmd_arr[], int cmd_length)
{
    /*
    - db_name = cmd_arr[2]
    - old_table_name = cmd_arr[3]
    - new_table_name = cmd_arr[4]
    */

    if (cmd_length == 2)
    {
        printf("provide database name\n");
        return 0;
    }
    else if (cmd_length == 3)
    {
        printf("provide old table name\n");
        return 0;
    }
    else if (cmd_length == 4)
    {
        printf("provide new table name\n");
        return 0;
    }
    if (!is_table_exists_and_valid(cmd_arr[2], cmd_arr[3]))
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[2], cmd_arr[3]);
        return 0;
    }
    char old_table_path[SIZE];
    make_table_path(cmd_arr[2], cmd_arr[3], old_table_path);
    char new_table_path[SIZE];
    make_table_path(cmd_arr[2], cmd_arr[4], new_table_path);
    rename(old_table_path, new_table_path);
    return 1;
}

int update_db(char *cmd_arr[], int cmd_length)
{
    /*
    - old_db_name = cmd_arr[2]
    - new_db_name = cmd_arr[3]
    */

    if (cmd_length == 2)
    {
        printf("provide old database name\n");
        return 0;
    }
    else if (cmd_length == 3)
    {
        printf("provide new database name\n");
        return 0;
    }
    if (!is_db_exists_and_valid(cmd_arr[2]))
    {
        printf("database %s doesn't exist\n", cmd_arr[2]);
        return 0;
    }
    char old_db_path[SIZE];
    make_db_path(cmd_arr[2], old_db_path);
    char new_db_path[SIZE];
    make_db_path(cmd_arr[3], new_db_path);
    rename(old_db_path, new_db_path);
    return 1;
}

// delete functionality

int delete_db(char *cmd_arr[], int cmd_length)
{
    /*
         cmd_arr[0] = delete
         cmd_arr[1] = db
         cmd_arr[2] = db_name
    */

    if (cmd_length == 2)
    {
        printf("Provide Database Name\n");
        return 0;
    }

    if (!is_db_exists_and_valid(cmd_arr[2]))
    {
        printf("database %s doesn't exist\n", cmd_arr[2]);
        return 0;
    }
    char db_path[SIZE];
    make_db_path(cmd_arr[2], db_path);
    int check = remove_db(db_path);
   
    return check;
}

int delete_table(char *cmd_arr[], int cmd_length)
{
    /*
        cmd_arr[0] = delete
        cmd_arr[1] = table
        cmd_arr[2] = db_name
        cmd_arr[3] = table_name
   */
    if (cmd_length == 2)
    {
        printf("provide database name\n");
        return 0;
    }
    else if (cmd_length == 3)
    {
        printf("provide table name\n");
        return 0;
    }
    if (!is_table_exists_and_valid(cmd_arr[2], cmd_arr[3]))
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[2], cmd_arr[3]);
        return 0;
    }

    int check = delete_table_path(cmd_arr[2], cmd_arr[3]);

    return check;
}

int delete_values(char *cmd_arr[], int cmd_length)
{
    /*
        cmd_arr[0] = delete
        cmd_arr[1] = values
        cmd_arr[2] = db_name
        cmd_arr[3] = table_name
        cmd_arr[4]......cmd_arr[n] = id
    */

    if (cmd_length == 2)
    {
        printf("provide database name\n");
        return 0;
    }
    else if (cmd_length == 3)
    {
        printf("provide table name\n");
        return 0;
    }
    else if (cmd_length == 4)
    {
        printf("provide id\n");
        return 0;
    }
    char original_table[SIZE];
    make_table_path(cmd_arr[2], cmd_arr[3], original_table);
    char temp_table[SIZE];
    make_table_path(cmd_arr[2], "temp", temp_table);

    FILE *src = fopen(original_table, "r");
    FILE *new_file = fopen(temp_table, "a");

    char *file_content = (char *)malloc(SIZE * sizeof(char));

    int cnt = 0;

    int first_cnt = 0;
    // to check which id is missing
    int arr[siz];

    memset(arr, 0, siz);
    while (fgets(file_content, SIZE, src))
    {
        // ignore the first row;
        if (first_cnt == 0)
        {
            fputs(file_content, new_file);
            first_cnt++;
            continue;
        }

        char *row[SIZE];
        int idx = split_string(file_content, ",", row);

        // traverse to through all the ids

        int ok = 1;
        for (int i = 4; i < cmd_length; i++)
        {
            if (atoi(row[0]) == atoi(cmd_arr[i]))
            {
                arr[atoi(cmd_arr[i])] = 1;
                cnt++;
                ok = 0;
                break;
            }
        }

        if (ok)
        {
            fputs(file_content, new_file);
        }

        free_str_arr(row, idx);
    }
    free(file_content);
    fclose(src);
    fclose(new_file);

    if (cmd_length - 4 != cnt)
    {

        remove(temp_table);

        printf("Following ids are not exist in the table\n");

        for (int i = 4; i < cmd_length; i++)
        {
            if (arr[atoi(cmd_arr[i])] == 0)
            {
                printf("%d\n", atoi(cmd_arr[i]));
            }
        }
        memset(arr, 0, siz);

        return 0;
    }
    else
    {
        remove(original_table);
        rename(temp_table, original_table);
        memset(arr, 0, siz);
        return 1;
    }
}
