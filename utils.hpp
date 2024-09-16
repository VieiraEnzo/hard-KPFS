#ifndef _UTILS_HPP_
#define _UTILS_HPP_

#include <bits/stdc++.h>
using namespace std;

extern int num_items;
extern int num_forfeits;

class Item{
  private:
    int weight;
    int value;
    int id;
    vector <int> local_forfeits; //trocar pra um set??
  public:
    Item(int weight, int value, int id){
      this->weight = weight;
      this->value = value;
      this->id = id;
    }
    void add_forfeit(int forfeit){
      #ifdef local
      assert(forfeit >= 0 && forfeit < num_forfeits);
      for(int other_forfeit : local_forfeits){
        assert(other_forfeit != forfeit);
      }
      #endif
      local_forfeits.push_back(forfeit);
    }
    vector <int> get_local_forfeits(){
      return local_forfeits;
    }
    int get_weight(){
      return weight;
    }
    int get_value(){
      return value;
    }
    int get_id(){
      return id;
    }
};

class Forfeit{
  private:
    int cardinality;
    int allowance;
    int cost;
    vector <bool> is_element;
  public:
    Forfeit(int cardinality, int allowance, int cost, vector<int> elements){
      this->cardinality = cardinality;
      this->allowance = allowance;
      this->cost = cost;
      #ifdef local
      assert(this->cardinality == elements.size());
      #endif
      is_element.assign(num_items, false);
      for(int element : elements){
        is_element[element] = true;
      }
    }
    int get_cardinality(){
      return cardinality;
    }
    int get_allowance(){
      return allowance;
    }
    int get_cost(){
      return cost;
    }
    bool is_inside(int item){
      return is_element[item];
    }
};

extern vector <Forfeit> forfeits;
extern vector <Item> items;

class Knapsack{
  private:
    int total_capacity;
    int used_capacity;
    int total_conflits;
    int used_conflicts;
    int profit;
    int size;
    vector <bool> is_element;
    vector <int> freq_forfeit;
  public:
    Knapsack(int total_capacity, int total_penalty){
      this->total_capacity = total_capacity;
      this->total_conflits = total_penalty;
      this->used_capacity = this->used_conflicts = this->profit = this->size = 0;
      is_element.assign(num_items, false);
      freq_forfeit.assign(num_forfeits, 0);
    }
    int get_total_capacity(){
      return total_capacity;
    }
    int get_used_capacity(){
      return used_capacity;
    }
    int get_size(){
      return size;
    }
    int get_profit(){
      return profit;
    }
    bool is_inside(int item){
      return is_element[item];
    }
    vector <int> get_freq_forfeit(){
      return freq_forfeit;
    }
    bool can_insert(int item){
      if(is_element[item]) return false;
      if(used_capacity + items[item].get_weight() > total_capacity) return false;
      int new_conflicts = 0;
      int curr_profit = items[item].get_value();
      for(int i : items[item].get_local_forfeits()){
        if(freq_forfeit[i] >= forfeits[i].get_allowance()){
          #ifdef local
          assert(forfeits[i].is_inside(item));
          #endif
          new_conflicts += 1;
          curr_profit -= forfeits[i].get_cost();
        }
      }
      return (used_conflicts + new_conflicts <= total_conflits && curr_profit > 0);
    }
    void insert_item(int item){
      #ifdef local
      assert(can_insert(item));
      #endif
      used_capacity += items[item].get_weight();
      profit += items[item].get_value();
      for(int i : items[item].get_local_forfeits()){
        if(freq_forfeit[i] >= forfeits[i].get_allowance()){
          #ifdef local
          assert(forfeits[i].is_inside(item));
          #endif          
          used_conflicts += 1;
          profit -= forfeits[i].get_cost();
        }
        if(forfeits[i].is_inside(item)) freq_forfeit[i] += 1;
      }
      is_element[item] = true;
      size += 1;
    }
    bool can_erase(int item){
      return is_element[item];
    }
    void erase_item(int item){
      #ifdef local
      assert(can_erase(item));
      #endif
      used_capacity -= items[item].get_weight();
      profit -= items[item].get_value();
      for(int i : items[item].get_local_forfeits()){
        if(freq_forfeit[i] > forfeits[i].get_allowance()){
          #ifdef local
          assert(forfeits[i].is_inside(item));
          #endif
          used_conflicts -= 1;
          profit += forfeits[i].get_cost();
        }
        if(forfeits[i].is_inside(item)) freq_forfeit[i] -= 1;
      }
      size -= 1;
      is_element[item] = false;
    }
    //asserts the knapsack is feasible and the cost is calculated correctly
    bool is_feasible(){
      #ifndef local
      assert(0);
      #endif
      int temp_used_capacity = 0;
      int temp_profit = 0;
      int temp_size = 0;
      for(int i = 0; i < num_items; i++){
        if(is_element[i]){
          temp_used_capacity += items[i].get_weight();
          temp_profit += items[i].get_value();
          temp_size += 1;
        }
      }
      if(temp_size != size){
        return false;
      }
      if(temp_used_capacity != used_capacity || used_capacity > total_capacity){
        return false;
      }
      int temp_used_conflicts = 0;
      for(int i = 0; i < num_forfeits; i++){
        int current_used_conflicts = 0;
        for(int j = 0; j < num_items; j++){
          if(forfeits[i].is_inside(j) && is_element[j]){
            current_used_conflicts += 1;
          }
        }
        temp_profit -= max(0, current_used_conflicts - forfeits[i].get_allowance())*forfeits[i].get_cost();
        temp_used_conflicts += max(0, current_used_conflicts - forfeits[i].get_allowance());
      }
      if(temp_profit != profit || profit < 0){
        return false;
      }
      if(temp_used_conflicts != used_conflicts || used_conflicts > total_conflits){
        return false;
      }
      return true;
    }
};

#endif