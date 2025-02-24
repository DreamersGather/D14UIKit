#pragma once

#include "Common/Precompile.h"

namespace d14engine::uikit::appearance
{
    void initialize();

#pragma region Interfaces

    struct Appearance
    {
        struct ThemeStyle
        {
            Wstring name = {}; D2D1_COLOR_F color = {};

            bool operator==(const ThemeStyle& rhs)
            {
                return name == rhs.name &&
                    color.r == rhs.color.r &&
                    color.g == rhs.color.g &&
                    color.b == rhs.color.b &&
                    color.a == rhs.color.a;
            }
        };
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

#define _D14_SET_THEME_DATA_MAP_DECL \
    using ThemeDataMap = std::unordered_map<Wstring, ThemeData>; \
    static ThemeDataMap g_themeData

#define _D14_REF_THEME_DATA_MAP_DECL(Type_Name) \
    using ThemeData = Type_Name::Appearance::ThemeData; \
    using ThemeDataMap = Type_Name::Appearance::ThemeDataMap; \
    static ThemeDataMap g_themeData;

#define _D14_SET_THEME_DATA_MAP_IMPL(Type_Name) \
    Type_Name::Appearance::ThemeDataMap Type_Name::Appearance::g_themeData = {}

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

        void generateTonedColors(const Appearance::ThemeStyle& style);
    };
    // Generated from the selected theme style dynamically.
    extern ColorGroup g_colorGroup;

#pragma endregion

#pragma region Macro Helpers

#define _D14_FIND_THEME_DATA(Mode_Name) \
    auto _itor = g_themeData.find(Mode_Name); \
    if (_itor == g_themeData.end()) \
    { \
        _itor = g_themeData.find(L"Light"); \
    } \
    auto& _ref = _itor->second /* fall through */

#define _D14_UPDATE_THEME_DATA_1(Data_Name) \
    Data_Name = _ref.Data_Name

#define _D14_UPDATE_THEME_DATA_2(Data_Name) \
    appearance.Data_Name = _ref.Data_Name

#define _D14_UPDATE_THEME_DATA_ARRAY_1(Data_Name) \
    memcpy(Data_Name, _ref.Data_Name, sizeof(_ref.Data_Name))

#define _D14_UPDATE_THEME_DATA_ARRAY_2(Data_Name) \
    memcpy(appearance.Data_Name, _ref.Data_Name, sizeof(_ref.Data_Name))

#pragma endregion

}
