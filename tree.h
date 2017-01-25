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
    using REF_TREE = std::shared_ptr<Tree<T>>;
    class Node {
    public:
        using REF_NODE = std::shared_ptr<Node>;
        REF_NODE _left, _right;
        T _value;
        bool _hasValue = 1;
        template <typename R, typename F>
        R fold(F operation, R init) const {
            if (!_hasValue)
                return init;
            else
                return operation(_value, _left->fold(operation, init), _right->fold(operation, init));
        };
        Node() : _hasValue(0) {}
        Node(T value, REF_NODE left, REF_NODE right) : _left(left), _right(right), _value(value) {};
    };
    std::shared_ptr<Node> _root;
    Tree(T value, REF_TREE left, REF_TREE right) : _root(value, left->_root, right->_root) {}
public:
    Tree() : _root(nullptr) {}
    Tree(std::shared_ptr<Node> root) : _root(root) {};
    Tree(REF_TREE root) : _root(root->_root) {}

    template <typename R, typename F>
    R fold(F operation, R init) const {
        return _root->fold(operation, init);
    };

    static REF_TREE createEmptyNode() {
        return REF_TREE(new Node());
    }

    static REF_TREE createValueNode(T value, REF_TREE left, REF_TREE right) {
        return REF_TREE(new Tree(value, left, right));
    }

    static REF_TREE createValueNode(T value) {
        return createValueNode(value, createEmptyNode(), createEmptyNode());
    }


    unsigned height() const {
        return fold<unsigned>([&](unsigned val, unsigned l_height, unsigned r_height){ return std::max(l_height, r_height) + 1; }, 0);
    }

    /*
    unsigned size() const {
        return fold<unsigned>([&](unsigned val, unsigned l_size, unsigned r_size) { return l_size + r_size + unsigned(_hasValue); }, 0);
    }
     */

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

    /*
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
    */


};



#endif //FUNCTIONAL_BINARY_TREE_TREE_H
