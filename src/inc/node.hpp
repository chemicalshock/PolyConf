// -------------------------------------------------------------
//
//!\file node.hpp
//!\brief Normalised configuration node for Polyconf
//!\author Colin J.D. Stewart
//
// -------------------------------------------------------------

#ifndef POLYCONF_NODE_HPP
#define POLYCONF_NODE_HPP

// inbuilt
#include <cstddef>
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace POLYCONF
{
    class NODE final
    {
    private:
        bool m_has_value = false;
        std::string m_value;
        std::map<std::string, std::vector<NODE>> m_children;
        std::vector<std::pair<std::string, std::size_t>> m_child_order;

    public:
        //!\brief Default constructor
        NODE() = default;

        //!\brief Construct node with scalar value
        explicit NODE(const std::string& value)
            : m_has_value(true),
              m_value(value)
        {
        }

        //!\brief Construct node with scalar value (move)
        explicit NODE(std::string&& value)
            : m_has_value(true),
              m_value(std::move(value))
        {
        }

        //!\brief Set scalar value
        void set_value(const std::string& value)
        {
            m_value = value;
            m_has_value = true;
        }

        //!\brief Set scalar value (move)
        void set_value(std::string&& value)
        {
            m_value = std::move(value);
            m_has_value = true;
        }

        //!\brief Get scalar value
        const std::string& value() const
        {
            return m_value;
        }

        //!\brief Check whether node has a scalar value
        bool has_value() const
        {
            return m_has_value;
        }

        //!\brief Add named child node
        void add_child(const std::string& name, const NODE& child)
        {
            std::vector<NODE>& children = m_children[name];

            children.push_back(child);
            m_child_order.push_back(std::make_pair(name, children.size() - 1));
        }

        //!\brief Add named child node (move)
        void add_child(const std::string& name, NODE&& child)
        {
            std::vector<NODE>& children = m_children[name];

            children.push_back(std::move(child));
            m_child_order.push_back(std::make_pair(name, children.size() - 1));
        }

        //!\brief Append named child node and return it
        NODE& append_child(const std::string& name)
        {
            std::vector<NODE>& children = m_children[name];

            children.push_back(NODE());
            m_child_order.push_back(std::make_pair(name, children.size() - 1));

            return children.back();
        }

        //!\brief Check whether named children exist
        bool has_child(const std::string& name) const
        {
            return m_children.find(name) != m_children.end();
        }

        //!\brief Get named child list
        std::vector<NODE>* get_children(const std::string& name)
        {
            std::map<std::string, std::vector<NODE>>::iterator it = m_children.find(name);

            if (it == m_children.end())
            {
                return nullptr;
            }

            return &it->second;
        }

        //!\brief Get named child list
        const std::vector<NODE>* get_children(const std::string& name) const
        {
            std::map<std::string, std::vector<NODE>>::const_iterator it = m_children.find(name);

            if (it == m_children.end())
            {
                return nullptr;
            }

            return &it->second;
        }

        //!\brief Get named child by index
        NODE* get_child(const std::string& name, std::size_t index = 0)
        {
            std::vector<NODE>* children = get_children(name);

            if (children == nullptr)
            {
                return nullptr;
            }

            if (index >= children->size())
            {
                return nullptr;
            }

            return &((*children)[index]);
        }

        //!\brief Get named child by index
        const NODE* get_child(const std::string& name, std::size_t index = 0) const
        {
            const std::vector<NODE>* children = get_children(name);

            if (children == nullptr)
            {
                return nullptr;
            }

            if (index >= children->size())
            {
                return nullptr;
            }

            return &((*children)[index]);
        }

        //!\brief Get last named child
        NODE* get_last_child(const std::string& name)
        {
            std::vector<NODE>* children = get_children(name);

            if (children == nullptr || children->empty())
            {
                return nullptr;
            }

            return &children->back();
        }

        //!\brief Get last named child
        const NODE* get_last_child(const std::string& name) const
        {
            const std::vector<NODE>* children = get_children(name);

            if (children == nullptr || children->empty())
            {
                return nullptr;
            }

            return &children->back();
        }

        //!\brief Get or create last named child
        NODE& get_or_create_last_child(const std::string& name)
        {
            NODE* child = get_last_child(name);

            if (child != nullptr)
            {
                return *child;
            }

            return append_child(name);
        }

        //!\brief Access all children
        const std::map<std::string, std::vector<NODE>>& children() const
        {
            return m_children;
        }

        //!\brief Access children in insertion order
        const std::vector<std::pair<std::string, std::size_t>>& child_order() const
        {
            return m_child_order;
        }
    };
}

#endif // POLYCONF_NODE_HPP
