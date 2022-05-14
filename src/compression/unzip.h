#include <bits/stdc++.h>
#include <file_utils.h>

using namespace std;

#ifndef Unzip_H
#define Unzip_H

string decode(const unordered_map<string, char> &decoding_dictionary, const string &encoded_text)
{
  string decoded_text = "";
  string codeword;

  for (auto item : encoded_text)
  {
    codeword += item;
    if (decoding_dictionary.count(codeword))
    {
      decoded_text += decoding_dictionary.at(codeword);
      codeword = "";
    }
  }

  return decoded_text;
}

unordered_map<string, char> get_decoding_dictionary_from_file(ifstream &in_file)
{
  int dictionarySize;
  in_file.read((char *)&dictionarySize, sizeof(dictionarySize));

  unordered_map<string, char> decoding_dictionary;

  for (int i = 0; i < dictionarySize; i++)
  {
    char character;
    in_file.read(&character, sizeof(character));

    vector<bool> binCodeword = read_vector_bool_from_file(in_file);

    string codeword;
    for (auto bit : binCodeword)
    {
      codeword += bit ? "1" : "0";
    }

    decoding_dictionary.insert(pair<string, char>(codeword, character));
  }

  return decoding_dictionary;
}

string get_encoded_text_from_file(ifstream &in_file)
{
  vector<bool> input = read_vector_bool_from_file(in_file);

  string encoded_text;
  for (auto bit : input)
  {
    encoded_text += bit ? "1" : "0";
  }

  return encoded_text;
}

#endif