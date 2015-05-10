#ifndef PANELWRAPPER_H
#define PANELWRAPPER_H

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

class KeyValues;
class CDmxElement;
namespace vgui
{
	class IClientPanel;
	class SurfacePlat;
}

class VPanelWrapper
{
public:
	virtual ~VPanelWrapper() = 0;
	virtual void *Init(uint, vgui::IClientPanel *) = 0;
	virtual void *SetPos(uint, int, int) = 0;
	virtual void *GetPos(uint, int &, int &) = 0;
	virtual void *SetSize(uint, int, int) = 0;
	virtual void *GetSize(uint, int &, int &) = 0;
	virtual void *SetMinimumSize(uint, int, int) = 0;
	virtual void *GetMinimumSize(uint, int &, int &) = 0;
	virtual void *SetZPos(uint, int) = 0;
	virtual void *GetZPos(uint) = 0;
	virtual void *GetAbsPos(uint, int &, int &) = 0;
	virtual void *GetClipRect(uint, int &, int &, int &, int &) = 0;
	virtual void *SetInset(uint, int, int, int, int) = 0;
	virtual void *GetInset(uint, int &, int &, int &, int &) = 0;
	virtual void *SetVisible(uint, bool) = 0;
	virtual void *IsVisible(uint) = 0;
	virtual void *SetParent(uint, uint) = 0;
	virtual void *GetChildCount(uint) = 0;
	virtual void *GetChild(uint, int) = 0;
	virtual void *GetChildren(uint) = 0;
	virtual void *GetParent(uint) = 0;
	virtual void *MoveToFront(uint) = 0;
	virtual void *MoveToBack(uint) = 0;
	virtual void *HasParent(uint, uint) = 0;
	virtual void *IsPopup(uint) = 0;
	virtual void *SetPopup(uint, bool) = 0;
	virtual void *IsFullyVisible(uint) = 0;
	virtual void *GetScheme(uint) = 0;
	virtual void *IsProportional(uint) = 0;
	virtual void *IsAutoDeleteSet(uint) = 0;
	virtual void *DeletePanel(uint) = 0;
	virtual void *SetKeyBoardInputEnabled(uint, bool) = 0;
	virtual void *SetMouseInputEnabled(uint, bool) = 0;
	virtual void *IsKeyBoardInputEnabled(uint) = 0;
	virtual void *IsMouseInputEnabled(uint) = 0;
	virtual void *Solve(uint) = 0;
	virtual const char *GetName(uint) = 0;
	virtual void *GetClassName(uint) = 0;
	virtual void *SendMessage(uint, KeyValues *, uint) = 0;
	virtual void *Think(uint) = 0;
	virtual void *PerformApplySchemeSettings(uint) = 0;
	virtual void PaintTraverse(uint, bool, bool) = 0;
	virtual void *Repaint(uint) = 0;
	virtual void *IsWithinTraverse(uint, int, int, bool) = 0;
	virtual void *OnChildAdded(uint, uint) = 0;
	virtual void *OnSizeChanged(uint, int, int) = 0;
	virtual void *InternalFocusChanged(uint, bool) = 0;
	virtual void *RequestInfo(uint, KeyValues *) = 0;
	virtual void *RequestFocus(uint, int) = 0;
	virtual void *RequestFocusPrev(uint, uint) = 0;
	virtual void *RequestFocusNext(uint, uint) = 0;
	virtual void *GetCurrentKeyFocus(uint) = 0;
	virtual void *GetTabPosition(uint) = 0;
	virtual void *Plat(uint) = 0;
	virtual void *SetPlat(uint, vgui::SurfacePlat *) = 0;
	virtual void *GetPanel(uint, char  const*) = 0;
	virtual void *IsEnabled(uint) = 0;
	virtual void *SetEnabled(uint, bool) = 0;
	virtual void *IsTopmostPopup(uint) = 0;
	virtual void *SetTopmostPopup(uint, bool) = 0;
	virtual void *SetMessageContextId(uint, int) = 0;
	virtual void *GetMessageContextId(uint) = 0;
	virtual void *GetUnpackStructure(uint) = 0;
	virtual void *OnUnserialized(uint, CDmxElement *) = 0;
	virtual void *SetSiblingPin(uint, uint, uchar, uchar) = 0;
	virtual void *Client(uint) = 0;
	virtual void *GetModuleName(uint) = 0;

};

#endif // PANELWRAPPER_H