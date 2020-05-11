#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deck.h"

int main (int argc, char * argv[])
{
    FILE * infile=NULL;
    char buffer[256];
    
    memset(&buffer, '\0', sizeof(buffer));
    
    int num_cards;
    int current[20000];
    int next[20000];
    int num_steps=0;
    int a[100];
    int b[100];
    int final_a;
    int final_b;
    
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s filename num_cards\n", argv[0]);
        exit(1);
    }
    
    num_cards=atoi(argv[2]);
    for (int i=0; i<num_cards; i++)
        current[i]=i;
    
    fprintf(stderr, "Opening file %s\n", argv[1]);
    
    infile=fopen(argv[1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        exit(1);
    }
    
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
            a[num_steps]=-1;
            b[num_steps]=-1;
        }
        else if (strstr(buffer, "deal with increment ") != NULL)
        {
            int num=atoi(buffer+20);
            a[num_steps]=num;
            b[num_steps]=0;
        }
        else if (strstr(buffer, "cut ") != NULL)
        {
            int num=atoi(buffer+4);
            a[num_steps]=1;
            b[num_steps]=-1*num;
        }
        num_steps++;
    }
    
    fclose(infile);
    
    final_a=a[0];
    final_b=b[0];
    
    for (int i=1; i<num_steps; i++)
    {
        final_a=((final_a*a[i])+num_cards)%num_cards;
        final_b=(((final_b*a[i])+b[i])+num_cards)%num_cards;
    }

    printf("The final equation is %d*x+%d mod %d\n", final_a, final_b, num_cards);

    combined_equation(next, current, final_a, final_b, num_cards);
    
    copy(current, next, num_cards);

    print(current, num_cards);
    
    for (int i=0; i<num_cards; i++)
    {
        if (current[i]==2019)
            printf("Position %d has value 2019\n", i);
    }
    
    return 0;
}
