//
// Created by nomoshen on 2020/9/29.
//
#include <map>
#include <iostream>
#include "base/ScopeGuard.h"
#include <mutex>

using namespace std;

class A {
public:
    void printF() {
        cout << "hello, world" << endl;
    }
};

int main() {
    A a;
    auto scopeGuard = MakeObjGuard(a, &A::printF);
}

