#include "ast.h"

namespace ast
{

template <class T>
void printChildren(std::ostream &out, std::vector<T> children, std::string prefix) {
    for (auto node : children)
        out << prefix << "child: " << node << std::endl;
}

NodeType ProgramNode::type() const {
    return (NodeType::PROGRAM);
}

void ProgramNode::print(std::ostream &out) const {
    out << "program" << std::endl;
    printChildren(out, children, std::string("    program-"));
    out << "end program" << std::endl;
}

NodeType AtomsNode::type() const {
    return (NodeType::ATOMS);
}

void AtomsNode::print(std::ostream &out) const {
    out << "atoms" << std::endl;
    printChildren(out, children, std::string("    atoms-"));
    out << "end atoms" << std::endl;
}

NodeType ListNode::type() const {
    return (NodeType::LIST);
}

void ListNode::print(std::ostream &out) const {
    out << "list (" << std::endl;
    printChildren(out, children, std::string("    list"));
    out << " )" << std::endl;
}

NodeType VectorNode::type() const {
    return (NodeType::VECTOR);
}

void VectorNode::print(std::ostream &out) const {
    out << "vector [" << std::endl;
    printChildren(out, children, std::string("    vector"));
    out << " ]" << std::endl;
}

NodeType MapNode::type() const {
    return (NodeType::MAP);
}

void MapNode::print(std::ostream &out) const {
    out << "map {" << std::endl;
    printChildren(out, children, std::string("    map"));
    out << " }" << std::endl;
}

LiteralNode::LiteralNode(LiteralType type, LiteralVariant literal) {
    this->token_type = type;
    this->literal = literal;
}

NodeType LiteralNode::type() const {
    return (NodeType::LITERAL);
}

void LiteralNode::print(std::ostream &out) const {
    out << "literal type= " << static_cast<std::string>(literalTypeToStr()(token_type));
    std::visit([&out](auto &arg) { out << ", literal val=" << arg << std::endl;}, literal);
    printChildren(out, children, std::string("    literal"));
}

NodePtr node(NodeType type) {
    switch (type) {
    case NodeType::PROGRAM:
        return NodePtr(new ProgramNode());
        break;
    case NodeType::LIST:
        return NodePtr(new ListNode());
        break;
    case NodeType::VECTOR:
        return NodePtr(new VectorNode());
        break;
    case NodeType::MAP:
        return NodePtr(new MapNode());
        break;
    default:
        return NULL;
        break;
    }
}

NodePtr node(LiteralType type, LiteralVariant literal) {
    return NodePtr(new LiteralNode(type, literal));
}

std::ostream &operator<<(std::ostream &out, const NodePtr &p) {
    p->print(out);
    return out;
}
} /* namespace ast */
