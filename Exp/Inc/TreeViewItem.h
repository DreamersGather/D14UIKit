#pragma once

#include "ViewItem.h"

namespace d14uikit
{
    class DllExport TreeViewItem : public ViewItem
    {
        _D14_UIKIT_PIMPL(TreeViewItem)

        explicit TreeViewItem(const std::wstring& text = L"ViewItem");

        int nodeLevel() const;

        void insertItem(const std::list<TreeViewItem*>& items, int index = 0);
        void appendItem(const std::list<TreeViewItem*>& items);

        void removeItem(int index, int count = 1);
        void clearAllItems();

        enum class State
        {
            Folded, Unfolded
        };
        constexpr static auto Folded = State::Folded;
        constexpr static auto Unfolded = State::Unfolded;

        State state() const;
        void setState(State state);

        struct Callback
        {
            std::function<void(TreeViewItem*, State)> onStateChange = {};
        };
        Callback& callback() const;

    protected:
        std::unique_ptr<Callback> pcallback = {};

        virtual void onStateChange(State state);
    };
}
