#include "Application.h"
#include "ConstraintLayout.h"
#include "GridLayout.h"
#include "Label.h"
#include "MainWindow.h"

using namespace d14uikit;

#define DEMO_NAME L"LayoutControl"

int main(int argc, char* argv[])
{
    float dpi = 96.0f;
    if (argc >= 2 && strcmp(argv[1], "HighDPI") == 0)
    {
        dpi = 192.0f;
    }
    Application app(DEMO_NAME, dpi);
    app.setMinSize(app.size());
    app.setResizable(true);

    //------------------------------------------- Initialize UI objects.

    MainWindow mwnd(DEMO_NAME);
    GridLayout centerLayout;
    mwnd.setContent(&centerLayout);
    centerLayout.setHorzCellCount(2);

    GridLayout::GeoInfo geoInfo = {};

    ConstraintLayout layout1;
    geoInfo = {};
    geoInfo.x = { 0, 1 };
    geoInfo.y = { 0, 1 };
    centerLayout.addElement(&layout1, geoInfo);

    ConstraintLayout::GeoInfo geoInfo1 = {};

    Label lbl_c1(L"C1");
    geoInfo1 = {};
    geoInfo1.keepWidth = false;
    geoInfo1.Left.ToLeft = 20;
    geoInfo1.Right.ToRight = 20;
    geoInfo1.keepHeight = false;
    geoInfo1.Top.ToTop = 20;
    geoInfo1.Bottom.ToTop = 120;
    layout1.addElement(&lbl_c1, geoInfo1);
    lbl_c1.setBkgnColor({ 0, 255, 0 });
    lbl_c1.setBkgnOpacity(0.5f);
    lbl_c1.setHorzAlign(Label::HCenter);

    Label lbl_c2(L"C2");
    lbl_c2.setHeight(400);
    geoInfo1 = {};
    geoInfo1.keepWidth = false;
    geoInfo1.Left.ToLeft = 20;
    geoInfo1.Right.ToRight = 20;
    geoInfo1.keepHeight = true;
    geoInfo1.Bottom.ToBottom = 20;
    layout1.addElement(&lbl_c2, geoInfo1);
    lbl_c2.setBkgnColor({ 0, 255, 255 });
    lbl_c2.setBkgnOpacity(0.5f);
    lbl_c2.setHorzAlign(Label::HCenter);

    GridLayout layout2;
    geoInfo = {};
    geoInfo.x = { 1, 1 };
    geoInfo.y = { 0, 1 };
    centerLayout.addElement(&layout2, geoInfo);
    layout2.setHorzCellCount(4);
    layout2.setVertCellCount(4);
    layout2.setHorzMargin(20);
    layout2.setVertMargin(20);

    GridLayout::GeoInfo geoInfo2 = {};

    Label lbl_g1(L"G1");
    geoInfo2 = {};
    geoInfo2.x = { 0, 1 };
    geoInfo2.y = { 0, 1 };
    layout2.addElement(&lbl_g1, geoInfo2);
    lbl_g1.setBkgnColor({ 255, 0, 0 });
    lbl_g1.setBkgnOpacity(0.5f);
    lbl_g1.setHorzAlign(Label::HCenter);

    Label lbl_g2(L"G2");
    geoInfo2 = {};
    geoInfo2.x = { 1, 3 };
    geoInfo2.y = { 0, 2 };
    layout2.addElement(&lbl_g2, geoInfo2);
    lbl_g2.setBkgnColor({ 0, 255, 0 });
    lbl_g2.setBkgnOpacity(0.5f);
    lbl_g2.setHorzAlign(Label::HCenter);

    Label lbl_g3(L"G3");
    geoInfo2 = {};
    geoInfo2.x = { 0, 1 };
    geoInfo2.y = { 1, 3 };
    geoInfo2.spacing.top = 10;
    geoInfo2.spacing.right = 10;
    layout2.addElement(&lbl_g3, geoInfo2);
    lbl_g3.setBkgnColor({ 255, 0, 255 });
    lbl_g3.setBkgnOpacity(0.5f);
    lbl_g3.setHorzAlign(Label::HCenter);

    Label lbl_g4(L"G4");
    lbl_g4.setSize({ 200, 200 });
    geoInfo2 = {};
    geoInfo2.fixedSize = true;
    geoInfo2.x = { 1, 3 };
    geoInfo2.y = { 2, 2 };
    layout2.addElement(&lbl_g4, geoInfo2);
    lbl_g4.setBkgnColor({ 0, 255, 255 });
    lbl_g4.setBkgnOpacity(0.5f);
    lbl_g4.setHorzAlign(Label::HCenter);

    //------------------------------------------- Set UI event callacks.

    return app.run();
}
