//
// Created by michal on 22.01.17.
//

#ifndef FUNCTIONAL_BINARY_TREE_TREE_H
#define FUNCTIONAL_BINARY_TREE_TREE_H

#include <functional>
#include <memory>


template <typename T>
class Tree {
    using REF_NODE = std::shared_ptr<Tree<T>>;
    REF_NODE _left, _right;
    T _value;
    Tree(T value, REF_NODE left, REF_NODE right) : _left(left), _right(right), _value(value) {}
    Tree(T value): _value(value) {}
public:
    Tree() {}
    Tree(REF_NODE root) : _left(root->_left), _right(root->_right), _value(root->_value) {}

    T fold(std::function<T (T, T, T)> operation,T init) {
        if (this == nullptr)
            return init;
        else
            return operation(_left->_value, _right->_value, _value);
    };

    static REF_NODE createEmptyNode() {
        return REF_NODE();
    }

    static REF_NODE createValueNode(T value) {
        return REF_NODE(new Tree(value));
    }

    static REF_NODE createValueNode(T value, REF_NODE left, REF_NODE right) {
        return REF_NODE(new Tree(value, left, right));
    }



};


#endif //FUNCTIONAL_BINARY_TREE_TREE_H
