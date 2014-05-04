
#include<stdio.h>
#include<pr_mt19937.h>

#define NN       100000000UL

int main(void){ 
   long i; unsigned int sum=0;
   bool greater=false;
   double percent=0.0;
   double max=(double)0xffffffff;
   mt19937_state state; mt19937_sse_state sse_state;
   mt19937_init_(&state); mt19937_get_sse_state_(&state,&sse_state);
   mt19937_print_state_(&state);
   for(i=0;i<NN;i++) {
    unsigned int val=mt19937_sse_generate_(&sse_state);
    if (val>0xffffffff) {
      greater=true;
    }
    double U=val/max;
    if (U>percent) {
      percent=U;
    }
    sum+=val;
   }
   printf("%ld MT19937 pseudorandom numbers generated using SSE instructions and 128-bit XMM-registers of CPU.\n",NN);
   printf("Fractional part of the total sum of generated numbers: %f\n",sum/4294967296.);
   printf("Next output value: %f\n",mt19937_sse_generate_(&sse_state)/4294967296.);
   if (greater) {
     printf("Greater than 32 bit.");
   } else {
    printf("Under 32 bit\n");
   }
   printf("max U %f\n", percent);
   return 0;
}
