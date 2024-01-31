function Start()
    transform:SetEulerRot(Maths.Vector3.new())
end

function LateStart()
end

function Update()
    transform:RotateEuler(Maths.Vector3.new(0, time:DeltaTime() * Maths.PI/8 , 0))
end

function LateUpdate()
end

-- CrystalEngine
-- 