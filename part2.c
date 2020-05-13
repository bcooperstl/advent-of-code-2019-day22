#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deck.h"

#define MAX_STEPS 256
#define MAX_HISTORY 100000

#define ONE 1ll // 1 as a long long
#define TWO 2ll // 2 as a long long
#define TEN 10ll // 10 as a long long
#define MINUS_ONE -1ll // -1 as long long.

// getting around 64 bit overflow in modular multiplication
long long int multiplication_by_stupidity(long long int a, long long int b, long long int modulo)
{
    long long int sign=ONE;
    long long int copy_a=a;
    long long int copy_b=b;
    long long int result=0;
    int power=0;
    
    if (copy_a < 0)
    {
        sign=sign*MINUS_ONE;
        copy_a=copy_a*MINUS_ONE; // make copy_a positive
    }
    if (copy_b < 0)
    {
        sign=sign*MINUS_ONE;
        copy_b=copy_b*MINUS_ONE; // make copy_b positive
    }
    
    while (copy_b > 0)
    {
        long long int val=copy_b%TEN;
        long long int intermed=(copy_a*val)%modulo;
        for (int i=0; i<power; i++)
        {
            intermed=(intermed*10)%modulo;
        }
        
        result=(result+intermed)%modulo;
        copy_b/=TEN; // divide 
        power++;
    }
    
    // flip the sign on the result if needed
    result=((result*sign)+modulo)%modulo;
    
    //printf("%lld*%lld mod %lld is %lld\n", a, b, modulo, result);
    return result;
}

long long int exponent_by_squaring(long long int base, long long int power, long long int modulo)
{
    if (power==0)
        return ONE; // 1 long long
    if (power==1)
        return base%modulo;
    if (power%2==0)  // even power - return  base^2 to the (power/2)
        return exponent_by_squaring(multiplication_by_stupidity(base, base, modulo), power/TWO, modulo);
    // odd power greater than 1 - return base* (base^2 to the ((power/2)-1) 
    return multiplication_by_stupidity(base, exponent_by_squaring(multiplication_by_stupidity(base, base, modulo), (power-ONE)/TWO, modulo), modulo);
}

long long int modulo_mult_inverse(long long int base, long long int modulo)
{
    // my modulo is prime, so i can use a^(m-2)==a^-1 mod m
    return exponent_by_squaring(base, modulo-TWO, modulo);
}

int main (int argc, char * argv[])
{
    FILE * infile=NULL;
    char buffer[256];
    
    memset(&buffer, '\0', sizeof(buffer));
    
    operation operations[MAX_STEPS];
    long long int source[MAX_STEPS];
    long long int final_A;
    long long int final_B;
    
    long long int num_cards;
    long long int target_position;
    long long int num_deals;
    long long int repeat_counter=0;
    long long int prior;
    
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s filename num_cards target_position num_deals\n", argv[0]);
        exit(1);
    }
    
    num_cards=strtoll(argv[2], NULL, 10);
    target_position=strtoll(argv[3], NULL, 10);
    num_deals=strtoll(argv[4], NULL, 10);
    
    fprintf(stderr, "Opening file %s\n", argv[1]);
    
    infile=fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        exit(1);
    }
    
    int operation_position=0;
    while (fgets(buffer, sizeof(buffer), infile))
    {
        char * eol = strchr(buffer, '\r');
        if (eol)
        {
            *eol='\0';
        }
        eol = strchr(buffer, '\n');
        if (eol)
        {
            *eol='\0';
        }
        
        if (strstr(buffer, "deal into new stack") != NULL)
        {
            operations[operation_position].op=OP_DEAL_NEW;
            operations[operation_position].a=-1;
            operations[operation_position].b=-1;
        }
        else if (strstr(buffer, "deal with increment ") != NULL)
        {
            operations[operation_position].op=OP_DEAL_INCREMENT;
            operations[operation_position].num=atoi(buffer+20);
            operations[operation_position].a=operations[operation_position].num;
            operations[operation_position].b=0;
        }
        else if (strstr(buffer, "cut ") != NULL)
        {
            operations[operation_position].op=OP_CUT;
            operations[operation_position].num=atoi(buffer+4);
            operations[operation_position].a=1;
            operations[operation_position].b=operations[operation_position].num*-ONE; // multiplied by -1 as a long long
        }
        operation_position++;
    }
    
    fclose(infile);
    
    final_A=operations[0].a;
    final_B=operations[0].b;
    
    for (int i=1; i<operation_position; i++)
    {
        final_A=((final_A*operations[i].a)+num_cards)%num_cards;
        final_B=((final_B*operations[i].a)+operations[i].b+num_cards)%num_cards;
    }
    
    printf("The final equation is %lld*x+%lld mod %lld\n", final_A, final_B, num_cards);
    
    printf("But we need to run this equation %lld times\n", num_deals);
    
    long long int geometric_A=exponent_by_squaring(final_A, num_deals, num_cards);
    long long int geometric_B=multiplication_by_stupidity(multiplication_by_stupidity(final_B, (ONE-geometric_A), num_cards), modulo_mult_inverse(ONE-final_A, num_cards), num_cards); // geometric_A equals final_A to the k, so resuing it instead of recalculating it
    //printf("geometric_A is %lld\n", geometric_A);
    //printf("geometric_B is %lld\n", geometric_B);
    
    printf("That results in the equation %lld*x+%lld mod %lld\n", geometric_A, geometric_B, num_cards);
    
    printf("But we need to invert that equation since we know the answer we want, but not the starting value\n");
    
    long long int inverse_A=modulo_mult_inverse(geometric_A, num_cards);
    
    long long int actual_result=multiplication_by_stupidity(target_position-geometric_B, inverse_A, num_cards);
    
    printf("By some miracle, the result is %lld\n", actual_result);
    
    return 0;
}
