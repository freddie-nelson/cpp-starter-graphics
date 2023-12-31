#pragma once

#include "AABB.h"
#include <stdexcept>

namespace Core
{
    /**
     * Represents a node in an AABB tree.
     */
    template <typename T>
    struct AABBTreeNode
    {
        AABBTreeNode *parent;
        AABBTreeNode *left;
        AABBTreeNode *right;

        T id;
        const AABB *aabb;
        AABB fatAabb;

        /**
         * Gets whether or not this node is the root of the tree.
         *
         * @returns Whether or not this node is the root of the tree.
         */
        bool isRoot()
        {
            return parent == nullptr;
        }

        /**
         * Gets whether or not this node is a leaf node.
         *
         * @returns Whether or not this node is a leaf node.
         */
        bool isLeaf()
        {
            return left == nullptr && right == nullptr;
        }

        /**
         * Gets whether or not this node is the left child of its parent.
         *
         * @returns Whether or not this node is the left child of its parent.
         */
        bool isLeftChild()
        {
            return !isRoot() && parent->left == this;
        }

        /**
         * Gets whether or not this node is the right child of its parent.
         *
         * @returns Whether or not this node is the right child of its parent.
         */
        bool isRightChild()
        {
            return !isRoot() && parent->right == this;
        }

        /**
         * Gets the sibling of this node.
         *
         * The sibling may be null.
         *
         * @returns The sibling of this node.
         */
        AABBTreeNode<T> *getSibling()
        {
            if (isRoot())
            {
                return nullptr;
            }

            return isLeftChild() ? parent->right : parent->left;
        }

        /**
         * Gets whether or not this node has a sibling.
         *
         * @returns Whether or not this node has a sibling.
         */
        bool hasSibling()
        {
            return getSibling() != nullptr;
        }
    };
}