#include "repl.hpp"

namespace Slisp::Repl {
    std::string read() {
        std::string result;

        std::cout << "slisp> ";
        std::getline(std::cin, result);

        return result;
    }

    Types::Value* eval(const std::string& s) {

    }

    void print(Types::Value* v) {
        std::cout << "; " << v->to_string() << "\n";
    }

    void repl() {
        std::string s = read();
        Types::Value* v = eval(s);
        print(v);
    }

    void repl_loop() {
        while (true) {
            repl();
        }
    }
}
