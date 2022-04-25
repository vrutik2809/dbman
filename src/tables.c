#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include "utils.h"
#include "tables.h"

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
    char path[SIZE] = "";
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

void reflect_table(char *db_name, char *table_name, Table *table)
{
    char table_path[SIZE];
    make_table_path(db_name, table_name, table_path);
    struct stat st;
    stat(table_path, &st);
    if (st.st_size == 0)
    {
        return;
    }
    char *fields[SIZE];
    int no_of_fields = get_all_fields_table(db_name, table_name, fields);
    create_table_struct(db_name, table_name, fields, no_of_fields, table);
    free_str_arr(fields, no_of_fields);
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
    - vals = [cmd_arr[4],cmd_arr[5],...]
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
    else if (!is_table_exists_and_valid(cmd_arr[2], cmd_arr[3]))
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[2], cmd_arr[3]);
        return 0;
    }
    else if (cmd_length == 4)
    {
        printf("provide row id to update\n");
        return 0;
    }

    Table *table = (Table *)malloc(sizeof(Table));
    reflect_table(cmd_arr[2], cmd_arr[3], table);
    if (table->num_of_fields != cmd_length - 4)
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

    char str[SIZE] = "rm -R ";
    int check = system(strcat(str, db_path));

    return check == -1 ? 0 : 1;
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

    if (!is_table_exists_and_valid(cmd_arr[2], cmd_arr[3]))
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[2], cmd_arr[3]);
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

    int id_present = 0;

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
        else
        {
            id_present = 1;
        }

        free_str_arr(row, idx);
    }
    free(file_content);
    fclose(src);
    fclose(new_file);

    if (cmd_length - 4 != cnt)
    {

        printf("Following ids are not exist in the table\n");

        for (int i = 4; i < cmd_length; i++)
        {
            if (arr[atoi(cmd_arr[i])] == 0)
            {
                printf("%d\n", atoi(cmd_arr[i]));
            }
        }
    }

    if (id_present)
    {
        printf("Following ids are successfully deleted\n");

        for (int i = 4; i < cmd_length; i++)
        {
            if (arr[atoi(cmd_arr[i])] == 1)
            {
                printf("%d\n", atoi(cmd_arr[i]));
            }
        }
    }

    remove(original_table);
    rename(temp_table, original_table);
    memset(arr, 0, siz);
    return 1;
}

void display_table(char *cmd_arr[], int cmd_length)
{

    /*
    - cmd_arr[1] = db_name
    - cmd_arr[2] = table_name
    */

    if (cmd_length == 1)
    {
        printf("provide database name\n");
        return;
    }
    else if (cmd_length == 2)
    {
        printf("provide table name\n");
        return;
    }
    else if (!is_table_exists_and_valid(cmd_arr[1], cmd_arr[2]))
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[1], cmd_arr[2]);
        return;
    }
    char table_path[SIZE];
    make_table_path(cmd_arr[1], cmd_arr[2], table_path);
    FILE *src = fopen(table_path, "r");
    char *file_content = (char *)malloc(SIZE * sizeof(char));
    int line_no = 0;
    int last_len = 0;
    while (fgets(file_content, SIZE, src))
    {
        if (line_no == 0)
        {
            char *header[SIZE];
            int header_len = split_string(file_content, ",", header);
            last_len = ID_LEN + (header_len - 1) * COL_LEN;
            header[header_len - 1][strlen(header[header_len - 1]) - 1] = '\0';
            print_csv_header(header, header_len);
        }
        else
        {
            char *row[SIZE];
            int row_len = split_string(file_content, ",", row);
            row[row_len - 1][strlen(row[row_len - 1]) - 1] = '\0';
            print_csv_row(row, row_len);
        }
        line_no++;
    }
    for (int i = 0; i < last_len; i++)
    {
        printf("-");
    }
    fclose(src);
    printf("\n");
}

