#ifndef BTREE_BTREE_NODE_SERIALIZATION_HPP
#define BTREE_BTREE_NODE_SERIALIZATION_HPP

#include "../index/BTreeNode.hpp"
#include <nlohmann/json.hpp>

namespace btree {
template <typename KeyType, typename ValueType>
void serialize(const BTreeNode<KeyType, ValueType>& node, nlohmann::json& json_node);

template <typename KeyType, typename ValueType>
std::shared_ptr<BTreeNode<KeyType, ValueType>> deserialize(const nlohmann::json& json_node, size_t t);
}

#endif