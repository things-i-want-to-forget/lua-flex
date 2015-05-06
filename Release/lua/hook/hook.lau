hook = {};

local Hooks = {};

function hook.GetTable()
	return Hooks;
end

function hook.Call(name, a, b, c, d, e, f, g)
	for k,v in next, (Hooks[name] or {}), nil do
		local ret = {v(a, b, c, d, e, f, g)}
		if ret[1] then
			return table.unpack(ret)
		end
	end
end

function hook.Add(name, id, func)

	Hooks[name] = Hooks[name] or {};
	Hooks[name][id] = func;
	
end

function hook.Remove(name, id)

	hook.Add(name, id, nil);
	
end