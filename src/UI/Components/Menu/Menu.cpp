#include "UI/Components/Menu/Menu.hpp"

void Menu::Clear()
{
    m_items.clear();
    m_selected = 0;
}

void Menu::Add(const MenuItem &item)
{
    m_items.push_back(item);
}

void Menu::MoveUp()
{
    if (m_selected > 0)
    {
        --m_selected;
    }
    else
    {
        m_selected = m_items.size() - 1;
    }
}

void Menu::MoveDown()
{
    if (m_selected + 1 < m_items.size())
    {
        ++m_selected;
    }
    else
    {
        m_selected = 0;
    }
}

size_t Menu::GetSelectedIndex() const
{
    return m_selected;
}

MenuItem &Menu::Selected()
{
    return m_items[m_selected];
}

const MenuItem &Menu::Item(size_t index) const
{
    return m_items[index];
}

MenuItem &Menu::Item(size_t index)
{
    return m_items[index];
}

size_t Menu::Count() const
{
    return m_items.size();
}

bool Menu::IsSelected(size_t index) const
{
    return index == m_selected;
}