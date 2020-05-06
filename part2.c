#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deck.h"

#define MAX_STEPS 256
#define MAX_HISTORY 100000

int main (int argc, char * argv[])
{
    FILE * infile=NULL;
    char buffer[256];
    
    memset(&buffer, '\0', sizeof(buffer));
    
    operation operations[MAX_STEPS];
    long long int source[MAX_STEPS];
    
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
        }
        else if (strstr(buffer, "deal with increment ") != NULL)
        {
            operations[operation_position].op=OP_DEAL_INCREMENT;
            operations[operation_position].num=atoi(buffer+20);
        }
        else if (strstr(buffer, "cut ") != NULL)
        {
            operations[operation_position].op=OP_CUT;
            operations[operation_position].num=atoi(buffer+4);
        }
        operation_position++;
    }
    
    fclose(infile);
    
    
    prior=target_position;
    
    for (int j=0; j<num_deals; j++)
    {    
        if (j%100000==0)
            printf("iteration %d\n", j);
        source[operation_position]=prior;
        for (int i=operation_position-1; i>=0; i--)
        {
            source[i]=getSource(source[i+1], operations[i], num_cards);
        }
        //printf("The source for %lld comes from %lld\n", prior, source[0]);
        if (source[0]==target_position)
            break;
        prior=source[0];
        repeat_counter++;
    }
    
    printf("Repeats to 2020 after %lld elements\n", repeat_counter);
    
    return 0;
}
