// automata_q3.cpp : This file contains the 'main' function. Program execution begins and ends there.
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <set>
#include <algorithm>
using namespace std;
class UF
{

public:
    vector<int> parents;
    vector<int> ranks;

    UF(int n)
    {
        parents.resize(n);
        ranks.resize(n);
        for (int i = 0; i < n; i++)
        {
            parents[i] = i;
            ranks[i] = 1;
        }
    }

    int find(int a)
    {
        if (parents[a] == a)
        {
            return a;
        }
        return parents[a] = find(parents[a]);
    }

    bool unite(int a, int b)
    {
        int p1 = find(a);
        int p2 = find(b);
        if (p1 == p2)
        {
            return false;
        }
        if (ranks[p1] <= ranks[p2])
        {
            parents[p1] = p2;
            ranks[p2] += ranks[p1];
        }
        else
        {
            parents[p2] = p1;
            ranks[p1] += ranks[p2];
        }
        return true;
    }
    bool allsame()
    {
        for (int i = 1; i < parents.size(); i++)
        {
            if (find(i - 1) != find(i))
                return false;
        }
        return true;
    }
};

int main(int argc, char* argv[])
{
    if (argc != 3) return 1;
    int n, k, a;
    set<int> accept;
    
	freopen(argv[1], "r", stdin);
	freopen(argv[2], "w", stdout);
    cin >> n >> k >> a;

    for (int i = 0; i < a; i++)
    {
        int q;
        cin >> q;
        accept.insert(q);
    }
    vector<vector<int>> to(n, vector<int>(26, -1));
    set<int> alphabets;
    for (int i = 0; i < k; i++)
    {
        int u, v;
        char x;
        scanf("%d %c %d", &u, &x, &v);
        to[u][x - 'a'] = v;
        alphabets.insert(x - 'a');
    }

    vector<vector<bool>> dp(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if ((accept.find(i) != accept.end() && accept.find(j) == accept.end()) || (accept.find(i) == accept.end() && accept.find(j) != accept.end()))
            {
                dp[i][j] = true;
            }
        }
    }

    bool still_marking = true;
    while (still_marking)
    {
        still_marking = false;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < i; j++)
            {
                if (dp[i][j])
                    continue;
                for (int k = 0; k < 26; k++)
                {
                    int delu = to[i][k];
                    int delv = to[j][k];

                    if (delu == -1 || delv == -1)
                        continue;

                    if (dp[max(delu, delv)][min(delu, delv)])
                    {
                        still_marking = true;
                        dp[i][j] = true;
                    }
                }
            }
        }
    }
    UF DSU(n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (dp[i][j])
                continue;
            DSU.unite(i, j);
        }
    }
    /*
    bool done = false;
    int cur = 0;
    while (!done)
    {
        bool has_cur = false;
        for (int i = 0; i < DSU.parents.size(); i++)
        {
            if (cur == DSU.parents[i])
            {
                has_cur = true;
                break;
            }
        }
        if (has_cur == false)
        {
            for (int i = 0; i < DSU.parents.size(); i++)
            {
                if (DSU.parents[i] > cur)
                {
                    DSU.parents[i]--;
                }
            }
        }
        cur++;
    }
    */
    set<int> number;
    for (auto x : DSU.parents)
    {
        number.insert(x);
    }
    vector<int> fvertices;
    for (auto x : number)
    {
        fvertices.push_back(x);
    }

    vector<int> v_pos(*max_element(fvertices.begin(), fvertices.end()) + 1, -1);
    for (int i = 0; i < fvertices.size(); i++)
    {
        v_pos[fvertices[i]] = i;
    }
    cout << endl << endl;
    cout << "____________________________________________________\n";
    cout << number.size() << " " << number.size() * 2 << " ";
    set<int> new_accept;

    for (int i = 0; i < DSU.parents.size(); i++)
    {
        if (accept.find(i) != accept.end())
        {
            new_accept.insert(v_pos[DSU.parents[i]]);
        }
    }

    cout << new_accept.size() << endl;
    for (auto x : new_accept)
    {
        cout << x << " ";
    }
    cout << endl;

    vector<vector<int>> f_trans(fvertices.size(), vector<int>(26, -1));

    for (int i = 0; i < n; i++)
    {
        for (char j : alphabets)
        {
            f_trans[v_pos[DSU.parents[i]]][j] = v_pos[DSU.parents[to[i][j]]];
          //  cout << v_pos[DSU.parents[i]] << " " << (char)(j + 'a') << " " << v_pos[DSU.parents[to[i][j]]] << endl;
        }
    }
    for (int i = 0; i < f_trans.size(); i++)
    {
        for (char j : alphabets)
        {
         //   f_trans[v_pos[DSU.parents[i]]][j] = v_pos[DSU.parents[to[i][j]]];
            cout << i << " " << (char)(j + 'a') << " " << f_trans[i][j] << endl;
        }
    }
}