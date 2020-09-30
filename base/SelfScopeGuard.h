//
// Created by nomoshen on 2020/9/30.
//

#ifndef BASE_LIBRARY_SELFSCOPEGUARD_H
#define BASE_LIBRARY_SELFSCOPEGUARD_H

template <typename F>
class SelfScopeGuard {
public:
    static SelfScopeGuard<F> MakeSelfScope(F func) {
        return SelfScopeGuard<F>(func);
    }

    ~SelfScopeGuard() {
        if (this->func_ != nullptr) {
            this->func_();
        }
    }

protected:
    SelfScopeGuard(F func): func_(func){}
private:
    F func_;
};

template <typename F>
inline SelfScopeGuard<F> MakeSelfScope(F fun) {
    return SelfScopeGuard<F>::MakeSelfScope(fun);
}

#endif //BASE_LIBRARY_SELFSCOPEGUARD_H
