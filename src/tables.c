#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "tables.h"

#define SIZE 1000

int create_table_struct(char *db_name,char *table_name,char *fields[],int num_of_fields,Table *table){
    char path[SIZE] = "";
    make_table_path(db_name,table_name,path);
    strcpy(table->db_name,db_name);
    strcpy(table->name,table_name);
    strcpy(table->path,path);
    table->num_of_fields = num_of_fields;
    for(int i = 0;i < num_of_fields;i++){
        table->fields[i] = (char *)malloc(sizeof(char) * strlen(fields[i]));
        strcpy(table->fields[i],fields[i]);
    }
    return 1;
}

void free_table_struct(Table *table){
    for(int i = 0;i < table->num_of_fields;i++){
        free(table->fields[i]);
        table->fields[i] = NULL;
    }
    free(table);
    table = NULL;
}

void print_table_struct(Table *table){
    printf("db_name: %s\n",table->db_name);
    printf("table_name: %s\n",table->name);
    printf("table_path: %s\n",table->path);
    printf("no_of_fields: %d\n",table->num_of_fields);
    printf("fields: ");
    for(int i = 0;i < table->num_of_fields;i++){
        printf("%s ",table->fields[i]);
    }
    printf("\n");
}

int get_all_fields_table(char *db_name,char *table_name,char *fields[]){
    char path[SIZE];
    make_table_path(db_name,table_name,path);
    FILE *src = fopen(path,"r");
    char *file_content = (char *)malloc(SIZE * sizeof(char *));
    fgets(file_content,SIZE,src);
    int idx = split_string(file_content,",",fields);
    fields[idx-1][strlen(fields[idx-1]) - 1] = '\0';
    free(file_content);
    return idx;
}

void reflect_table(char *db_name,char *table_name,Table *table){
    if(!is_table_exists_and_valid(db_name,table_name)) return;
    char *fields[SIZE];
    int no_of_fields = get_all_fields_table(db_name,table_name,fields);
    create_table_struct(db_name,table_name,fields,no_of_fields,table);
    free_str_arr(fields,no_of_fields);
}

int is_id_exists(Table *table,int id){
    FILE *src = fopen(table->path,"r");
    char *file_content = (char *)malloc(SIZE * sizeof(char *));
    int sts = 0;
    while(fgets(file_content,SIZE,src)){
        char *row[SIZE];
        int idx = split_string(file_content,",",row);
        if(atoi(row[0]) == id){
            sts = 1;
            break;
        }
        free_str_arr(row,idx);
    }
    fclose(src);
    free(file_content);
    return sts;
}

int update_values(Table *table,int id,char *vals[],int no_of_vals){
    if(!is_table_exists_and_valid(table->db_name,table->name)){
        printf("table doesn't exist or invalid\n");
        return 0;
    }
    else if(table->num_of_fields != no_of_vals){
        printf("no of values is not same is no of fields\n");
        return 0;
    }
    char temp_table[SIZE];
    make_table_path(table->db_name,"temp",temp_table);
    rename(table->path,temp_table);
    FILE *src = fopen(temp_table,"r");
    FILE *dest = fopen(table->path,"a");
    char *file_content = (char *)malloc(SIZE * sizeof(char *));
    int sts = 0,is_updating = 1;
    while(fgets(file_content,SIZE,src)){
        if(is_updating){
            char *row[SIZE];
            char temp_content[SIZE];
            strcpy(temp_content,file_content);
            int idx = split_string(temp_content,",",row);
            if(atoi(row[0]) == id){
                char join_str[SIZE] = "";
                join_string(vals,no_of_vals,",",join_str);
                fputs(join_str, dest);
                sts = 1;
                is_updating = 0;
            }
            else{
                fputs(file_content,dest);
            }
            free_str_arr(row,idx);
        }
        else{
            fputs(file_content,dest);
        }
    }
    free(file_content);
    fclose(src);
    fclose(dest);
    remove(temp_table);
    if(!sts){
        printf("id %d doesn't exist in table %s.%s\n",id,table->db_name,table->name);
        return 0;
    }
    return 1;
}