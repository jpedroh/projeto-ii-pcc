#include <bits/stdc++.h>

using namespace std;

#ifndef FileUtils_H
#define FileUtils_H

void write_vector_bool_to_file(ofstream &out_file, const vector<bool> &data)
{
  vector<bool>::size_type n = data.size();
  out_file.write((const char *)&n, sizeof(vector<bool>::size_type));
  for (vector<bool>::size_type i = 0; i < n;)
  {
    unsigned char aggr = 0;
    for (unsigned char mask = 1; mask > 0 && i < n; ++i, mask <<= 1)
    {
      if (data.at(i))
      {
        aggr |= mask;
      }
    }
    out_file.write((const char *)&aggr, sizeof(unsigned char));
  }
}

vector<bool> read_vector_bool_from_file(ifstream &in_file)
{
  vector<bool> data;
  vector<bool>::size_type n;
  in_file.read((char *)&n, sizeof(vector<bool>::size_type));
  data.resize(n);
  for (vector<bool>::size_type i = 0; i < n;)
  {
    unsigned char aggr;
    in_file.read((char *)&aggr, sizeof(unsigned char));
    for (unsigned char mask = 1; mask > 0 && i < n; ++i, mask <<= 1)
    {
      data.at(i) = aggr & mask;
    }
  }

  return data;
}

#endif