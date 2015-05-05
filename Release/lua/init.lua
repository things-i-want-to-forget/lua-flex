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

hook.Add("Paint", "hackdeplayer", function()
	if(not IsInGame()) then return; end
	surface.SetFont(font);
	surface.SetTextColor(0,0,0,255);
	for k,v in next, player.GetAll(), nil do
		local scr, onscr = v:GetPos():ToScreen()
		if(onscr) then
			surface.SetTextPos(scr.x, scr.y);
			surface.DrawText(v:Nick());
		end
	end
end);