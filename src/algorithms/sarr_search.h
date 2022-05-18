#include <bits/stdc++.h>

using namespace std;

#ifndef Sarr_search_H
#define Sarr_search_H

pair<int, int> lex_cmp(string &x, string &y, int n, int m)
{
    int xs = (int)x.size() - n, ys = (int)y.size() - m;
    int size = min(xs, ys), lcp = 0;

    for(int i = 0; i < size; i++){
        if(x[i+n] == y[i+m])
            lcp++;
        else if(x[i+n] < y[i+m])
            return make_pair(-1, lcp);
        else
            return make_pair(1, lcp);
    }

    if(xs == ys)
        return make_pair(0, lcp);
    else if(xs < ys)
        return make_pair(-1, lcp);
    else
        return make_pair(1, lcp);
}

int bs(string &txt, string &pat, vector<int> &sarr, vector<int> &llcp, vector<int> &rlcp)
{
    pair<int, int> cmp_first = lex_cmp(pat, txt, 0, sarr[0]);
    pair<int, int> cmp_last = lex_cmp(pat, txt, 0, sarr.back());

    if (cmp_first.first <= 0)
        return -1;
    else if(cmp_last.first > 0)
        return sarr.size() - 1;
    else if(cmp_last.first == 0)
        return sarr.size() - 2;
    else { //binary search
        int l = 0, r = (int)sarr.size() - 1, mid;
        while((r - l) > 1){
            mid = (l + r)/2;
            pair<int, int> cmp_aux = lex_cmp(pat, txt, 0, sarr[mid]);

            if (cmp_aux.first == 0)
                return (mid-1);

            if(cmp_aux.first > 0){
                // M ----- M' ----- R
                // P -> lcp(P, M) = cmp_aux.second
                l = mid;
                int aux_m = (mid + r)/2;
                int aux_lcp = llcp[aux_m];

                if(cmp_aux.second < aux_lcp)
                    l = aux_m;
                else if(cmp_aux.second > aux_lcp)
                    r = aux_m;
                else {
                    //Comparando P[cmp_aux.second:] c/ txt[sarr[aux_m] + cmp_aux.second:] 
                    pair<int, int> cmp_mid = lex_cmp(pat, txt, cmp_aux.second, sarr[aux_m] + cmp_aux.second);

                    if (cmp_mid.first > 0)
                        l = aux_m;
                    else
                        r = aux_m;
                }
            } else {
                // L ----- M' ----- M
                // P -> lcp(P, M) = cmp_aux.second
                r = mid;
                int aux_m = (l + mid)/2;
                int aux_lcp = rlcp[aux_m];

                if(cmp_aux.second < aux_lcp)
                    r = aux_m;
                else if(cmp_aux.second > aux_lcp)
                    l = aux_m;
                else {
                    //Comparando P[cmp_aux.second:] c/ txt[sarr[aux_m] + cmp_aux.second:] 
                    pair<int, int> cmp_mid = lex_cmp(pat, txt, cmp_aux.second, sarr[aux_m] + cmp_aux.second);

                    if (cmp_mid.first > 0)
                        l = aux_m;
                    else
                        r = aux_m;
                }

            }
        }

        return l; 
    }
}

vector<int> search_sarr(string &txt, string &pat, vector<int> &sarr, vector<int> &llcp, vector<int> &rlcp)
{
    vector<int> occ;
    string pat_aux = pat+char(127);
    int l = bs(txt, pat, sarr, llcp, rlcp) + 1, r = bs(txt, pat_aux, sarr, llcp, rlcp);
    if (l <= r){
        for(int i = l; i <= r; i++)
            occ.push_back(sarr[i]);
    }

    return occ;
}

void read_vector_from_file(vector<int> &arr, ifstream &in_file)
{

    for (int i = 0; i < (int)arr.size(); i++)
    {
        int val;
        in_file.read(reinterpret_cast<char *>(&val), sizeof(int));
        if (in_file.bad())
            throw runtime_error("Falha ao ler o vetor no arquivo!");
        arr[i] = val;
    }
}

#endif