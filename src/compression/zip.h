#include <bits/stdc++.h>
#include <file_utils.h>

using namespace std;

#ifndef Zip_H
#define Zip_H

struct HuffmanNode
{
  set<char> letters;
  int occurrences;
  HuffmanNode *left = nullptr;
  HuffmanNode *right = nullptr;

  HuffmanNode(set<char> letters, int occurrences)
  {
    this->letters = letters;
    this->occurrences = occurrences;
  }

  HuffmanNode(set<char> letters, int occurrences, HuffmanNode *left, HuffmanNode *right)
  {
    this->letters = letters;
    this->occurrences = occurrences;
    this->left = left;
    this->right = right;
  }
};

struct HuffmanNodeCompare
{
  bool operator()(HuffmanNode *a, HuffmanNode *b)
  {
    return b->occurrences < a->occurrences;
  }
};

void codes_help(HuffmanNode *root, const vector<bool> &code, std::unordered_map<char, vector<bool>> &encoding_dictionary)
{
  if (root->left == nullptr && root->right == nullptr)
  {
    encoding_dictionary[*root->letters.begin()] = code;
    return;
  }

  vector<bool> code0(code);
  vector<bool> code1(code);

  code0.push_back(false);
  code1.push_back(true);

  codes_help(root->left, code0, encoding_dictionary);
  codes_help(root->right, code1, encoding_dictionary);
}

std::unordered_map<char, vector<bool>> get_encoding_dictionary(HuffmanNode *root)
{
  std::unordered_map<char, vector<bool>> encoding_dictionary;
  codes_help(root, vector<bool>(), encoding_dictionary);
  return encoding_dictionary;
}

vector<bool> encode(const std::unordered_map<char, vector<bool>> &encoding_dictionary, const string &text)
{
  vector<bool> encoded_text;

  for (auto item : text)
  {
    encoded_text.insert(encoded_text.end(), encoding_dictionary.at(item).begin(), encoding_dictionary.at(item).end());
  }

  return encoded_text;
}

HuffmanNode *build_huffman_tree(const std::unordered_map<char, int> &occurrences)
{
  vector<HuffmanNode *> nodes;
  for (auto letter : occurrences)
  {
    set<char> letters;
    letters.insert(letter.first);
    nodes.push_back(new HuffmanNode(letters, letter.second));
  }

  priority_queue<HuffmanNode *, vector<HuffmanNode *>, HuffmanNodeCompare> heap(nodes.begin(), nodes.end());

  while (heap.size() > 1)
  {
    auto left = heap.top();
    heap.pop();
    auto right = heap.top();
    heap.pop();

    set<char> newLetters;
    newLetters.insert(left->letters.begin(), left->letters.end());
    newLetters.insert(right->letters.begin(), right->letters.end());
    auto node = new HuffmanNode(newLetters, left->occurrences + right->occurrences, left, right);
    heap.push(node);
  }

  return heap.top();
}

void write_encoded_file(ofstream &out_file, const std::unordered_map<char, vector<bool>> &encoding_dictionary, const vector<bool> &encoded_text)
{
  int dictSize = encoding_dictionary.size();
  out_file.write(reinterpret_cast<const char *>(&dictSize), sizeof(dictSize));

  for (auto pair : encoding_dictionary)
  {
    out_file.write(&pair.first, sizeof(char));
    write_vector_bool_to_file(out_file, pair.second);
  }

  write_vector_bool_to_file(out_file, encoded_text);
  out_file.close();
}

#endif