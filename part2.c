#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deck.h"

#define MAX_STEPS 256

int main (int argc, char * argv[])
{
    FILE * infile=NULL;
    char buffer[256];
    
    memset(&buffer, '\0', sizeof(buffer));
    
    operation operations[MAX_STEPS];
    long long int source[MAX_STEPS];
    
    long long int num_cards;
    long long int target_card;
    long long int num_deals;
    
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s filename num_cards target_card num_deals\n", argv[0]);
        exit(1);
    }
    
    num_cards=strtoll(argv[2]);
    target_card=strtoll(argv[3]);
    num_deals=strtoll(argv[4]);
    
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
    
    source[operation_position]=target_card;
    
    for (int i=operation_position-1; i>=0; i--)
    {
        source[i]=getSource(source[i+1], operation[i]);
    }
        
    print(current, num_cards);
    
    for (int i=0; i<num_cards; i++)
    {
        if (current[i]==2019)
            printf("Position %d has value 2019\n", i);
    }
    
    return 0;
}
