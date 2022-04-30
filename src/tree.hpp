#ifndef tree_hpp
#define tree_hpp

#include <iostream>
#include <queue>

namespace adt
{
    template<typename T>
    class tree
    {
    public:

        tree(T t_root)
        {
            m_data = t_root;
        }

        ~tree()
        {
            if (hasleft())
            {
                delete m_left;
            }

            if (hasright())
            {
                delete m_right;
            }
        }

        bool operator==(tree<T>* other)
        {
            return (m_data == other->m_data);
        }

        tree<T>* pushleft(T t_data)
        {
            m_left = new tree<T>(t_data);
            return m_left;
        }

        tree<T>* pushleft(tree* t_tree)
        {
            m_left = t_tree;
            return t_tree;
        }

        tree<T>* pushright(T t_data)
        {
            m_right = new tree<T>(t_data);
            return m_right;
        }

        tree<T>* pushright(tree* t_tree)
        {
            m_right = t_tree;
            return t_tree;
        }

        bool hasleft()
        {
            return (m_left != nullptr);
        }

        bool hasright()
        {
            return (m_right != nullptr);
        }

        std::string preorder(tree* t_node = nullptr)
        {
            if (t_node == nullptr)
            {
                t_node = this;
            }

            std::string order = t_node->m_data;

            if (t_node->hasleft())
            {
                order += t_node->preorder(t_node->m_left);
            }

            if (t_node->hasright())
            {
                order += t_node->preorder(t_node->m_right);
            }

            return order;
        }

        std::string postorder(tree* t_node = nullptr)
        {
            if (t_node == nullptr)
            {
                t_node = this;
            }

            std::string order = "";

            if (t_node->hasleft())
            {
                order += t_node->postorder(t_node->m_left);
            }

            if (t_node->hasright())
            {
                order += t_node->postorder(t_node->m_right);
            }

            order += t_node->m_data;

            return order;
        }

        std::string inorder(tree* t_node = nullptr)
        {
            if (t_node == nullptr)
            {
                t_node = this;
            }

            std::string order = "";

            if (t_node->hasleft())
            {
                order += t_node->inorder(t_node->m_left);
            }

            order += t_node->m_data;

            if (t_node->hasright())
            {
                order += t_node->inorder(t_node->m_right);
            }


            return order;
        }

        tree<T>* find(T t_data)
        {
            std::queue<tree<T>*> Q;
            Q.push(this);

            while (!Q.empty())
            {
                tree<T>* v = Q.front();
                Q.pop();
                if (v->m_data == t_data)
                {
                    return v;
                }

                if (v->hasleft())
                {
                    Q.push(v->m_left);
                }

                if (v->hasright())
                {
                    Q.push(v->m_right);
                }
            }

            return nullptr;
        }

        bool has(T t_in)
        {
            return (find(t_in) != nullptr);
        }

        tree<T>* left()
        {
            return m_left;
        }

        tree<T>* right()
        {
            return m_right;
        }

        T data()
        {
            return m_data;
        }

    private:

        T m_data;
        tree* m_parent = nullptr;
        tree* m_left = nullptr;
        tree* m_right = nullptr;
    };
}

#endif