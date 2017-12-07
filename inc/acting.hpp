#ifndef ACTING_H
#define ACTING_H

#include <memory>
#include <variant>
#include "ast.h"
#include "env.hpp"
#include "caf/all.hpp"

namespace acting {
using namespace caf;

struct ActorState;

class FakeEnv {
public:
    env::SafeEnv env;
};

class StartMsg {
public:
    FakeEnv env;
    ast::NodePtr subtree;
};

class ResultMsg {
public:
    ast::NodePtr tree;
    ast::LiteralVariant ret_val;
};

template <class Inspector>
typename Inspector::result_type inspect(Inspector &f, StartMsg &x) {
    return f(meta::type_name("StartMessage"));
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector &f, ResultMsg &x) {
    return f(meta::type_name("ResultMessage"));
}

using list = typed_actor<replies_to<StartMsg>::with<ResultMsg>>;
using vector = typed_actor<replies_to<StartMsg>::with<ResultMsg>>;

vector::behavior_type vectorActor(vector::pointer self);
list::behavior_type listActor(list::stateful_pointer<ActorState> self);
}
#endif
