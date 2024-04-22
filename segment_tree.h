#include <bits/stdc++.h>
typedef long long int lli;
using namespace std;
template <typename T,typename operation,T identity_element>
class SegmentTree {
private:
    vector<T> tree;
    vector<T> arr;
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
    T min_index(T a, T b){
        if(min(a,b)==-1){
            return max(a,b);
        }
        return min(a,b);
    }
    T first_up(lli node,lli start,lli end,lli element,lli left){
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
        if (r < start or end <= l) {
            return identity_element; 
        }
        if (l <= start and end <= r) {
            return tree[node];
        }
        lli mid = (start + end) / 2;
        return op(rec_accum(2 * node, start, mid, l, r) ,rec_accum(2 * node + 1, mid, end, l, r));
    }

    void update(lli node, lli start, lli end, lli idx, lli val) {
        if (start == end-1) {
            arr[idx] = val;
            tree[node] = val;
        } 
        else {
            lli mid = (start + end) / 2;
            if (idx < mid) {
                update(2 * node, start, mid, idx, val);
            }
            else {
                update(2 * node + 1, mid, end, idx, val);
            }
            tree[node] = op(tree[2 * node],tree[2 * node + 1]);
        }
    }
    void print(){
        int t=0;
        int v=1;
        for(int i = 1; i < tree.size(); i++){
            cout<<tree[i]<<" ";
            t++;
            if(t==v){
                cout<<"\n";
                v*=2;
                t=0;
            }
        }
        cout<<"--------------------\n";
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
        build(1, 0, n);
    }
    T accumulate(lli l, lli r) {
        return rec_accum(1, 0, n, l, r);
    }
    T first_upper(lli element,lli left = 0){
        return first_up(1,0,n,element,left);
    }
    void updateValue(lli idx, lli val) {
        update(1, 0, n, idx, val);
    }
    void print_tree(){
        print();
    }
};
