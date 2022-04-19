int split_string(char *str, char *dlm,char *arr[]);
void join_string(char *str_arr[],int idx,char *dlm,char *join_str);
void free_str_arr(char *arr[],int idx);
int is_db_exists_and_valid(char *db_name);
int is_table_exists_and_valid(char *db_name,char *table_name);
void make_table_path(char *db_name,char *table_name,char *table_path);
int delete_table_path(char *db_name,char *table_name);
void make_db_path(char *db_name,char *db_path);


void man(); // to display all the available features
