struct table{
    char name[100]; // table name
    char db_name[100]; // database name
    char path[100]; // database path
    int num_of_fields; // total numbe of fields
    char *fields[100]; // fields name
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


// delete functionaly
int delete_db(char *cmd_arr[],int cmd_length); // delete the given database
int delete_table(char *cmd_arr[],int cmd_length); // delete the given table
int delete_values(char *cmd_arr[],int cmd_length); // delete all the values specified by the id
