local v_meta = FindMetaTable"Vector";
local a_meta = FindMetaTable"Angle";

function v_meta:__tostring()
	return self.x.."  "..self.y.."  "..self.z;
end

function a_meta:__tostring()
	return self.p.."  "..self.y.."  "..self.r;
end

MASK_SHOT = 0x4600400B;

include "hook/hook.lau";
include "extensions/entity.lau";

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
		local scr, onscr = v:GetPos():ToScreen();
		if(onscr) then
			surface.SetTextPos(scr.x, scr.y);
			surface.DrawText(v:Nick());
		end
		
		local r = trace.Ray(v:ShootPos(), v:ShootPos() + v:EyeAngles():Vector() * 8096, MASK_SHOT, function(e)
			return false;
		end);
		local epos, eposonscr = r.endpos:ToScreen();
		local spos, sposonscr = r.startpos:ToScreen();
		
		if(eposonscr and sposonscr) then
			surface.SetDrawColor(255,255,0,255);
			surface.DrawLine(spos.x, spos.y, epos.x, epos.y);
		end
		
	end
end);

local lastaim;
--[[
hook.Add("CreateMove", "noobaim", function(cmd)

	local lp = LocalPlayer();
	if(not lastaim and lp:Health() > 0 and lp:CanShoot()) then
		
		local shootpos = lp:ShootPos();
		local world;
	
		for k,v in next, player.GetAll(), nil do
			if(v ~= lp) then
				local bones = v:SetupBones();
				
				for i = 1, v:GetHitboxCount() do
					local group = v:GetHitboxGroup(i);
					if(group <= 3 and group > 0) then
						local bbmin, bbmax = v:GetHitboxBB(i);
						local mid = (bbmin + bbmax) / 2;
						world = mid:Transform(bones[v:GetHitboxBone(i)]);
						
						if(util.CanAutowall(v, shootpos, world)) then
							print"FUCK"
							goto _found;
						end
						world = nil;
					end
				end
			end
		end
		
		::_found::
		if(world) then
			
			local antirecoil = lp:Punch() * -2;
			
			local ang = (world - shootpos);
			
			lastaim = ang + antirecoil;
			
			cmd.p = lastaim.p;
			cmd.y = lastaim.y;
			cmd.r = 0;
			
			cmd.sendpacket = false;
			
		end
	
	elseif(lastaim) then
	
		lastaim = false;
		local ang = cmd.angles * 2 - lastaim;
		
		cmd.p = ang.p;
		cmd.y = ang.y;
		cmd.r = 0;
		
		cmd.sendpacket = true;
		
	end
	
end)]]