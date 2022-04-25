#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "utils.h"

int split_string(char *str, char *dlm, char *arr[])
{
    int idx = 0;
    char tmp[SIZE];
    strcpy(tmp, str);
    char *token = strtok(tmp, dlm);

    while (token != NULL)
    {
        arr[idx] = (char *)malloc(strlen(token) * sizeof(char));
        strcpy(arr[idx++], token);
        token = strtok(NULL, dlm);
    }
    return idx;
}

void join_string(char *str_arr[], int idx, char *dlm, char *join_str)
{
    for (int i = 0; i < idx; i++)
    {
        strcat(join_str, str_arr[i]);
        if (i != idx - 1)
            strcat(join_str, dlm);
        else
            strcat(join_str, "\n");
    }
}

void free_str_arr(char *arr[], int idx)
{
    for (int i = 0; i < idx; i++)
    {
        free(arr[i]);
        arr[i] = NULL;
    }
}

int is_db_exists_and_valid(char *db_name)
{
    char db_path[SIZE] = "";
    make_db_path(db_name, db_path);
    struct stat st;
    if (stat(db_path, &st) == 0 && S_ISDIR(st.st_mode))
        return 1;
    else
        return 0;
}

int is_table_exists_and_valid(char *db_name, char *table_name)
{
    char table_path[SIZE] = "";
    make_table_path(db_name, table_name, table_path);
    struct stat st;
    if (stat(table_path, &st) == 0 && !S_ISDIR(st.st_mode))
        return 1;
    else
        return 0;
}

int is_file_exists(char *file_path){
    struct stat st;
    return stat(file_path,&st) == 0 ? 1: 0; 
}

int is_file_empty(char *file_path){
    struct stat st;
    stat(file_path, &st);
    return st.st_size == 0 ? 1:0;
}

void make_db_path(char *db_name, char *db_path)
{
    int size = strlen(DATA_PATH) + strlen(db_name);
    char *temp = (char *)malloc(sizeof(char) * size);
    strcpy(temp, DATA_PATH);
    strcat(temp, db_name);
    strcpy(db_path, temp);
    free(temp);
}

void make_table_path(char *db_name, char *table_name, char *table_path)
{
    int size = strlen(DATA_PATH) + strlen(db_name) + strlen(table_name);
    char *temp = (char *)malloc(sizeof(char) * size);
    strcpy(temp, DATA_PATH);
    strcat(temp, db_name);
    strcat(temp, "/");
    strcat(temp, table_name);
    strcat(temp, ".csv");
    strcpy(table_path, temp);
    free(temp);
}

int delete_table_path(char *db_name, char *table_name)
{
    int size = strlen(DATA_PATH) + strlen(db_name) + strlen(table_name);
    char *temp = (char *)malloc(sizeof(char) * size);
    strcpy(temp, DATA_PATH);
    strcat(temp, db_name);
    strcat(temp, "/");
    strcat(temp, table_name);
    strcat(temp, ".csv");

    if (remove(temp) == 0)
    {
        free(temp);
        return 1;
    }
    else
    {
        free(temp);
        return 0;
    }
}

void man()
{
    printf("\n");

    printf("There are following commands:\n");

    printf(" \u2022 create Database\n");
    printf("\t[cmd : create db <db_name> ]\n");

    printf(" \u2022 create table\n");
    printf("\t[cmd : create table <db_name> <table_name>]\n");

    printf(" \u2022 insert fields\n");
    printf("\t[cmd : insert fields <db_name> <table_name> <field_1> <field_2> ... ]\n");

    printf(" \u2022 insert values\n");
    printf("\t[cmd : insert values <db_name> <table_name> <val_1> <val_2> ... ]\n");

    printf(" \u2022 insert values async\n");
    printf("\t[cmd : insert values <db_name> <table_name_1> <csv_file_name_1> <table_name_2> <csv_file_name_2> ... ]\n");

    printf(" \u2022 fetch\n");
    printf("\t[cmd : fetch <db_name> <table_name> <id_1> <id_2> ... ]\n");

    printf(" \u2022 update Database\n");
    printf("\t[cmd : update db <db_name_old> <db_name_new> ]\n");

    printf(" \u2022 update table\n");
    printf("\t[cmd : update table <db_name> <table_name_old> <table_name_new> ]\n");

    printf(" \u2022 update values\n");
    printf("\t[cmd : update values <db_name> <table_name> <id> <val_1> <val_2> ]\n");

    printf(" \u2022 delete Database\n");
    printf("\t[cmd : delete db <db_name> ]\n");

    printf(" \u2022 delete table\n");
    printf("\t[cmd : delete table <db_name> <table_name> ]\n");

    printf(" \u2022 delete values\n");
    printf("\t[cmd : delete values <db_name> <table_name> <id_1> <id_2> ... ]\n");

    printf(" \u2022 display table\n");
    printf("\t[cmd : display <db_name> <table_name> ]\n");

    printf(" \u2022 exit\n");
    printf("\t[cmd : exit ]\n");

    printf("\n");
}

void print_csv_header(char *header[], int len)
{
    // printf("\033[0;34m"); // blue colour
    int header_length = ID_LEN + (len - 1) * COL_LEN;
    for (int i = 0; i < header_length; i++)
    {
        printf("-");
    }
    printf("\n");
    printf("| id  |");

    for (int i = 1; i < len; i++)
    {
        int left_margin = (COL_LEN - strlen(header[i])) / 2;
        int right_margin = (COL_LEN - strlen(header[i])) % 2 == 0 ? left_margin : left_margin + 1;
        for (int j = 0; j < left_margin; j++)
        {
            printf(" ");
        }
        printf("%s", header[i]);
        for (int j = 0; j < right_margin; j++)
        {
            printf(j == right_margin - 1 ? "|" : " ");
        }
    }
    printf("\n");
    for (int i = 0; i < header_length; i++)
    {
        printf("-");
    }
    printf("\n");
    // printf("\033[0m"); // default colour
}

void print_csv_row(char *row[], int row_len)
{
    printf("|");
    for (int i = 0; i < row_len; i++)
    {
        int current_col_len = i == 0 ? ID_LEN - 1 : COL_LEN;
        int left_margin = (current_col_len - strlen(row[i])) / 2;
        int right_margin = (current_col_len - strlen(row[i])) % 2 == 0 ? left_margin : left_margin + 1;
        for (int j = 0; j < left_margin; j++)
        {
            printf(" ");
        }
        printf("%s", row[i]);
        for (int j = 0; j < right_margin; j++)
        {
            printf(j == right_margin - 1 ? "|" : " ");
        }
    }
    printf("\n");
}