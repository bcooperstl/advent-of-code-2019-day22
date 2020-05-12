#ifndef __DECK_H__
#define __DECK_H__

void copy(int * to, int * from, int num_cards);
void deal_new_stack(int * to, int * from, int num_cards);
void cut_n_card(int * to, int * from, int num_to_cut, int num_cards);
void deal_increment_n(int * to, int * from, int num_increment, int num_cards);
void combined_equation(int * to, int * from, int a, int b, int num_cards);
void print(int * cards, int num_cards);


#define OP_DEAL_NEW 1
#define OP_CUT 2
#define OP_DEAL_INCREMENT 3

struct operation{
    int op;
    int num;
    // a and b for f(x)=ax+b mod m
    long long int a;
    long long int b;  
};

typedef struct operation operation;

long long int getSourceDealNew(long long int dest, long long int num_cards);
long long int getSourceCut(long long int dest, long long int num_cut_cards, long long int num_cards);
long long int getSourceDealIncrement(long long int dest, long long int num_increment_cards, long long int num_cards);

long long int getSource(long long int dest, operation op, long long int num_cards);


#endif
