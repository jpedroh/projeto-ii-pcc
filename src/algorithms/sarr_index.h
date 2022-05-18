#include <bits/stdc++.h>

using namespace std;

#ifndef Sarr_index_H
#define Sarr_index_H


void build_P(vector<vector<int>> &P, string &txt)
{
    int n = (int)txt.size();
    vector<int> firstP;

    for(auto chr : txt)
        firstP.push_back((int)chr);
    P[0] = firstP;

    for(int k = 1; k <= (int)P.size() - 1; k++){
        int l = (1 << (k-1)); // 2^(k-1)

        vector<tuple<int, int, int>> tupleAux(n); 
        for(int i = 0; i < n; i++){
            int s = (i+l >= n ? -1 : P[k-1][i+l]); //caso n tenha o elemento, fica -1
            tupleAux[i] = {P[k-1][i], s, i};
        }
        sort(tupleAux.begin(), tupleAux.end());
        
        vector<int> auxP(n);
        int order = 0;
        auxP[get<2>(tupleAux[0])] = order;

        for(int i = 1; i < n; i++){
            if((get<0>(tupleAux[i]) != get<0>(tupleAux[i-1])) || ( get<1>(tupleAux[i]) != get<1>(tupleAux[i-1])))
                order++;
            auxP[get<2>(tupleAux[i])] = order;
        }

        P[k] = auxP;
    }
}

void build_sarr(vector<int> &sa, vector<int> &p)
{
    for(int i = 0; i < (int)sa.size(); i++)
        sa[p[i]] = i;
}

int lcp(vector<vector<int>> &P, int i, int j)
{
    int n = (int)P[0].size();
    if(i == j)
        return n-i; //tamanho do sufixo do idx i
    int k = (int)P.size()-1, lcp=0;
    while((k >= 0) && (i < n) && (j < n)){
        int l = (1 << k);
        if(P[k][i] == P[k][j]){
            lcp+=l;
            i+=l;
            j+=l;
        }
        k--;
    }
    return lcp;
}

void build_LR_lcp(string &txt, vector<vector<int>> &P, vector<int> &sa, vector<int> &llcp, vector<int> &rlcp, int l, int r)
{
    if(r-l <= 1)
        return;
    int h = ((l+r)/2);
    llcp[h] = lcp(P, sa[l], sa[h]);
    rlcp[h] = lcp(P, sa[h], sa[r]);
    build_LR_lcp(txt, P, sa, llcp, rlcp, l, h);
    build_LR_lcp(txt, P, sa, llcp, rlcp, h, r);
}

void write_vector_in_file(vector<int> &arr, ofstream &file)
{
    for (auto val : arr) {
        file.write(reinterpret_cast<const char *>(&val), sizeof(int));
        if (file.bad()) {
            throw runtime_error("Falha ao escrever o vetor no arquivo!");
        }
    }
}

#endif