#include <vector>
#include "bandit/bandit.h"
#include "inc/ast.h"
#include "inc/builtin.hpp"

using namespace snowhouse;
using namespace bandit;

go_bandit([]() {
    describe("proc_add", [](){
        it("should add 2 numbers", [&](){
            auto vec = std::vector<ast::LiteralNode>();
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 5));
            AssertThat(
                std::get<int>(depp::proc_add(vec).literal),
                Equals(28));
        });
    });

});
