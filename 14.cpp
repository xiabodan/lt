#include <iostream>
#include <mutex>          // std::mutex
#include <tr1/memory>

using namespace std;

class Lock {
public:
    explicit Lock(std::mutex* pm)
        : mutexPtr(pm, unlock) {
        lock(mutexPtr.get());
    }
private:
    tr1::shared_ptr<std::mutex> mutexPtr;
};

int main() {

    return 0;
}
