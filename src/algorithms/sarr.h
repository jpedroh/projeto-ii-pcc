#include <vector>
#include <string>
#include <fstream>
#include <math.h>
#include <tuple>
#include <algorithm>
#include <iostream>

using namespace std;

#ifndef Sarr_H
#define Sarr_H

class Sarr
{
    private:
    pair<int, int> lex_cmp(string x, string y)
    {
        int size = min((int)x.size(), (int)y.size()), lcp = 0;

        for(int i = 0; i < size; i++){
            if(x[i] == y[i])
                lcp++;
            else if(x[i] < y[i])
                return make_pair(-1, lcp);
            else
                return make_pair(1, lcp);
        }

        if(x.size() == y.size())
            return make_pair(0, lcp);
        else if(x.size() < y.size())
            return make_pair(-1, lcp);
        else
            return make_pair(1, lcp);
    }

    int succ(string &txt, string &pat, vector<int> &sarr, vector<int> &llcp, vector<int> &rlcp)
    {
        pair<int, int> cmp_first = lex_cmp(pat, txt.substr(sarr[0]));
        pair<int, int> cmp_last = lex_cmp(pat, txt.substr(sarr.back()));

        if (cmp_first.first <= 0)
            return 0;
        else if(cmp_last.first > 0)
            return sarr.size();
        else { //binary search
            int l = 0, r = (int)sarr.size() - 1, mid, MID;
            while((r - l) > 1){
                mid = (l + r)/2;
                if (cmp_first.second >= cmp_last.second){ // lcp
                    if (cmp_first.second <= llcp[mid]){
                        pair<int, int> cmp_mid = lex_cmp(pat.substr(cmp_first.second), txt.substr(sarr[mid] + cmp_first.second));
                        MID = cmp_first.second + cmp_mid.second;
                    } else
                        MID = llcp[mid];
                } else {
                    if (cmp_last.second <= rlcp[mid]){
                        pair<int, int> cmp_mid = lex_cmp(pat.substr(cmp_last.second), txt.substr(sarr[mid] + cmp_last.second));
                        MID = cmp_last.second + cmp_mid.second;
                    } else
                        MID = rlcp[mid];
                }

                if ((MID == (int)pat.size()) || (pat[MID] <= txt[sarr[mid] + MID])){
                    r = mid; cmp_last.second = MID;
                } else{
                    l = mid; cmp_first.second = MID;
                }
            }

            return r; 
        }
    }

    int pred(string &txt, string &pat, vector<int> &sarr, vector<int> &llcp, vector<int> &rlcp)
    {
        pair<int, int> cmp_first = lex_cmp(pat, txt.substr(sarr[0]));
        pair<int, int> cmp_last = lex_cmp(pat, txt.substr(sarr.back()));

        if (cmp_first.first < 0)
            return -1;
        else if(cmp_last.first >= 0)
            return sarr.size() - 1;
        else { //binary search
            int l = 0, r = (int)sarr.size() - 1, mid, MID;
            while((r - l) > 1){
                mid = (l + r)/2;
                if (cmp_first.second >= cmp_last.second){ // lcp
                    if (cmp_first.second <= llcp[mid]){
                        pair<int, int> cmp_mid = lex_cmp(pat.substr(cmp_first.second), txt.substr(sarr[mid] + cmp_first.second));
                        MID = cmp_first.second + cmp_mid.second;
                    } else
                        MID = llcp[mid];
                } else {
                    if (cmp_last.second <= rlcp[mid]){
                        pair<int, int> cmp_mid = lex_cmp(pat.substr(cmp_last.second), txt.substr(sarr[mid] + cmp_last.second));
                        MID = cmp_last.second + cmp_mid.second;
                    } else
                        MID = rlcp[mid];
                }

                if ((MID == (int)pat.size()) || (pat[MID] > txt[sarr[mid] + MID])){
                    l = mid; cmp_first.second = MID;
                } else{
                    r = mid; cmp_last.second = MID;
                }
            }

            return l; 
        }
    }

