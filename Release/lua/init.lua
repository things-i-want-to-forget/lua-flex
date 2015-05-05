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
	surface.SetFont(font);
	surface.SetTextColor(255,255,255,255);
	if(not IsInGame()) then 
		surface.SetTextPos(200,200);
		surface.DrawText"wtf";
		return;
	end
	for k,v in next, player.GetAll(), nil do
		local scr, onscr = v:GetPos():ToScreen()
		if(onscr) then
			surface.SetTextPos(scr.x, scr.y);
			surface.DrawText(v:Nick());
		end
	end
end);