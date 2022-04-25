#define DATA_PATH "../data/"
#define SIZE 1000
#define ID_LEN 7
#define COL_LEN 25

int split_string(char *str, char *dlm,char *arr[]);
void join_string(char *str_arr[],int idx,char *dlm,char *join_str);
void free_str_arr(char *arr[],int idx);
int is_db_exists_and_valid(char *db_name);
int is_table_exists_and_valid(char *db_name,char *table_name);
int is_file_exists(char *file_path);
int is_file_empty(char *file_path);
void make_table_path(char *db_name,char *table_name,char *table_path);
int delete_table_path(char *db_name,char *table_name);
void make_db_path(char *db_name,char *db_path);
void man();
void print_csv_header(char *header[],int len);
void print_csv_row(char *row[], int row_len);
