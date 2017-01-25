//
// Created by michal on 22.01.17.
//

#ifndef FUNCTIONAL_BINARY_TREE_TREE_H
#define FUNCTIONAL_BINARY_TREE_TREE_H

#include <functional>
#include <memory>
#include <vector>

enum Traversal {
    INORDER, POSTORDER, PREORDER
};

template <typename T>
class Tree {
    using REF_NODE = std::shared_ptr<Tree<T>>;
    REF_NODE _left, _right;
    T _value;
    bool _hasValue = 1;
    Tree(T value, REF_NODE left, REF_NODE right) : _left(left), _right(right), _value(value) {}
public:
    Tree() : _hasValue(0) {}
    Tree(REF_NODE root) : Tree(root->_value, root->_left, root->_right) {}

    template <typename R, typename F>
    R fold(F operation, R init) const {
        if (!_hasValue)
            return init;
        else
            return operation(_value, _left->fold(operation, init), _right->fold(operation, init));
    };

    static REF_NODE createEmptyNode() {
        return REF_NODE(new Tree());
    }

    static REF_NODE createValueNode(T value, REF_NODE left, REF_NODE right) {
        return REF_NODE(new Tree(value, left, right));
    }

    static REF_NODE createValueNode(T value) {
        return createValueNode(value, createEmptyNode(), createEmptyNode());
    }


    unsigned height() const {
        return fold<unsigned>([&](unsigned val, unsigned l_height, unsigned r_height){ return std::max(l_height, r_height) + 1; }, 0);
    }

    unsigned size() const {
        return fold<unsigned>([&](unsigned val, unsigned l_size, unsigned r_size) { return l_size + r_size + unsigned(_hasValue); }, 0);
    }

    /**
     * max, min, is_bst, is_node_with_value
     */
    using NODE_INFO = std::tuple<T, T, bool, bool>;

    bool is_bst() const {
        return std::get<2>(fold([](T val, NODE_INFO l_tree, NODE_INFO r_tree){
            bool l_bst = std::get<2>(l_tree), r_bst = std::get<2>(r_tree);

            T l_max = std::get<0>(l_tree), l_min = std::get<1>(l_tree),
                r_max = std::get<0>(r_tree), r_min = std::get<1>(r_tree);

            bool left_valid = std::get<3>(l_tree);
            bool right_valid = std::get<3>(r_tree);

            if (!left_valid)
                l_max = l_min = val;
            if (!right_valid)
                r_max = r_min = val;

            bool node_bst = (val >= l_max) && (val <= r_min);

            return std::make_tuple(std::max(val, r_max), std::min(val, l_min), l_bst && r_bst && node_bst, true);
        }, std::make_tuple(0, 0, true, false)));
    }

    static const Traversal inorder = INORDER;
    static const Traversal preorder = PREORDER;
    static const Traversal postorder = POSTORDER;

    template <typename F>
    void apply(F operation, const Traversal traversal) {
        if (!_hasValue)
            return;

        if (traversal == inorder) {
            _left->apply(operation, traversal);
            operation(_value);
            _right->apply(operation, traversal);
        }
        if (traversal == postorder) {
            _left->apply(operation, traversal);
            _right->apply(operation, traversal);
            operation(_value);
        }
        if (traversal == preorder) {
            operation(_value);
            _left->apply(operation, traversal);
            _right->apply(operation, traversal);
        }
    }

    template <typename F>
    T accumulate(F operation, T init, const Traversal traversal) {
        T result = init;
        apply([&](T el){ result = operation(result, el); } , traversal);
        return result;
    }

    void print(const Traversal traversal=inorder) {
        apply([](T el){ std::cout << el << " "; }, traversal);
        std::cout << std::endl;
    }



};



#endif //FUNCTIONAL_BINARY_TREE_TREE_H
