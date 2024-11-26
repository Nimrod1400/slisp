#include "vm.hpp"

namespace Slisp::VirtualMachine {
    VirtualMachine::VirtualMachine() :
        m_objects { new Cons { nullptr } },
        m_root { new Cons { nullptr } }
    { }

    void VirtualMachine::mark() {
        m_root->mark_unreachable();
    }

    void VirtualMachine::sweep() {
        Cons* curr_cons = m_objects;
        Cons* prev_cons = nullptr;

        while (curr_cons) {
            if (!curr_cons->car()->is_reachable() && !prev_cons) {
                m_objects = static_cast<Cons*>(curr_cons->cdr());
                delete curr_cons;
                curr_cons = m_objects;
            }
            else if (!curr_cons->car()->is_reachable() && prev_cons) {
                prev_cons->set_cdr(curr_cons->cdr());
                delete curr_cons;
                curr_cons = static_cast<Cons*>(prev_cons->cdr());
            }
            else {
                prev_cons = curr_cons;
                curr_cons = static_cast<Cons*>(prev_cons->cdr());
            }
        }

        for (Cons* cons = m_objects; cons == cons->cdr(); cons != nullptr) {
            cons->car()->mark_unreachable();
        }
    }

    Value* VirtualMachine::push(Value *o) {
        Cons* new_cons = new Cons { o };
        track(o);

        Cons* last_cons = m_root;
        while (last_cons) {
            last_cons = static_cast<Cons*>(m_root->cdr());
        }

        last_cons->set_cdr(new_cons);
        return o;
    }

    Value* VirtualMachine::track(Value *o) {
        Cons* new_cons = new Cons { o };

        Cons* last_cons = m_objects;
        while (last_cons) {
            last_cons = static_cast<Cons*>(last_cons->cdr());
        }

        last_cons->set_cdr(new_cons);
        return o;
    }
}
