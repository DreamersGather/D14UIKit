#include "Common/Precompile.h"

#include "LabelArea.h"

#include "Common.h"

#include "UIKit/LabelArea.h"

using namespace d14engine;

namespace d14uikit
{
    LabelArea::LabelArea(const std::wstring& text)
        :
        LabelArea(uikit::makeUIObject<uikit::LabelArea>(text)) { }

    _D14_UIKIT_CTOR(LabelArea)
        :
        Label(uiobj),
        pimpl(std::make_shared<Impl>())
    {
        pimpl->uiobj = uiobj;
    }

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
