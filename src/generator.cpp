#include "inc/generator.hpp"
#include "inc/utils.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace gen {
std::shared_ptr<env::Symbol> Generator::handleRunner(ast::NodePtr ptr) {
    std::shared_ptr<env::Symbol> sym;

    if (ptr->type() != ast::NodeType::LITERAL)
        ptr = walkTree(ptr);

    auto runner = std::dynamic_pointer_cast<ast::LiteralNode>(ptr);
    if (runner) {
        switch(runner->token_type) {
        case ast::LiteralType::IDENT:
        case ast::LiteralType::RESERVED:
            sym = env->getSymbol(std::get<std::string>(runner->literal));
            break;
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

ast::LiteralNodePtr Generator::walkTree(const ast::NodePtr ptr) {
    auto runner = ptr->children[0];
    auto sym = handleRunner(runner);
    ast::LiteralNode ret(ast::LiteralNode(ast::LiteralType::NIL, false));

    if (sym && sym->type == env::SymbolType::FUNC) {
        auto deps = std::vector<ast::LiteralNode>();
        for (auto child : depp::offset(ptr->children, 1)) {
            std::cout << "walkTree:" << child << std::endl;
            switch (child->type()) {
            case ast::NodeType::LIST:
            case ast::NodeType::VECTOR:
                deps.push_back(*walkTree(child));
                break;
            case ast::NodeType::LITERAL:
                deps.push_back(*std::dynamic_pointer_cast<ast::LiteralNode>(child));
                break;
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
