#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
  enum deg_class { FAIL, ORDINARY, THIRD, TWOTWO, TWOONE, FIRST };
  int mark;
  enum deg_class grade;
  char med_certificate;

  scanf("%d\n", &mark); /* in range 0..100 */
  if (mark < 45) grade      =  FAIL;
  else if (mark < 50) grade =  ORDINARY;
  else if (mark < 55) grade =  THIRD;
  else if (mark < 65) grade =  TWOTWO;
  else if (mark < 80) grade =  TWOONE;
  else grade                =  FIRST;

  if ((med_certificate = getchar()) == 'y' && grade < FIRST) grade++;

  switch(grade) {
    case FAIL     : printf("Student failed\n"); break;
    case ORDINARY : printf("Student obtained ordinary\n"); break;
    case THIRD    : printf("Student obtained third\n"); break;
    case TWOTWO   : printf("Student obtained two two\n"); break;
    case TWOONE   : printf("Student obtained two one\n"); break;
    case FIRST    : printf("Student obtained first\n"); break;
  }

  return 0;
}
