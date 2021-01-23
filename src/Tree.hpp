#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <type_traits>
#include <vector>

#include "ErrorReporter.hpp"

/**
 * @brief The namespace in which all the projects under Shroon exist.
 */
namespace Shroon
{
    /**
     * @brief The namespace in which all the ShroonStructure API exists.
     */
    namespace Structure
    {
        /**
         * @defgroup DataStructures
         * This group contains all the data structures in this library.
         * @{
         */

        /**
         * @brief It defines one node in a tree of nodes.
         *
         * @tparam ValueType The type of value this node stores.
         */
        template <typename ValueType>
        class TreeNode
        {
        public:
            /**
             * @brief Constructs a \p TreeNode<ValueType> containing \p value as a child of \p parent.
             *
             * @param value The value to store in this node.
             * @param parent Pointer to parent of this node. \p nullptr is default and means this is
             * root node.
             */
            TreeNode(const ValueType & value, TreeNode<ValueType> * parent = nullptr)
                : m_Base(value), m_Parent(parent)
            {
            }

            /**
             * @brief Constructs a \p TreeNode<ValueType> containing \p value as a child of \p parent.
             *
             * @param value The value to store in this node.
             * @param parent Pointer to parent of this node. \p nullptr is default and means this is
             * root node.
             */
            TreeNode(ValueType && value, TreeNode<ValueType> * parent = nullptr)
                : m_Base(std::move(value)), m_Parent(parent)
            {
            }

            /**
             * @brief Constructs a \p TreeNode<ValueType> containing value which is constructed using
             * \p cargs as a child of \p parent.
             *
             * @tparam CArgs Parameter pack of types of the args of the constructor of \p ValueType.
             *
             * @param parent Pointer to parent of this node. \p nullptr means this is root node.
             * @param cargs Parameter pack containing args for the constructor of \p ValueType.
             */
            template <typename... CArgs>
            TreeNode(TreeNode<ValueType> * parent, CArgs &&... cargs)
                : m_Base(std::forward<CArgs>(cargs)...), m_Parent(parent)
            {
            }

            /**
             * @brief Returns an l-value reference to the stored value in this node.
             *
             * @return An l-value reference to the stored value.
             */
            inline ValueType & Get()
            {
                return m_Base;
            }

            /**
             * @brief Returns a const l-value reference to the stored value in this node.
             *
             * @return A const l-value reference to the stored value.
             */
            inline const ValueType & Get() const
            {
                return m_Base;
            }

            /**
             * @brief Returns a pointer to the parent node.
             *
             * @return A pointer to the parent node. Returns \p nullptr if it is the root node.
             */
            inline TreeNode<ValueType> * GetParent()
            {
                return m_Parent;
            }

            /**
             * @brief Returns a const pointer to the parent node.
             *
             * @return A const pointer to the parent node. Returns \p nullptr if it is the root node.
             */
            inline const TreeNode<ValueType> * GetParent() const
            {
                return m_Parent;
            }

            /**
             * @brief Returns the number of children this node has.
             *
             * @return Number of children this node has.
             */
            inline size_t ChildrenCount() const
            {
                return m_Children.size();
            }

            /**
             * @brief Returns the \p index th child of this node.
             *
             * @param index Index of the child to return. \p 0 by default.
             *
             * @return An l-value reference to the \p index th child of this node.
             */
            TreeNode<ValueType> & GetChild(size_t index = 0)
            {
                if (index >= m_Children.size())
                {
                    ErrorReporter("Invalid child index " + std::to_string(index) + " in TreeNode.", true);
                }
                
                return *m_Children[index];
            }

            /**
             * @brief Returns the \p index th child of this node.
             *
             * @param index Index of the child to return. \p 0 by default.
             *
             * @return A const l-value reference to the \p index th child of this node.
             */
            const TreeNode<ValueType> & GetChild(size_t index = 0) const
            {
                if (index >= m_Children.size())
                {
                    ErrorReporter("Invalid child index " + std::to_string(index) + " in TreeNode.", true);
                }
                
                return *m_Children[index];
            }

            /**
             * @brief Adds a child node which contains \p value, to this node.
             *
             * @param value The value which the sibling node will contain.
             *
             * @return The child node which just got added.
             */
            TreeNode<ValueType> & AddChild(const ValueType & value)
            {
                return *m_Children.emplace_back(new TreeNode<ValueType>(value), this);
            }

