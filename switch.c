#include <stdio.h>
#include <stdlib.h>

int main() {
  float result, value;
  char operator;

  scanf("%f\n", &result);

  while ((operator = getchar()) != '=') {
    scanf("%f\n", &value);

    switch (operator) {
      case '+': result += value;
                break;
      case '-': result -= value;
                break;
      case '*': result *= value;
                break;
      case '/': result /= value;
                break;
      case '%': result *= (value / 100);
                break;
      default:  printf("Incorrect operator '%c', try again\n", operator);
                break;
    }
  }
  printf("%f\n", result);

  return EXIT_SUCCESS;
}
