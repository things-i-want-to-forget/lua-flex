#ifndef MATSYSTEMSURFACE_H
#define MATSYSTEMSURFACE_H

#include "matrix.h"
#include "color.h"
#include "engines.h"

#include "iappsystem.h"

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

class IMaterial;
enum InputEvent_t { };
enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,	// custom generated font -
};

//vguimatsurface.dll | VGUI_Surface
namespace vgui
{
	class IntRect;
	class IHTML;
	class IHTMLEvents;
	namespace ISurface
	{
		enum SurfaceFeature_t { };
	}
	enum DrawTexturedRectParms_t { };
}
class ImageFormat;
enum FontCharRenderInfo { };
enum FontVertex_t { };
enum FontDrawType_t { };
enum InputContextHandle_t__ { };

extern EngineVersion version;

class CMatSystemSurface : public IAppSystem
{
	template <typename t>
	inline t get(unsigned short which)
	{
		return t((*(char ***)this)[which]);
	}
private:
	virtual void *RunFrame(void) = 0;
	virtual void *CSGGetEmbeddedPanel(void) = 0;
	virtual void *SetEmbeddedPanel(uint) = 0;
	virtual void *PushMakeCurrent(uint, bool) = 0;
	virtual void *PopMakeCurrent(uint) = 0;
	virtual void CSGDrawSetColor(int, int, int, int) = 0;
	virtual void CSGDrawSetColor(Color) = 0;
	virtual void CSGDrawFilledRect(int, int, int, int) = 0;
	virtual void *DrawFilledRectArray(vgui::IntRect *, int) = 0;
	virtual void CSGDrawOutlinedRect(int, int, int, int) = 0;
	virtual void CSGDrawLine(int, int, int, int) = 0;
	virtual void *DrawPolyLine(int *, int *, int) = 0;
	virtual void *DrawSetApparentDepth(float) = 0;
	virtual void *DrawClearApparentDepth(void) = 0;
	virtual void CSGDrawSetTextFont(ulong) = 0;
	virtual void CSGDrawSetTextColor(int, int, int, int) = 0;
	virtual void CSGDrawSetTextColor(Color) = 0;
public:
	inline void DrawSetColor(Color col)
	{
		return CSGDrawSetColor(col);
	}
	inline void DrawFilledRect(int a, int b, int c, int d)
	{
		return CSGDrawFilledRect(a, b, c, d);
	}
	inline void DrawOutlinedRect(int a, int b, int c, int d)
	{
		return CSGDrawOutlinedRect(a, b, c, d);
	}
	inline void DrawLine(int a, int b, int c, int d)
	{
		return CSGDrawLine(a, b, c, d);
	}
	inline void DrawSetTextFont(ulong a)
	{
		return CSGDrawSetTextFont(a);
	}
	inline void DrawSetTextColor(Color col)
	{
		return CSGDrawSetTextColor(col);
	}
	inline void DrawSetTextPos(int x, int y)
	{
		return CSGDrawSetTextPos(x, y);
	}
	inline void DrawPrintText(const wchar_t *text, int len, int drawtype = 0)
	{
		return CSGDrawPrintText(text, len, (FontDrawType_t)drawtype);
	}
	inline void DrawGetTextSize(ulong font, wchar_t const *text, int &w, int &h)
	{
		get<void(__thiscall *)(void *, ulong, wchar_t const*, int &, int &)>(80)(this, font, text, w, h);
	};
#ifdef CreateFont
#undef CreateFont
#endif
	inline ulong CreateFont(void)
	{
		return CSGCreateFont();
	}
	inline bool SetFontGlyphSet(ulong font, char  const* font_name,
		int tall, int weight, int blur, int scanlines, int flags, int a = 0, int b = 0)
	{
		return CSGSetFontGlyphSet(font, font_name, tall, weight, blur, scanlines, flags, a, b);
	}
private:
	virtual void CSGDrawSetTextPos(int, int) = 0;
	virtual void *DrawGetTextPos(int &, int &) = 0;
	virtual void CSGDrawPrintText(wchar_t  const*, int, FontDrawType_t) = 0;
	virtual void *DrawUnicodeChar(wchar_t, FontDrawType_t) = 0;
	virtual void *DrawFlushText(void) = 0;
	virtual void *CreateHTMLWindow(vgui::IHTMLEvents *, uint) = 0;
	virtual void *PaintHTMLWindow(vgui::IHTML *) = 0;
	virtual void *DeleteHTMLWindow(vgui::IHTML *) = 0;
	virtual void *DrawGetTextureId(char  const*) = 0;
	virtual void *DrawGetTextureFile(int, char *, int) = 0;
	virtual void *DrawSetTextureFile(int, char  const*, int, bool) = 0;
	virtual void *DrawSetTextureRGBA(int, uchar  const*, int, int) = 0;
	virtual void *DrawSetTexture(int) = 0;
	virtual void *DeleteTextureByID(int) = 0;
	virtual void *DrawGetTextureSize(int, int &, int &) = 0;
	virtual void *DrawTexturedRect(int, int, int, int) = 0;
	virtual void *IsTextureIDValid(int) = 0;
	virtual void *CreateNewTextureID(bool) = 0;
	virtual void *GetScreenSize(int &, int &) = 0;
	virtual void *SetAsTopMost(uint, bool) = 0;
	virtual void *BringToFront(uint) = 0;
	virtual void *SetForegroundWindow(uint) = 0;
	virtual void *SetPanelVisible(uint, bool) = 0;
	virtual void *SetMinimized(uint, bool) = 0;
	virtual void *IsMinimized(uint) = 0;
	virtual void *FlashWindow(uint, bool) = 0;
	virtual void *SetTitle(uint, wchar_t  const*) = 0;
	virtual void *SetAsToolBar(uint, bool) = 0;
	virtual void *CreatePopup(uint, bool, bool, bool, bool, bool) = 0;
	virtual void *SwapBuffers(uint) = 0;
	virtual void *Invalidate(uint) = 0;
	virtual void *SetCursor(ulong) = 0;
	virtual void *IsCursorVisible(void) = 0;
	virtual void *ApplyChanges(void) = 0;
	virtual void *IsWithin(int, int) = 0;
	virtual void *HasFocus(void) = 0;
	virtual void *SupportsFeature(vgui::ISurface::SurfaceFeature_t) = 0;
	virtual void *RestrictPaintToSinglePanel(uint, bool) = 0;
	virtual void *SetModalPanel(uint) = 0;
	virtual void *GetModalPanel(void) = 0;
	virtual void *UnlockCursor(void) = 0;
	virtual void *LockCursor(void) = 0;
	virtual void *SetTranslateExtendedKeys(bool) = 0;
	virtual void *GetTopmostPopup(void) = 0;
	virtual void *SetTopLevelFocus(uint) = 0;
	virtual ulong CSGCreateFont(void) = 0;
	virtual bool CSGSetFontGlyphSet(ulong, char  const*, int, int, int, int, int, int, int) = 0;
	virtual void *AddCustomFontFile(char  const*) = 0;
	virtual void *GetFontTall(ulong) = 0;
	virtual void *GetFontAscent(ulong, wchar_t) = 0;
	virtual void *IsFontAdditive(ulong) = 0;
	virtual void *GetCharABCwide(ulong, int, int &, int &, int &) = 0;
	virtual void *GetCharacterWidth(ulong, int) = 0;
	virtual void *GetTextSize(ulong, wchar_t  const*, int &, int &) = 0;
	virtual void *GetNotifyPanel(void) = 0;
	virtual void *SetNotifyIcon(uint, ulong, uint, char  const*) = 0;
	virtual void *PlaySound(char  const*) = 0;
	virtual void *GetPopupCount(void) = 0;
	virtual void *GetPopup(int) = 0;
	virtual void *ShouldPaintChildPanel(uint) = 0;
	virtual void *RecreateContext(uint) = 0;
	virtual void *AddPanel(uint) = 0;
	virtual void *ReleasePanel(uint) = 0;
	virtual void *MovePopupToFront(uint) = 0;
	virtual void *MovePopupToBack(uint) = 0;
	virtual void *SolveTraverse(uint, bool) = 0;
	virtual void *PaintTraverse(uint) = 0;
	virtual void *EnableMouseCapture(uint, bool) = 0;
	virtual void *GetWorkspaceBounds(int &, int &, int &, int &) = 0;
	virtual void *GetAbsoluteWindowBounds(int &, int &, int &, int &) = 0;
	virtual void *GetProportionalBase(int &, int &) = 0;
	virtual void *CalculateMouseVisible(void) = 0;
	virtual void *NeedKBInput(void) = 0;
	virtual void *HasCursorPosFunctions(void) = 0;
	virtual void *SurfaceGetCursorPos(int &, int &) = 0;
	virtual void *SurfaceSetCursorPos(int, int) = 0;
	virtual void *DrawTexturedLine(FontVertex_t  const&, FontVertex_t  const&) = 0;
	virtual void *DrawOutlinedCircle(int, int, int, int) = 0;
	virtual void *DrawTexturedPolyLine(FontVertex_t  const*, int) = 0;
	virtual void *DrawTexturedSubRect(int, int, int, int, float, float, float, float) = 0;
	virtual void *DrawTexturedPolygon(int, FontVertex_t *, bool) = 0;
	virtual void *GetTitle(uint) = 0;
	virtual void *IsCursorLocked(void) = 0;
	virtual void *SetWorkspaceInsets(int, int, int, int) = 0;
	virtual void *DrawWordBubble(int, int, int, int, int, Color, Color, bool, int, int, int) = 0;
	virtual void *DrawGetUnicodeCharRenderInfo(wchar_t, FontCharRenderInfo &) = 0;
	virtual void *DrawRenderCharFromInfo(FontCharRenderInfo  const&) = 0;
	virtual void *DrawSetAlphaMultiplier(float) = 0;
	virtual void *DrawGetAlphaMultiplier(void) = 0;
	virtual void *SetAllowHTMLJavaScript(bool) = 0;
	virtual void *OnScreenSizeChanged(int, int) = 0;
	virtual void *CreateCursorFromFile(char  const*, char  const*) = 0;
	virtual void *DrawGetTextureMatInfoFactory(int) = 0;
	virtual void *PaintTraverseEx(uint, bool) = 0;
	virtual void *GetZPos(void) = 0;
	virtual void *SetPanelForInput(uint) = 0;
	virtual void *DrawFilledRectFastFade(int, int, int, int, int, int, uint, uint, bool) = 0;
	virtual void *DrawFilledRectFade(int, int, int, int, uint, uint, bool) = 0;
	virtual void *DrawSetTextureRGBAEx(int, uchar  const*, int, int, ImageFormat) = 0;
	virtual void *DrawSetTextScale(float, float) = 0;
	virtual void *SetBitmapFontGlyphSet(ulong, char  const*, float, float, int) = 0;
	virtual void *AddBitmapFontFile(char  const*) = 0;
	virtual void *SetBitmapFontName(char  const*, char  const*) = 0;
	virtual void *GetBitmapFontName(char  const*) = 0;
	virtual void *ClearTemporaryFontCache(void) = 0;
	virtual void *GetIconImageForFullPath(char  const*) = 0;
	virtual void *DrawUnicodeString(wchar_t  const*, FontDrawType_t) = 0;
	virtual void *PrecacheFontCharacters(ulong, wchar_t *) = 0;
	virtual void *GetFontName(ulong) = 0;
	virtual void *ForceScreenSizeOverride(bool, int, int) = 0;
	virtual void *ForceScreenPosOffset(bool, int, int) = 0;
	virtual void *OffsetAbsPos(int &, int &) = 0;
	virtual void *SetAbsPosForContext(int, int, int) = 0;
	virtual void *GetAbsPosForContext(int, int &, int &) = 0;
	virtual void *ResetFontCaches(void) = 0;
	virtual void *IsScreenSizeOverrideActive(void) = 0;
	virtual void *IsScreenPosOverrideActive(void) = 0;
	virtual void *DestroyTextureID(int) = 0;
	virtual void *GetTextureNumFrames(int) = 0;
	virtual void *DrawSetTextureFrame(int, int, uint *) = 0;
	virtual void *GetClipRect(int &, int &, int &, int &) = 0;
	virtual void *SetClipRect(int, int, int, int) = 0;
	virtual void *DrawTexturedRectEx(vgui::DrawTexturedRectParms_t *) = 0;
	virtual void *GetKernedCharWidth(ulong, wchar_t, wchar_t, wchar_t, float &, float &, float &) = 0;
	virtual void *DrawUpdateRegionTextureRGBA(int, int, int, uchar  const*, int, int, ImageFormat) = 0;
	virtual void *BHTMLWindowNeedsPaint(vgui::IHTML *) = 0;
	virtual void *DrawSetTextureRGBALinear(int, uchar  const*, int, int) = 0;
	virtual void *GetWebkitHTMLUserAgentString(void) = 0;
	virtual void *AccessChromeHTMLController(void) = 0;
	virtual void *SetAppDrivesInput(bool) = 0;
	virtual void *EnableWindowsMessages(bool) = 0;
	virtual void *Begin3DPaint(int, int, int, int, bool) = 0;
	virtual void *End3DPaint(bool) = 0;
	virtual void *DisableClipping(bool) = 0;
	virtual void *SetMouseCallbacks(void(*)(int &, int &), void(*)(int, int)) = 0;
	virtual void *InstallPlaySoundFunc(void(*)(char  const*)) = 0;
	virtual void *DrawColoredCircle(int, int, float, int, int, int, int) = 0;
	virtual void *DrawColoredText(ulong, int, int, int, int, int, int, char  const*, ...) = 0;
	virtual void *DrawColoredTextRect(ulong, int, int, int, int, int, int, int, int, char  const*, ...) = 0;
	virtual void *DrawTextHeight(ulong, int, int &, char *, ...) = 0;
	virtual void *DrawTextLen(ulong, char  const*, ...) = 0;
	virtual void *DrawPanelIn3DSpace(uint, VMatrix  const&, int, int, float, float) = 0;
	virtual void *DrawSetTextureMaterial(int, IMaterial *) = 0;
	virtual void *HandleInputEvent(InputEvent_t  const&) = 0;
	virtual void *Set3DPaintTempRenderTarget(char  const*) = 0;
	virtual void *Reset3DPaintTempRenderTarget(void) = 0;
	virtual void *DrawGetTextureMaterial(int) = 0;
	virtual void *SetInputContext(InputContextHandle_t__ *) = 0;
	virtual void *DrawSetSubTextureRGBA(int, int, int, uchar  const*, int, int) = 0;
	virtual void *DrawTexturedSubRectGradient(int, int, int, int, float, float, float, float, Color, Color, bool) = 0;
};
#endif // MATSYSTEMSURFACE_H
