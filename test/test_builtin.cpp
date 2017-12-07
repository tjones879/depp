#include <vector>
#include "bandit/bandit.h"
#include "inc/ast.h"
#include "inc/builtin.hpp"

using namespace snowhouse;
using namespace bandit;

go_bandit([]() {
    describe("proc_add", [](){
        std::vector<ast::LiteralNode> vec;
        before_each([&]() {
            vec = std::vector<ast::LiteralNode>();
        });
        it("should add 1 int", [&](){
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 5));
            AssertThat(
                std::get<int>(depp::proc_add(vec).literal),
                Equals(5));
        });
        it("should add 2 ints", [&](){
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 5));
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 3));
            AssertThat(
                std::get<int>(depp::proc_add(vec).literal),
                Equals(8));
        });
        it("should add 1 double", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 10.0));
            AssertThat(
                std::get<double>(depp::proc_add(vec).literal),
                Equals(10.0));
        });
        it("should add 2 doubles", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 10.0));
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 25.0));
            AssertThat(
                std::get<double>(depp::proc_add(vec).literal),
                Equals(35.0));
        });
        it("should add 1 int 1 double", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 5));
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 10.0));
            AssertThat(
                std::get<double>(depp::proc_add(vec).literal),
                Equals(15.0));
        });
        it("should add 1 double 1 int", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 10.0));
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 5));
            AssertThat(
                std::get<double>(depp::proc_add(vec).literal),
                Equals(15.0));
        });
        it("should add 1 string", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::STRING, std::string("Hi")));
            AssertThat(
                std::get<std::string>(depp::proc_add(vec).literal),
                Equals("Hi"));
        });
        it("should concat 2 strings", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::STRING, std::string("Hi")));
            vec.push_back(ast::LiteralNode(ast::LiteralType::STRING, std::string("Hi")));
            AssertThat(
                std::get<std::string>(depp::proc_add(vec).literal),
                Equals("HiHi"));
        });
    });

    describe("proc_sub", []() {
        std::vector<ast::LiteralNode> vec;
        before_each([&]() {
            vec = std::vector<ast::LiteralNode>();
        });

        it("should subtract 2 ints", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 15));
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 9));
            AssertThat(
                std::get<int>(depp::proc_sub(vec).literal),
                Equals(6));
        });
        it("should subtract 2 doubles", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 10.0));
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 7.0));
            AssertThat(
                std::get<double>(depp::proc_sub(vec).literal),
                Equals(3.0));
        });
        it("should subtract double from int", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 9));
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 3.0));
            AssertThat(
                std::get<double>(depp::proc_sub(vec).literal),
                Equals(6.0));
        });
        it("should subtract int from double", [&]() {
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, 19.0));
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, 8));
            AssertThat(
                std::get<double>(depp::proc_sub(vec).literal),
                Equals(11.0));
        });
    });

    describe("proc_mult", []() {
        std::vector<ast::LiteralNode> vec;
        before_each([&]() {
            vec = std::vector<ast::LiteralNode>();
        });

        it("should multiply 1 int", [&]() {
            auto a = 15;
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, a));
            AssertThat(
                std::get<int>(depp::proc_mult(vec).literal),
                Equals(a));
        });
        it("should multiply 2 ints", [&]() {
            auto a = 3;
            auto b = 7;
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, a));
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, b));
            AssertThat(
                std::get<int>(depp::proc_mult(vec).literal),
                Equals(a*b));
        });
        it("should multiply 1 double", [&]() {
            double a = 8.0;
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, a));
            AssertThat(
                std::get<double>(depp::proc_mult(vec).literal),
                Equals(a));
        });
        it("should multiply 2 doubles", [&]() {
            double a = 90.0;
            double b = 0.5;
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, a));
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, b));
            AssertThat(
                std::get<double>(depp::proc_mult(vec).literal),
                Equals(a*b));
        });
        it("should multiply a double and int", [&]() {
            int a = 2;
            double b = 3.0;
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, b));
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, a));
            AssertThat(
                std::get<double>(depp::proc_mult(vec).literal),
                Equals(a*b));
        });
        it("should multiply a int and double", [&]() {
            int a = 1;
            double b = 7.0;
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, a));
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, b));
            AssertThat(
                std::get<double>(depp::proc_mult(vec).literal),
                Equals(a*b));
        });
    });

    describe("proc_div", []() {
        std::vector<ast::LiteralNode> vec;
        before_each([&]() {
            vec = std::vector<ast::LiteralNode>();
        });

        it("should divide 2 ints", [&]() {
            int a = 5;
            int b = 1;
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, a));
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, b));
            AssertThat(
                std::get<int>(depp::proc_div(vec).literal),
                Equals(a/b));
        });
        it("should divide 2 doubles", [&]() {
            double a = 9.0;
            double b = 3.0;
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, a));
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, b));
            AssertThat(
                std::get<double>(depp::proc_div(vec).literal),
                Equals(a/b));
        });
        it("should divide double by int", [&]() {
            double a = 10.0;
            int b = 2;
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, a));
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, b));
            AssertThat(
                std::get<double>(depp::proc_div(vec).literal),
                Equals(a/b));
        });
        it("should divide int by double", [&]() {
            int a = 12;
            double b = 6.0;
            vec.push_back(ast::LiteralNode(ast::LiteralType::INTEGER, a));
            vec.push_back(ast::LiteralNode(ast::LiteralType::REAL, b));
            AssertThat(
                std::get<double>(depp::proc_div(vec).literal),
                Equals(a/b));
        });
    });

});
