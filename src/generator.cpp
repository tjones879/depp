#include "generator.hpp"
#include <iostream>
#include <memory>
#include <vector>

namespace gen {
    void handleDef(std::shared_ptr<env::Environment> env, ast::NodePtr parent, int position);
    void handleQuote(env::EnvironmentPtr env, ast::NodePtr parent, int position);

    Generator::Generator(const ast::NodePtr ptr) {
        env = std::make_shared<env::Environment>();
        ast = ptr;
    }

    Generator::Generator(env::EnvironmentPtr en, const ast::NodePtr &ptr) : env(en), ast(ptr) { }

    void Generator::dumpEnv(std::ostream &out) {
        env->print(out);
    }

    void ReservedHandler::generate() {
        checkSymbols(nullptr, ast, 0);
    }

    void ReservedHandler::checkSymbols(ast::NodePtr parent, ast::NodePtr node, int position) {
        // Short circuit evaluation will not always work, so we must separate the conditions
        if (!node) {
            return;
        } else if (node->type() == ast::NodeType::LITERAL) {
            auto literal = std::dynamic_pointer_cast<ast::LiteralNode>(node);

            if (literal->token_type == ast::LiteralType::RESERVED) {
                auto val = std::get<std::string>(literal->literal);
                if (val == "def")
                    handleDef(env, parent, position);
                else if (val == "'")
                    handleQuote(env, parent, position);
            }
        } else {
            for (int pos = 0; pos < node->children.size(); pos++) {
                checkSymbols(node, node->children.at(pos), pos);
            }
        }
    }

    void handleDef(env::EnvironmentPtr env, ast::NodePtr parent, int position) {
        // Check for proper usage of def, throw error if incorrect
        try {
            if (position + 2 >= parent->children.size())
                throw std::out_of_range("Incorrect def special form");
            // Always access the node immediately following def, this must be a literal identifier
            auto symbol = std::dynamic_pointer_cast<ast::LiteralNode>(parent->children.at(position + 1));
            auto key = std::get<std::string>(symbol->literal);
            // The next value in this special form will be the value of the symbol

            env->addSymbol(key, parent->children.at(position + 2));
        } catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        } catch (const std::bad_variant_access &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void handleQuote(env::EnvironmentPtr env, ast::NodePtr parent, int position) {
        // Quotes will convert a list of atoms to be
        // interpreted as a literal list. Similar to
        // the concept of a vector
        try {
            auto next = parent->children.at(position + 1);
            if (next->type() != ast::NodeType::LIST)
                throw std::invalid_argument("Element following quote must be a list");
            parent->children[position + 1] = ast::node(ast::LiteralType::LIST, *std::dynamic_pointer_cast<ast::ListNode>(next).get());
        } catch (const std::out_of_range &e) {
            std::cerr << e.what() << std::endl;
        } catch (const std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
    }

}
