#include "vm.hpp"

namespace Slisp::VirtualMachine {
    VirtualMachine::VirtualMachine(std::size_t gc_threshold) :
        m_gc_threshold { gc_threshold },
        m_tracked_size { 0 },
        m_objects { nullptr },
        m_root { new Cons { nullptr } }
    { }

    void VirtualMachine::mark() {
        m_root->cdr()->mark_reachable();
    }

    void VirtualMachine::sweep() {
        Cons* curr_cons = m_objects;
        Cons* prev_cons = nullptr;

        while (curr_cons) {
            if (!curr_cons->car()->is_reachable() && !prev_cons) {
                m_objects = static_cast<Cons*>(curr_cons->cdr());
                delete curr_cons->car();
                delete curr_cons;
                curr_cons = m_objects;
            }
            else if (!curr_cons->car()->is_reachable() && prev_cons) {
                prev_cons->set_cdr(curr_cons->cdr());
                delete curr_cons->car();
                delete curr_cons;
                curr_cons = static_cast<Cons*>(prev_cons->cdr());
            }
            else {
                prev_cons = curr_cons;
                curr_cons = static_cast<Cons*>(prev_cons->cdr());
            }
        }

        Cons* cons = m_objects;
        while (cons) {
            cons->mark_unreachable();
            cons = static_cast<Cons*>(cons->cdr());
        }
    }

    void VirtualMachine::m_delete_objects() {
        Cons* curr_cons = m_objects;
        Cons* next_cons = nullptr;

        while (curr_cons) {
            next_cons = static_cast<Cons*>(curr_cons->cdr());
            delete curr_cons->car();
            delete curr_cons;
            curr_cons = next_cons;
        }
    }

    void VirtualMachine::m_delete_root() {
        Cons* curr_cons = m_root;
        Cons* next_cons = nullptr;

        while (curr_cons) {
            next_cons = static_cast<Cons*>(curr_cons->cdr());
            delete curr_cons;
            curr_cons = next_cons;
        }
    }

    VirtualMachine::~VirtualMachine() {
        m_delete_objects();
        m_delete_root();
    }
}
