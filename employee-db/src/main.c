#include <stdio.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdlib.h>
#include "common.h"
#include "file.h"
#include "parse.h"


void print_usage(char *argv[]){
    printf("Usage: %s -n -f <path_to_db> -a <name,address,hours> -l \n", argv[0]);
    printf("\t -n - create new database file\n");
    printf("\t -f - (required) path to database file\n");
    printf("\t -a - add new employee\n");
    printf("\t -l - list all the employees\n");
    return;
}


int main(int argc, char *argv[]) {

    int c = 0;
    int dbfd = -1;
    char *filepath = NULL;
    char *addstring = NULL;
    bool new_file = false;
    bool list = false;
    struct dbheader_t *header = NULL;
    struct employee_t *employees = NULL;

    while ((c = getopt(argc, argv, "nf:a:l")) != -1) {
        switch(c) {
            case 'n':
                new_file = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'a':
                addstring = optarg;
                break;
            case 'l':
                list = true;
                break;
            case '?':
                printf("Unknown argument: %d\n", c);
            default:
                return STATUS_ERROR;
        }
    }

    if (filepath == NULL) {
        printf("You must specify filepath!");
        print_usage(argv);
        return -1;
    }

    if (new_file == true) {
        dbfd = create_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to create database file!\n");
            return -1;
        }
        if (create_db_header(dbfd, &header) == STATUS_ERROR) {
            return -1;
        }
    } else {
        dbfd = open_db_file(filepath);
        if (dbfd == STATUS_ERROR) {
            printf("Unable to open the database file!\n");
            return -1;
        }
        if (validate_db_header(dbfd, &header) == STATUS_ERROR) {
            printf("Failed to validate db header\n");
            return -1;
        }
    }

    if (read_employees(dbfd, header, &employees) == STATUS_ERROR){
        printf("Cant read employees from db\n");
        return -1;
    }

    if (addstring != NULL) {
        header->count++;
        employees = realloc(employees, header->count * sizeof(struct employee_t));
        if (employees == NULL){
            header->count--;
            printf("Cannot realloc employees");
            return -1;
        }
        add_employee(header, employees, addstring);
    }

    if (list == true) {
        list_employees(header, employees);
    }

    output_file(dbfd, header, employees);
    return 0;

}
