#pragma once

#include <vector>

#include "MenuItem.hpp"

class Menu
{
public:
    void Clear();

    void Add(const MenuItem &item);

    void MoveUp();

    void MoveDown();

    size_t GetSelectedIndex() const;

    MenuItem &Selected();

    const MenuItem &Item(size_t index) const;

    MenuItem &Item(size_t index);

    size_t Count() const;

    bool IsSelected(size_t index) const;

    const std::vector<MenuItem> &Items() const;

private:
    std::vector<MenuItem> m_items;

    size_t m_selected = 0;
};