            /**
             * @brief Adds a child node which contains \p value, to this node.
             *
             * @param value The value which the sibling node will contain.
             *
             * @return The child node which just got added.
             */
            TreeNode<ValueType> & AddChild(ValueType && value)
            {
                return *m_Children.emplace_back(new TreeNode<ValueType>(std::move(value)), this);
            }

            /**
             * @brief Adds a child node which contains value constructed using \p cargs, to this node.
             *
             * @tparam CArgs Parameter pack of types of the args of the constructor of \p ValueType.
             *
             * @param cargs Parameter pack containing args for constructor of \p ValueType which will
             * be stored in the child node.
             *
             * @return The child node which just got added.
             */
            template <typename... CArgs>
            TreeNode<ValueType> & AddChild(CArgs &&... cargs)
            {
                return *m_Children.emplace_back(new TreeNode<ValueType>(this, cargs...));
            }

            /**
             * @brief Adds a sibling node (node of same parent) which contains \p value, to this node.
             *
             * @param value The value which the sibling node will contain.
             *
             * @return The sibling node which just got added.
             */
            TreeNode<ValueType> * AddSibling(const ValueType & value)
            {
                if (!m_Parent)
                {
                    return nullptr;
                }

                return &m_Parent->AddChild(value);
            }

            /**
             * @brief Adds a sibling node (node of same parent) which contains \p value, to this node.
             *
             * @param value The value which the sibling node will contain.
             *
             * @return The sibling node which just got added.
             */
            TreeNode<ValueType> * AddSibling(ValueType && value)
            {
                if (!m_Parent)
                {
                    return nullptr;
                }

                return &m_Parent->AddChild(std::move(value));
            }

            /**
             * @brief Adds a sibling node (node with same parent) which contains value constructed
             * using \p cargs, to this node.
             *
             * @tparam CArgs Parameter pack of types of the args of the constructor of \p ValueType.
             *
             * @param cargs Parameter pack containing args for constructor of \p ValueType which will
             * be stored in sibling node.
             *
             * @return The sibling node which just got added.
             */
            template <typename... CArgs>
            TreeNode<ValueType> * AddSibling(CArgs &&... cargs)
            {
                if (!m_Parent)
                {
                    return nullptr;
                }

                return &m_Parent->AddChild(std::forward<CArgs>(cargs)...);
            }

            /**
             * @brief Returns an l-value reference to the stored value in this node.
             *
             * @return An l-value reference to the stored value.
             */
            inline ValueType & operator*()
            {
                return m_Base;
            }

            /**
             * @brief Returns a const l-value reference to the stored value in this node.
             *
             * @return A const l-value reference to the stored value.
             */
            inline const ValueType & operator*() const
            {
                return m_Base;
            }

            /**
             * @brief Returns the \p index th child of this node.
             *
             * @param index Index of the child to return. \p 0 by default.
             *
             * @return An l-value reference to the \p index th child of this node.
             */
            TreeNode<ValueType> & operator[](size_t index)
            {
                return *m_Children[index];
            }

            /**
             * @brief Returns the \p index th child of this node.
             *
             * @param index Index of the child to return. \p 0 by default.
             *
             * @return A const l-value reference to the \p index th child of this node.
             */
            const TreeNode<ValueType> & operator[](size_t index) const
            {
                return *m_Children[index];
            }

        private:
            ValueType m_Base;

            TreeNode<ValueType> * m_Parent;
            std::vector<std::unique_ptr<TreeNode<ValueType>>> m_Children;
        };

        /**
         * @brief It defines a tree data structure. It only consists of the root node, other nodes
         * are accessed using the interface of \p TreeNode class.
         *
         * @tparam ValueType The type of value, nodes of this tree store.
         */
        template <typename ValueType>
        class Tree
        {
        public:
            /**
             * @brief Constructs a \p Tree<ValueType> with root node containing \p value.
             *
             * @param value The value to store in root node.
             */
            Tree(const ValueType & value)
                : m_Root(value)
            {
            }

            /**
             * @brief Constructs a \p Tree<ValueType> with root node containing \p value.
             *
             * @param value The value to store in root node.
             */
            Tree(ValueType && value)
                : m_Root(std::move(value))
            {
            }

