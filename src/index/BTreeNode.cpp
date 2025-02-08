#include "../../include/blankdb/index/BTreeNode.hpp"
#include <algorithm>
#include <stdexcept>

namespace btree {
template <typename KeyType, typename ValueType>
void BTreeNode<KeyType, ValueType>::insert(const KeyType& key, const ValueType& value) {
    if (is_leaf_) {
        // Если узел является листовым, просто добавляем ключ и значение
        auto index = find_key_index(key);
        keys_.insert(keys_.begin() + index, key);
        values_.insert(values_.begin() + index, value);
    } else {
        // Если узел не является листовым, ищем подходящий дочерний узел
        size_t index = find_key_index(key);
        NodePtr child = children_[index];
        if (child->is_full()) {
            // Если дочерний узел заполнен, разделяем его
            split_child(index, child);
            if (key > keys_[index]) {
                ++index;
            }
        }
        children_[index]->insert(key, value);
    }
}

template <typename KeyType, typename ValueType>
void BTreeNode<KeyType, ValueType>::split_child(size_t index, NodePtr child) {
    // Создаем новый узел для разделения
    NodePtr new_child = std::make_shared<BTreeNode<KeyType, ValueType>>(child->is_leaf_, min_degree_);
    new_child->keys_ = std::vector<KeyType>(child->keys_.begin() + min_degree_, child->keys_.end());
    new_child->values_ = std::vector<ValueType>(child->values_.begin() + min_degree_, child->values_.end());
    if (!child->is_leaf_) {
        new_child->children_ = std::vector<NodePtr>(child->children_.begin() + min_degree_, child->children_.end());
    }

    // Обновляем текущий узел
    child->keys_.erase(child->keys_.begin() + min_degree_, child->keys_.end());
    child->values_.erase(child->values_.begin() + min_degree_, child->values_.end());
    if (!child->is_leaf_) {
        child->children_.erase(child->children_.begin() + min_degree_, child->children_.end());
    }

    // Добавляем средний ключ в текущий узел
    keys_.insert(keys_.begin() + index, child->keys_.back());
    values_.insert(values_.begin() + index, child->values_.back());
    child->keys_.pop_back();
    child->values_.pop_back();

    // Добавляем новый узел в список дочерних
    children_.insert(children_.begin() + index + 1, new_child);
}

template <typename KeyType, typename ValueType>
ValueType BTreeNode<KeyType, ValueType>::search(const KeyType& key) const {
    size_t index = find_key_index(key);
    if (index < keys_.size() && keys_[index] == key) {
        return values_[index];
    }
    if (is_leaf_) {
        throw std::runtime_error("Key not found");
    }
    return children_[index]->search(key);
}

template <typename KeyType, typename ValueType>
void BTreeNode<KeyType, ValueType>::remove(const KeyType& key) {
    size_t index = find_key_index(key);
    if (index < keys_.size() && keys_[index] == key) {
        if (is_leaf_) {
            // Если узел является листовым, удаляем ключ
            keys_.erase(keys_.begin() + index);
            values_.erase(values_.begin() + index);
        } else {
            // Если узел не является листовым, заменяем ключ предшествующим или последующим
            NodePtr predecessor = children_[index];
            while (!predecessor->is_leaf()) {
                predecessor = predecessor->children_.back();
            }
            keys_[index] = predecessor->keys_.back();
            values_[index] = predecessor->values_.back();
            predecessor->remove(predecessor->keys_.back());
        }
    } else {
        if (is_leaf_) {
            throw std::runtime_error("Key not found");
        }
        NodePtr child = children_[index];
        if (child->keys_.size() == min_degree_ - 1) {
            if (index > 0 && children_[index - 1]->keys_.size() >= min_degree_) {
                // Заимствуем ключ из предыдущего брата
                child->keys_.insert(child->keys_.begin(), keys_[index - 1]);
                child->values_.insert(child->values_.begin(), values_[index - 1]);
                keys_[index - 1] = children_[index - 1]->keys_.back();
                values_[index - 1] = children_[index - 1]->values_.back();
                children_[index - 1]->keys_.pop_back();
                children_[index - 1]->values_.pop_back();
            } else if (index < children_.size() - 1 && children_[index + 1]->keys_.size() >= min_degree_) {
                // Заимствуем ключ из следующего брата
                child->keys_.push_back(keys_[index]);
                child->values_.push_back(values_[index]);
                keys_[index] = children_[index + 1]->keys_.front();
                values_[index] = children_[index + 1]->values_.front();
                children_[index + 1]->keys_.erase(children_[index + 1]->keys_.begin());
                children_[index + 1]->values_.erase(children_[index + 1]->values_.begin());
            } else {
                // Объединяем узлы
                if (index > 0) {
                    merge_children(index - 1);
                    --index;
                } else {
                    merge_children(index);
                }
                child = children_[index];
            }
        }
        child->remove(key);
    }
}

template <typename KeyType, typename ValueType>
size_t BTreeNode<KeyType, ValueType>::find_key_index(const KeyType& key) const {
    return std::distance(keys_.cbegin(), std::lower_bound(keys_.cbegin(), keys_.cend(), key));
}

template <typename KeyType, typename ValueType>
void BTreeNode<KeyType, ValueType>::merge_children(size_t index) {
    NodePtr left = children_[index];
    NodePtr right = children_[index + 1];

    left->keys_.push_back(keys_[index]);
    left->values_.push_back(values_[index]);
    left->keys_.insert(left->keys_.end(), right->keys_.begin(), right->keys_.end());
    left->values_.insert(left->values_.end(), right->values_.begin(), right->values_.end());
    if (!left->is_leaf_) {
        left->children_.insert(left->children_.end(), right->children_.begin(), right->children_.end());
    }

    keys_.erase(keys_.begin() + index);
    values_.erase(values_.begin() + index);
    children_.erase(children_.begin() + index + 1);
}
} // namespace btree