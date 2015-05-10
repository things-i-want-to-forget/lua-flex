#include "vtable.h"

namespace hook {
	extern VTable *client_mode_vt;
	extern VTable *prediction_vt;
	extern VTable *panel_vt;
	extern VTable *client_vt;

	void InitHooks();
}