            /**
             * @brief Constructs a \p Tree<ValueType> with root node containing value which is
             * constructed using \p cargs.
             *
             * @tparam CArgs Parameter pack of types of the args of the constructor of \p ValueType.
             *
             * @param cargs Parameter pack containing args for the constructor of \p ValueType.
             */
            template <typename... CArgs>
            Tree(CArgs &&... cargs)
                : m_Root(std::forward<CArgs>(cargs)...)
            {
            }

            /**
             * @brief Returns an l-value reference to the root node.
             *
             * @return An l-value reference to the root node.
             */
            inline TreeNode<ValueType> & GetRoot()
            {
                return m_Root;
            }

            /**
             * @brief Returns a const l-value reference to the root node.
             *
             * @return A const l-value reference to the root node.
             */
            inline const TreeNode<ValueType> & GetRoot() const
            {
                return m_Root;
            }

            /**
             * @brief Returns an array of pointer to all the nodes at a certain height in the tree.
             *
             * @param height The height from which nodes are returned.
             *
             * @return An \p std::vector of pointer to all the nodes at a certain height in the tree.
             * An array of size 1 means no nodes are on that height. The array also has some
             * \p nullptr elements which separate children of on node from children of other nodes.
             *
             * Example - In the following tree:
             *
             * ```
             *     A
             *    / \
             *   B   C
             *  /\   /\
             * D  E F  G
             * ```
             *
             * If we retrieve nodes at height 2 we expect <tt>[D, E, F, G]</tt> but we get
             * <tt>[nullptr, D, E, nullptr, F, G]</tt>. The \p nullptr means the node afterwards are
             * children of the same parent and when another \p nullptr comes it means that the parent
             * of the following nodes are different than the previous one.
             * 
             * In this case:
             * * 0: \p nullptr - Following nodes are children of first parent node (B).
             * * 1: \p D - Child of one node (B).
             * * 2: \p E - Child of one node (B).  
             * * 3: \p nullptr - Following nodes are children of second parent node (C).
             * * 4: \p F - Child of another node (C).
             * * 5: \p G - Child of another node (C).
             */
            std::vector<TreeNode<ValueType> *> GetNodesAtHeight(size_t height)
            {
                std::vector<TreeNode<ValueType> *> nodes{nullptr};
            
                if (height == 0)
                {
                    nodes.emplace_back(&m_Root);
            
                    nodes.emplace_back(nullptr);
                }
                else
                {
                    auto parentNodes = std::move(GetNodesAtHeight(height - 1));
                    
                    for (const TreeNode<ValueType> * node : parentNodes)
                    {
                        if (!node)
                        {
                            continue;
                        }
            
                        for (size_t i = 0; i < node->ChildrenCount(); i++)
                        {
                            nodes.emplace_back(&node->GetChild(i));
                        }
            
                        if (node->ChildrenCount() != 0)
                        {
                            nodes.emplace_back(nullptr);
                        }
                    }
                }
            
                return nodes;
            }

            /**
             * @brief Returns an array of pointer to all the nodes at a certain height in the tree.
             *
             * @param height The height from which nodes are returned.
             *
             * @return An \p std::vector of pointer to all the nodes at a certain height in the tree.
             * An array of size 1 means no nodes are on that height. The array also has some
             * \p nullptr elements which separate children of on node from children of other nodes.
             *
             * Example - In the following tree:
             *
             * ```
             *     A
             *    / \
             *   B   C
             *  /\   /\
             * D  E F  G
             * ```
             *
             * If we retrieve nodes at height 2 we expect <tt>[D, E, F, G]</tt> but we get
             * <tt>[nullptr, D, E, nullptr, F, G]</tt>. The \p nullptr means the node afterwards are
             * children of the same parent and when another \p nullptr comes it means that the parent
             * of the following nodes are different than the previous one.
             * 
             * In this case:
             * * 0: \p nullptr - Following nodes are children of first parent node (B).
             * * 1: \p D - Child of one node (B).
             * * 2: \p E - Child of one node (B).  
             * * 3: \p nullptr - Following nodes are children of second parent node (C).
             * * 4: \p F - Child of another node (C).
             * * 5: \p G - Child of another node (C).
             */
            std::vector<const TreeNode<ValueType> *> GetNodesAtHeight(size_t height) const
            {
                std::vector<const TreeNode<ValueType> *> nodes{nullptr};
            
                if (height == 0)
                {
                    nodes.emplace_back(&m_Root);
            
                    nodes.emplace_back(nullptr);
                }
                else
                {
                    auto parentNodes = std::move(GetNodesAtHeight(height - 1));
                    
                    for (const TreeNode<ValueType> * node : parentNodes)
                    {
                        if (!node)
                        {
                            continue;
                        }
            
                        for (size_t i = 0; i < node->ChildrenCount(); i++)
                        {
                            nodes.emplace_back(&node->GetChild(i));
                        }
            
                        if (node->ChildrenCount() != 0)
                        {
                            nodes.emplace_back(nullptr);
                        }
                    }
                }
            
                return nodes;
            }

