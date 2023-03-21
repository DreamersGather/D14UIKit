﻿#include "Common/Precompile.h"

#ifdef _D14_UIKIT_EXP_PY_BIND

#include "VirtualKeyCode.h"

namespace d14uikit
{
    void Py_InitVirtualKeyCode(py::module_& m)
    {
        m.attr("vk_lbutton") = 0x01;
        m.attr("vk_rbutton") = 0x02;
        m.attr("vk_cancel") = 0x03;
        m.attr("vk_mbutton") = 0x04;
        m.attr("vk_back") = 0x08;
        m.attr("vk_tab") = 0x09;
        m.attr("vk_clear") = 0x0c;
        m.attr("vk_return") = 0x0d;
        m.attr("vk_shift") = 0x10;
        m.attr("vk_control") = 0x11;
        m.attr("vk_menu") = 0x12;
        m.attr("vk_pause") = 0x13;
        m.attr("vk_capital") = 0x14;
        m.attr("vk_kana") = 0x15;
        m.attr("vk_hangeul") = 0x15;
        m.attr("vk_hangul") = 0x15;
        m.attr("vk_junja") = 0x17;
        m.attr("vk_final") = 0x18;
        m.attr("vk_hanja") = 0x19;
        m.attr("vk_kanji") = 0x19;
        m.attr("vk_escape") = 0x1b;
        m.attr("vk_convert") = 0x1c;
        m.attr("vk_nonconvert") = 0x1d;
        m.attr("vk_accept") = 0x1e;
        m.attr("vk_modechange") = 0x1f;
        m.attr("vk_space") = 0x20;
        m.attr("vk_prior") = 0x21;
        m.attr("vk_next") = 0x22;
        m.attr("vk_end") = 0x23;
        m.attr("vk_home") = 0x24;
        m.attr("vk_left") = 0x25;
        m.attr("vk_up") = 0x26;
        m.attr("vk_right") = 0x27;
        m.attr("vk_down") = 0x28;
        m.attr("vk_select") = 0x29;
        m.attr("vk_print") = 0x2a;
        m.attr("vk_execute") = 0x2b;
        m.attr("vk_snapshot") = 0x2c;
        m.attr("vk_insert") = 0x2d;
        m.attr("vk_delete") = 0x2e;
        m.attr("vk_help") = 0x2f;
        m.attr("vk_lwin") = 0x5b;
        m.attr("vk_rwin") = 0x5c;
        m.attr("vk_apps") = 0x5d;
        m.attr("vk_numpad0") = 0x60;
        m.attr("vk_numpad1") = 0x61;
        m.attr("vk_numpad2") = 0x62;
        m.attr("vk_numpad3") = 0x63;
        m.attr("vk_numpad4") = 0x64;
        m.attr("vk_numpad5") = 0x65;
        m.attr("vk_numpad6") = 0x66;
        m.attr("vk_numpad7") = 0x67;
        m.attr("vk_numpad8") = 0x68;
        m.attr("vk_numpad9") = 0x69;
        m.attr("vk_multiply") = 0x6a;
        m.attr("vk_add") = 0x6b;
        m.attr("vk_separator") = 0x6c;
        m.attr("vk_subtract") = 0x6d;
        m.attr("vk_decimal") = 0x6e;
        m.attr("vk_divide") = 0x6f;
        m.attr("vk_f1") = 0x70;
        m.attr("vk_f2") = 0x71;
        m.attr("vk_f3") = 0x72;
        m.attr("vk_f4") = 0x73;
        m.attr("vk_f5") = 0x74;
        m.attr("vk_f6") = 0x75;
        m.attr("vk_f7") = 0x76;
        m.attr("vk_f8") = 0x77;
        m.attr("vk_f9") = 0x78;
        m.attr("vk_f10") = 0x79;
        m.attr("vk_f11") = 0x7a;
        m.attr("vk_f12") = 0x7b;
        m.attr("vk_f13") = 0x7c;
        m.attr("vk_f14") = 0x7d;
        m.attr("vk_f15") = 0x7e;
        m.attr("vk_f16") = 0x7f;
        m.attr("vk_f17") = 0x80;
        m.attr("vk_f18") = 0x81;
        m.attr("vk_f19") = 0x82;
        m.attr("vk_f20") = 0x83;
        m.attr("vk_f21") = 0x84;
        m.attr("vk_f22") = 0x85;
        m.attr("vk_f23") = 0x86;
        m.attr("vk_f24") = 0x87;
        m.attr("vk_numlock") = 0x90;
        m.attr("vk_scroll") = 0x91;
        m.attr("vk_lshift") = 0xa0;
        m.attr("vk_rshift") = 0xa1;
        m.attr("vk_lcontrol") = 0xa2;
        m.attr("vk_rcontrol") = 0xa3;
        m.attr("vk_lmenu") = 0xa4;
        m.attr("vk_rmenu") = 0xa5;
        m.attr("vk_processkey") = 0xe5;
        m.attr("vk_attn") = 0xf6;
        m.attr("vk_crsel") = 0xf7;
        m.attr("vk_exsel") = 0xf8;
        m.attr("vk_ereof") = 0xf9;
        m.attr("vk_play") = 0xfa;
        m.attr("vk_zoom") = 0xfb;
        m.attr("vk_noname") = 0xfc;
        m.attr("vk_pa1") = 0xfd;
        m.attr("vk_oem_clear") = 0xfe;
    }
}

#endif
