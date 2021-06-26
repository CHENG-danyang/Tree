#ifndef _TREE_
#define _TREE_
#include <iostream>
#include <utility>
namespace tree
{
    //-------------------------------rb_tree----------------------------
    using std::pair;
    enum Color
    {
        red = false,
        black = true
    };
    template <typename key_type, typename elem_type>
    class rb_tree_node
    {
    public:
        template <typename T_1, typename T_2>
        friend class rb_tree;
        typedef rb_tree_node *Node;
        typedef unsigned int size_type;

    private:
        key_type key;
        elem_type elem;
        Node left;
        Node right;
        Node p;
        Color color;

    public:
        rb_tree_node(const pair<key_type, elem_type> &value) : key(value.first), elem(value.second), left(nullptr), right(nullptr), p(nullptr), color(red) {}
        rb_tree_node() : color(black) {}
        rb_tree_node(const rb_tree_node &node)
        {
            key = node->key;
            elem = node->elem;
            left = node->left;
            right = node->right;
            p = node->p;
            color = node->color;
        }
        ~rb_tree_node() = default;
    };
    template <typename key_type, typename elem_type>
    class rb_tree
    {
    public:
        using size_type = unsigned int;
        typedef typename rb_tree_node<key_type, elem_type>::Node Node;
        rb_tree() : root(nullptr), num(0){};
        Node minimum(Node node) const
        {
            if (node == nullptr)
                return node;
            while (node->left != nullptr)
                node = node->left;
            return node;
        }
        Node maximum(Node node) const
        {
            if (node == nullptr)
                return node;
            while (node->right != nullptr)
                node = node->right;
        }
        Node search(const key_type &value)
        {
            Node node = root;
            while (node != nullptr && node->key != value)
                if (value < node->key)
                    node = node->left;
                else if (value > node->key)
                    node = node->right;
            return node;
        }
        void insert(const pair<key_type, elem_type> &);
        void erase(const key_type &Key);
        Node successor(Node x) const
        {
            if (x->right != nullptr)
                return minimum(x->right);
            Node y = x->p;
            while (y != nullptr && x == y->right)
            {
                x = y;
                y = y->p;
            }
            return y;
        }
        Node predecessor(Node x)
        {
            if (x->left != nullptr)
                return maximum(x->left);
            Node y = x->p;
            while (y != nullptr && x == y->left)
            {
                x = y;
                y = y->p;
            }
        }
        elem_type Elem(Node node) const { return node->elem; }
        size_type size() const { return this->num; }

