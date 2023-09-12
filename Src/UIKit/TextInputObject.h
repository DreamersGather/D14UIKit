#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit
{
    struct CompositionForm { POINT origin = {}; LONG height = {}; };

    struct TextInputObject
    {
        virtual Optional<CompositionForm> getCompositionForm() const
        {
            return std::nullopt;
        }

        void OnInputString(WstrParam str)
        {
            onInputStringHelper(str);

            if (f_onInputString) f_onInputString(this, str);
        }

        Function<void(TextInputObject*, WstrParam)> f_onInputString = {};

        void onTextChange(WstrParam text)
        {
            onTextChangeHelper(text);

            if (f_onTextChange) f_onTextChange(this, text);
        }

        Function<void(TextInputObject*, WstrParam)> f_onTextChange = {};

    protected:
        virtual void onInputStringHelper(WstrParam str) { }
        virtual void onTextChangeHelper(WstrParam text) { }
    };
}
