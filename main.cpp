#include <iostream>
#include <vector>
#include <math.h>
#include <iterator> //ostream_iterator
#include <list>
#include "MStorageRedundantVector.h"
#include <windows.h>
#include <fstream>
#include <string>
#include <stdlib.h> //time, srand
#include <time.h>

using namespace std;
//#pragma GCC optimize 0
extern
unsigned long long timer() /*{
  __asm__ __volatile__(
	   "CPUID\n"
	   "rdtsc\n"
	   );
}*/;


struct Judge {
    MStorage &x;
    Judge(MStorage &x):x(x) {
    }
    //used for finding best point to start backtracking from
    bool operator()(const ull &a, const ull &b) { //true for A wins, false for B wins
        vector<int> A=x.fromId(a);
        vector<int> B=x.fromId(b);
        bool judgeSays=(*max_element(A.begin(), A.end()) < *max_element(B.begin(), B.end()));
        return judgeSays;
    }
};


    int getC(int m, vector<int> &jobs) {
        int sum=accumulate(jobs.begin(),jobs.end(),0);
        int mx=(int)1.0**max_element(jobs.begin(),jobs.end());
        return max(
                    2*sum/m//+(mx-(sum/m)%mx)
                   ,
                    mx
                   );
    }

class MScheduler {
    vector<int> jobs;
    public:
    MStorageReduntantVector x;
    //private:
    int j;
    int m;
    public:

    int getC() {
        return ::getC(m, jobs);
    }
    MScheduler(int m, vector<int> jobs):m(m),j(jobs.size()),jobs(jobs) {
        x=MStorageReduntantVector(m, jobs.size(), getC()+1);
    }

    private:
    void handleTrue(vector<int> addr) {
        addr[0]+=1;
        for(int i=1; i<m+1; ++i) {
            if(addr[i]+jobs[addr[0]-1]>getC())
                continue;
            addr[i]+=jobs[addr[0]-1];
            x.setTrue(addr);
            //cout<<"trying to set true on "<<addr<<endl;
            addr[i]-=jobs[addr[0]-1];
        }
    }
    bool getValueAt(ull i) {
        return x.getValueAt(i);
    }
    bool getOppositeValueToValueAt(ull i) {
        return !x.getValueAt(i);
    }
    vector<int> findMax() { //==find the starting point of backtracking
        list<ull> vals;
        //remove_copy_if(x.getX().begin()+x.getStartOfNthJobPage(j), x.getX().end(), vals.begin(), getOppositeValueToValueAt); //leave only true values set on last page
        ull st=x.getStartOfNthJobPage(jobs.size()+0);
        ull en=x.getStartOfNthJobPage(jobs.size()+1);
        for(ull i=st; i<en; ++i)
            if(x.getValueAt(i))
                vals.push_back(i);
        //std::copy(vals.begin(), vals.end(), ostream_iterator<ull>(cout, "\n"));
        ull bestPointId=*max_element(vals.begin(),vals.end(), Judge(x));
        return x.fromId(bestPointId);
    }
    public:
    void solve() {
        x.clear();
        x.setTrue(x.fromId(0)); //initial value (more at page 146 in handbook on scheduilng)
        for(int i=0; i<j; ++i) {
            //handleJobPage(i);
            //int i=0;
            ull st=x.getStartOfNthJobPage(i);
            ull en=x.getStartOfNthJobPage(i+1);
            for(ull k=st; k<en; ++k) {
                if(x.getValueAt(k)==true) {
                    //cout<<k<<endl;
                    //cout<<x.fromId(k)<<endl;
                    handleTrue(x.fromId(k));
                }
            }
        }
        //x.setTrue(x.fromStr("7 0 0 0"));
    }
    void backtrack() {
        vector<int> startFrom=findMax();
        cout<<">>start from: "<<startFrom<<endl;
        //return;
        int jobid=jobs.size()-1;
        startFrom[0]-=1;
        while(jobid>=0) {
            for(int k=0; k<m; ++k) {//check all possible moves back
                if(startFrom[k+1]<jobs[jobid]) continue;
                startFrom[k+1]-=jobs[jobid];
                //cout<<"checking "<<startFrom<<endl;
                if(x.getValueAt(x.toId(startFrom))) {
                    cout<<"job id="<<jobid<<" with cost="<<jobs[jobid]<<" goes to proc i="<<k<<endl;
                    startFrom[0]-=1;
                    //cout<<"new start from="<<startFrom<<endl;
                    break;
                }
                startFrom[k+1]+=jobs[jobid];
            }
            jobid--;
        }
    }
    friend ostream &operator<<(ostream &cout, MScheduler &s) {
        cout<<s.m<<"\t"<<s.j<<" zadan\t"<<s.getC();
        return cout;
    }
};

