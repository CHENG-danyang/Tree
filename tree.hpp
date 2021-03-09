#ifndef _TREE_
#define _TREE_
#include <iostream>
namespace tree
{
    template <typename T>
    class search_tree;
    template <typename T>
    class avl_tree;
    template <typename T>
    class rb_tree;
    template <typename T>
    std::ostream &operator<<(std::ostream &, const typename T::Node);
    template <typename elem_type>
    class search_tree_node
    {
    public:
        template <typename T>
        friend std::ostream &operator<<(std::ostream &, const typename T::Node);
        template <typename T>
        friend class search_tree;
        typedef search_tree_node *Node;

    private:
        elem_type elem;
        Node p;
        Node left;
        Node right;

    public:
        search_tree_node(const elem_type &value) : elem(value), p(nullptr), left(nullptr), right(nullptr) {}
    };
    template <typename elem_type>
    class search_tree
    {
    public:
        typedef typename search_tree_node<elem_type>::Node Node;
        typedef Node iterator;

    private:
        Node root;

    protected:
        void trans_plant(Node x, Node y) //将y嫁接到x的位置;
        {
            if (x == nullptr)
                this->root = y;
            else if (x == x->p->left)
                x->p->left = y;
            else
                x->p->right = y;
            if (y != nullptr)
                y->p = x->p;
        }

    public:
        search_tree() : root(nullptr) {}

        virtual Node search(const elem_type &value)
        {
            Node x = this->root;
            while (x != nullptr && value != x->elem)
            {
                if (x->elem > value)
                    x = x->left;
                else if (x->elem < value)
                    x = x->right;
            }
            return x;
        }
        void destory()
        {
            destory(root);
        }
        void destory(Node node)
        {
            if (node->left != nullptr)
                destory(node->left);
            if (node->right != nullptr)
                destory(node->right);
            delete node;
        }
        Node min(Node x) const
        {
            while (x->left != nullptr)
                x = x->left;
            return x;
        }
        Node max(Node x) const
        {
            while (x->left != nullptr)
                x = x->right;
            return x;
        }
        Node predecessor(Node x) const //查找x的先驱
        {
            if (x->left != nullptr)
                return max(x->left);
            Node y = x->p;
            while (y != nullptr && x == x->p->left)
            {
                x = y;
                y = y->p;
            }
            return y;
        }
        Node successor(Node x) const //查找x的后驱
        {
            if (x->right != nullptr)
                return min(x->right);
            Node y = x.p;
            while (y != nullptr &&x = x->p->right)
            {
                x = y;
                y = y->p;
            }
            return y;
        }
        virtual void insert(const elem_type &);
        virtual void erase(typename search_tree<elem_type>::Node);
    };
    template <typename elem_type>
    void search_tree<elem_type>::insert(const elem_type &value)
    {
        typename search_tree<elem_type>::Node node = new search_tree_node<elem_type>(value);
        typename search_tree<elem_type>::Node x = this->root;
        typename search_tree<elem_type>::Node y = nullptr; //x的p节点
        while (x != nullptr)
        {
            y = x;
            if (value > x->elem)
                x = x->right;
            else if (value < x->elem)
                x = x->left;
        }
        node->p = y;
        if (y == nullptr)
            this->root = node;
        else if (value > y->elem)
            y->right = node;
        else
            y->left = node;
    }
    template <typename elem_type>
    void search_tree<elem_type>::erase(typename search_tree<elem_type>::Node node) //删除例程，注：节点node默认已在树中
    {
        if (node->left == nullptr)
            trans_plant(node, node->right);
        else if (node->right == nullptr)
            trans_plant(node->p, node);
        else
        {
            typename search_tree<elem_type>::Node y = min(node->right);
            if (y->p != node)
            {
                trans_plant(y, y->right);
                y->right = node->right;
                y->right->p = y;
            }
            trans_plant(node, y);
            y->left = node->left;
            y->left->p = y;
        }
        delete node;
    }
    template <typename elem_type>
    class avl_tree_node
    {
    public:
        template <typename T>
        friend std::ostream &operator<<(std::ostream &, const typename T::Node);
        template <typename T>
        friend class avl_tree;
        typedef avl_tree_node *Node;

    private:
        elem_type elem;
        size_t height;
        Node left;
        Node right;
        Node p;

    public:
        avl_tree_node(const elem_type &value, Node node) : elem(value), height(0), left(node), right(node), p(node) {}
    };
    template <typename elem_type>
    class avl_tree
    {
    public:
        typedef typename avl_tree_node<elem_type>::Node Node;

    private:
        Node root;
        void right_rotate(Node);
        void left_rotate(Node);
        void right_rotate_twice(Node);
        void left_rotate_twice(Node);
        static size_t Height(Node node)
        {
            if (node == nullptr)
                return 0;
            else
                return node->height;
        }

