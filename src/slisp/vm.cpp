#include "vm.hpp"

namespace Slisp::VirtualMachine {
    VirtualMachine::VirtualMachine() :
        m_objects { nullptr },
        m_root { nullptr }
    { }

    void VirtualMachine::mark() {

    }

    void VirtualMachine::sweep() {

    }

    VmObject* VirtualMachine::push(VmObject *o) {
        Cons* new_cons = new Cons { o };
        track(o);

        Cons* last_cons = m_root;
        while (last_cons) {
            last_cons = dynamic_cast<Cons*>(m_root->cdr());
        }

        last_cons->set_cdr(new_cons);
        return o;
    }

    VmObject* VirtualMachine::track(VmObject *o) {
        Cons* new_cons = new Cons { o };

        Cons* last_cons = m_objects;
        while (last_cons) {
            last_cons = dynamic_cast<Cons*>(last_cons->cdr());
        }

        last_cons->set_cdr(new_cons);
        return o;
    }
}
