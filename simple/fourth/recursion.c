#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void perform_operation(float *answer, char operator, float operand) {
  float result;

  switch(operator) {
    case '+': *answer += operand;
              break;
    case '-': *answer -= operand;
              break;
    case '*': *answer *= operand;
              break;
    case '/': *answer /= operand;
              break;
  }
}

char* solve(char *input, float *answer) {
  int read_count;
  float new_number;
  char operator = '+';

  while (operator != ')' && operator != '=') {
    new_number = 0;

    if (input[0] == '(') {
      printf("found new sub-expression!\n");
      input = solve(input+1, &new_number);
    } else {
      printf("input starts with %c\n", input[0]);
      sscanf(input, "%f%n", &new_number, &read_count);
      input += read_count;
    }
    printf("%.1f %c %.1f\n", *answer, operator, new_number);

    perform_operation(answer, operator, new_number);

    operator = input[0];
    input++;
  }

  printf("exiting sub-expression or end\n");
  return input;
}

int main(int argc, const char *argv[]) {
  float answer = 0;
  char input[] = "2+3*(4-(6-5))=";

  solve(input, &answer);
  printf("The result is %f.\n", answer);

  return EXIT_SUCCESS;
}
