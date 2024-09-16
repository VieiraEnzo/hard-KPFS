#ifndef _CONSTRUCTIVE_HPP_
#define _CONSTRUCTIVE_HPP_

#include <bits/stdc++.h>
#include "utils.hpp"
using namespace std;

extern int num_items;
extern int num_forfeits;
extern vector <Forfeit> forfeits;
extern vector <Item> items;

int insert_single_item(Knapsack&);
void simple_greedy(Knapsack&);
void carousel_greedy(Knapsack&, int, int, double);

#endif