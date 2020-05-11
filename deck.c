#include <stdio.h>
#include <stdlib.h>

#include "deck.h"

void copy(int * to, int * from, int num_cards)
{
    for (int i=0; i<num_cards; i++)
        to[i]=from[i];
}

void combined_equation(int * to, int * from, int a, int b, int num_cards)
{
    for (int i=0; i<num_cards; i++)
    {
        int dest=((a*i)+b+num_cards)%num_cards;
        to[dest]=from[i];
    }
}

void deal_new_stack(int * to, int * from, int num_cards)
{
    for (int i=0; i<num_cards; i++)
        to[num_cards-i-1]=from[i];
}

void cut_n_card(int * to, int * from, int num_to_cut, int num_cards)
{
    if (num_to_cut > 0)
    {
        for (int i=0; i<num_to_cut; i++)
            to[num_cards-num_to_cut+i]=from[i];
        for (int i=num_to_cut; i<num_cards; i++)
            to[i-num_to_cut]=from[i];
    }
    else
    {
        int abs_num_to_cut=-1*num_to_cut;
        for (int i=0; i<abs_num_to_cut; i++)
            to[i]=from[num_cards-abs_num_to_cut+i];
        for (int i=abs_num_to_cut; i<num_cards; i++)
            to[i]=from[i-abs_num_to_cut];
    }
}

void deal_increment_n(int * to, int * from, int num_increment, int num_cards)
{
    for (int i=0; i<num_cards; i++)
        to[(num_increment*i)%num_cards]=from[i];
}

void print(int * cards, int num_cards)
{
    for (int i=0; i<num_cards; i++)
    {
        if (i%20==0)
            printf("%05d: ", i);
        printf("%d ", cards[i]);
        if ((i==num_cards-1)||(i%20==19))
            printf("\n");
    }
}

long long int getSourceDealNew(long long int dest, long long int num_cards)
{
    return (num_cards-1)-dest;
}

long long int getSourceCut(long long int dest, long long int num_cut_cards, long long int num_cards)
{
    // num_cut_cards=3, num_cards=10
    // 0 1 2 3 4 5 6 7 8 9
    // cut 3
    // 3 4 5 6 7 8 9 0 1 2
    
    // num_cut_cards=-4, num_cards=10
    // 0 1 2 3 4 5 6 7 8 9
    // cut -4
    // 6 7 8 9 0 1 2 3 4 5
    
    if (num_cut_cards > 0)
    {
        long long int switch_location=num_cards-num_cut_cards; // 7
        if (dest >= switch_location)
            return dest-switch_location; // destination of 8 return 8-7=1
        else
            return dest+num_cut_cards; // destination of 4 return 4+3=7
    }
    else
    {
        long long int switch_location=num_cut_cards*-1ll; // that's negative 1 as a long long
        if (dest >= switch_location)
            return dest-switch_location; // destination of 7 returns 7-4=3
        else
            return num_cards+num_cut_cards+dest; // destination of 1 returns 10+-4+1=7
    }
}

long long int getSourceDealIncrement(long long int dest, long long int num_increment_cards, long long int num_cards)
{
    // num_increment_cards=3, num_cards=10
    // 0 1 2 3 4 5 6 7 8 9
    // increment 3
    // 0 7 4 1 8 5 2 9 6 3   10%3=1 (num cards % num_increment_cards)
    // increment 7
    // 0 3 6 9 2 5 8 1 4 7   10%7=3
    int remainder_per_row=num_cards%num_increment_cards;
    long long int pos=0;
    long long int used=0;
    
    for (int i=0; i<num_increment_cards; i++)
    {
        if ((dest-pos)%num_increment_cards == 0) // in the row
        {
            used+=((dest-pos)/num_increment_cards);
            break;
        }
        else
        {
            used+=(((num_cards-pos)/num_increment_cards)+1);
            pos=num_increment_cards-((num_cards-pos)%num_increment_cards);
        }
    }
    
    // for increment 3. searching for what goes to 8
    // used: 4 6 
    // pos:  2 
    
    // for increment 7. searching for what goes to 8
    // used: 2 3 4 
    // pos:  4 1 
    
    return used;
}

long long int getSource(long long int dest, operation op, long long int num_cards)
{
    if (op.op==OP_DEAL_NEW)
        return getSourceDealNew(dest, num_cards);
    if (op.op==OP_CUT)
        return getSourceCut(dest, op.num, num_cards);
    if (op.op==OP_DEAL_INCREMENT)
        return getSourceDealIncrement(dest, op.num, num_cards);
    fprintf(stderr, "Invalid operation %d\n", op.op);
    exit(1);
}
