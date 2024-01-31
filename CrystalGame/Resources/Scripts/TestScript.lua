function Start()
    -- ... content ...
    print('Start called on RgbaScript.lua')

    local color1 = Maths.RGBA.new(1, 2, 3, 4)
    local color2 = Maths.RGBA.new(5, 6, 7, 8)

    local result1 = Maths.RGBA.Add(color1, color2)
    local result2 = Maths.RGBA.Sub(color1, color2)

    print(result1.r, result1.g, result1.b, result1.a)
    print(result2.r, result2.g, result2.b, result2.a)

    local result3 = result2:toRGB()
    print(result3.r, result3.g, result3.b)

    local hsva = Maths.HSVA.new(1,1,1,1)
    print(hsva.h, hsva.s, hsva.v, hsva.a)

    local rgba = Maths.HSVAtoRGBA(hsva);
    print(rgba.r, rgba.g, rgba.b, rgba.a)

    local tag = Scenes.ObjectTag.new("ColorTag");
    print(tag.name)

    local objectType = Scenes.GameObjectType.Model;
    print(Scenes.GameObjectTypeToString(objectType))

    local gameObject = Scenes.GameObject.new(80, "TheGameObject", nil, objectType)
    print(gameObject.name)

    local vector1 = Maths.Vector2.new(1, 1)
    print(vector1.x, vector1.y)

    local vector2 = Maths.Vector2.new(5, 5)
    print(vector2.x, vector2.y)

    local vector3 = Maths.Vector2.Add(vector1, 5)
    print(vector3.x, vector3.y)

    print(Maths.Vector2.Equ(vector1, vector1))

    print(Maths.Vector2.Equ(vector1, vector1))

    local lightObject = Scenes.LightObject.new(81, "TheLightObject", nil, nil)
    print(lightObject.name)

end

function LateStart()
    -- ... content ...
    -- print('LateStart called')
end

function Update()
    -- ... content ...
    -- print('Update called')
end

function LateUpdate()
    -- ... content ...
    -- print('LateUpdate called')
end

--
-- CrystalEngine
-- 