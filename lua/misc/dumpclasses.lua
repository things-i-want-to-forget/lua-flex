local class = engine.GetAllClasses();

local f = io.open("C:\\aim-flex\\netvars.log", "wb");

local function output(str)

    f:write(str.."\r\n");

end

local function DumpRecvTable(table)

    for i = 1, table:GetPropertyCount() do

        local prop = table:GetProperty(i - 1);


        if (prop:GetName() == "baseclass") then

            DumpRecvTable(prop:GetDatatable());

        else

            output("\t"..prop:GetName());

        end

    end

end

while(class) do

    output(class:GetName());

    DumpRecvTable(class:GetRecvTable(), {});

    class = class:Next();

end

f:close();
