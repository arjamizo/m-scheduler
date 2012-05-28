#include <vector>
using namespace std;
typedef unsigned long long ull;
class MStorage {
    public:
    virtual bool isTrue(vector<int> addr){};
    virtual bool setTrue(vector<int> addr){};
    virtual void setAll(bool value){};
    virtual vector<int> fromId(ull id){};
};
