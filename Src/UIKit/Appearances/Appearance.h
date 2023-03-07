#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::appearance
{
    void initialize();

#pragma region Interfaces

    struct Appearance
    {
        virtual void changeTheme(WstrParam themeName) = 0;
    };

#define _D14_SET_APPEARANCE_GETTER(Type_Name) \
    appearance::Type_Name::Appearance& getAppearance() \
    { \
        return appearance::Type_Name::appearance; \
    } \
    const appearance::Type_Name::Appearance& getAppearance() const \
    { \
        return appearance::Type_Name::appearance; \
    }

#define _D14_SET_THEME_STYLE_MAP_DECL \
    using ThemeStyleMap = std::unordered_map<Wstring, ThemeStyle>; \
    static ThemeStyleMap g_themeStyles

#define _D14_REF_THEME_STYLE_MAP_DECL(Type_Name) \
    using ThemeStyle = Type_Name::Appearance::ThemeStyle; \
    using ThemeStyleMap = Type_Name::Appearance::ThemeStyleMap; \
    static ThemeStyleMap g_themeStyles;

#define _D14_SET_THEME_STYLE_MAP_IMPL(Type_Name) \
    Type_Name::Appearance::ThemeStyleMap Type_Name::Appearance::g_themeStyles = {}

    // Consider the following scenario:
    // 
    //     student <--- student_appear
    //        |
    // fashion_student <--- fashion_student_appear <--- student_appear
    // 
    // which is a typical diamond problem and can be solved with virtual
    // inheritance, but here we use another way (i.e. proxy-method):
    // 
    //     student <--- student_appear ---> (pass reference) ----->
    //        |                                                   |
    // fashion_student <--- fashion_student_appear <--- student_appear_proxy

    template<typename T>
    struct AppearanceProxy
    {
        static_assert(std::is_base_of<Appearance, T>::value,
            "T must inherit d14engine::uikit::appearance::Appearance");

        virtual void changeTheme(T& appearance, WstrParam themeName) = 0;
    };

#pragma endregion

#pragma region Reg Variables

    struct ColorGroup // idle, hover, down...
    {
        D2D1_COLOR_F primary = {}, secondary = {}, tertiary = {};

        void generateTonedColors();
    };
    // Generated from the selected theme style dynamically.
    extern ColorGroup g_colorGroup;

#pragma endregion

#pragma region Macro Helpers

#define _D14_FIND_THEME_STYLE(Theme_Name) \
    auto _itor  = g_themeStyles.find(themeName); \
    if ( _itor == g_themeStyles.end()) \
    { \
         _itor  = g_themeStyles.find(L"Light"); \
    } \
    auto& _ref  = _itor->second /* fall through */

#define _D14_UPDATE_THEME_STYLE_DATA_1(Data_Name) \
    Data_Name = _ref.Data_Name

#define _D14_UPDATE_THEME_STYLE_DATA_2(Data_Name) \
    appearance.Data_Name = _ref.Data_Name

#define _D14_UPDATE_THEME_STYLE_DATA_ARRAY_1(Data_Name) \
    memcpy(Data_Name, _ref.Data_Name, sizeof(_ref.Data_Name))

#define _D14_UPDATE_THEME_STYLE_DATA_ARRAY_2(Data_Name) \
    memcpy(appearance.Data_Name, _ref.Data_Name, sizeof(_ref.Data_Name))

#pragma endregion

}
