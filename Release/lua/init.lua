local v_meta = FindMetaTable"Vector";
local a_meta = FindMetaTable"Angle";

function v_meta:__tostring()
	return self.x.."  "..self.y.."  "..self.z;
end

function a_meta:__tostring()
	return self.p.."  "..self.y.."  "..self.r;
end

include "hook/hook.lau";

local font = surface.CreateFont();
-- font, textname, tall, weight, https://developer.valvesoftware.com/wiki/EFontFlags
surface.SetFontGlyph(font, "Arial", 13, 300, 0x200);

hook.Add("Paint", "wow", function() 
	surface.SetFont(font);
	surface.SetTextPos(200, 200);
	surface.DrawText("lololol");
end);