#ifndef ACTING_H
#define ACTING_H

#include <memory>
#include <variant>
#include "ast.h"
#include "env.hpp"
#include "caf/all.hpp"
#include "caf/io/all.hpp"

namespace acting {
using namespace caf;

class StartMsg {
public:
    int a;
};

class ResultMsg {
public:
    int b;
};

template <class Inspector>
typename Inspector::result_type inspect(Inspector &f, StartMsg &x) {
    return f(meta::type_name("StartMessage"));
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector &f, ResultMsg &x) {
    return f(meta::type_name("ResultMessage"));
}

using vector = typed_actor<replies_to<StartMsg>::with<ResultMsg>>;

vector::behavior_type vectorActor(vector::pointer self);
void startActing();

}
#endif
