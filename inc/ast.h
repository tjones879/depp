#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace ast {
class ListNode;
class LiteralNode;

typedef std::variant<int, double, bool, std::string, std::vector<LiteralNode>>
    LiteralVariant;

template <class E>
struct enumToString {
    enumToString() {}
    virtual std::string operator()(E val) = 0;
};

template <class E>
class TypeConverter {
private:
    const E type;
    enumToString<E> converter;
    E strToType(const std::string &str);

public:
    TypeConverter(E t, enumToString<E> ets)
        : type(t)
        , converter(ets)
    {
    }
    std::string operator()()
    {
        return converter(type);
    }
};

enum class NodeType : int { PROGRAM, ATOMS, LIST, VECTOR, MAP, LITERAL };

enum class LiteralType : int {
    BOOL,
    NIL,
    STRING,
    REAL,
    INTEGER,
    IDENT,
    KEYWORD,
    RESERVED,
    LIST
};

struct literalTypeToStr : enumToString<LiteralType> {
    virtual std::string operator()(LiteralType val)
    {
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
        case LiteralType::LIST:
            return "LIST";
        }
    }
};

class Node {
public:
    std::vector<std::shared_ptr<Node>> children;

    virtual NodeType type() const = 0;
    virtual void print(std::ostream &out) const = 0;
    virtual ~Node(){};
};

typedef std::shared_ptr<Node> NodePtr;

class ProgramNode : public Node {
public:
    NodeType type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<ProgramNode> ProgramNodePtr;

class AtomsNode : public Node {
public:
    NodeType type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<AtomsNode> AtomsNodePtr;

class ListNode : public Node {
public:
    NodeType type() const;
    void print(std::ostream &out) const;
    bool operator==(const ListNode &other) const;
    bool operator!=(const ListNode &other) const;
};

typedef std::shared_ptr<ListNode> ListNodePtr;

class VectorNode : public Node {
public:
    NodeType type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<VectorNode> VectorNodePtr;

class MapNode : public Node {
public:
    NodeType type() const;
    void print(std::ostream &out) const;
};

typedef std::shared_ptr<MapNode> MapNodePtr;

class LiteralNode : public Node {
public:
    LiteralType token_type;
    LiteralVariant literal;
    LiteralNode(LiteralType type, LiteralVariant literal);
    LiteralNode(const ListNodePtr& list);
    LiteralNode(std::vector<LiteralNode> vec);
    NodeType type() const;
    void print(std::ostream &out) const;
    bool operator==(const LiteralNode &other) const;
    bool operator!=(const LiteralNode &other) const;
};

typedef std::shared_ptr<LiteralNode> LiteralNodePtr;
NodePtr node(NodeType type);
NodePtr node(LiteralType type, LiteralVariant literal);

std::ostream &operator<<(std::ostream &out, const NodePtr &p);
} /* namespace ast */
#endif /* AST_H */
