# Database Manipulator

This is CLI based linux application where user can use various functionality like creating database, creating table, updating database, updating table. It uses Process Management concept to enhance the performance of the operations.

## Steps to run the project
- clone this repository using `git clone https://github.com/vrutik2809/dbman.git`
- `cd dbman/src`
- `make all`
- `../bin/main.out`

## List of commands
|Command|Description|Throwable Erros|
|-------|-----------|---------------|
|`create db <db_name>`|create database named `db_name`|<ul><li>database already exists</li></ul>|
|`create table <db_name> <table_name>`|create table named `table_name` in `db_name` database|<ul><li>database doesn't exist</li> <li>table already exist</li></ul>|
|`insert fields <db_name> <table_name> <field_1> <field_2> ...`|initialize the table by inserting listed fields in `<db_name>.<table_name>` table|<ul><li>database doesn't exist</li> <li>table doesn't exist</li> <li>table already initialized</li></ul>|
|`insert values <db_name> <table_name> <val_1> <val_2> ...`|insert values in `<db_name>.<table_name>` table|<ul><li>database doesn't exist</li> <li>table doesn't exist</li> <li>numbet of values is not same as number of fields</li></ul>|
|`insert values async <db_name> <table_name_1> <csv_file_name_1> <table_name_2> <csv_file_name_2> ...`|insert all values of `csv_file_name` into corresponding `<db_name>.<table_name>` table asynchronously by creating individual process for each tuple of csv file and table|<ul><li>database doesn't exist</li> <li>table doesn't exist</li> <li>csv file doesn't exist</li> <li>numbet of fields in csv file is not same as number of fields in corresponding table</li></ul>|
|`display <db_name> <table_name>`|display `<db_name>.<table_name>` table|<ul><li>database doesn't exist</li> <li>table doesn't exist</li></ul>|
|`fetch <db_name> <table_name> <id_1> <id_2> ...`|fetch rows of listed ids from `<db_name>.<table_name>` table|<ul><li>database doesn't exist</li> <li>table doesn't exist</li> <li>id doesn't exist</li></ul>|
|`update db <db_name_old> <db_name_new>`|update database name from `db_name_old` to `db_name_new`|<ul><li>database doesn't exist</li></ul>|
|`update table <db_name> <table_name_old> <table_name_new>`|update table name from `table_name_old` to `table_name_new`|<ul><li>database doesn't exist</li> <li>table doesn't exist</li></ul>|
|`update values <db_name> <table_name> <id> <val_1> <val_2>`|update row of `id` in `<db_name>.<table_name>` table|<ul><li>database doesn't exist</li> <li>table doesn't exist</li> <li>id doesn't exist</li></ul>|
|`delete db <db_name>`|delete `db_name` database|<ul><li>database doesn't exist</li></ul>|
|`delete table <db_name> <table_name>`|delete `<db_name>.<table_name>` table|<ul><li>database doesn't exist</li> <li>table doesn't exist</li></ul>|
|`delete values <db_name> <table_name> <id>`|delete row of `id` from `<db_name>.<table_name>` table|<ul><li>database doesn't exist</li> <li>table doesn't exist</li> <li>id doesn't exist</li></ul>|
|`man`|print manual for all commands|
|`exit`|exit from the application|

## Demo
### preview of make and create command

![first](https://user-images.githubusercontent.com/74986666/174778555-176eb8d0-963f-4ce1-8ed3-1bce6f78cd62.gif)
### preview of insert and insert asynchronous command

![second](https://user-images.githubusercontent.com/74986666/174778597-49927a64-503a-416f-9487-08f31da262b0.gif)
### preview of update and delete command

![third](https://user-images.githubusercontent.com/74986666/174778616-49d3c47a-1d8d-4be4-9406-77b17c9c7eb2.gif)

