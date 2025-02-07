#ifndef BTREE_BTREE_NODE_HPP
#define BTREE_BTREE_NODE_HPP

#include <vector>
#include <memory>

namespace btree {
template <typename KeyType, typename ValueType>
class BTreeNode {
public:
    using NodePtr = std::shared_ptr<BTreeNode<KeyType, ValueType>>;

    BTreeNode(bool is_leaf, size_t t)
        : is_leaf_(is_leaf), min_degree_(t) {}

    // Вставка ключа и значения в узел
    void insert(const KeyType& key, const ValueType& value);

    // Разделение узла пополам
    void split_child(size_t index, NodePtr child);

    // Поиск значения по ключу
    ValueType search(const KeyType& key) const;

    // Удаление ключа из узла
    void remove(const KeyType& key);

    // Получение ссылки на дочерний узел
    NodePtr get_child(size_t index) const { return children_[index]; }

    // Проверка, является ли узел листовым
    bool is_leaf() const { return is_leaf_; }

    // Проверка, заполнен ли узел
    bool is_full() const { return keys_.size() >= 2 * min_degree_ - 1; }

private:
    bool is_leaf_;
    size_t min_degree_;

    std::vector<KeyType> keys_;          // Хранимые ключи
    std::vector<ValueType> values_;      // Соответствующие значения
    std::vector<NodePtr> children_;      // Дочерние узлы (пусты для листьев)

    // Вспомогательные методы
    size_t find_key_index(const KeyType& key) const;
    void merge_children(size_t index);
};
} // namespace btree

#endif