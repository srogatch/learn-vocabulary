#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <filesystem>
#include <locale>
using namespace std;

vector<vector<wstring>> vocabulary;

vector<wstring> SplitLine(const wstring& line) {
  vector<wstring> ans;
  wstring last;
  for(size_t i=0; i<line.size(); i++) {
    if(line[i] == L',') {
      ans.emplace_back(last);
      last.clear();
      continue;
    }
    last.push_back(line[i]);
  }
  ans.emplace_back(last);
  return ans;
}

int main() {
  locale mylocale("en_US.UTF8");
  locale::global(mylocale);

  wcout.imbue(mylocale);
  wcin.imbue(mylocale);

  wcout << L"Enter the vocabulary file name: ";
  wstring voc_fn;
  wcin >> voc_fn;
  wifstream ifs{filesystem::path(voc_fn)};
  wstring line;
  if(!getline(ifs, line)) {
    wcout << L"No header found." << endl;
    return 1;
  }
  vocabulary.emplace_back(SplitLine(line));
  for(int i=0; i<vocabulary[0].size(); i++) {
    if(vocabulary[0][i].empty()) {
      wcout << L"Language titles cannot be empty in the header line." << endl;
      return 2;
    }
  }
  while(getline(ifs, line)) {
    vocabulary.emplace_back(SplitLine(line));
  }
  if(vocabulary.size() < 2) {
    wcout << L"No words in the vocabulary" << endl;
    return 3;
  }
  random_device rd;
  mt19937_64 rng(rd());
  for(;;) {
    const int i_word = rng() % (vocabulary.size() - 1) + 1;
    const int i_src = rng() % vocabulary[0].size();
    wstring src = vocabulary[i_word][i_src];
    if(src.empty()) {
      continue;
    }
    wcout << "+++\n" << vocabulary[0][i_src] << ": " << src << endl;
    for(int i=0; i<vocabulary[i_word].size(); i++) {
      if(i == i_src) {
        continue;
      }
      if(vocabulary[i_word][i].empty()) {
        continue;
      }
      wcout << vocabulary[0][i] << ": ";
      wstring trans;
      if(!getline(wcin, trans)) {
        wcout << L"User request to exit" << endl;
        break;
      }
      if(vocabulary[i_word][i] != trans) {
        wcout << L"Wrong. Correct: " << vocabulary[i_word][i] << endl;
      }
    }
  }
  return 0;
}