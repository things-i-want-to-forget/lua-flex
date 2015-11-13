local class = engine.GetAllClasses();

print"ayy"

local i = 100;

while(class) do

    print(class:GetName(), class:GetID());

    class = class:Next();

end