    public:
        avl_tree() : root(nullptr) {}
        void insert(const elem_type &value)
        {
            if (root == nullptr)
                root = new avl_tree_node<elem_type>(value);
            else
                insert(root, value);
        }
        Node search(const elem_type &value)
        {
            auto x = this->root;
            while (x != nullptr && value != x->elem)
            {
                if (x->elem > value)
                    x = x->left;
                else if (x->elem < value)
                    x = x->right;
            }
            return x;
        }
        Node insert(Node, const elem_type &value);
    };
    template <typename elem_type>
    void avl_tree<elem_type>::right_rotate(typename avl_tree<elem_type>::Node x)
    {
        typename avl_tree<elem_type>::Node y = x->left;
        x->left = y->right;
        if (y->right != nullptr)
            y->right->p = x;
        y->p = x->p;
        if (x->p == nullptr)
            this->root = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->right = x;
        x->p = y;
        y->height = std::max(Height(y->left), Height(y->right)) + 1;
        x->height = std::max(Height(x->left), Height(y)) + 1;
    }
    template <typename elem_type>
    void avl_tree<elem_type>::left_rotate(typename avl_tree<elem_type>::Node x)
    {
        typename avl_tree<elem_type>::Node y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->p = x;
        y->p = x->p;
        if (x->p == nullptr)
            this->root = y;
        else if (x == x->p->left)
            x->p->left = y;
        else
            x->p->right = y;
        y->left = x;
        x->p = y;
        y->height = std::max(Height(y->right), Height(y->left)) + 1;
        x->height = std::max(Height(x->left), Height(y)) + 1;
    }
    //双旋转方向：第一次旋转方向
    template <typename elem_type>
    void avl_tree<elem_type>::right_rotate_twice(typename avl_tree<elem_type>::Node x)
    {
        right_rotate(x);
        left_rotate(x->p->p);
    }
    template <typename elem_type>
    void avl_tree<elem_type>::left_rotate_twice(typename avl_tree<elem_type>::Node x)
    {
        left_rotate(x);
        right_rotate(x->p->p);
    }
    template <typename elem_type>
    typename avl_tree<elem_type>::Node avl_tree<elem_type>::insert(typename avl_tree<elem_type>::Node node, const elem_type &value)
    {
        if (node == nullptr)
        {
            node = new avl_tree_node<elem_type>(value);
        }
        else if (value < node->elem)
        {
            node->left = insert(node->left, value);
            node->left->p = node;
            if (Height(node->left) - Height(node->right) == 2)
            {
                if (value < node->left->elem)
                {
                    right_rotate(node);
                    node = node->p;
                }
                else
                {
                    left_rotate_twice(node->left);
                }
            }
        }
        else
        {
            node->right = insert(node->right, value);
            node->right->p = node;
            if (Height(node->right) - Height(node->left) == 2)
            {
                if (value > node->right->elem)
                {
                    left_rotate(node);
                    node = node->p;
                }
                else
                    right_rotate_twice(node->right);
            }
        }
        node->height = std::max(Height(node->left), Height(node->right)) + 1;
        return node;
    }
    enum Color
    {
        red = false,
        black = true
    };
    template <typename elem_type>
    class rb_tree_node
    {
    public:
        template <typename T>
        friend class rb_tree;
        typedef rb_tree_node *Node;
        typedef unsigned int size_type;
        template <typename T>
        friend std::ostream &operator<<(std::ostream &, const typename T::Node);

    private:
        elem_type elem;
        Node left;
        Node right;
        Node p;
        Color color;

    public:
        rb_tree_node(const elem_type &value) : elem(value), left(nullptr), right(nullptr), p(nullptr), color(red) {}
        rb_tree_node() : color(black) {}
        rb_tree_node(const rb_tree_node &node)
        {
            elem = node->elem;
            left = node->left;
            right = node->right;
            p = node->p;
            color = node->color;
        }
        ~rb_tree_node() = default;
    };
    template <typename elem_type>
    class rb_tree
    {
    public:
        using size_type = unsigned int;
        typedef typename rb_tree_node<elem_type>::Node Node;
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
        Node search(const elem_type &value)
        {
            Node node = root;
            while (node != nullptr && node->elem != value)
                if (value < node->elem)
                    node = node->left;
                else if (value > node->elem)
                    node = node->right;
            return node;
        }
        void insert(const elem_type &elem);
        void erase(const elem_type &elem);
        Node successor(Node x)
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
    template <typename elem_type>
    void rb_tree<elem_type>::insert(const elem_type &value)
    {
        Node z = new rb_tree_node<elem_type>(value);
        Node x = root;
        Node y = nullptr;
        while (x != nullptr)
        {
            y = x;
            if (value == x->elem)
                break;
            if (value < x->elem)
                x = x->left;
            else
                x = x->right;
        }
        if (x != nullptr && value == x->elem)
            return;
        ++num;
        z->p = y;
        if (y == nullptr)
            root = z;
        else if (value < y->elem)
            y->left = z;
        else
            y->right = z;
        rb_tree_insert_fixup(z);
    }
    template <typename elem_type>
    void rb_tree<elem_type>::rb_tree_insert_fixup(Node z)
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
    template <typename elem_type>
    void rb_tree<elem_type>::erase(const elem_type &value)
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
    template <typename elem_type>
    void rb_tree<elem_type>::rb_tree_erase_fixup(Node x, Node p)
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