int fetch(char *cmd_arr[], int cmd_length)
{
    /*
        cmd_arr[0] = fetch
        cmd_arr[1] = db_name
        cmd_arr[2] = table name
        cmd_arr[3]......cmd_arr[n] = id

    */

    if (cmd_length == 1)
    {
        printf("provide database name\n");
        return 0;
    }
    else if (cmd_length == 2)
    {
        printf("provide table name\n");
        return 0;
    }
    else if (cmd_length == 3)
    {
        printf("provide id\n");
        return 0;
    }

    if (!is_table_exists_and_valid(cmd_arr[1], cmd_arr[2]))
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[1], cmd_arr[2]);
        return 0;
    }

    int cnt = 0;

    Table *table = (Table *)malloc(sizeof(Table));
   
    reflect_table(cmd_arr[1], cmd_arr[2], table);
    int ok = 0;

    int arr[siz];

    memset(arr, 0, siz);


    int anyone_present = 0;
    for (int i = 3; i < cmd_length; i++)
    {
        if (is_id_exists(table, atoi(cmd_arr[i])))
        {
            arr[atoi(cmd_arr[i])] = 1;
            anyone_present = 1;
            continue;
        }
        else
        {

            ok = 1;
        }
    }

    free_table_struct(table);

    char original_table[SIZE];
    make_table_path(cmd_arr[1], cmd_arr[2], original_table);

    FILE *src = fopen(original_table, "r");

    char *file_content = (char *)malloc(SIZE * sizeof(char));

    int first_cnt = 0;
    int last_len = 0;
    printf("\n");
    while (fgets(file_content, SIZE, src))
    {
        // the first row;
        char *row[SIZE];
        int idx = split_string(file_content, ",", row);
        if (first_cnt == 0 && anyone_present)
        {
            last_len = ID_LEN + (idx - 1) * COL_LEN;
            row[idx - 1][strlen(row[idx - 1]) - 1] = '\0';
            print_csv_header(row, idx);
            first_cnt++;
            continue;
        }

        // traverse to through all the ids

        int ok = 0;
        for (int i = 3; i < cmd_length; i++)
        {
            if (atoi(row[0]) == atoi(cmd_arr[i]))
            {

                ok = 1;
                break;
            }
        }

        if (ok)
        {
            row[idx - 1][strlen(row[idx - 1]) - 1] = '\0';
            print_csv_row(row, idx);
        }

        free_str_arr(row, idx);
    }
    for (int i = 0; i < last_len; i++)
    {
        printf("-");
    }
    free(file_content);
    fclose(src);
    printf("\n");

    if (ok)
    {
        printf("Following ids are not exist in the table\n");

        for (int i = 3; i < cmd_length; i++)
        {
            if (arr[atoi(cmd_arr[i])] == 0)
            {
                printf("%d\n", atoi(cmd_arr[i]));
            }
        }
    }


    printf("\n");

    return 1;
}

int create_db(char *cmd_arr[], int cmd_length)
{
    /*
        cmd_arr[0] = create
        cmd_arr[1] = db
        cmd_arr[2] = db_name
    */

    if (cmd_length == 2)
    {
        printf("provide database name\n");
        return 0;
    }
    if (is_db_exists_and_valid(cmd_arr[2]))
    {
        printf("database %s is already exist\n", cmd_arr[2]);
        return 0;
    }
    char db_path[SIZE];
    make_db_path(cmd_arr[2], db_path);

    char str[SIZE] = "mkdir ";
    int check = system(strcat(str, db_path));

    return check == -1 ? 0 : 1;
}