        private:
            TreeNode<ValueType> m_Root;
        };

        /**
         * @}
         */
    }
}

/**
 * @defgroup OutputStreamOperators
 * This group contains all output stream operators defined in this library for data structures.
 * @{
 */

/**
 * @brief Outputs the data inside \p node to \p out.
 *
 * @tparam ValueType The type of data which the node contains. Used for template parameters of \p TreeNode.
 *
 * @param out The <tt>std::ostream &</tt> to output the content of \p node to.
 * @param node The \p TreeNode<ValueType> whose content needs to be written to \p out.
 *
 * @return \p out after outputting content to it.
 */
template <typename ValueType>
std::ostream & operator<<(std::ostream & out, const Shroon::Structure::TreeNode<ValueType> & node)
{
    return out<<node.Get();
}

/**
 * @brief Outputs the \p tree to \p out.
 *
 * @tparam ValueType The type of data which the nodes of \p tree contain. Used for template parameters of \p Tree.
 *
 * @param out The <tt>std::ostream &</tt> to output the content of \p tree to.
 * @param tree The \p Tree<ValueType> whose content needs to be written to \p out.
 *
 * @return \p out after outputting content to it.
 *
 * Example - For the following tree,
 * 
 * ```
 *     A
 *    / \
 *   B   C
 *  /\   /\
 * D  E F  G
 * ```
 * 
 * the output will be:
 * 
 * ```
 *    [A]
 *   [B C]
 * [D E][F G]
 * ```
 * 
 * Here, nodes inside <tt>[]</tt> mean they have the same parent.
 */
template <typename ValueType>
std::ostream & operator<<(std::ostream & out, const Shroon::Structure::Tree<ValueType> & tree)
{
    std::vector<const Shroon::Structure::TreeNode<ValueType> *> nodes = std::move(tree.GetNodesAtHeight(0));

    std::vector<std::string> lines;

    for (size_t i = 1; nodes.size() > 1; i++)
    {
        std::string line;

        for (size_t i = 0; i < nodes.size(); i++)
        {
            if (i > 0)
            {
                line += " ";
            }

            if (!nodes[i])
            {
                if (i == 0)
                {
                    line += '[';
                }
                else if (i == nodes.size() - 1)
                {
                    line += ']';
                }
                else
                {
                    line += "][";
                }
            }
            else
            {
                std::stringstream ss;

                ss<<**nodes[i];

                line += ss.str();
            }
        }

        lines.emplace_back(std::move(line));

        nodes = std::move(tree.GetNodesAtHeight(i));
    }

    size_t largestSize = 0;

    for (size_t i = 0; i < lines.size(); i++)
    {
        if (lines[i].size() > largestSize)
        {
            largestSize = lines[i].size();
        }
    }

    for (auto & line : lines)
    {
        if (line.size() < largestSize)
        {
            size_t numTotalSpaces = largestSize - line.size();
            size_t numLeadingSpaces = 0, numTrailingSpaces = 0;

            if (numTotalSpaces % 2)
            {
                numLeadingSpaces = numTotalSpaces / 2;
                numTrailingSpaces = numTotalSpaces / 2;
            }
            else
            {
                numLeadingSpaces = (numTotalSpaces - 1) / 2;
                numTrailingSpaces = (numTotalSpaces + 1) / 2;
            }

            for (size_t i = 0; i < numLeadingSpaces; i++)
            {
                out<<' ';
            }

            out<<line;

            for (size_t i = 0; i < numTrailingSpaces; i++)
            {
                out<<' ';
            }

            out<<"\n";
        }
        else
        {
            out<<line<<"\n";
        }
    }

    return out;
}

/**
 * @}
 */
