#ifndef __DECK_H__
#define __DECK_H__

void copy(int * to, int * from, int num_cards);
void deal_new_stack(int * to, int * from, int num_cards);
void cut_n_card(int * to, int * from, int num_to_cut, int num_cards);
void deal_increment_n(int * to, int * from, int num_increment, int num_cards);
void print(int * cards, int num_cards);

#endif
