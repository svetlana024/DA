#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <tuple>
#include <sstream>

using namespace std;

int l = 0;
int r = 0;
int i = 1;
int k;

int Min(int first, int second){
  return( first <= second ? first
          : second);
}

int Max(int first, int second){
  return( first > second ? first
          : second);
}


void
ZFunction(vector<unsigned int> &pattern, vector<tuple<int, int, unsigned int>> &text, std::vector<int> &z){

    vector<unsigned int>::iterator it_pattern = pattern.begin();
    vector<tuple<int, int, unsigned int>>::iterator it_text = text.begin();
    z[i] = Max(0, Min(r - k + 1, z[k - l]));
    //std::cout << " l = " << l << ", r = " << r << " first z[" << i << "] = " << z[i] << std::endl;
    while( (it_pattern + z[i]) != pattern.end() && (it_text + z[i])!= text.end() && *(it_pattern + z[i]) == get<2>(*(it_text + z[i]))){
      z[i]++;
    }
    //std::cout << "z[" << i << "] = " << z[i] << std::endl;
    if(k + z[i] - 1 > r){
      l = k;
      r = k + z[i] - 1;
    }
    if(z[i] == pattern.size()){
      std::cout << get<0>(*it_text) << ", " << get<1>(*it_text) << std::endl;
    }
    text.erase(it_text);
    k++;
}

int main(){
    vector<unsigned int> pattern;
    vector<tuple<int, int, unsigned int>> text;

    string input, token;
    //double t = clock();

     do{
      getline(cin, input);
    } while(input.empty());

    stringstream patternSS(input);

    while (patternSS >> token) {
        pattern.push_back(stoul(token));
    }

    std::vector<int> z;
    z.resize(pattern.size(), 0);

    std::vector<unsigned int>::iterator it_pattern = pattern.begin();
    z[0] = 0;
    for( i = 1; i < pattern.size(); i++){
      z[i] = Max(0, Min(r - i + 1, z[i - l]));
      //std::cout << " l = " << l << ", r = " << r << " first z[" << i << "] = " << z[i] << std::endl;
      while(i + z[i] < pattern.size() && *(it_pattern + z[i]) == *(it_pattern + i + z[i])){
        z[i]++;
      }
      //std::cout << "z[" << i << "] = " << z[i] << std::endl;
      if(i + z[i] - 1 > r){
        l = i;
        r = i + z[i] - 1;
      }
    }
    z.resize(z.size() + 1);
    k = i;

    int line_count = 1;
    int word_count = 1;
    int key = 0;

    while (getline(cin, input)) {
        stringstream textSS(input);
        while (textSS >> token){
            text.push_back(make_tuple(line_count, word_count, stoul(token)));
            ++word_count;
            ++key;
            if( key == pattern.size()){
            	ZFunction(pattern, text, z);
                key--;
            }
         }
         line_count++;
         word_count = 1;
    }
    //std::cout << (clock() - t) / CLOCKS_PER_SEC << std::endl;
    return 0;
}
