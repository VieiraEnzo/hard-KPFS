#include <bits/stdc++.h>
#include "utils.hpp"
using namespace std;

#ifdef local
  #include "/mnt/c/Users/caiod/codaycoda/debug.hpp"
  #define pr(...) debug(#__VA_ARGS__, __VA_ARGS__)
  #define prs(...) debug_nameless(__VA_ARGS__)
#else
  #define pr(...) 69
  #define prs(...) 69
#endif

extern int num_items;
extern int num_forfeits;
extern vector <Forfeit> forfeits;
extern vector <Item> items;

deque <int> inserted_items;
int insert_single_item(Knapsack& knapsack, int tau){
  vector <int> candidates;
  for(int i = 0; i < num_items; i++){
    if(knapsack.can_insert(i)){
      candidates.push_back(i);
    }
  }

  vector <int> freq_forfeit = knapsack.get_freq_forfeit();
  int best_item = -1;
  double best_ratio = 0;
  for(int item : candidates){
    int associated_violations = 0;
    int associated_cost = 0;
    for(int i : items[item].get_local_forfeits()){
      if(freq_forfeit[i] >= forfeits[i].get_allowance()){
        #ifdef local
        assert(forfeits[i].is_inside(item));
        #endif
        associated_cost += forfeits[i].get_cost();
        associated_violations += 1;
      }  
    }
    // double curr_ratio = (double) (items[item].get_value()-associated_cost) / (items[item].get_weight());
    double curr_ratio = (double) (items[item].get_value()-associated_cost) / (items[item].get_weight()+tau*associated_violations);
    if(curr_ratio > best_ratio){
      best_ratio = curr_ratio;
      best_item = item; 
    }
  }
  if(best_item != -1){
    #ifdef local
    assert(knapsack.can_insert(best_item));
    #endif
    knapsack.insert_item(best_item);
    inserted_items.push_back(best_item);
  }

  #ifdef local
  assert(knapsack.is_feasible());
  #endif

  return best_item;
}

void simple_greedy(Knapsack& knapsack, int tau){
  int last_added_item = 0;
  while(last_added_item != -1){
    last_added_item = insert_single_item(knapsack, tau);
  }
}

void carousel_greedy(Knapsack& knapsack, int alpha, int tau, double beta){
  simple_greedy(knapsack, tau);
  int num_erase_items = beta * knapsack.get_size();
  int erased_items = 0;
  while(erased_items < num_erase_items && knapsack.get_size() > 0){
    #ifdef local
    assert(inserted_items.size() > 0);
    #endif
    int item = inserted_items.back();
    inserted_items.pop_back();
    if(knapsack.can_erase(item)){
      knapsack.erase_item(item);
      erased_items += 1;
    }
  }
  int iterations = alpha * knapsack.get_size();
  for(int i = 0; i < iterations; i++){
    bool removed = false;
    while(!removed && knapsack.get_size() > 0){
      int item = inserted_items.front();
      inserted_items.pop_front();
      if(knapsack.can_erase(item)){
        knapsack.erase_item(item);
        removed = true;
      }     
    }
    insert_single_item(knapsack, tau);
  }
  simple_greedy(knapsack, tau);
}