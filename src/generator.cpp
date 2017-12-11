#include "inc/generator.hpp"
#include "inc/builtin.hpp"
#include "inc/utils.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace gen {
std::shared_ptr<env::Symbol> Generator::handleRunner(ast::NodePtr ptr, ast::NodePtr parent) {
    std::shared_ptr<env::Symbol> sym;

    if (ptr->type() != ast::NodeType::LITERAL)
        ptr = walkTree(ptr);

    auto runner = std::dynamic_pointer_cast<ast::LiteralNode>(ptr);
    if (runner) {
        switch(runner->token_type) {
        case ast::LiteralType::IDENT:
        case ast::LiteralType::RESERVED: {
            auto str = std::get<std::string>(runner->literal);
            if (str == "def") {
                sym = std::make_shared<env::Applicable>(env::buildDef(env, depp::proc_def));
            } else if (str == "defun") {
                env::defun f =
                    [](env::EnvironmentPtr env, ast::LiteralNode args, ast::NodePtr parent) -> ast::LiteralNode {
                        // We must capture the environment where this function was defined.
                        auto gen = Generator(env, parent);
                        return ast::LiteralNode(ast::LiteralType::NIL, false);
                    };
                sym = std::make_shared<env::Applicable>(env::buildFunc(env, parent, f));
            } else {
                sym = env->getSymbol(std::get<std::string>(runner->literal));
            }
            break;
        }
        default:
            break;
        }
    }
    return sym;
}

Generator::Generator(const ast::NodePtr ptr) {
    env = std::make_shared<env::Environment>();
    ast = ptr;
}

Generator::Generator(env::EnvironmentPtr en, const ast::NodePtr &ptr) : env(en), ast(ptr) { }

void Generator::dumpEnv(std::ostream &out) {
    env->print(out);
}

ast::LiteralNode checkQuote(ast::LiteralNodePtr current, ast::NodePtr next) {
    if (current->token_type == ast::LiteralType::RESERVED)
        if (std::get<std::string>(current->literal) == "'") {
            auto list = std::dynamic_pointer_cast<ast::ListNode>(next);
            return depp::proc_quote(list);
        }
    return ast::LiteralNode(ast::LiteralType::NIL, false);
}

ast::LiteralNodePtr Generator::walkTree(const ast::NodePtr ptr) {
    auto runner = ptr->children[0];
    auto sym = handleRunner(runner, ptr);
    ast::LiteralNode ret(ast::LiteralNode(ast::LiteralType::NIL, false));

    if (sym && sym->type == env::SymbolType::FUNC) {
        auto deps = std::vector<ast::LiteralNode>();
        for (auto child = ptr->children.begin() + 1; child < ptr->children.end(); child++) {
            switch ((*child)->type()) {
            case ast::NodeType::LIST:
            case ast::NodeType::VECTOR:
                deps.push_back(*walkTree(*child));
                break;
            case ast::NodeType::LITERAL: {
                auto current = std::dynamic_pointer_cast<ast::LiteralNode>(*child);
                auto quoted = checkQuote(current, child[1]);
                if (quoted.token_type != ast::LiteralType::NIL) {
                    deps.push_back(quoted);
                    quoted.print(std::cout);
                    child++;
                } else {
                    deps.push_back(*current);
                }
                break;
            }
            default:
                break;
            }
        }

        auto app = std::dynamic_pointer_cast<env::Applicable>(sym);
        ret = app->apply(deps);
        ret.print(std::cout);
    }

    return std::make_shared<ast::LiteralNode>(ret);
}
}
