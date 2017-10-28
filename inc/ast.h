#ifndef AST_H
#define AST_H

#include <vector>
#include <variant>
#include <string>
#include <iostream>
#include <memory>

namespace ast
{
typedef std::variant<int, double, bool, std::string> LiteralVariant;

template <class E>
struct enumToString {
    enumToString() {}
    virtual std::string operator() (E val)=0;
};

template <class E>
class TypeConverter {
private:
    const E type;
    enumToString<E> converter;
    E strToType(const std::string &str);
public:
    TypeConverter(E t, enumToString<E> ets) : type(t), converter(ets) {}
    std::string operator() () { return converter(type); }
};

enum class NodeType: int {
    PROGRAM,
    ATOMS,
    LIST,
    VECTOR,
    MAP,
    LITERAL
};

enum class LiteralType : int {
    BOOL,
    NIL,
    STRING,
    REAL,
    INTEGER,
    IDENT,
    KEYWORD,
    RESERVED
};

struct literalTypeToStr : enumToString<LiteralType> {
    virtual std::string operator() (LiteralType val) {
        switch (val) {
            case LiteralType::BOOL:
                return "BOOL";
            case LiteralType::NIL:
                return "NIL";
            case LiteralType::STRING:
                return "STRING";
            case LiteralType::REAL:
                return "REAL";
            case LiteralType::INTEGER:
                return "INTEGER";
            case LiteralType::IDENT:
                return "IDENT";
            case LiteralType::KEYWORD:
                return "KEYWORD";
            case LiteralType::RESERVED:
                return "RESERVED";
        }
    }
};

class Node {
public:
    std::vector<std::shared_ptr<Node>> children;

    virtual int type() const = 0;
    virtual void print(std::ostream &out) const = 0;
    virtual ~Node() {};
};

typedef std::shared_ptr<Node> NodePtr;

class ProgramNode : public Node {
public:
    int type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<ProgramNode> ProgramNodePtr;

class AtomsNode : public Node {
public:
    int type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<AtomsNode> AtomsNodePtr;

class ListNode : public Node {
public:
    int type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<ListNode> ListNodePtr;

class VectorNode : public Node {
public:
    int type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<VectorNode> VectorNodePtr;

class MapNode : public Node {
public:
    int type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<MapNode> MapNodePtr;

class LiteralNode : public Node {
public:
    LiteralType token_type;
    LiteralVariant literal;
    LiteralNode(LiteralType type, LiteralVariant literal);
    int type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<LiteralNode> LiteralNodePtr;
NodePtr node(NodeType type);
NodePtr node(LiteralType type, LiteralVariant literal);

std::ostream &operator<<(std::ostream &out, const NodePtr &p);
} /* namespace ast */
#endif /* AST_H */
