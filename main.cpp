#include <bits/stdc++.h>
#include "utils.hpp"
#include "constructive.hpp"
using namespace std;

#ifdef local
  #include "/mnt/c/Users/caiod/codaycoda/debug.hpp"
  #define pr(...) debug(#__VA_ARGS__, __VA_ARGS__)
  #define prs(...) debug_nameless(__VA_ARGS__)
#else
  #define pr(...) 69
  #define prs(...) 69
#endif

int num_items;
int num_forfeits;
vector <Forfeit> forfeits;
vector <Item> items;

int solve(Knapsack knapsack){
  carousel_greedy(knapsack, 2, 5, 0.05);
  return knapsack.get_profit();
}

int scenario = -1;
pair<int, double> process_file(const string &file_path){
  #ifdef local
  assert(scenario != -1);
  #endif
  ifstream file(file_path.c_str());
  int capacity;
  file >> num_items >> num_forfeits >> capacity;
  forfeits.clear();
  items.clear();
  vector <int> profits(num_items);
  for(int i = 0; i < num_items; i++){
    file >> profits[i];
  }
  vector <int> weights(num_items);
  for(int i = 0; i < num_items; i++){
    file >> weights[i];
  }
  for(int i = 0; i < num_items; i++){
    items.push_back(Item(weights[i], profits[i], i));
  }
  for(int i = 0; i < num_forfeits; i++){
    int allowance, cost, cardinality;
    file >> allowance >> cost >> cardinality; 
    vector <int> elements(cardinality);
    for(int j = 0; j < cardinality; j++){
      file >> elements[j];
      items[elements[j]].add_forfeit(i);
      #ifdef local
      assert(elements[j] >= 0 && elements[j] < num_items);
      #endif
    }
    forfeits.push_back(Forfeit(cardinality, allowance, cost, elements));
  }
  int max_forfeits;
  if(scenario < 3){
    if(num_items == 300){
      max_forfeits = round((double)num_items/15);
    }else if(num_items == 500){
      max_forfeits = round((double)num_items/25);
    }else if(num_items == 700){
      max_forfeits = round((double)num_items/35);
    }else if(num_items == 800){
      max_forfeits = round((double)num_items/45);
    }else if(num_items == 1000){
      max_forfeits = round((double)num_items/55);
    }else{
      #ifdef local
      assert(0);
      #endif
    }
  }else{
    max_forfeits = round((double)num_items/15);
  }

  auto start = std::chrono::high_resolution_clock::now();
  int best_profit = solve(Knapsack(capacity, max_forfeits));
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_taken = end - start;

  file.close();
  return {best_profit, time_taken.count()};
}

int main(int argc, char *argv[]){
  if(argc != 4){
    cout << "usage: " << endl;
    cout << argv[0] << " " << " <input_folder_name> <output_folder_name> <scenario>" << endl;
    cout << "<input_folder_name>: nome da pasta de instâncias" << endl;
    cout << "<output_folder_name>: nome da pasta de resultados" << endl;
    cout << "<scenario>: identificado do cenario da instancia" << endl;
    return 1;
  }
  scenario = stoi(argv[3]);
  namespace fs = std::filesystem;
  string input_path = argv[1];
  if(!(fs::exists(input_path) && fs::is_directory(input_path))){
    cerr << "Directory does not exist or is not a directory\n";
    return 1;
  }
  auto average = [&](vector <double> v){
    #ifdef local
    assert(v.size() > 0);
    #endif
    double sum = 0;
    for(auto x : v){
      sum += x;
    }
    return sum/v.size();
  };
  int num_executions = 1;
  int iterations = 0;
  vector <double> final_profits, final_times;
  for(const auto &entry : fs::directory_iterator(input_path)){
    if(entry.is_regular_file()){
      iterations += 1;
      vector <double> profits;
      vector <double> times;
      for(int i = 0; i < num_executions; i++){
        cout << "executing instance " << i << " for " << entry.path().string() << "..."  << endl;
        auto [profit, time] = process_file(entry.path().string());
        profits.push_back(profit);
        times.push_back(time);
      }
      double profit = average(profits);
      final_profits.push_back(profit);
      double time = average(times);
      final_times.push_back(time);
    }
  }
  string result_name = (string)argv[2] + "/result.txt";
  std::ofstream outfile(result_name);
  outfile << "folder/filename: " << input_path << endl;
  outfile << "solution costs: ";
  double total_profit = 0;
  for(auto cost : final_profits){
    outfile << cost << " ";
    total_profit += cost;
  }
  outfile << endl;
  outfile << "average cost: " << (total_profit)/iterations << endl;
  outfile << "solution times: ";
  double total_time = 0;
  for(auto time : final_times){
    outfile << time << " ";
    total_time += time;
  }
  outfile << endl;
  outfile << "average time: " << (total_time)/iterations << endl;

  return 0;
}