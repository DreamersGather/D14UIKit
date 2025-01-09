#pragma once

#include "StatefulObject.h"
#include "ViewItem.h"

_D14_UIKIT_FWDEF(TreeViewItem)

namespace d14uikit
{
    class DllExport TreeViewItem : public ViewItem
    {
        friend class TreeView;

        _D14_UIKIT_PIMPL(TreeViewItem)

        explicit TreeViewItem(const std::wstring& text = L"TreeViewItem");

        int nodeLevel() const;

        void insertItem(const std::list<TreeViewItem*>& items, int index = 0);
        void appendItem(const std::list<TreeViewItem*>& items);

        void removeItem(int index, int count = 1);
        void clearAllItems();

        int itemCount() const;

        enum class State
        {
            Folded, Unfolded
        };
        constexpr static auto Folded = State::Folded;
        constexpr static auto Unfolded = State::Unfolded;

        _D14_UIKIT_STATEFUL_OBJECT_DECL(TreeViewItem)
    };
}