int create_table(char *cmd_arr[], int cmd_length)
{

    /*
        cmd_arr[0] = create
        cmd_arr[1] = table
        cmd_arr[2] = db_name
        cmd_arr[3] = table_name
    */

    if (cmd_length == 2)
    {
        printf("provide database name\n");
        return 0;
    }

    if (cmd_length == 3)
    {
        printf("provide table name\n");
        return 0;
    }

    if(!is_db_exists_and_valid(cmd_arr[2])){
        printf("database %s doesn't exist\n", cmd_arr[2]);
        return 0;
    }

    if (is_table_exists_and_valid(cmd_arr[2], cmd_arr[3]))
    {
        printf("table %s.%s is already exist\n", cmd_arr[2], cmd_arr[3]);
        return 0;
    }
    char table_path[SIZE] = "";
    make_table_path(cmd_arr[2], cmd_arr[3], table_path);

    char str[SIZE] = "touch ";
    int check = system(strcat(str, table_path));

    return check == -1 ? 0 : 1;
}

int insert_fields(char *cmd_arr[], int cmd_length)
{
    /*
    - cmd_arr[2] = db_name
    - cmd_arr[3] = table_name
    - [cmd_arr[4],cmd_arr[5],...] = array of fields
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
    else if (!is_table_exists_and_valid(cmd_arr[2], cmd_arr[3]))
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[2], cmd_arr[3]);
        return 0;
    }
    else if(cmd_length == 4){
        printf("provide atleast one field\n");
        return 0;
    }

    char table_path[SIZE];
    make_table_path(cmd_arr[2], cmd_arr[3], table_path);

    if(!is_file_empty(table_path)){
        printf("table %s.%s already initialized\n", cmd_arr[2], cmd_arr[3]);
        return 0;
    }

    if (strcmp(cmd_arr[4], "id") == 0)
    {
        FILE *src = fopen(table_path, "w");
        char header[SIZE] = "";
        join_string(&cmd_arr[4], cmd_length - 4, ",", header);
        fputs(header, src);
        fclose(src);
    }
    else
    {
        FILE *src = fopen(table_path, "w");
        char header[SIZE] = "";
        strcat(header,"id,");
        join_string(&cmd_arr[4], cmd_length - 4, ",", header);
        fputs(header, src);
        fclose(src);
    }

    return 1;
}

int get_last_id_from_table(char *table_path)
{
    FILE *table_file = fopen(table_path, "r");
    char *file_content = (char *)malloc(SIZE * sizeof(char));
    while (fgets(file_content, SIZE, table_file))
    {
    }
    char *last_row[SIZE];
    int row_argc = split_string(file_content, ",", last_row);
    int id = atoi(last_row[0]);
    free_str_arr(last_row, row_argc);
    fclose(table_file);
    free(file_content);
    return id;
}

int insert_values(char *cmd_arr[], int cmd_length)
{
    /*
    - cmd_arr[2] = db_name
    - cmd_arr[3] = table_name
    - [cmd_arr[4],cmd_arr[5],...] = array of values
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
    else if (!is_table_exists_and_valid(cmd_arr[2], cmd_arr[3]))
    {
        printf("table %s.%s doesn't exist\n", cmd_arr[2], cmd_arr[3]);
        return 0;
    }
    char table_path[SIZE];
    make_table_path(cmd_arr[2], cmd_arr[3], table_path);
    if (is_file_empty(table_path))
    {
        printf("table %s.%s is not initialized\n", cmd_arr[2], cmd_arr[3]);
        printf("use `insert fields` command to initialize the table\n");
        printf("use `man` command to know more about `insert field commad`\n");
        return 0;
    }
    Table *table = (Table *)malloc(sizeof(Table));
    reflect_table(cmd_arr[2], cmd_arr[3], table);
    if (table->num_of_fields - 1 != cmd_length - 4)
    {
        printf("number of values is not same as number of fields\n");
        free_table_struct(table);
        return 0;
    }
    int id = get_last_id_from_table(table->path) + 1;
    FILE *src = fopen(table->path, "a");
    char line_buff[SIZE] = "";
    sprintf(line_buff, "%d", id);
    strcat(line_buff, ",");
    char vals[SIZE] = "";
    join_string(&cmd_arr[4], cmd_length - 4, ",", vals);
    strcat(line_buff, vals);
    fputs(line_buff, src);
    fclose(src);
    free_table_struct(table);
    return 1;
}

int insert_values_async_helper(char *db_name, char *table_name, char *csv_file_name)
{
    if (!is_table_exists_and_valid(db_name, table_name))
    {
        return 0;
    }
    else if (!is_file_exists(csv_file_name))
    {
        return -1;
    }
    char table_path[SIZE] = "";
    make_table_path(db_name, table_name, table_path);
    int id = get_last_id_from_table(table_path) + 1;
    Table *table = (Table *) malloc(sizeof(Table));
    reflect_table(db_name,table_name,table);
    FILE *table_dest = fopen(table_path, "a");
    FILE *csv_src = fopen(csv_file_name,"r");
    char *file_content = (char *)malloc(SIZE * sizeof(char));
    int sts = 1;
    while (fgets(file_content, SIZE, csv_src))
    {
        if(sts){
            char *fields[SIZE];
            int num_of_fields = split_string(file_content,",",fields);
            if(table->num_of_fields - 1!= num_of_fields){
                sts = 0;
                break;
            }
            free_str_arr(fields,num_of_fields);
        }
        char line_buff[2 * SIZE] = "";
        sprintf(line_buff,"%d",id++);
        strcat(line_buff,",");
        strcat(line_buff,file_content);
        fputs(line_buff,table_dest);
    }
    free_table_struct(table); 
    fputs("\n",table_dest);
    fclose(table_dest);
    fclose(csv_src);
    free(file_content);
    return sts? 1:-2;
}

int insert_values_async(char *cmd_arr[], int cmd_length)
{
    /*
    - cmd_arr[3] = db_name
    - (cmd_arr[4],cmd_arr[5]) = (table_name_1,csv_file_name_1)
    - (cmd_arr[6],cmd_arr[7]) = (table_name_2,csv_file_name_2)
    - ...
    - (cmd_arr[cmd_length-2],cmd_arr[cmd_length-1]) = (table_name_<cmd_length-5>,csv_file_name_<cmd_length-5>)

    ==> (cmd_length - 4) % 2 == 0 && cmd_length >= 6
    */
    if (cmd_length == 3)
    {
        printf("provide database name\n");
        return 0;
    }
    else if (!is_db_exists_and_valid(cmd_arr[3]))
    {
        printf("database %s doesn't exist\n",cmd_arr[3]);
        return 0;
    }
    else if (cmd_length < 6)
    {
        printf("provide atleast one pair (table_name,csv_file_name)\n");
        return 0;
    }
    else if ((cmd_length - 4) % 2 != 0)
    {
        printf("last table name or csv file name is missing\n");
        return 0;
    }
    int no_of_pairs = (cmd_length - 4) / 2;
    for (int i = 0; i < no_of_pairs; i++)
    {
        if (!fork())
        {
            int sts = insert_values_async_helper(cmd_arr[3],cmd_arr[4 + 2 * i],cmd_arr[5 + 2 * i]);
            if(sts == 0){
                printf("table %s.%s doesn't exist\n",cmd_arr[3],cmd_arr[4 + 2 * i]);
                exit(-1);
            }
            else if(sts == -1){
                printf("csv file %s doesn't exist\n",cmd_arr[5 + 2 * i]);
                exit(-1);
            }
            else if(sts == -2){
                printf("no of fields in csv file %s is not same as table %s\n",cmd_arr[5 + 2 * i],cmd_arr[4 + 2 * i]);
                exit(-1);
            }
            exit(0);
        }
    }
    for (int i = 0; i < no_of_pairs; i++)
    {
        int sts;
        int child_pid = wait(&sts);
        if(WIFEXITED(sts) && WEXITSTATUS(sts) == 0){
            printf("content of file %s is added successfully in table %s.%s\n",cmd_arr[5 + 2 * i],cmd_arr[3],cmd_arr[4 + 2 * i]);
        }
    }
    return 1;
}