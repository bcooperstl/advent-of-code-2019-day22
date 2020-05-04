#include <stdio.h>

#include "deck.h"

void copy(int * to, int * from, int num_cards)
{
    for (int i=0; i<num_cards; i++)
        to[i]=from[i];
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
