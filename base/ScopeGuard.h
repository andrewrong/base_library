//
// Created by 沈梦叶 on 2020/9/25.
//

#ifndef BASE_LIBRARY_SCOPEGUARD_H
#define BASE_LIBRARY_SCOPEGUARD_H

class ScopeGuardImplBase {
public:
    ScopeGuardImplBase() noexcept: dismissed_(false) {}

    //赋值构造函数被删除
    ScopeGuardImplBase &operator=(const ScopeGuardImplBase &) = delete;

    void disMiss() const noexcept {
        //虽然是const的调用，但是dissmiss可以被修改
        this->dismissed_ = false;
    }

protected:
    ~ScopeGuardImplBase() = default;

    // copy构造函数,外部调用不了
    ScopeGuardImplBase(const ScopeGuardImplBase &other) noexcept
            : dismissed_(other.dismissed_) {
        other.disMiss();
    }

    template<typename T>
    static void safeExecute(T &t) throw() {
        if (!t.dismissed_) {
            try {
                t.Execute();
            } catch (...) {

            }
        }
    }

private:
    mutable bool dismissed_;
};

typedef const ScopeGuardImplBase &ScopeGuard;

//超过范围之后调用的函数不带参数
template<typename F>
class ScopeGuardImpl0 : public ScopeGuardImplBase {
public:
    static ScopeGuardImpl0<F> MakeGuard(F func) {
        return ScopeGuardImpl0<F>(func);
    }

    ~ScopeGuardImpl0() noexcept {
        this->safeExecute(*this);
    }

    void Execute() {
        func_();
    }

protected:
    //构造函数保护起来，保证不能用户自己构造出来;
    ScopeGuardImpl0(F func) : func_(func) {}

private:
    F func_;
};

template<typename F>
inline ScopeGuardImpl0<F> MakeGuard(F fun) {
    return ScopeGuardImpl0<F>::MakeGuard(fun);
}

template<class Obj, class F>
class ObjScopeGuardImpl0 : public ScopeGuardImplBase {
public:
    static ObjScopeGuardImpl0<Obj, F> MakeObjGuard(Obj &obj, F func) {
        return ObjScopeGuardImpl0(func, obj);
    }

    ~ObjScopeGuardImpl0() {
        safeExecute(*this);
    }

    void Execute() {
        (this_.*memFunc_)();
    }

protected:
    ObjScopeGuardImpl0(F func, Obj &obj) : memFunc_(func), this_(obj) {}

private:
    F memFunc_;
    Obj &this_;
};

template <class Obj, typename F>
inline ObjScopeGuardImpl0<Obj, F> MakeObjGuard(Obj& obj, F func) {
    return ObjScopeGuardImpl0<Obj, F>::MakeObjGuard(obj, func);
}

#endif //BASE_LIBRARY_SCOPEGUARD_H
