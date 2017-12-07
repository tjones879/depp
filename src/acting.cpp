#include "inc/acting.hpp"
#include <stack>
#include <utility>

namespace acting {

struct ActorState {
    std::vector<ResultMsg> deps;
    size_t children;
};

vector::behavior_type vectorActor(vector::pointer self) {
    auto handle_err = [=](const error &err) {
        aout(self) << "FAILURE" << std::endl;
    };
    return {
        [=](StartMsg b) -> ResultMsg {
            auto deps = std::stack<ast::LiteralNode>();
            for (auto child : b.subtree->children) {
                //TODO: COMPLETE THIS CHILD
                //deps.push(<RESULT OF CHILD>);
            }
            // TODO: COMPLETE THIS COMMAND
            
        }
    };
}

list::behavior_type listActor(list::stateful_pointer<ActorState> self) {
    auto handle_err = [=](const error &err) {
        aout(self) << "FAILURE" << std::endl;
    };
    return {
        [=](StartMsg b) -> ResultMsg {
            self->state.children = 0;
            self->state.deps = std::vector<ResultMsg>(b.subtree->children.size());
            for (auto i = 1; i < b.subtree->children.size(); i++) {
                auto &child = b.subtree->children[i];
                if (child->type() != ast::NodeType::LITERAL) {
                    StartMsg msg{nullptr, child};
                    // Construct a fakeEnv

                    // If the node is a list node, spawn a list actor
                    if (child->type() == ast::NodeType::LIST) {
                        auto actor = self->system().spawn(listActor);
                        self->request(actor, infinite, msg).await(
                            [=](ResultMsg result) {
                                self->state.deps[i] = result;
                                // We want to see if we have completed all dependencies
                                if (self->state.deps.size() == self->state.children) {
                                    // TODO EXECUTE THE CURRENT COMMAND
                                }
                            }, handle_err);
                    } else if (child->type() == ast::NodeType::VECTOR) {
                        auto actor = self->system().spawn(vectorActor);
                        self->request(actor, infinite, msg).await(
                            [=](ResultMsg result) {
                                self->state.deps[i] = result;
                                // We want to see if we have completed all dependencies
                                if (self->state.deps.size() == self->state.children) {
                                    // TODO EXECUTE THE CURRENT COMMAND
                                }
                            }, handle_err);
                    }
                } else {
                    auto literalnode = std::dynamic_pointer_cast<ast::LiteralNode>(child);
                    self->state.deps[i] = ResultMsg{literalnode, literalnode->literal};
                }
            }
        }
    };
}
}
