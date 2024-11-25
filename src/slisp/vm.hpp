#ifndef VM_HPP_
#define VM_HPP_

#include <stack>
#include <forward_list>
#include <list>
#include "types.hpp"

namespace Slisp::VirtualMachine {
    using namespace Slisp::Types;

    template <class T>
    class VirtualMachine {
    public:
        VirtualMachine();

        void mark();
        void sweep();
        VmObject* push(VmObject *o);
        VmObject* track(VmObject *o);

    private:
        struct VmObject {
            T* object;
            bool is_reachable;
        };

        std::list<VmObject> *m_objects;
        std::list<VmObject> *m_root;
    };
}

#endif // VM_HPP_
