#include "../../include/blankdb/utils/Serialization.hpp"

namespace btree {
template <typename KeyType, typename ValueType>
void serialize(const BTreeNode<KeyType, ValueType>& node, nlohmann::json& json_node) {
    json_node["is_leaf"] = node.is_leaf();
    json_node["keys"] = node.keys_;
    json_node["values"] = node.values_;

    if (!node.is_leaf()) {
        json_node["children"] = nlohmann::json::array();
        for (const auto& child : node.children_) {
            nlohmann::json child_json;
            serialize(*child, child_json);
            json_node["children"].push_back(child_json);
        }
    }
}

template <typename KeyType, typename ValueType>
std::shared_ptr<BTreeNode<KeyType, ValueType>> deserialize(const nlohmann::json& json_node, size_t t) {
    auto node = std::make_shared<BTreeNode<KeyType, ValueType>>(json_node["is_leaf"], t);
    node->keys_ = json_node["keys"];
    node->values_ = json_node["values"];

    if (!json_node["is_leaf"]) {
        for (const auto& child_json : json_node["children"]) {
            node->children_.push_back(deserialize(child_json, t));
        }
    }

    return node;
}
} // namespace btree