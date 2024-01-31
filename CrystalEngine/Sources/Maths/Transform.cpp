#include "Maths/Maths.h"
using namespace Maths;


// ----- Constructors ----- //
Transform::Transform(const bool& _isCamera)
    : scale(1), isCamera(_isCamera)
{
    UpdateLocalMat();
}

Transform::Transform(const Vector3& position, const Quaternion& rotation, const Vector3& _scale, const bool& _isCamera)
    : pos(position), rot(rotation), scale(_scale), isCamera(_isCamera), eulerRot(rotation.ToEuler())
{
    UpdateLocalMat();
}

// ----- Position ----- //
Vector3 Transform::GetPosition() const { return pos; }
Vector3 Transform::GetWorldPosition() const { return worldPos; }
void    Transform::SetPosition(const Vector3& position) { pos  = position; UpdateLocalMat(); }
void    Transform::Move       (const Vector3& movement) { pos += movement; UpdateLocalMat(); }

// ----- Forward ----- //
Vector3 Transform::Forward() const                    { return (Vector4(0,0,1,0) * (rot.ToMatrix() * parentMat)).ToVector3().GetNormalized(); }
Vector3 Transform::Up()      const                    { return (Vector4(0,1,0,0) * (rot.ToMatrix() * parentMat)).ToVector3().GetNormalized(); }
Vector3 Transform::Right()   const                    { return (Vector4(1,0,0,0) * (rot.ToMatrix() * parentMat)).ToVector3().GetNormalized(); }
void    Transform::SetForward(const Vector3& forward) { rot = AngleAxis(2*PI, forward); UpdateLocalMat(); }

// ----- Rotation ----- //
Quaternion Transform::GetRotation() const                     { return rot;      }
Vector3    Transform::GetEulerRot() const                     { return eulerRot; }
void       Transform::SetRotation(const Quaternion& rotation) { rot = rotation;                 eulerRot = rot.ToEuler(); UpdateLocalMat(); }
void       Transform::Rotate     (const Quaternion& rotation) { rot = rotation.RotateQuat(rot); eulerRot = rot.ToEuler(); UpdateLocalMat(); }
void       Transform::SetEulerRot(const Vector3& euler)       { eulerRot  = euler; rot = Quaternion::FromEuler(eulerRot); UpdateLocalMat(); }
void       Transform::RotateEuler(const Vector3& euler)       { eulerRot += euler; rot = Quaternion::FromEuler(eulerRot); UpdateLocalMat(); }

// ----- Scale ----- //
Vector3 Transform::GetScale()        const         { return scale;   }
Vector3 Transform::GetUniformScale() const         { return std::max(scale.x, std::max(scale.y, scale.z)); }
void    Transform::SetScale(const Vector3& _scale) { scale = _scale;  UpdateLocalMat(); }
void    Transform::Scale   (const Vector3& _scale) { scale += _scale; UpdateLocalMat(); }

// ----- Is Camera ----- //
bool Transform::IsCamera() const                   { return isCamera; }
void Transform::SetIsCamera(const bool& _isCamera) { isCamera = _isCamera; UpdateLocalMat(); }

// ----- Multiple Values ----- //
void Transform::SetPosRot(const Vector3& position, const Quaternion& rotation)                        { pos = position; rot = rotation; UpdateLocalMat(); }
void Transform::SetValues(const Vector3& position, const Quaternion& rotation, const Vector3& _scale) { pos = position; rot = rotation; scale = _scale; UpdateLocalMat(); }

void Transform::SetParentMat(const Mat4& mat)
{
    parentMat = mat;
    worldMat  = localMat * parentMat;
    worldPos  = (pos.ToVector4() * parentMat).ToVector3(true);
}

// ----- Interpolation ----- //
Transform Transform::Lerp(const Transform& start, const Transform& dest, const float& val, const bool& useSlerp)
{
    const Vector3    lerpPos   = Vector3::Lerp(start.GetPosition(), dest.GetPosition(), val);
    const Quaternion lerpRot   = useSlerp ? Quaternion::SLerp(start.GetRotation(), dest.GetRotation(), val)
                                          : Quaternion::NLerp(start.GetRotation(), dest.GetRotation(), val);
    const Vector3    lerpScale = Vector3::Lerp(start.GetScale(), dest.GetScale(), val);
    return Transform(lerpPos, lerpRot, lerpScale);
}

// ----- Matrices ----- //
void Transform::UpdateLocalMat() 
{
    localMat = Mat4::FromTransform( pos, rot, scale);
    worldMat = localMat * parentMat;
    worldPos = (pos.ToVector4() * parentMat).ToVector3(true);
    
    if (isCamera) viewMat = Mat4::FromTransform(-pos, rot, { 1, 1, 1 }, true);
}
