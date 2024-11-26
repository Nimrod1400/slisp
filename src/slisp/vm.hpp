#ifndef VM_HPP_
#define VM_HPP_

#include <utility>
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

        template <class T, class... Args>
        T* push(Args&&... args) {
            T* v = track<T>(std::forward<Args>(args)...);
            Cons* new_cons = new Cons { v };

            Cons* last_cons = m_root;
            while (last_cons) {
                last_cons = static_cast<Cons*>(last_cons->cdr());
            }

            last_cons->set_cdr(new_cons);
            return v;
        }

        template <class T, class... Args>
        T* track(Args&&... args) {
            T* v = new T(std::forward<Args>(args)...);
            Cons* new_cons = new Cons { v };

            Cons* last_cons = m_objects;
            while (last_cons) {
                last_cons = static_cast<Cons*>(last_cons->cdr());
            }

            last_cons->set_cdr(new_cons);
            return v;
        }

        ~VirtualMachine();

    private:
        Cons *m_objects;
        Cons *m_root;

        void m_delete_objects();
        void m_delete_root();
    };
}

#endif // VM_HPP_
