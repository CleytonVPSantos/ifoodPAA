#include <bits/stdc++.h>

using namespace std;

bool compare(pair<float, int> a, pair<float, int> b){
    if (a.first == -1) return true;
    if (b.first == -1) return false;
    if (a.first == b.first) return a.second > b.second;
    return a.first > b.first;
}

int main()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>,
                        decltype(&compare) > heap(compare);
    heap.push(make_pair(1, 1));
    heap.push(make_pair(2, 2));
    heap.push(make_pair(3, 3));
    heap.push(make_pair(-1, 4));
    double a = -1;
    cout << a << endl;
}