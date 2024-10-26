#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "common.h"
#include "parse.h"


void output_file(int fd, struct dbheader_t *header, struct employee_t *employees) {
    if (fd < 0) {
        printf("Got bad file descriptor\n");
        return;
    }
    int real_count = header->count;
    header->magic = htonl(header->magic);
    header->version = htons(header->version);
    header->count = htons(header->count);
    header->filesize = htonl(sizeof(struct dbheader_t) + (sizeof(struct employee_t) * real_count));

    lseek(fd, 0, SEEK_SET);
    write(fd, header, sizeof(struct dbheader_t));

    int i = 0;
    for (i = 0; i < real_count; i++) {
        employees[i].hours = htonl(employees[i].hours);
        write(fd, &employees[i], sizeof(struct employee_t));
    }
    return;
}


int create_db_header(int fd, struct dbheader_t **headerOut){
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL) {
        printf("Cannot allocate memory for header\n");
        return STATUS_ERROR;
    }
    header->version = HEADER_VERSION;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof(struct dbheader_t);
    *headerOut = header;
    return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t **headerOut){
    if (fd < 0) {
        printf("Got bad file descriptor\n");
        return STATUS_ERROR;
    }
    
    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t));
    if (header == NULL) {
        printf("Cannot allocate memory for header\n");
        return STATUS_ERROR;
    }

    if (read(fd, header, sizeof(struct dbheader_t)) == -1) {
        perror("read");
        return STATUS_ERROR;
    }

    header->magic = ntohl(header->magic);
    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->filesize = ntohl(header->filesize);

    if (header->magic != HEADER_MAGIC) {
        printf("Wrong magic for file\n");
        free(header);
        return STATUS_ERROR;
    }

    if (header->version != HEADER_VERSION) {
        printf("Wrong version of the file\n");
        free(header);
        return STATUS_ERROR;
    }

    struct stat dbstat = {};
    if (fstat(fd, &dbstat) == -1){
        perror("fstat");
        free(header);
        return STATUS_ERROR;
    }
    if (header->filesize != dbstat.st_size){
        printf("Wrong filesize\n");
        free(header);
        return STATUS_ERROR;
    }

    *headerOut = header;
    return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *header, struct employee_t **employeesOut){
    if (fd < 0) {
        printf("Got bad file descriptor\n");
        return STATUS_ERROR;
    }
    int count = header->count;
    struct employee_t *employees = calloc(count, sizeof(struct employee_t));
    if (employees == NULL) {
        printf("Cannot allocate memory for employees\n");
        return STATUS_ERROR;
    }
    read(fd, employees, count * sizeof(struct employee_t));
    int i = 0;
    for (i = 0; i < count; i++) {
        employees[i].hours = htonl(employees[i].hours);
    }
    *employeesOut = employees;
    return STATUS_SUCCESS;
}

int add_employee(struct dbheader_t *header, struct employee_t *employees, char *addstring){
    char *name = strsep(&addstring, ",");
    char *address = strsep(&addstring, ",");
    char *hours = strsep(&addstring, ",");
    int i = header->count - 1;
    strncpy(employees[i].name, name, MAX_NAME);
    strncpy(employees[i].address, address, MAX_ADDRESS);
    employees[i].hours = atoi(hours);
    return STATUS_SUCCESS;
}

void list_employees(struct dbheader_t *header, struct employee_t *employees) {
    int i = 0;
    for (; i < header->count; i++) {
        printf("%s | %s | %d\n", employees[i].name, employees[i].address, employees[i].hours);
    }
}
