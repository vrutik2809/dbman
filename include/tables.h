struct table{
    char name[100];
    char db_name[100];
    char path[100];
    int num_of_fields;
    char *fields[100];
};

typedef struct table Table;

int create_table_struct(char *db_name,char *table_name,char *fields[],int num_of_fields,Table *table);
void free_table_struct(Table *table);
void print_table_struct(Table *table);
int get_all_fields_table(char *db_name,char *table_name,char *fields[]);
void reflect_table(char *db_name,char *table_name,Table *table,int *st);
int is_id_exists(Table *table,int id);
int update_values_helper(Table *table,int id,int no_of_vals,char *vals[]);
int update_values(char *cmd_arr[], int cmd_length);
int update_table(char *cmd_arr[],int cmd_length);
int update_db(char *cmd_arr[],int cmd_length);
