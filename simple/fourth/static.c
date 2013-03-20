#include <stdio.h>

int Static_Demo(int Init) {
   static int Hidden;
   if (Init==0)
     Hidden = Init;
   else
     Hidden++;

   if (Hidden<3)
     return Init;

   return Hidden;
}

void main() {
   printf("%d\n",Static_Demo(0));
   printf("%d\n",Static_Demo(1));
   printf("%d\n",Static_Demo(2));
   printf("%d\n",Static_Demo(3));
   printf("%d\n",Static_Demo(4));
   printf("%d\n",Static_Demo(1));
   printf("%d\n",Static_Demo(-10));
}