    private:
        Node root;
        size_type num;
        void rb_tree_insert_fixup(Node);
        void rb_tree_erase_fixup(Node, Node);
        void left_rotate(const Node x)
        {
            Node y = x->right;
            x->right = y->left;
            if (y->left != nullptr)
                y->left->p = x;
            y->p = x->p;
            if (x->p == nullptr)
                root = y;
            else if (x == x->p->left)
                x->p->left = y;
            else
                x->p->right = y;
            y->left = x;
            x->p = y;
        }
        void right_rotate(const Node x)
        {
            Node y = x->left;
            x->left = y->right;
            if (y->right != nullptr)
                y->right->p = x;
            y->p = x->p;
            if (x->p == nullptr)
                root = y;
            else if (x == x->p->left)
                x->p->left = y;
            else
                x->p->right = y;
            y->right = x;
            x->p = y;
        }
        void transplant(const Node u, const Node v)
        {
            if (u->p == nullptr)
                root = v;
            else if (u == u->p->left)
                u->p->left = v;
            else
                u->p->right = v;
            if (v != nullptr)
                v->p = u->p;
        }
    };
    template <typename key_type, typename elem_type>
    void rb_tree<key_type, elem_type>::insert(const pair<key_type, elem_type> &value)
    {
        Node z = new rb_tree_node<key_type, elem_type>(value);
        Node x = root;
        Node y = nullptr;
        while (x != nullptr)
        {
            y = x;
            if (value.first == x->key)
                break;
            if (value.first < x->key)
                x = x->left;
            else
                x = x->right;
        }
        if (x != nullptr && value.first == x->key)
            return;
        ++num;
        z->p = y;
        if (y == nullptr)
            root = z;
        else if (value.first < y->key)
            y->left = z;
        else
            y->right = z;
        rb_tree_insert_fixup(z);
    }
    template <typename key_type, typename elem_type>
    void rb_tree<key_type, elem_type>::rb_tree_insert_fixup(Node z)
    {
        while (z->p != nullptr && z->p->color == red)
        {
            if (z->p == z->p->p->left)
            {
                Node y = z->p->p->right;
                if (y != nullptr && y->color == red)
                {
                    z->p->color = black;
                    y->color = black;
                    z->p->p->color = red;
                    z = y->p;
                }
                else if (z == z->p->right)
                {
                    z = z->p;
                    left_rotate(z);
                }

                else if (z == z->p->left)
                {
                    z->p->color = black;
                    z->p->p->color = red;
                    right_rotate(z->p->p);
                }
            }
            else
            {
                if (z->p == z->p->p->right)
                {
                    Node y = z->p->p->left;
                    if (y != nullptr && y->color == red)
                    {
                        z->p->color = black;
                        y->color = black;
                        z->p->p->color = red;
                        z = y->p;
                    }
                    else if (z == z->p->left)
                    {
                        z = z->p;
                        right_rotate(z);
                    }
                    else if (z == z->p->right)
                    {
                        z->p->color = black;
                        z->p->p->color = red;
                        left_rotate(z->p->p);
                    }
                }
            }
            root->color = black;
        }
        root->color = black;
    }
    template <typename key_type, typename elem_type>
    void rb_tree<key_type, elem_type>::erase(const key_type &value)
    {
        Node x = nullptr;
        Node p;
        Node z = search(value);
        if (z == nullptr)
            return;
        --num;
        Node y = z;
        Color color = y->color;
        if (z->left == nullptr)
        {
            x = z->right;
            p = z->p;
            transplant(z, z->right);
        }
        else if (z->right == nullptr)
        {
            x = z->left;
            p = z->p;
            transplant(z, z->left);
        }
        else
        {
            y = successor(z);
            color = y->color;
            x = y->right;
            if (y->p == z)
            {
                if (x != nullptr)
                    x->p = y;
                p = y;
            }
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->p = y;
                p = y->p;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }
        delete z;
        z = nullptr;
        if (color == black)
            rb_tree_erase_fixup(x, p);
    }
    template <typename key_type, typename elem_type>
    void rb_tree<key_type, elem_type>::rb_tree_erase_fixup(Node x, Node p)
    {
        if (root == nullptr)
            return;
        while (x == nullptr || (x != root && x->color == black))
        {
            if (p->left == x)
            {
                Node w = p->right;
                if (w->color == red)
                {
                    p->color = red;
                    w->color = black;
                    left_rotate(p);
                    w = p->right;
                }
                if ((w->left == nullptr || w->left->color == black) && (w->right == nullptr || w->right->color == black))
                {
                    w->color = red;
                    x = p;
                    p = p->p;
                }
                else if (w->right == nullptr || w->right->color == black)
                {
                    if (w->left != nullptr)
                        w->left->color = black;
                    w->color = red;
                    right_rotate(w);
                    w = p->right;
                }
                else
                {
                    w->color = p->color;
                    p->color = black;
                    if (w->right != nullptr)
                        w->right->color = black;
                    left_rotate(p);
                    x = root;
                }
            }
            else
            {
                Node w = p->left;
                if (w->color == red)
                {
                    p->color = red;
                    w->color = black;
                    right_rotate(p);
                    w = p->left;
                }
                if ((w->left == nullptr || w->left->color == black) && (w->right == nullptr || w->right->color == black))
                {
                    w->color = red;
                    x = p;
                    p = p->p;
                }
                else if (w->left == nullptr || w->left->color == black)
                {
                    if (w->right != nullptr)
                        w->right->color = black;
                    w->color = red;
                    left_rotate(w);
                    w = p->left;
                }
                else
                {
                    w->color = p->color;
                    p->color = black;
                    if (w->left != nullptr)
                        w->left->color = black;
                    right_rotate(p);
                    x = root;
                }
            }
        }
        x->color = black;
    }
    template <typename T>
    std::ostream &operator<<(std::ostream &os, const typename T::Node node)
    {
        os << node->elem;
        return os;
    }
} // namespace tree

#endif 