void demo() {
    int jobss[]={1,2,3,4};
    vector<int> jobs(jobss,jobss+4);
    //delete []j;
    MScheduler schedule(4, jobs);
    size_t time1=GetTickCount();
    ull timer1=timer();
    schedule.solve();
    /*uncomment this row to see all trues in all dimensions*/
    //schedule.x.printAllTrues();
    //schedule.x.printAllTrues();
    schedule.backtrack();
    size_t time2=GetTickCount();
    ull timer2=timer();
    cout<<(time2-time1)<<"[ms]="
    <<(timer2-timer1)<<"[processor ticks]"<<endl
    ;
    //return;
}


string readFrom(istream &cin) {
    int m, j, jobtime;
    cout<<"ile procesorow?"<<endl;
    cin>>m;
    cout<<m<<endl;
    cout<<"ile zadan do uszeregowania?"<<endl;
    cin>>j;
    cout<<j<<endl;
    vector<int> jobs;
    for(int i=0; i<j; ++i) {
        cout<<"podaj "<<(i+1)<<"-te zadanie"<<endl;
        string tmp;
        cin>>tmp;
        if(tmp==string("...")) {
            cout<<"ok Lazy Guy, filling with last given job time="<<jobtime<<endl;
            //system("pause");
            break;
        }
        //else ...
        stringstream ss(tmp.c_str());
        ss>>jobtime;
        jobs.push_back(jobtime);
        cout<<jobtime<<endl;
    }
    for(int i=jobs.size(); i<j; ++i)
        jobs.push_back(jobtime);
    copy(jobs.begin(), jobs.end(), ostream_iterator<int>(cout, " ")); cout<<endl;
    ull estimtedSize=jobs.size()+1;
    estimtedSize*=pow(getC(m,jobs),m);
    cout<<"estimated size="<<estimtedSize<<"\nis it ok?"<<endl;
    //if(estimtedSize>=2519424000ULL) return "cant be done, too big\n";
    //2162410432ULL
    if(estimtedSize>=1719926784ULL) {
        cout<<"cant be done, too big\n";
        return "cant be done, too big\n";
    }

    //cout<<700000000ULL<<endl;
    //system("pause");
    MScheduler schedule(m, jobs);
    size_t time1=GetTickCount();
    ull timer1=timer();
    schedule.solve();//fill table with true values meaning available combinations (sth like DSP search for best solution)
    //schedule.x.printAllTrues();
    schedule.backtrack();
    size_t time2=GetTickCount();
    ull timer2=timer();
    //ofstream out("output.txt", ios_base::app);
    stringstream out;
    out<<schedule<<"\t"<<(1.0*(time2-time1)/1000)<<"\t"<<(timer2-timer1)<<"\t"<<jobs[0]<<endl;
    cout<<"machines processors no_jobs time[ms] time[CPU clock]\n";
    cout<<schedule<<" "<<(1.0*(time2-time1)/1000)<<" "<<(timer2-timer1)<<endl;
    //out.close();
    return out.str();
}

string readFrom(const char *text) {
    stringstream ss(text);
    return readFrom(ss);
}

char text[2560];
int main(int argc, char *argv[])
{
    if(argc==2) {
        demo();
    }
    //srand(time(NULL));
    //readFrom(cin);

    //readFrom("5 2 1 ..."); return 0;
    //demo();

    if(argc>=2+3 && string(argv[1])==string("single")) {
        //m, j, j[0][, ...]
        stringstream ss;
        for(int i=2; i<argc; ++i)
            ss<<argv[i]<<" ";
        readFrom(ss.str().c_str());
    }

    if (argc>=2 && string(argv[1])==string("stdin"))
    {
        readFrom(cin);
    }

    //return 0;
    int config2[]={4 //ilosc procesorow
                , 0, 5, 30 //ilosc prac: minval, step, maxval
                , 0, 10, 100 //czas trwania zadan: minval, step, maxval
                };
    int config[]={
            5,2,2,10,0,1,10    };
    if(argc==1+7+1 && string(argv[1])==string("ranges")) {
        cout<<"read config from params\n";
        for(int i=2; i<argc; ++i) {
            stringstream ss(argv[i]);
            cout<<i<<": "<<ss.str()<<endl;
            int v;
            ss>>v;
            config[i-2]=v;
        }
    }
    if(1) {
        copy(config, config+7, ostream_iterator<int> (cout, " "));
        cout<<endl;
        //system("pause");
        int m=config[0];
        for(int j=config[1]; j<=config[3]; j+=config[2]) {
            //if(j==5) continue;
                for(int t=config[4]; t<=config[6]; t+=config[5]) {
                    if(j<=0 || t<=0) continue;
                printf("loop: j=%d t=%d\n", j, t);
                //continue;
                stringstream ss;
                ss<<m
                //<<" "<<j
                <<".txt";
                sprintf(text, "%d %d %d ...", m, j, t);
                cout<<"running string: readFrom("<<text<<")"<<endl;
                string str=readFrom((const char*)text);
                size_t found=str.find(string("."));
                if(found!=string::npos)
                    str.replace(found, 1, string(","));
                //system("pause");
                ofstream out(ss.str().c_str(), ios_base::app);
                out<<str;
                out.close();
            }
        }
    }
    //readFrom("4 23 50 40 ..."); //70mln, sypie sie
    return 0;
}
