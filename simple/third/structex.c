#include <stdio.h>
#include <stdlib.h>

enum deptcode { sales, personnel, packing, engineering };

typedef enum deptcode DEPT;

struct person {
  int age, salary;
  DEPT department;
  char name[12];
  char address[6][20];
};

typedef struct person Employee;

void read_line(char * str) {
  int i=0; char next;

  while((next=getchar()) != '\n') {
    str[i] = next;
    i++;
  }
  str[i] = '\0';
}

void print_employee(Employee emp) {
  int i;
  printf(" %d %d %d\n", emp.age, emp.salary, emp.department);
  printf("%s\n", emp.name);
  for (i=0; i<=5; i++)
    printf("%s\n", emp.address[i]);
}

int main(int argc, char const *argv[])
{
  Employee emp;
  int i;

  scanf("%d", &emp.age);
  scanf("%d", &emp.salary);
  scanf("%d\n", (int *) &emp.department);

  read_line(emp.name);
  for (i=0; i<=5; i++)
    read_line(emp.address[i]);
  print_employee(emp);

  return 0;
}
