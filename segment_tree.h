#include <bits/stdc++.h>
typedef long long int lli;
using namespace std;
template <typename T,typename operation,T identity_element>
class SegmentTree {
private:
    vector<T> tree;
    vector<T> arr;
    vector<pair<T,T>>lazy;
    vector<bool>was;
    operation op;
    lli n;
    void build(lli node, lli start, lli end) {
        if (start == end-1) {
            tree[node] = arr[start];
        } 
        else {
            lli mid = (start + end) / 2;
            build(2 * node, start, mid);
            build(2 * node + 1, mid, end);
            tree[node] = op(tree[2 * node],tree[2 * node + 1]);
        }
    }
    //редактировать push
    void push(lli node, lli start, lli end) {
        if (was[node]) {
            if (lazy[node].first == 1) {
                tree[node] = lazy[node].second * (end - start);
                if (start != end - 1) {
                    lazy[2 * node] = lazy[node];
                    lazy[2 * node + 1] = lazy[node];
                    was[2 * node] = true;
                    was[2 * node + 1] = true;
                }
            } 
            else if (lazy[node].first == 2 && lazy[node].second != 0) { // Проверка на ненулевое значение 
                tree[node] += lazy[node].second * (end - start);
                if (start != end - 1) {
                    if (lazy[2 * node].first != 1) {
                        lazy[2 * node].first = 2;
                    }
                    if (lazy[2 * node + 1].first != 1) {
                        lazy[2 * node + 1].first = 2;
                    }
                    lazy[2 * node].second += lazy[node].second;
                    lazy[2 * node + 1].second += lazy[node].second;
                    was[2 * node] = true;
                    was[2 * node + 1] = true;
                }
            }
            lazy[node] = {0, 0};
            was[node] = false;
        }
        // print();
    }
    T min_index(T a, T b){
        if(min(a,b)==-1){
            return max(a,b);
        }
        return min(a,b);
    }
    T first_up(lli node,lli start,lli end,lli element,lli left){
        push(node, start, end); 
        if(tree[node]<element or end<=left){
            return identity_element;
        }
        lli mid=(start + end) / 2;
        if(start>=left){
            if(start==end-1){
                return start;
            }
            if(tree[2*node]>=element){
                return first_up(2*node,start,mid,element,left);
            }
            return first_up(2*node+1,mid,end,element,left);
        }
        return min_index(first_up(2*node,start,mid,element,left),first_up(2*node+1,mid,end,element,left));
    }

    T rec_accum(lli node, lli start, lli end, lli l, lli r) {
        // Применить отложенные операции перед проверкой условий
        push(node, start, end);  
        if (r <= start or end <= l) {
            return identity_element; 
        }
        if (l <= start and end <= r) {
            return tree[node];
        }
        lli mid = (start + end) / 2;
        return op(rec_accum(2 * node, start, mid, l, r) ,rec_accum(2 * node + 1, mid, end, l, r));
    }
    void R(lli node, lli start, lli end, lli left, lli right, lli val, lli type) {
        push(node, start, end); // Применить отложенные операции перед проверкой условий
        if (start >= left && end <= right) {
            if (type == 1) {
                lazy[node] = {1, val};
            } 
            else {
                if (lazy[node].first != 1) {
                    lazy[node].first = 2;
                }
                lazy[node].second += val;
            }
            was[node] = true;
            push(node, start, end); // Применить отложенные операции после обновления
        }
        else {
            lli mid = (start + end) / 2;
            push(2*node,start,mid);
            push(2*node+1,mid,end);
            if (right <= mid) {
                R(2 * node, start, mid, left, right, val, type);
            } 
            else if (mid <= left) {
                R(2 * node + 1, mid, end, left, right, val, type);
            } 
            else {
                R(2 * node, start, mid, left, right, val, type);
                R(2 * node + 1, mid, end, left, right, val, type);
            }
            tree[node] = op(tree[2 * node], tree[2 * node + 1]); 
        }
    }
public:
    SegmentTree(operation _op,vector<lli>& input): op(_op) {
        n = input.size();
        lli N = 0;
        while (1 << (N) < n) {
            N++;
        }
        n = 1 << (N);
        input.resize(n,identity_element);
        arr = input;
        tree.resize(2*n,identity_element);
        lazy.resize(2*n);
        was.resize(2*n);
        build(1, 0, n);
    }
    T accumulate(lli l, lli r) {
        return rec_accum(1, 0, n, l, r);
    }
    T first_upper(lli element,lli left = 0){
        return first_up(1,0,n,element,left);
    }
    // установить значение val на отрезке [l, r-1]
    void set(lli l, lli r, lli val) {
        R(1,0,n,l, r, val, 1); 
    }
    // прибавить val к каждому элементу на отрезке [l, r-1]
    void add(lli l, lli r, lli val) {
        R(1,0,n,l, r, val, 2); 
    }
};
