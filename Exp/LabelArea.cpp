#include "Common/Precompile.h"

#include "LabelArea.h"

#include "Label.h"
#include "Panel.h"

#include "UIKit/LabelArea.h"

using namespace d14engine;

namespace d14uikit
{
    LabelArea::LabelArea(const std::wstring& text)
        :
        LabelArea(Passkey{})
    {
        Panel::pimpl->uiobj =
        Label::pimpl->uiobj =
        LabelArea::pimpl->uiobj =
        uikit::makeUIObject<uikit::LabelArea>(text);

        Panel::initialize();
        Label::initialize();
        LabelArea::initialize();
    }

    LabelArea::LabelArea(Passkey)
        :
        Label(Label::Passkey{}),
        pimpl(std::make_shared<Impl>()) { }

    void LabelArea::initialize() { }

    int LabelArea::caretOffset() const
    {
        return (int)pimpl->uiobj->indicatorPosition();
    }

    void LabelArea::setCaretOffset(int value)
    {
        pimpl->uiobj->setIndicatorPosition(value);
    }

    Range LabelArea::hiliteRange() const
    {
        auto& range = pimpl->uiobj->hiliteRange();
        return { (int)range.offset, (int)range.count };
    }

    void LabelArea::setHiliteRange(const Range& value)
    {
        pimpl->uiobj->setHiliteRange(
        {
            (size_t)value.offset, (size_t)value.count
        });
    }

    void LabelArea::performCommandCtrlA()
    {
        pimpl->uiobj->performCommandCtrlA();
    }

    void LabelArea::performCommandCtrlC()
    {
        pimpl->uiobj->performCommandCtrlC();
    }
}
