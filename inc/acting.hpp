#ifndef ACTING_H
#define ACTING_H

#include <memory>
#include <variant>
#include "ast.h"
#include "env.hpp"
#include "caf/all.hpp"
#include "caf/io/all.hpp"

using namespace caf;

class FakeEnv {
public:
    env::SafeEnv env;
};

class StartMsg {
public:
    //FakeEnv env;
    //ast::NodePtr subtree;
    int a;
};

class ResultMsg {
public:
    //ast::NodePtr tree;
    //ast::LiteralVariant ret_val;
    int b;
};

struct ActorState {
    //std::vector<ResultMsg> deps;
    //size_t children;
    int child;
};

/*
template <class Inspector>
typename Inspector::result_type inspect(Inspector &f, StartMsg &x) {
    return f(meta::type_name("StartMessage"), x.a);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector &f, ResultMsg &x) {
    return f(meta::type_name("ResultMessage"), x.b);
}

template <class Inspector>
typename Inspector::result_type inspect(Inspector &f, ActorState &x) {
    return f(meta::type_name("ActorState"), x.child);
}

using list = typed_actor<replies_to<StartMsg>::with<ResultMsg>>;
using vector = typed_actor<replies_to<StartMsg>::with<ResultMsg>>;

vector::behavior_type vectorActor(vector::pointer self);
list::behavior_type listActor(list::stateful_pointer<ActorState> self);
void startActing(ast::NodePtr root, env::SafeEnv global);
*/
#endif
