#include <iostream>
#include <sstream> //stringstream
#include <functional>
#include <algorithm>
#include <numeric>
#include <string>
#include <cassert>
#include "MStorage.h"
using namespace std;



ostream &operator<<(ostream &cout, const vector<ull> &vec) {
    std::copy(vec.begin(), vec.end(), ostream_iterator<ull>(cout, " "));
    //cout<<endl;
    return cout;
}
ostream &operator<<(ostream &cout, const vector<int> &vec) {
    std::copy(vec.begin(), vec.end(), ostream_iterator<int>(cout, " "));
    //cout<<endl;
    return cout;
}
//multidimensional storage
class MStorageReduntantVector:public MStorage {
    vector<bool> x;
    int m; //dimensions
    int c; //length of each dimension
    int j; //number of possible values in first dimension = number of jobs
    vector<ull> scheme;
    public:
    MStorageReduntantVector() {}
    MStorageReduntantVector(int m, int j, int c):m(m),c(c),j(j) {
        //creates (m+1)-dimension space
        //and each dimension is addressed by integer from (0..c-1)
        scheme.push_back(pow(c, m));
        for(int i=m-1; i>=0; --i)
            scheme.push_back(pow(c, i));
        cout<<scheme<<endl;
        x.resize(((ull)j+1)*pow((ull)c,(ull)m));
        cout<<"rozmiar calej przestrzeni: "<<x.size()<<"="<<(j+1)<<"*pow("<<c<<","<<m<<")"<<endl;
    }
    ull getStartOfNthJobPage(int n){
        //assert(n>=0);
        //assert(n<=j+1);
        return scheme[0]*(n);
    }
    const vector<bool> &getX() {
        return x;
    }
    ull toId(vector<int> addr) {
        assert(addr.size()==m+1);
        if(addr[0]>j+1)
            return 0;
        assert(addr[0]<=j+1); //przez to 2 dni...
        for(int i=1; i<m+1; ++i)
            if(addr[i]>c)
                return 0;
        for(int i=1; i<=m; ++i)
            assert(addr[i]<=c);

        return inner_product(scheme.begin(), scheme.end(), addr.begin(), 0);
    }
    vector<int> fromId(ull id) {
        vector<int> adr;
        ull i=id;
        for(int pos=0; pos<m+1; ++pos) {
            adr.push_back(i/scheme[pos]);
            ull mult=scheme[pos];
            i-=mult*adr.back();
        }
        return adr;
    }
    void printAllTrues() {
        for(int i=0; i<x.size(); ++i)
            if(x[i])
                cout<<"["<<i<<"\t: "<<fromId(i)<<"]"<<endl;
        cout<<endl;
    }
    vector<int> fromStr(const char *addr) {
        stringstream ss(addr);
        vector<int> adr;
        for(int i=0; i<m+1; ++i) {
            int n;
            ss>>n;
            adr.push_back(n);
        }
        //cout<<"read: "<<adr;
        return adr;
    }
    bool operator[](vector<int> addr) {
        //vector<int> adr=fromStr(addr);
        return x[toId(addr)];
    }
    bool setTrue(ull at) {
        vector<int> dest=fromId(at);
        //cout<<"setting true at "<<at<<" = "<<dest<<endl;
        return x[at]=true;
    }
    bool setTrue(vector<int> at) {
        return setTrue(toId(at));
    }
    int getValueAt(ull id) {
        return x[id];
    }
    int getValueAt(vector<int> addr) {
        return x[toId(addr)];
    }
    void clear() {
        fill(x.begin(), x.end(), 0);
    }
};

void storageDemo() {
    MStorageReduntantVector storage(3,3,14);
    storage[storage.fromStr("0 0 0 15")];
    cout<<storage.fromId(14);
    //storage.printAllTrues();
}
