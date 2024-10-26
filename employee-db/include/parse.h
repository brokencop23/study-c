#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x4c4c4144
#define HEADER_VERSION 0x1
#define MAX_NAME 255
#define MAX_ADDRESS 255

struct dbheader_t {
    unsigned int magic;
    unsigned short version;
    unsigned short count;
    unsigned int filesize;
};

struct employee_t {
    char name[MAX_NAME];
    char address[MAX_ADDRESS];
    unsigned int hours;
};


int create_db_header(int fd, struct dbheader_t **headerOut);
int validate_db_header(int fd, struct dbheader_t **headerOut);
int read_employees(int fd, struct dbheader_t *header, struct employee_t **employeesOut);
int add_employee(struct dbheader_t *header, struct employee_t *employees, char *addstring);
void output_file(int fd, struct dbheader_t *header, struct employee_t *employees);
void list_employees(struct dbheader_t *header, struct employee_t *employees);

#endif
