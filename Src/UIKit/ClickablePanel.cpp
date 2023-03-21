#include "Common/Precompile.h"

#include "UIKit/ClickablePanel.h"

namespace d14engine::uikit
{
    void ClickablePanel::onMouseButtonPress(Event& e)
    {
        onMouseButtonPressHelper(e);

        if (f_onMouseButtonPress) f_onMouseButtonPress(this, e);
    }

    void ClickablePanel::onMouseButtonRelease(Event& e)
    {
        onMouseButtonReleaseHelper(e);

        if (f_onMouseButtonRelease) f_onMouseButtonRelease(this, e);
    }

    void ClickablePanel::onMouseButtonPressHelper(Event& e)
    {
        // This method intentionally left blank.
    }

    void ClickablePanel::onMouseButtonReleaseHelper(Event& e)
    {
        // This method intentionally left blank.
    }

    void ClickablePanel::onMouseLeaveHelper(MouseMoveEvent& e)
    {
        Panel::onMouseLeaveHelper(e);

        onMouseLeaveWrapper(e);
    }

    void ClickablePanel::onMouseLeaveWrapper(MouseMoveEvent& e)
    {
        m_hasLeftPressed = m_hasRightPressed = m_hasMiddlePressed = false;
    }

    void ClickablePanel::onMouseButtonHelper(MouseButtonEvent& e)
    {
        Panel::onMouseButtonHelper(e);

        onMouseButtonWrapper(e);
    }

    void ClickablePanel::onMouseButtonWrapper(MouseButtonEvent& e)
    {
        Event be = {};
        be.cursorPoint = e.cursorPoint;

        bool leftDown = e.state.leftDown() || (!enableDoubleClick && e.state.leftDblclk());
        bool rightDown = e.state.rightDown() || (!enableDoubleClick && e.state.rightDblclk());
        bool middleDown = e.state.middleDown() || (!enableDoubleClick && e.state.middleDblclk());

        if (leftDown || rightDown || middleDown)
        {
            if (leftDown)
            {
                m_hasLeftPressed = true;
                be.flag = Event::Flag::Left;
            }
            else if (rightDown)
            {
                m_hasRightPressed = true;
                be.flag = Event::Flag::Right;
            }
            else if (middleDown)
            {
                m_hasMiddlePressed = true;
                be.flag = Event::Flag::Middle;
            }
            else be.flag = Event::Flag::Unknown;

            onMouseButtonPress(be);
        }
        else if (m_hasLeftPressed || m_hasRightPressed || m_hasMiddlePressed)
        {
            if (e.state.leftUp() || e.state.rightUp() || e.state.middleUp())
            {
                if (e.state.leftUp())
                {
                    m_hasLeftPressed = false;
                    be.flag = Event::Flag::Left;
                }
                else if (e.state.rightUp())
                {
                    m_hasRightPressed = false;
                    be.flag = Event::Flag::Right;
                }
                else if (e.state.middleUp())
                {
                    m_hasMiddlePressed = false;
                    be.flag = Event::Flag::Middle;
                }
                else be.flag = Event::Flag::Unknown;

                onMouseButtonRelease(be);
            }
        }
    }

    void ClickablePanel::setEnabled(bool value)
    {
        Panel::setEnabled(value);

        setEnabledWrapper(value);
    }

    void ClickablePanel::setEnabledWrapper(bool value)
    {
        m_hasLeftPressed = m_hasRightPressed = m_hasMiddlePressed = false;
    }
}
