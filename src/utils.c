#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include "utils.h"

#define DATA_PATH "../data/"
#define SIZE 1000

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

int remove_db(char *db_name)
{
    DIR *d = opendir(db_name);

    size_t path_len = strlen(db_name);

    int r = -1;

    if (d)
    {
        struct dirent *p;

        r = 0;

        while (!r && (p = readdir(d)))
        {
            int r2 = -1;

            char *buf;
            size_t len;

            // we can skip the "." and ".." as we don't want to recurse on them

            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
            {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;

            buf = malloc(len);

            if (buf)
            {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", db_name, p->d_name);

                if (!stat(buf, &statbuf))
                {
                    if (S_ISDIR(statbuf.st_mode))
                    {
                        r2 = remove_db(buf);
                    }
                    else
                    {
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }

    if (!r)
    {
        r = rmdir(db_name);
    }

    return !r;
}

void help()
{
    printf("\n");
    printf("\033[0;31m"); // red colour
    printf("There are following commands:\n");
    printf("\033[0m"); // default colour

    printf("\033[0;33m"); // yellow colour

    printf(" \u2022 create Database\n");
    printf(" \u2022 create table\n");
    printf(" \u2022 insert fields\n");
    printf(" \u2022 insert values\n");
    printf(" \u2022 insert fields\n");
    printf(" \u2022 fetch\n");
    printf(" \u2022 update Database\n");
    printf(" \u2022 update table\n");
    printf(" \u2022 update values\n");
    printf(" \u2022 delete Database\n");
    printf(" \u2022 delete table\n");
    printf(" \u2022 delete values\n");
    printf(" \u2022 exit\n");

    printf("\033[0m"); // default colour
    printf("\n");
}