    vector<int> search_sarr(string &txt, string &pat, vector<int> &sarr, vector<int> &llcp, vector<int> &rlcp)
    {
        vector<int> occ;
        int l = succ(txt, pat, sarr, llcp, rlcp), r = pred(txt, pat, sarr, llcp, rlcp);
        if (l <= r){
            for(int i = l; i <= r; i++)
                occ.push_back(sarr[i]);
        }

        return occ;
    }
    vector<vector<int>> build_P(const string &txt)
    {
        int n = (int)txt.size();
        vector<vector<int>> P; 
        vector<int> auxP;

        for(auto chr : txt)
            auxP.push_back((int)chr);
        P.push_back(auxP);

        int log2N = (int)ceil(log2(n));

        for(int k = 1; k <= log2N; k++){
            int l = (1 << (k-1)); // 2^(k-1)

            vector<tuple<int, int, int>> tupleAux; 
            for(int i = 0; i < n; i++){
                int s = (i+l >= n ? -1 : P[k-1][i+l]); //caso n tenha o elemento, fica -1
                tupleAux.push_back({P[k-1][i], s, i});
            }
            sort(tupleAux.begin(), tupleAux.end());

            vector<int> auxP(n);
            int order = 0;
            auxP[get<2>(tupleAux[0])] = order;

            for(int i = 1; i < n; i++){
                if(get<0>(tupleAux[i]) != get<0>(tupleAux[i-1]) ||  get<1>(tupleAux[i]) != get<1>(tupleAux[i-1]))
                    order++;
                auxP[get<2>(tupleAux[i])] = order;
            }

            P.push_back(auxP);
        }

        return P;
    }

    vector<int> build_sarr(vector<int> &p)
    {
        vector<int> sa((int)p.size());
        for(int i = 0; i < (int)sa.size(); i++)
            sa[p[i]] = i;
        return sa;
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

    void build_LR_lcp(const string &txt, vector<vector<int>> &P, vector<int> &sa, vector<int> &llcp, vector<int> &rlcp, int l, int r)
    {
        if(r-l <= 1)
            return;
        int h = ((l+r)/2);
        llcp[h] = lcp(P, sa[l], sa[h]);
        rlcp[h] = lcp(P, sa[h], sa[r]);
        build_LR_lcp(txt, P, sa, llcp, rlcp, l, h);
        build_LR_lcp(txt, P, sa, llcp, rlcp, h, r);
    }

    public:
    void index(const string &txt, FILE *indexed_file)
    {
        int n = (int)txt.size();
        vector<int> llcp(n, -1), rlcp(n, -1);
        auto P = build_P(txt);
        auto sarr = build_sarr(P.back());
        build_LR_lcp(txt, P, sarr, llcp, rlcp, 0, (int)sarr.size()-1);
        
        fwrite(&n, sizeof(int), 1, indexed_file);
        fwrite(&P[0][0], sizeof(int), n, indexed_file);
        fwrite(&sarr[0], sizeof(int), n, indexed_file);
        fwrite(&llcp[0], sizeof(int), n, indexed_file);
        fwrite(&rlcp[0], sizeof(int), n, indexed_file);
    };

    pair<long long, vector<string>> search(vector<string> &patterns, string &file_name)
    {
        int n;
        long long total = 0;
        vector<string> lines_occ;
        FILE *indexed_file = fopen(file_name.c_str(), "rb");

        while(fread(&n, sizeof(int), 1, indexed_file) && n){
            vector<int> p0(n), sarr(n), llcp(n), rlcp(n);
            int result;
            result = fread(&p0[0], sizeof(int), n, indexed_file);
            result = fread(&sarr[0], sizeof(int), n, indexed_file);
            result = fread(&llcp[0], sizeof(int), n, indexed_file);
            result = fread(&rlcp[0], sizeof(int), n, indexed_file);
            bool inclused_line = false;

            string txt(p0.begin(), p0.end());
            for(string pat : patterns){
                auto occ = search_sarr(txt, pat, sarr, llcp, rlcp);
                if(!occ.empty() && !inclused_line){
                    lines_occ.push_back(txt);
                    inclused_line = true;
                }
                total += (long long)occ.size();
            }
        }

        fclose(indexed_file);
        return make_pair(total, lines_occ);
    }
}; 
#endif