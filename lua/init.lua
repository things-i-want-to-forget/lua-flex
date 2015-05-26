include "hook/hook.lau";

include "extensions/enums.lau";
include "extensions/vector.lau";
include "extensions/angle.lau";
include "extensions/entity.lau";
include "extensions/usercmd.lau";

include"scripts/loader.lau"

hook.Call("Initialize") --called after everything is loaded so you can setup things

print"[lua-flex] loaded!"