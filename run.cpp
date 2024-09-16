#include <bits/stdc++.h>
#include <cstdio>
using namespace std;

#ifdef local
  #include "/mnt/c/Users/caiod/codaycoda/debug.hpp"
  #define pr(...) debug(#__VA_ARGS__, __VA_ARGS__)
  #define prs(...) debug_nameless(__VA_ARGS__)
#else
  #define pr(...) 69
  #define prs(...) 69
#endif

void run(string input, string output, string scenario){
  string command = "./main " + input + " " + output + " " + scenario;
  pr(command);
  system(command.c_str());
}

int main(int argc, char *argv[]){
  if(argc != 2){
    cout << "se mata" << endl;
    return 0;
  }
  string scenario = argv[1];
  vector <string> types = {"/correlated_sc", "/fully_correlated_sc", "/not_correlated_sc"};
  vector <string> sizes = {"/300", "/500", "/700", "/800", "/1000"};
  for(string type : types){
    for(string size : sizes){
      string input = "instances/scenario"+scenario+type+scenario+size;
      string output = "results/scenario"+scenario+type+scenario+size;
      run(input, output, scenario);
    }
  }
  return 0;
}