#include "env.hpp"
#include "core_procedures.hpp"

namespace Slisp::Env {
    void populate_env(Env& env) {
        Types::Procedure plus_procedure =
            { Types::SlispFunction { plus_proc } };
        Types::Procedure minus_procedure =
            { Types::SlispFunction { minus_proc } };
        Types::Procedure multiply_procedure =
            { Types::SlispFunction { miltiply_proc } };
        Types::Procedure divide_procedure =
            { Types::SlispFunction { divide_proc } };

        env["+"] = plus_procedure;
        env["-"] = minus_procedure;
        env["*"] = multiply_procedure;
        env["/"] = divide_procedure;
    }
}
