struct table{
    char name[100]; // table name
    char db_name[100]; // database name
    char path[100]; // database path
    int num_of_fields; // total number of fields
    char *fields[100]; // fields name
};

typedef struct table Table;

int create_table_struct(char *db_name,char *table_name,char *fields[],int num_of_fields,Table *table);
void free_table_struct(Table *table);
void print_table_struct(Table *table);
int get_all_fields_table(char *db_name,char *table_name,char *fields[]);
void reflect_table(char *db_name,char *table_name,Table *table);
int is_id_exists(Table *table,int id);
int update_values_helper(Table *table,int id,int no_of_vals,char *vals[]);
int update_values(char *cmd_arr[], int cmd_length);
int update_table(char *cmd_arr[],int cmd_length);
int update_db(char *cmd_arr[],int cmd_length);
int delete_db(char *cmd_arr[],int cmd_length);
int delete_table(char *cmd_arr[],int cmd_length);
int delete_values(char *cmd_arr[],int cmd_length);
void display_table(char *cmd_arr[], int cmd_length);
int fetch(char *cmd_arr[],int cmd_length);
int create_db(char *cmd_arr[],int cmd_length);
int create_table(char *cmd_arr[],int cmd_length);
int insert_fields(char *cmd_arr[], int cmd_length);
int get_last_id_from_table(char *table_path);
int insert_values(char *cmd_arr[], int cmd_length);
int insert_values_async(char *cmd_arr[],int cmd_length);
