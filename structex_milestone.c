#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADDR_LEN 6
#define STR_LEN 20

enum deptcode { sales, personnel, packing, engineering };

typedef enum deptcode DEPT;

struct person {
  int age, salary;
  DEPT department;
  char name[STR_LEN];
  char address[ADDR_LEN][STR_LEN];
};

typedef struct person Employee;

void print_employee(Employee emp) {
  int i=0;

  printf("Name: %s\n", emp.name);
  printf("Age: %d\n", emp.age);
  printf("Salary: %d\n", emp.salary);
  for (i = 0; i < ADDR_LEN; i++) 
    printf("Address line %d/%d: %s\n", i+1, ADDR_LEN, emp.address[i]);
}

void read_stdin(char *line) {
  fgets(line, STR_LEN, stdin);
}

void read_int(char *line, int *dest) {
  read_stdin(line);
  sscanf(line, "%d", dest);
}

void read_char(char *line, char *dest) {
  read_stdin(line);
  sscanf(line, "%c", dest);
}

void read_line(char *line, char *dest) {
  int line_len;

  read_stdin(line);
  line_len = strlen(line);

  strncpy(dest, line, line_len - 1);
  dest[line_len-1] = '\0';
}

int main(int argc, char const *argv[]) {

  int i;
  char reply;
  char line[STR_LEN];
  Employee emp;

  while (reply != 'n') {
    printf("Insert name: ");
    read_line(line, emp.name);

    printf("Insert age: ");
    read_int(line, &emp.age);

    printf("Insert salary: ");
    read_int(line, &emp.salary);

    for (i = 0; i < ADDR_LEN; ++i)
    {
      printf("Insert address line %d/%d: ", i + 1, ADDR_LEN);
      read_line(line, emp.address[i]);
    }

    printf("\nYou have inserted this employee.\n\n");
    print_employee(emp);

    do {
      printf("\nDo you wish to continue (y/n)? ");
      read_char(line, &reply);
    } while (reply != 'n' && reply != 'y');
  }

  return EXIT_SUCCESS;
}
