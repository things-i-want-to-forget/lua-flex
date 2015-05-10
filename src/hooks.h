#include "vtable.h"

namespace hook {
	VTable *client_mode_vt = 0;
	VTable *prediction_vt = 0;
	VTable *panel_vt = 0;
	VTable *client_vt = 0;

	void Init_Hooks();
}