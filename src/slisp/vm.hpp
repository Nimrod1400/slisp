#ifndef VM_HPP_
#define VM_HPP_

#include <stack>
#include <forward_list>
#include <list>
#include "types.hpp"

namespace Slisp::VirtualMachine {
    using namespace Slisp::Types;

    class VirtualMachine {
    public:
        VirtualMachine();

        void mark();
        void sweep();
        Value* push(Value* o);
        Value* track(Value* o);

    private:
        Cons *m_objects;
        Cons *m_root;
    };
}

#endif // VM_HPP_
