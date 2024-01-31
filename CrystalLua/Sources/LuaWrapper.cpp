#include "LuaWrapper.h"

#include "sol/sol.hpp"

#include "Maths/Color.h"
#include "Maths/Vector2.h"
#include "Maths/Vector3.h"
#include "Maths/Vector4.h"
#include "Maths/Transform.h"
#include "Maths/Vertex.h"

#include "Debug/Logger.h"

#include "Resources/Texture.h"
#include "Resources/RenderTexture.h"
#include "Resources/Cubemap.h"
#include "Resources/Material.h"
#include "Resources/Mesh.h"
#include "Resources/SubMesh.h"
#include "Resources/ShaderProgram.h"
#include "Resources/Shader.h"
#include "Resources/ResourceManager.h"
#include "Resources/Font.h"

#include "Core/TimeManager.h"
#include "Core/Engine.h"

#include "Render/Renderer.h"
#include "Render/Light.h"
#include "Render/LightManager.h"
#include "Render/Camera.h"
#include "Render/CameraManager.h"
#include "Render/PostProcessor.h"

#include "Scenes/GameObject.h"
#include "Scenes/SceneGraph.h"
#include "Scenes/SceneManager.h"
#include "Scenes/Scene.h"

#include "Interface/Widget.h"

#include "Scripts/ObjectScript.h"
#include "Scripts/LuaScript.h"
#include "Scripts/ShaderScript.h"

void Wrapper::LuaWrapper::Bind(sol::state& state)
{
    //std::cout << "Bind state at: " << &state << std::endl;

     /// --- Maths --- ///
     auto mathNamespace = state["Maths"].get_or_create<sol::table>();
     {
         // --- Arithmetic --- //
         mathNamespace["PI"] = PI;

         mathNamespace["roundInt"] = &Maths::roundInt;
         mathNamespace["floorInt"] = &Maths::floorInt;
         mathNamespace["ceilInt"]  = &Maths::ceilInt;
         mathNamespace["signof"]   = &Maths::signof;

         // Degrees/Radians conversions.
         mathNamespace["degToRad"] = &Maths::degToRad;
         mathNamespace["radToDeg"] = &Maths::radToDeg;

         // Clamp.
         mathNamespace["clamp"]      = &Maths::clamp;
         mathNamespace["clampUnder"] = &Maths::clampUnder;
         mathNamespace["clampAbove"] = &Maths::clampAbove;

         // Lerp.
         mathNamespace["lerp"]    = &Maths::lerp;
         mathNamespace["getLerp"] = &Maths::getLerp;
         mathNamespace["remap"]   = &Maths::remap;

         // Powers of 2.
         mathNamespace["isPowerOf2"]       = &Maths::isPowerOf2;
         mathNamespace["getPowerOf2Under"] = &Maths::getPowerOf2Under;
         mathNamespace["getPowerOf2Above"] = &Maths::getPowerOf2Above;

         // --- Vector2 --- //
         auto vec2 = mathNamespace.new_usertype<Maths::Vector2>(
             "Vector2",
             sol::constructors<
             Maths::Vector2(),
             Maths::Vector2(const float&),
             Maths::Vector2(const float&, const float&),
             Maths::Vector2(const Maths::Vector2&, const Maths::Vector2&),
             Maths::Vector2(const float&, const float&, const float&)
             >()
             );
    
         vec2["Add"] = sol::overload(
             [](const Maths::Vector2& v1, const Maths::Vector2& v2) -> Maths::Vector2 { return v1 + v2; },
             [](const Maths::Vector2& v1, const float& f) -> Maths::Vector2 { return v1 + f; }
         );
    
         vec2["Sub"] = sol::overload(
             [](const Maths::Vector2& v1, const Maths::Vector2& v2) -> Maths::Vector2 { return v1 - v2; },
             [](const Maths::Vector2& v1, const float& f) -> Maths::Vector2 { return v1 - f; }
         );
    
         vec2["Mult"] = sol::overload(
             [](const Maths::Vector2& v1, const Maths::Vector2& v2) -> Maths::Vector2 { return v1 * v2; },
             [](const Maths::Vector2& v1, const float& f) -> Maths::Vector2 { return v1 * f; }
         );
    
         vec2["Div"] = sol::overload(
             [](const Maths::Vector2& v1, const Maths::Vector2& v2) -> Maths::Vector2 { return v1 / v2; },
             [](const Maths::Vector2& v1, const float& f) -> Maths::Vector2 { return v1 / f; }
         );
    
         vec2["Equ"] = [](const Maths::Vector2& v1, const Maths::Vector2& v2) -> bool { return v1 == v2; };
    
         vec2["Dif"] = [](const Maths::Vector2& v1, const Maths::Vector2& v2) -> bool { return v1 != v2; };
    
         vec2["x"] = &Maths::Vector2::x;
         vec2["y"] = &Maths::Vector2::y;
    
         vec2["Dot"]   = &Maths::Vector2::Dot;
         vec2["Cross"] = &Maths::Vector2::Cross;
    
         vec2["GetLength"] = &Maths::Vector2::GetLength;
         vec2["SetLength"] = &Maths::Vector2::SetLength;
    
         vec2["Normalize"]     = &Maths::Vector2::Normalize;
         vec2["GetNormalized"] = &Maths::Vector2::GetNormalized;
    
         vec2["Negate"]     = &Maths::Vector2::Negate;
         vec2["GetNegated"] = &Maths::Vector2::GetNegated;
    
         vec2["CopySign"]      = &Maths::Vector2::CopySign;
         vec2["GetCopiedSign"] = &Maths::Vector2::GetCopiedSign;
    
         vec2["GetAngle"] = sol::overload(
             static_cast<float (Maths::Vector2::*)()                      const>(&Maths::Vector2::GetAngle),
             static_cast<float (Maths::Vector2::*)(const Maths::Vector2&) const>(&Maths::Vector2::GetAngle)
         );
    
         vec2["GetAngleSigned"] = &Maths::Vector2::GetAngleSigned;
    
         vec2["Rotate"] = &Maths::Vector2::Rotate;
         vec2["RotateAsPoint"] = &Maths::Vector2::RotateAsPoint;
    
         vec2["GetNormal"] = &Maths::Vector2::GetNormal;
    
         vec2["GetDistanceFromPoint"] = &Maths::Vector2::GetDistanceFromPoint;
    
         vec2["Lerp"] = &Maths::Vector2::Lerp;
    
         vec2["ToString"] = &Maths::Vector2::ToString;
    
         // --- Vector3 --- //
         auto vec3 = mathNamespace.new_usertype<Maths::Vector3>(
             "Vector3",
             sol::constructors<
             Maths::Vector3(),
             Maths::Vector3(const float&),
             Maths::Vector3(const float&, const float&, const float&),
             Maths::Vector3(const Maths::Vector3&, const Maths::Vector3&),
             Maths::Vector3(const Maths::Vector3&, const float&)
             >()
             );
    
         vec3["Add"] = sol::overload(
             [](const Maths::Vector3& v1, const Maths::Vector3& v2) -> Maths::Vector3 { return v1 + v2; },
             [](const Maths::Vector3& v1, const float& f) -> Maths::Vector3 { return v1 + f; }
         );
    
         vec3["Sub"] = sol::overload(
             [](const Maths::Vector3& v1, const Maths::Vector3& v2) -> Maths::Vector3 { return v1 - v2; },
             [](const Maths::Vector3& v1, const float& f) -> Maths::Vector3 { return v1 - f; }
         );
    
         vec3["Mult"] = sol::overload(
             [](const Maths::Vector3& v1, const Maths::Vector3& v2) -> Maths::Vector3 { return v1 * v2; },
             [](const Maths::Vector3& v1, const float& f) -> Maths::Vector3 { return v1 * f; }
         );
    
         vec3["Div"] = sol::overload(
             [](const Maths::Vector3& v1, const Maths::Vector3& v2) -> Maths::Vector3 { return v1 / v2; },
             [](const Maths::Vector3& v1, const float& f) -> Maths::Vector3 { return v1 / f; }
         );
    
         vec3["Equ"] = [](const Maths::Vector3& v1, const Maths::Vector3& v2) -> bool { return v1 == v2; };
    
         vec3["Dif"] = [](const Maths::Vector3& v1, const Maths::Vector3& v2) -> bool { return v1 != v2; };
    
         vec3["x"] = &Maths::Vector3::x;
         vec3["y"] = &Maths::Vector3::y;
         vec3["z"] = &Maths::Vector3::z;
    
         vec3["Dot"] = &Maths::Vector3::Dot;
         vec3["Cross"] = &Maths::Vector3::Cross;
    
         vec3["GetLength"] = &Maths::Vector3::GetLength;
         vec3["SetLength"] = &Maths::Vector3::SetLength;
    
         vec3["Normalize"] = &Maths::Vector3::Normalize;
         vec3["GetNormalized"] = &Maths::Vector3::GetNormalized;
    
         vec3["Negate"] = &Maths::Vector3::Negate;
         vec3["GetNegated"] = &Maths::Vector3::GetNegated;
    
         vec3["CopySign"] = &Maths::Vector3::CopySign;
         vec3["GetCopiedSign"] = &Maths::Vector3::GetCopiedSign;
    
         vec3["GetDistanceFromPoint"] = &Maths::Vector3::GetDistanceFromPoint;
    
         vec3["Lerp"] = &Maths::Vector3::Lerp;
    
         vec3["ToVector4"] = &Maths::Vector3::ToVector4;
         vec3["ToString"] = &Maths::Vector3::ToString;
    
         // --- Vector4 --- // 
         auto vec4 = mathNamespace.new_usertype<Maths::Vector4>(
             "Vector4",
             sol::constructors<
             Maths::Vector4(),
             Maths::Vector4(const float&, const float&, const float&, const float&),
             Maths::Vector4(const Maths::Vector3&, const float&)
             >()
             );
    
         vec4["Mult"] = [](const Maths::Vector4& v1, const Maths::Mat4& mt4) -> Maths::Vector4 { return v1 * mt4; };
    
         vec4["Equ"] = [](const Maths::Vector4& v1, const Maths::Vector4& v2) -> bool { return v1 == v2; };
    
         vec4["Dif"] = [](const Maths::Vector4& v1, const Maths::Vector4& v2) -> bool { return v1 != v2; };
    
         vec4["x"] = &Maths::Vector4::x;
         vec4["y"] = &Maths::Vector4::y;
         vec4["z"] = &Maths::Vector4::z;
         vec4["w"] = &Maths::Vector4::w;
    
         vec4["Homogenize"] = &Maths::Vector4::Homogenize;
         vec4["GetHomogenized"] = &Maths::Vector4::GetHomogenized;
    
         vec4["ToVector3"] = &Maths::Vector4::ToVector3;
         vec4["ToString"] = &Maths::Vector4::ToString;
    
         // --- AngleAxis --- //
         auto angleAxis = mathNamespace.new_usertype<Maths::AngleAxis>(
             "AngleAxis",
             sol::constructors<
             Maths::AngleAxis(),
             Maths::AngleAxis(const float&, const Maths::Vector3&),
             Maths::AngleAxis(const Maths::Quaternion&),
             Maths::AngleAxis(const Maths::Mat4&)
             >()
             );
    
         angleAxis["angle"] = &Maths::AngleAxis::angle;
         angleAxis["axis"] = &Maths::AngleAxis::axis;
    
         angleAxis["FromQuaternion"] = &Maths::AngleAxis::FromQuaternion;
         angleAxis["FromMatrix"] = &Maths::AngleAxis::FromMatrix;
    
         angleAxis["ToQuaternion"] = &Maths::AngleAxis::ToQuaternion;
         angleAxis["ToMatrix"] = &Maths::AngleAxis::ToMatrix;
         angleAxis["ToString"] = &Maths::AngleAxis::ToString;
    
         // --- Matrix<2, 2> --- //
         auto mat2 = mathNamespace.new_usertype<Maths::Mat2>(
             "Mat2",
             sol::constructors<
             Maths::Mat2(const bool&),
             Maths::Mat2(const Maths::Mat2&),
             Maths::Mat2(const float&, const float&,
                 const float&, const float&)
             >()
             );
    
         mat2["Add"] = sol::overload(
             [](const Maths::Mat2& m1, const Maths::Mat2& m2) -> Maths::Mat2 { return m1 + m2; },
             [](const Maths::Mat2& m1, const float& f) -> Maths::Mat2 { return m1 + f; }
         );
    
         mat2["Sub"] = sol::overload(
             [](const Maths::Mat2& m1, const Maths::Mat2& m2) -> Maths::Mat2 { return m1 - m2; },
             [](const Maths::Mat2& m1, const float& f) -> Maths::Mat2 { return m1 - f; }
         );
    
         mat2["Mult"] = sol::overload(
             [](const Maths::Mat2& m1, const Maths::Mat2& m2) -> Maths::Mat2 { return m1 * m2; },
             [](const Maths::Mat2& m1, const float& f) -> Maths::Mat2 { return m1 * f; }
         );
    
         mat2["Div"] = [](const Maths::Mat2& m1, const float& f) -> Maths::Mat2 { return m1 / f; };
    
         mat2["GetRow"] = [](Maths::Mat2& m, int index) { return m[index]; };
    
         mat2["Negate"] = [](const Maths::Mat2& m) { return -m; };
         mat2["Pow"] = &Maths::Mat2::Pow;
    
         mat2["GetRows"] = &Maths::Mat2::GetRows;
         mat2["GetColumns"] = &Maths::Mat2::GetColumns;
         mat2["GetMatrixValue"] = [](const Maths::Mat2& m, int i, int j) { return m[i][j]; };
         mat2["IsIdentity"] = &Maths::Mat2::IsIdentity;
         mat2["IsSymmetrical"] = &Maths::Mat2::IsSymmetrical;
    
         mat2["Det2"] = &Maths::Mat2::Det2;
         mat2["Inv2"] = &Maths::Mat2::Inv2;
    
         mat2["Transpose"] = &Maths::Mat2::Transpose;
         mat2["GetTransposed"] = &Maths::Mat2::GetTransposed;
    
         mat2["ToString"] = &Maths::Mat2::ToString;
    
         // --- Matrix<3, 3> --- //
         auto mat3 = mathNamespace.new_usertype<Maths::Mat3>(
             "Mat3",
             sol::constructors<
             Maths::Mat3(const bool&),
             Maths::Mat3(const Maths::Mat3&),
             Maths::Mat3(
                 const float&, const float&, const float&,
                 const float&, const float&, const float&,
                 const float&, const float&, const float&)
             >()
             );
    
         mat3["Add"] = sol::overload(
             [](const Maths::Mat3& m1, const Maths::Mat3& m2) -> Maths::Mat3 { return m1 + m2; },
             [](const Maths::Mat3& m1, const float& f) -> Maths::Mat3 { return m1 + f; }
         );
    
         mat3["Sub"] = sol::overload(
             [](const Maths::Mat3& m1, const Maths::Mat3& m2) -> Maths::Mat3 { return m1 - m2; },
             [](const Maths::Mat3& m1, const float& f) -> Maths::Mat3 { return m1 - f; }
         );
    
         mat3["Mult"] = sol::overload(
             [](const Maths::Mat3& m1, const Maths::Mat3& m2) -> Maths::Mat3 { return m1 * m2; },
             [](const Maths::Mat3& m1, const float& f) -> Maths::Mat3 { return m1 * f; }
         );
    
         mat3["Div"] = [](const Maths::Mat3& m1, const float& f) -> Maths::Mat3 { return m1 / f; };
    
         mat3["GetRow"] = [](Maths::Mat3& m, int index) { return m[index]; };
    
         mat3["Negate"] = [](const Maths::Mat3& m) { return -m; };
         mat3["Pow"] = &Maths::Mat3::Pow;
    
         mat3["GetRows"] = &Maths::Mat3::GetRows;
         mat3["GetColumns"] = &Maths::Mat3::GetColumns;
         mat3["GetMatrixValue"] = [](const Maths::Mat2& m, int i, int j) { return m[i][j]; };
         mat3["IsIdentity"] = &Maths::Mat3::IsIdentity;
         mat3["IsSymmetrical"] = &Maths::Mat3::IsSymmetrical;
    
         mat3["Inv3"] = &Maths::Mat3::Inv3;
         mat3["Det3"] = &Maths::Mat3::Det3;
    
         mat3["Transpose"] = &Maths::Mat3::Transpose;
         mat3["GetTransposed"] = &Maths::Mat3::GetTransposed;
    
         mat3["ToAngleAxis"] = &Maths::Mat3::ToAngleAxis;
         mat3["ToQuaternion"] = &Maths::Mat3::ToQuaternion;
         mat3["ToString"] = &Maths::Mat3::ToString;
    
         // --- Matrix<4, 4> --- //
         auto mat4 = mathNamespace.new_usertype<Maths::Mat4>(
             "Mat4",
             sol::constructors<
             Maths::Mat4(const bool&),
             Maths::Mat4(const Maths::Mat4&),
             Maths::Mat4(const Maths::Vector3&),
             Maths::Mat4(const Maths::AngleAxis&),
             Maths::Mat4(const Maths::Quaternion&),
             Maths::Mat4(
                 const float&, const float&, const float&, const float&,
                 const float&, const float&, const float&, const float&,
                 const float&, const float&, const float&, const float&,
                 const float&, const float&, const float&, const float&),
             Maths::Mat4(const Maths::Mat2&, const Maths::Mat2&, const Maths::Mat2&, const Maths::Mat2&)
             >()
             );
    
         mat4["Add"] = sol::overload(
             [](const Maths::Mat4& m1, const Maths::Mat4& m2) -> Maths::Mat4 { return m1 + m2; },
             [](const Maths::Mat4& m1, const float& f) -> Maths::Mat4 { return m1 + f; }
         );
    
         mat4["Sub"] = sol::overload(
             [](const Maths::Mat4& m1, const Maths::Mat4& m2) -> Maths::Mat4 { return m1 - m2; },
             [](const Maths::Mat4& m1, const float& f) -> Maths::Mat4 { return m1 - f; }
         );
    
         mat4["Mult"] = sol::overload(
             [](const Maths::Mat4& m1, const Maths::Mat4& m2) -> Maths::Mat4 { return m1 * m2; },
             [](const Maths::Mat4& m1, const float& f) -> Maths::Mat4 { return m1 * f; }
         );
    
         mat4["Div"] = [](const Maths::Mat4& m1, const float& f) -> Maths::Mat4 { return m1 / f; };
    
         mat4["FromTranslation"] = &Maths::Mat4::FromTranslation;
         mat4["FromPitch"] = &Maths::Mat4::FromPitch;
         mat4["FromRoll"] = &Maths::Mat4::FromRoll;
         mat4["FromYaw"] = &Maths::Mat4::FromYaw;
         mat4["FromEuler"] = &Maths::Mat4::FromEuler;
         mat4["FromAngleAxis"] = &Maths::Mat4::FromAngleAxis;
         mat4["FromQuaternion"] = &Maths::Mat4::FromQuaternion;
         mat4["FromScale"] = &Maths::Mat4::FromScale;
         mat4["FromTransform"] = &Maths::Mat4::FromTransform;
    
         mat4["GetRow"] = [](Maths::Mat4& m, int index) { return m[index]; };
    
         mat4["Negate"] = [](const Maths::Mat4& m) { return -m; };
         mat4["Pow"] = &Maths::Mat4::Pow;
    
         mat4["GetRows"] = &Maths::Mat4::GetRows;
         mat4["GetColumns"] = &Maths::Mat4::GetColumns;
         mat4["GetMatrixValue"] = [](const Maths::Mat4& m, int i, int j) { return m[i][j]; };
         mat4["IsIdentity"] = &Maths::Mat4::IsIdentity;
         mat4["IsSymmetrical"] = &Maths::Mat4::IsSymmetrical;
    
         mat4["Inv4"] = &Maths::Mat4::Inv4;
         mat4["Det4"] = &Maths::Mat4::Det4;
    
         mat4["Transpose"] = &Maths::Mat4::Transpose;
         mat4["GetTransposed"] = &Maths::Mat4::GetTransposed;
    
         mat4["ToAngleAxis"] = &Maths::Mat4::ToAngleAxis;
         mat4["ToQuaternion"] = &Maths::Mat4::ToQuaternion;
         mat4["ToString"] = &Maths::Mat4::ToString;
    
         // --- Transform --- //
         auto transform = mathNamespace.new_usertype<Maths::Transform>(
             "Transform",
             sol::constructors<
             Maths::Transform(const bool&),
             Maths::Transform(const Maths::Vector3&, const Maths::Quaternion&, const Maths::Vector3&, const bool&)
             >()
             );
    
         transform["GetPosition"]      = &Maths::Transform::GetPosition;
         transform["GetWorldPosition"] = &Maths::Transform::GetWorldPosition;
         transform["SetPosition"]      = &Maths::Transform::SetPosition;
         transform["Move"]             = &Maths::Transform::Move;
    
         transform["Forward"]    = &Maths::Transform::Forward;
         transform["Up"]         = &Maths::Transform::Up;
         transform["Right"]      = &Maths::Transform::Right;
         transform["SetForward"] = &Maths::Transform::SetForward;
    
         transform["GetRotation"] = &Maths::Transform::GetRotation;
         transform["GetEulerRot"] = &Maths::Transform::GetEulerRot;
         transform["SetRotation"] = &Maths::Transform::SetRotation;
         transform["Rotate"]       = &Maths::Transform::Rotate;
         transform["SetEulerRot"] = &Maths::Transform::SetEulerRot;
         transform["RotateEuler"] = &Maths::Transform::RotateEuler;
    
         transform["GetScale"]        = &Maths::Transform::GetScale;
         transform["GetUniformScale"] = &Maths::Transform::GetUniformScale;
         transform["SetScale"]        = &Maths::Transform::SetScale;
         transform["Scale"]           = &Maths::Transform::Scale;

         transform["IsCamera"]    = &Maths::Transform::IsCamera;
         transform["SetIsCamera"] = &Maths::Transform::SetIsCamera;
    
         transform["SetPosRot"] = &Maths::Transform::SetPosRot;
         transform["SetValues"] = &Maths::Transform::SetValues;
    
         transform["GetLocalMat"]  = &Maths::Transform::GetLocalMat;
         transform["GetParentMat"] = &Maths::Transform::GetParentMat;
         transform["GetWorldMat"]  = &Maths::Transform::GetWorldMat;
         transform["SetParentMat"] = &Maths::Transform::SetParentMat;
    
         transform["Lerp"] = &Maths::Transform::Lerp;
    
         // --- Quaternion --- //
         auto quaternion = mathNamespace.new_usertype<Maths::Quaternion>(
             "Quaternion",
             sol::constructors<
             Maths::Quaternion(),
             Maths::Quaternion(const float&),
             Maths::Quaternion(const float&, const float&, const float&, const float&),
             Maths::Quaternion(const Maths::Vector3&),
             Maths::Quaternion(const Maths::AngleAxis&),
             Maths::Quaternion(const Maths::Mat4&)
             >()
             );
    
         quaternion["Add"] = sol::overload(
             [](const Maths::Quaternion& q1, const Maths::Quaternion& q2) -> Maths::Quaternion { return q1 + q2; },
             [](const Maths::Quaternion& q1, const float& f) -> Maths::Quaternion { return q1 + f; }
         );
    
         quaternion["Sub"] = sol::overload(
             [](const Maths::Quaternion& q1, const Maths::Quaternion& q2) -> Maths::Quaternion { return q1 - q2; },
             [](const Maths::Quaternion& q1, const float& f) -> Maths::Quaternion { return q1 - f; }
         );
    
         quaternion["Mult"] = sol::overload(
             [](const Maths::Quaternion& q1, const Maths::Quaternion& q2) -> Maths::Quaternion { return q1 * q2; },
             [](const Maths::Quaternion& q1, const float& f) -> Maths::Quaternion { return q1 * f; }
         );
    
         quaternion["Div"] = sol::overload(
             [](const Maths::Quaternion& q1, const Maths::Quaternion& q2) -> Maths::Quaternion { return q1 / q2; },
             [](const Maths::Quaternion& q1, const float& f) -> Maths::Quaternion { return q1 / f; }
         );
    
         quaternion["Equ"] = [](const Maths::Quaternion& q1, const Maths::Quaternion& q2) -> bool { return q1 == q2; };
    
         quaternion["w"] = &Maths::Quaternion::w;
         quaternion["x"] = &Maths::Quaternion::x;
         quaternion["y"] = &Maths::Quaternion::y;
         quaternion["z"] = &Maths::Quaternion::z;
    
         quaternion["FromPitch"] = &Maths::Quaternion::FromPitch;
         quaternion["FromRoll"] = &Maths::Quaternion::FromRoll;
         quaternion["FromYaw"] = &Maths::Quaternion::FromYaw;
         quaternion["FromEuler"] = &Maths::Quaternion::FromEuler;
         quaternion["FromAngleAxis"] = &Maths::Quaternion::FromAngleAxis;
         quaternion["FromMatrix"] = &Maths::Quaternion::FromMatrix;
    
         quaternion["Dot"] = &Maths::Quaternion::Dot;
         quaternion["GetModulus"] = &Maths::Quaternion::GetModulus;
    
         quaternion["Normalize"] = &Maths::Quaternion::Normalize;
         quaternion["GetNormalized"] = &Maths::Quaternion::GetNormalized;
    
         quaternion["Conjugate"] = &Maths::Quaternion::Conjugate;
         quaternion["GetConjugate"] = &Maths::Quaternion::GetConjugate;
    
         quaternion["Inverse"] = &Maths::Quaternion::Inverse;
         quaternion["GetInverse"] = &Maths::Quaternion::GetInverse;
    
         quaternion["RotateQuat"] = &Maths::Quaternion::RotateQuat;
         quaternion["RotateVec"] = &Maths::Quaternion::RotateVec;
    
         quaternion["Lerp"] = &Maths::Quaternion::Lerp;
         quaternion["NLerp"] = &Maths::Quaternion::NLerp;
         quaternion["SLerp"] = &Maths::Quaternion::SLerp;
    
         quaternion["ToAngleAxis"] = &Maths::Quaternion::ToAngleAxis;
         quaternion["ToMatrix"] = &Maths::Quaternion::ToMatrix;
         quaternion["ToEuler"] = &Maths::Quaternion::ToEuler;
         quaternion["ToString"] = &Maths::Quaternion::ToString;
    
         // --- RGBA --- //
         auto rgba = mathNamespace.new_usertype<Maths::RGBA>(
             "RGBA",
             sol::constructors<
             Maths::RGBA(float),
             Maths::RGBA(float, float, float, float),
             Maths::RGBA(Maths::RGB, float)
             >()
             );
    
         rgba["Add"] = sol::overload(
             [](const Maths::RGBA& a, const Maths::RGBA& b) -> Maths::RGBA { return a + b; },
             [](const Maths::RGBA& a, float b) -> Maths::RGBA { return a + b; }
         );
    
         rgba["Sub"] = sol::overload(
             [](const Maths::RGBA& a, const Maths::RGBA& b) -> Maths::RGBA { return a - b; },
             [](const Maths::RGBA& a, float b) -> Maths::RGBA { return a - b; }
         );
    
         rgba["r"] = &Maths::RGBA::r;
         rgba["g"] = &Maths::RGBA::g;
         rgba["b"] = &Maths::RGBA::b;
         rgba["a"] = &Maths::RGBA::a;
    
         rgba["toRGB"] = &Maths::RGBA::toRGB;
    
         // --- RGB --- //
         auto rgb = mathNamespace.new_usertype<Maths::RGB>(
             "RGB",
             sol::constructors<
             Maths::RGB(float),
             Maths::RGB(float, float, float),
             Maths::RGB(Maths::RGBA)
             >()
             );
    
         rgb["r"] = &Maths::RGB::r;
         rgb["g"] = &Maths::RGB::g;
         rgb["b"] = &Maths::RGB::b;
    
         rgb["toRGBA"] = &Maths::RGB::toRGBA;
    
         // --- HSVA --- //
         auto hsva = mathNamespace.new_usertype<Maths::HSVA>(
             "HSVA",
             sol::constructors<
             Maths::HSVA(),
             Maths::HSVA(float),
             Maths::HSVA(float, float, float, float),
             Maths::HSVA(Maths::HSV, float)
             >()
             );
    
         hsva["h"] = &Maths::HSVA::h;
         hsva["s"] = &Maths::HSVA::s;
         hsva["v"] = &Maths::HSVA::v;
         hsva["a"] = &Maths::HSVA::a;
    
         hsva["toHSV"] = &Maths::HSVA::toHSV;
    
         // --- HSV --- //
         auto hsv = mathNamespace.new_usertype<Maths::HSV>(
             "HSV",
             sol::constructors<
             Maths::HSV(),
             Maths::HSV(float),
             Maths::HSV(float, float, float),
             Maths::HSV(Maths::HSVA)
             >()
             );
    
         hsv["h"] = &Maths::HSV::h;
         hsv["s"] = &Maths::HSV::s;
         hsv["v"] = &Maths::HSV::v;
    
         hsv["toHSVA"] = &Maths::HSV::toHSVA;
    
         auto tangentVertex = mathNamespace.new_usertype<Maths::TangentVertex>(
              "TangentVertex",
              sol::meta_function::equal_to,
              &Maths::TangentVertex::operator==
             );
    
         tangentVertex["pos"]       = &Maths::TangentVertex::pos;
         tangentVertex["uv"]        = &Maths::TangentVertex::uv;
         tangentVertex["normal"]    = &Maths::TangentVertex::normal;
         tangentVertex["tangent"]   = &Maths::TangentVertex::tangent;
         tangentVertex["bitangent"] = &Maths::TangentVertex::bitangent;
    
         mathNamespace["ColorGetHue"] = &Maths::ColorGetHue;
         mathNamespace["ColorLerp"]   = &Maths::ColorLerp;
         mathNamespace["BlendHSV"]    = &Maths::BlendHSV;
         mathNamespace["RGBAtoHSVA"]  = &Maths::RGBAtoHSVA;
         mathNamespace["HSVAtoRGBA"]  = &Maths::HSVAtoRGBA;
         mathNamespace["ColorShift"]  = &Maths::ColorShift;
     }
    
     /// --- Scenes --- ///
     auto sceneNamespace = state["Scenes"].get_or_create<sol::table>();
     {
         // --- ObjectTag --- //
         auto objectTag = sceneNamespace.new_usertype<Scenes::ObjectTag>(
             "ObjectTag",
             sol::constructors<
             Scenes::ObjectTag(std::string)
             >()
         );
         
         objectTag["name"]   = &Scenes::ObjectTag::name;
         objectTag["tagged"] = &Scenes::ObjectTag::tagged;
         
         objectTag["Tag"]   = &Scenes::ObjectTag::Tag;
         objectTag["Untag"] = &Scenes::ObjectTag::Untag;
    
         // --- GameObjectType --- //
         sceneNamespace.new_enum<Scenes::GameObjectType>("GameObjectType",
             {
                 { "Empty",      Scenes::GameObjectType::Empty  },
                 { "Model",      Scenes::GameObjectType::Model  },
                 { "Instanced",  Scenes::GameObjectType::Instanced  },
                 { "Camera",     Scenes::GameObjectType::Camera  },
                 { "Cubemap",    Scenes::GameObjectType::Cubemap  },
                 { "Canvas",     Scenes::GameObjectType::Canvas  },
                 { "Light",      Scenes::GameObjectType::Light  },
             }
         );
    
         sceneNamespace["GameObjectTypeToString"] = &Scenes::GameObjectTypeToString;
    
         // --- GameObject --- //
         auto gameObject = sceneNamespace.new_usertype<Scenes::GameObject>(
             "GameObject",
             sol::constructors<
             Scenes::GameObject(Scenes::Scene*, const size_t&, const std::string&, Scenes::GameObject*, const Scenes::GameObjectType&)
             >()
         );
         
         gameObject["name"]      = &Scenes::GameObject::name;
         gameObject["transform"] = &Scenes::GameObject::transform;
         //gameObject["rigidBody"] = &Scenes::GameObject::rigidBody;
    
         gameObject["PlayModeStart"]     = &Scenes::GameObject::PlayModeStart;
         gameObject["PlayModeLateStart"] = &Scenes::GameObject::PlayModeLateStart;
         gameObject["RecursiveUpdate"]   = &Scenes::GameObject::RecursiveUpdate;
         
         gameObject["AddScript"]    = &Scenes::GameObject::AddScript;
         gameObject["GetScript"]    = &Scenes::GameObject::GetScript;
         gameObject["RemoveScript"] = sol::overload(
             static_cast<void (Scenes::GameObject::*)(const Scripts::ObjectScript*)>(&Scenes::GameObject::RemoveScript),
             static_cast<void (Scenes::GameObject::*)(const size_t&)>(&Scenes::GameObject::RemoveScript)
         );
    
         gameObject["AddChild"]    = &Scenes::GameObject::AddChild;
         gameObject["RemoveChild"] = &Scenes::GameObject::RemoveChild;
         gameObject["SetParent"]   = &Scenes::GameObject::SetParent;
    
         gameObject["GetTags"]   = &Scenes::GameObject::GetTags;
         gameObject["AddTag"]    = &Scenes::GameObject::AddTag;
         gameObject["RemoveTag"] = &Scenes::GameObject::RemoveTag;
         gameObject["ClearTags"] = &Scenes::GameObject::ClearTags;
    
         gameObject["GetId"]            = &Scenes::GameObject::GetId;
         gameObject["GetType"]          = &Scenes::GameObject::GetType;
         gameObject["GetParent"]        = &Scenes::GameObject::GetParent;
         gameObject["GetChildren"]      = &Scenes::GameObject::GetChildren;
         gameObject["FindInChildren"]   = sol::overload(
             static_cast<Scenes::GameObject* (Scenes::GameObject::*)(const size_t&) const>(&Scenes::GameObject::FindInChildren),
             static_cast<Scenes::GameObject* (Scenes::GameObject::*)(const std::string&) const>(&Scenes::GameObject::FindInChildren)
         );
    
         // --- LightObject --- //
         auto lightObject = sceneNamespace.new_usertype<Scenes::LightObject>(
             "LightObject",
             sol::bases<Scenes::GameObject>(),
             sol::constructors<
             Scenes::LightObject(Scenes::Scene*, const size_t&, const std::string&, Render::Light*, Scenes::GameObject*)
             >()
             );
    
         lightObject["light"] = &Scenes::LightObject::light;
    
         // --- ModelObject --- //
         auto modelObject = sceneNamespace.new_usertype<Scenes::ModelObject>(
             "ModelObject",
             sol::bases<Scenes::GameObject>(),
             sol::constructors<
             Scenes::ModelObject(Scenes::Scene*, const size_t&, const std::string&, Resources::Mesh*, Scenes::GameObject*)
             >()
             );
    
         modelObject["mesh"] = &Scenes::ModelObject::mesh;
    
         modelObject["draw"] = &Scenes::ModelObject::Draw;
    
         // --- CameraObject --- //
         auto cameraObject = sceneNamespace.new_usertype<Scenes::CameraObject>(
             "CameraObject",
             sol::bases<Scenes::GameObject>(),
             sol::constructors<
             Scenes::CameraObject(Scenes::Scene*, const size_t&, const std::string&, Render::Camera*, Scenes::GameObject*)
             >()
             );
    
         cameraObject["camera"] = &Scenes::CameraObject::camera;
    
         // --- InstancedObject --- //
         auto instancedObject = sceneNamespace.new_usertype<Scenes::InstancedObject>(
             "InstancedObject",
             sol::bases<Scenes::GameObject>(),
             sol::constructors<
             Scenes::InstancedObject(Scenes::Scene*, const size_t&, const std::string&, Resources::Mesh*, const size_t&, Scenes::GameObject*)
             >()
             );
    
         instancedObject["instanceCount"] = &Scenes::InstancedObject::instanceCount;
         instancedObject["mesh"] = &Scenes::InstancedObject::mesh;
         instancedObject["transforms"] = &Scenes::InstancedObject::transforms;
    
         instancedObject["draw"] = &Scenes::InstancedObject::Draw;
    
         // --- CubemapObject --- //
         auto cubemapObject = sceneNamespace.new_usertype<Scenes::CubemapObject>(
             "CubemapObject",
             sol::bases<Scenes::GameObject>(),
             sol::constructors<
             Scenes::CubemapObject(Scenes::Scene*, const size_t&, const std::string&, Resources::Cubemap*, Scenes::GameObject*)
             >()
             );
    
         cubemapObject["mesh"] = &Scenes::CubemapObject::mesh;
         cubemapObject["cubemap"] = &Scenes::CubemapObject::cubemap;
    
         cubemapObject["Draw"] = &Scenes::CubemapObject::Draw;
    
         // --- CanvasObject --- //
         auto canvasObject = sceneNamespace.new_usertype<Scenes::CanvasObject>(
             "CanvasObject",
             sol::bases<Scenes::GameObject>(),
             sol::constructors<
             Scenes::CanvasObject(Scenes::Scene*, const size_t&, const std::string&, Scenes::GameObject*)
             >()
             );
    
         canvasObject["AddEmptyWidget"]  = &Scenes::CanvasObject::AddEmptyWidget;
         canvasObject["AddTextWidget"]   = &Scenes::CanvasObject::AddTextWidget;
         canvasObject["AddButtonWidget"] = &Scenes::CanvasObject::AddButtonWidget;
         canvasObject["RemoveWidget"]       = [](Scenes::CanvasObject& self, const Interface::Widget* widget){ self.RemoveWidget(widget); };
         canvasObject["RemoveWidgetWithId"] = [](Scenes::CanvasObject& self, const size_t& widgetId){ self.RemoveWidget(widgetId); };
         canvasObject["RemoveWidget"] = sol::overload(
             static_cast<void (Scenes::CanvasObject::*)(const Interface::Widget* widget)>(&Scenes::CanvasObject::RemoveWidget),
             static_cast<void (Scenes::CanvasObject::*)(const size_t& widgetId)>(&Scenes::CanvasObject::RemoveWidget)
         );
    
         canvasObject["FindWidget"] = sol::overload(
             static_cast<Interface::Widget* (Scenes::CanvasObject::*)(const size_t & widgetId)>(&Scenes::CanvasObject::FindWidget), 
             static_cast<Interface::Widget* (Scenes::CanvasObject::*)(const std::string & widgetname)>(&Scenes::CanvasObject::FindWidget)
             );
         canvasObject["GetRoot"]    = &Scenes::CanvasObject::GetRoot;
    
         canvasObject["Update"] = &Scenes::CanvasObject::Update;
         canvasObject["Draw"] = &Scenes::CanvasObject::Draw;
    
         // --- SceneState --- //
         sceneNamespace.new_enum<Scenes::SceneState>("SceneState",
             {
                 { "Unloaded", Scenes::SceneState::Unloaded  },
                 { "Loaded",   Scenes::SceneState::Loaded  },
                 { "Playing",  Scenes::SceneState::Playing  },
                 { "Paused",   Scenes::SceneState::Paused  },
             }
         );
             
         // --- Scene --- //
         auto scene = sceneNamespace.new_usertype<Scenes::Scene>(
             "Scene",
             sol::constructors<
             Scenes::Scene(const std::string&)
             >()
             );
    
         scene["Save"]   = &Scenes::Scene::Save;
         scene["Load"]   = &Scenes::Scene::Load;
         scene["Unload"] = &Scenes::Scene::Unload;
         scene["Start"]  = &Scenes::Scene::Start;
         scene["Pause"]  = &Scenes::Scene::Pause;
         scene["Resume"] = &Scenes::Scene::Resume;
         //scene["Stop"]   = &Scenes::Scene::Stop;
         scene["Update"] = &Scenes::Scene::Update;
         scene["Render"] = &Scenes::Scene::Render;
    
         scene["IsLoaded"]         = &Scenes::Scene::IsLoaded;
         scene["IsPlaying"]        = &Scenes::Scene::IsPlaying;
         scene["IsPaused"]         = &Scenes::Scene::IsPaused;
         scene["GetState"]         = &Scenes::Scene::GetState;
         scene["GetCameraManager"] = &Scenes::Scene::GetCameraManager;
         scene["GetLightManager"]  = &Scenes::Scene::GetLightManager;
         scene["GetSceneGraph"]    = &Scenes::Scene::GetSceneGraph;
    
         // --- SceneManager --- //
         auto sceneManager = sceneNamespace.new_usertype<Scenes::SceneManager>(
             "SceneManager",
             sol::constructors<
             Scenes::SceneManager()
             >()
             );
    
         sceneManager["Exists"] = &Scenes::SceneManager::Exists;
         sceneManager["Loaded"] = &Scenes::SceneManager::Loaded;
         sceneManager["Create"] = &Scenes::SceneManager::Create;
         sceneManager["Delete"] = &Scenes::SceneManager::Delete;
    
         sceneManager["Get"] = &Scenes::SceneManager::Get;
         sceneManager["GetActiveScene"] = &Scenes::SceneManager::GetActiveScene;
         sceneManager["SetActiveScene"] = &Scenes::SceneManager::SetActiveScene;
    
         sceneManager["StartActiveScene"]  = &Scenes::SceneManager::StartActiveScene;
         sceneManager["UpdateActiveScene"] = &Scenes::SceneManager::UpdateActiveScene;
         sceneManager["RenderActiveScene"] = &Scenes::SceneManager::RenderActiveScene;
     }
    
     /// --- Utils --- ///
     auto utilsNamespace = state["Utils"].get_or_create<sol::table>();
     {
         // --- SubMeshPipelineData --- //
         auto submeshPipelineData = utilsNamespace.new_usertype<Utils::SubMeshPipelineData>(
             "SubmeshPipelineData",
             sol::constructors<
             Utils::SubMeshPipelineData()
             >()
             );
    
         submeshPipelineData["vbo"] = &Utils::SubMeshPipelineData::VBO;
         submeshPipelineData["ebo"] = &Utils::SubMeshPipelineData::EBO;
         submeshPipelineData["vao"] = &Utils::SubMeshPipelineData::VAO;
    
         submeshPipelineData["isEmpty"] = &Utils::SubMeshPipelineData::IsEmpty;
    
         // --- RenderTexturePipelineData --- //
         auto renderTexturePipelineData = utilsNamespace.new_usertype<Utils::RenderTexturePipelineData>(
             "RenderTexturePipelineData",
             sol::constructors<
             Utils::RenderTexturePipelineData()
             >()
             );
    
         renderTexturePipelineData["fbo"] = &Utils::RenderTexturePipelineData::FBO;
         renderTexturePipelineData["rbo"] = &Utils::RenderTexturePipelineData::RBO;
         renderTexturePipelineData["id"] = &Utils::RenderTexturePipelineData::ID;
    
         renderTexturePipelineData["isEmpty"] = &Utils::RenderTexturePipelineData::IsEmpty;
    
         // --- ScreenRectPipelineData --- //
         auto screenRectPipelineData = utilsNamespace.new_usertype<Utils::ScreenRectPipelineData>(
             "ScreenRectPipelineData",
             sol::constructors<
             Utils::ScreenRectPipelineData()
             >()
             );
    
         screenRectPipelineData["vao"] = &Utils::ScreenRectPipelineData::VAO;
         screenRectPipelineData["vbo"] = &Utils::ScreenRectPipelineData::VBO;
    
         screenRectPipelineData["isEmpty"] = &Utils::ScreenRectPipelineData::IsEmpty;
     }
    
     /// --- Resources --- ///
     auto resourcesNamespace = state["Resources"].get_or_create<sol::table>();
     {
         // --- Resource --- ///
         auto resource = resourcesNamespace.new_usertype<Resources::Resource>(
             "Resource",
             sol::no_constructor
         );
             
         //auto pure_send_to_pipeline = []() -> void { throw std::logic_error("Pure virtual function called"); };
             
         //resource["load"] = &Resources::Resource::Load;
         //resource["sendToPipeline"] = pure_send_to_pipeline;
             
         resource["GetName"]           = &Resources::Resource::GetName;
         resource["GetType"]           = &Resources::Resource::GetType;
         resource["IsLoaded"]          = &Resources::Resource::IsLoaded;
         resource["WasSentToPipeline"] = &Resources::Resource::WasSentToPipeline;
         resource["IsUsed"]            = &Resources::Resource::IsUsed;
    
         resource["SetLoadingDone"] = &Resources::Resource::SetLoadingDone;
         resource["AddUser"]        = &Resources::Resource::AddUser;
         resource["DelUser"]        = &Resources::Resource::DelUser;
    
         resourcesNamespace["ResourceTypeToString"] = &Resources::ResourceTypeToString;
    
         resourcesNamespace.new_enum<Resources::ResourceType>("ResourceType",
             {
                 { "Undefined",  Resources::ResourceType::Undefined  },
                 { "Texture",  Resources::ResourceType::Texture  },
                 { "RenderTexture",  Resources::ResourceType::RenderTexture  },
                 { "Cubemap",  Resources::ResourceType::Cubemap  },
                 { "Material",  Resources::ResourceType::Material  },
                 { "Mesh",  Resources::ResourceType::Mesh  },
                 { "VertexShader",  Resources::ResourceType::VertexShader  },
                 { "FragmentShader",  Resources::ResourceType::FragmentShader  },
                 { "ShaderProgram",  Resources::ResourceType::ShaderProgram  },
                 { "ShaderProgram",  Resources::ResourceType::Font },
             }
         );
    
         // --- Texture --- //
         auto texture = resourcesNamespace.new_usertype<Resources::Texture>(
              "Texture",
              sol::bases<Resources::Resource>(),
              sol::constructors<
              Resources::Texture(const std::string&)
              >()
          );
    
         texture["Load"] = &Resources::Texture::Load;
         texture["SendToPipeline"] = &Resources::Texture::SendToPipeline;
         texture["SetFlippedVertically"] = &Resources::Texture::SetFlippedVertically;
    
         texture["SetId"] = &Resources::Texture::SetId;
    
         texture["GetId"] = &Resources::Texture::GetId;
         texture["GetWidth"] = &Resources::Texture::GetWidth;
         texture["GetHeight"] = &Resources::Texture::GetHeight;
         texture["GetData"] = &Resources::Texture::GetData;
         texture["GetChannels"] = &Resources::Texture::GetChannels;
    
         texture["GetResourceType"] = &Resources::Texture::GetResourceType;
    
         // --- Material --- //
         auto material = resourcesNamespace.new_usertype<Resources::Material>(
              "Material",
              sol::bases<Resources::Resource>(),
              sol::constructors<
              Resources::Material(const std::string&)
              >()
          );
    
         material["ambient"] = &Resources::Material::ambient;
         material["diffuse"] = &Resources::Material::diffuse;
         material["specular"] = &Resources::Material::specular;
    
         material["shininess"] = &Resources::Material::shininess;
         material["transparency"] = &Resources::Material::transparency;
    
    
         material["Load"] = &Resources::Material::Load;
         material["SendToPipeline"] = &Resources::Material::SendToPipeline;
    
         material["SetParams"] = &Resources::Material::SetParams;
         
         material["GetResourceType"] = &Resources::Material::GetResourceType;
    
         // --- ShaderProgram --- //
         auto shaderProgram = resourcesNamespace.new_usertype<Resources::ShaderProgram>(
              "ShaderProgram",
              sol::bases<Resources::Resource>(),
              sol::constructors<
              Resources::ShaderProgram(const std::string&)
              >()
         );
    
         shaderProgram["AttachShader"] = &Resources::ShaderProgram::AttachShader;
         //shaderProgram["attachShaders"] = sol::overload(
         //    static_cast<void(Resources::ShaderProgram::*)(size_t, ...)>(&Resources::ShaderProgram::AttachShaders)
         //),
    
         shaderProgram["Load"]           = &Resources::ShaderProgram::Load;
         shaderProgram["SendToPipeline"] = &Resources::ShaderProgram::SendToPipeline;
    
         shaderProgram["SetScript"] = &Resources::ShaderProgram::SetScript;
         shaderProgram["SetId"]     = &Resources::ShaderProgram::SetId;
    
         shaderProgram["GetId"]              = &Resources::ShaderProgram::GetId;
         shaderProgram["GetAttachedShaders"] = &Resources::ShaderProgram::GetAttachedShaders;
         shaderProgram["GetShaderScript"]    = &Resources::ShaderProgram::GetShaderScript;
         
         shaderProgram["GetResourceType"]    = &Resources::ShaderProgram::GetResourceType;
    
         resourcesNamespace.new_enum<Resources::SubMeshType>("SubMeshType",
             {
                 { "Static",   Resources::SubMeshType::Static   },
                 { "Animated", Resources::SubMeshType::Animated },
             }
         );
        
         // --- SubMesh --- //
         auto subMesh = resourcesNamespace.new_usertype<Resources::SubMesh>(
              "Mesh",
              sol::bases<Resources::Resource>(),
              sol::no_constructor
              );
    
         subMesh["GetName"]           = &Resources::SubMesh::GetName;
         subMesh["GetType"]           = &Resources::SubMesh::GetType;
         subMesh["GetVertexCount"]    = &Resources::SubMesh::GetVertexCount;
         subMesh["GetPipelineData"]   = &Resources::SubMesh::GetPipelineData;
         subMesh["IsLoaded"]          = &Resources::SubMesh::IsLoaded;
         subMesh["WasSentToPipeline"] = &Resources::SubMesh::WasSentToPipeline;
         subMesh["GetShaderProgram"]  = &Resources::SubMesh::GetShaderProgram;
         subMesh["GetMaterial"]       = &Resources::SubMesh::GetMaterial;
    
         subMesh["SetLoadingDone"]          = &Resources::SubMesh::SetLoadingDone;
         subMesh["SetPipelineTransferDone"] = &Resources::SubMesh::SetPipelineTransferDone;
         subMesh["SetPipelineData"]         = &Resources::SubMesh::SetPipelineData;
         subMesh["SetShaderProgram"]        = &Resources::SubMesh::SetShaderProgram;
         subMesh["SetMaterial"]             = &Resources::SubMesh::SetMaterial;
          
         // --- StaticSubMesh --- //
         auto staticSubMesh = resourcesNamespace.new_usertype<Resources::StaticSubMesh>(
              "StaticSubMesh",
              sol::bases<Resources::SubMesh>(),
              sol::constructors<
              Resources::StaticSubMesh(const std::string&)
              >()
              );
         
         staticSubMesh["LoadVertices"] = sol::overload(
             static_cast<void (Resources::StaticSubMesh::*)(const std::array<std::vector<float>, 3>& vertexData, std::array<std::vector<uint32_t>, 3> vertexIndices)>(&Resources::StaticSubMesh::LoadVertices),
             static_cast<void (Resources::StaticSubMesh::*)(const std::vector<Maths::TangentVertex>& vertexData, const std::vector<unsigned int>& vertexIndices)>(&Resources::StaticSubMesh::LoadVertices)
         );
         
         staticSubMesh["GetVertices"] = &Resources::StaticSubMesh::GetVertices;
         staticSubMesh["GetIndices"]  = &Resources::StaticSubMesh::GetIndices;
    
         // --- AnimatedSubMesh --- //
         auto animatedSubMesh = resourcesNamespace.new_usertype<Resources::AnimatedSubMesh>(
              "AnimatedSubMesh",
              sol::bases<Resources::SubMesh>(),
              sol::constructors<
              Resources::AnimatedSubMesh(const std::string&)
              >()
              );
    
         animatedSubMesh["LoadVertices"]           = &Resources::AnimatedSubMesh::LoadVertices;
    
         //animatedSubMesh["SendVerticesToPipeline"] = &Resources::AnimatedSubMesh::SendVerticesToPipeline;
         
         animatedSubMesh["SetBoneMatricesBuffer"] = &Resources::AnimatedSubMesh::SetBoneMatricesBuffer;
    
         animatedSubMesh["GetVertices"]           = &Resources::AnimatedSubMesh::GetVertices;
         animatedSubMesh["GetIndices"]            = &Resources::AnimatedSubMesh::GetIndices;
         animatedSubMesh["GetBoneMatricesBuffer"] = &Resources::AnimatedSubMesh::GetBoneMatricesBuffer;
    
         // --- Mesh --- //
         auto mesh = resourcesNamespace.new_usertype<Resources::Mesh>(
              "Mesh",
              sol::bases<Resources::Resource>(),
              sol::constructors<
              Resources::Mesh(const std::string&)
              >()
              );
    
         mesh["Load"]           = &Resources::Mesh::Load;
         mesh["SendToPipeline"] = &Resources::Mesh::SendToPipeline;
    
         mesh["AreSubMeshesLoaded"]          = &Resources::Mesh::AreSubMeshesLoaded;
         mesh["WereSubMeshesSentToPipeline"] = &Resources::Mesh::WereSubMeshesSentToPipeline;
         
         mesh["GetVertexCount"]  = &Resources::Mesh::GetVertexCount;
         mesh["GetSubMeshCount"] = &Resources::Mesh::GetSubMeshCount;
         //mesh["GetAnimator"]     = &Resources::Mesh::GetAnimator; //TODO: Bind Animator
         //mesh["GetSkeleton"]     = &Resources::Mesh::GetSkeleton; //TODO: Bind Skeleton
         //mesh["SetSkeleton"]     = &Resources::Mesh::SetSkeleton;
    
         mesh["GetResourceType"] = &Resources::Mesh::GetResourceType;
    
         // --- CubemapSide --- //
         resourcesNamespace.new_enum<Resources::CubemapSide>(
              "CubemapSide",
              {
                  { "Right",  Resources::CubemapSide::Right },
                  { "Left",   Resources::CubemapSide::Left },
                  { "Top",    Resources::CubemapSide::Top },
                  { "Bottom", Resources::CubemapSide::Bottom },
                  { "Front",  Resources::CubemapSide::Front },
                  { "Back",   Resources::CubemapSide::Back },
              }
         );
    
         // --- Cubemap --- //
         auto cubemap = resourcesNamespace.new_usertype<Resources::Cubemap>(
              "Cubemap",
              sol::bases<Resources::Resource>(),
              sol::constructors<
              Resources::Cubemap(const std::string&)
              >()
              );
    
          cubemap["id"] = &Resources::Cubemap::id;
          //cubemap["sides"] = &Resources::Cubemap::sides; TODO: cannot bind since array....
    
          cubemap["Load"] = &Resources::Cubemap::Load;
          cubemap["SendToPipeline"] = &Resources::Cubemap::SendToPipeline;
    
          cubemap["SetCubemapSide"] = &Resources::Cubemap::SetCubemapSide;
    
          cubemap["GetResourceType"] = &Resources::Cubemap::GetResourceType;
    
         // --- VertexShader --- //
         auto vertexShader = resourcesNamespace.new_usertype<Resources::VertexShader>(
             "VertexShader",
             sol::bases<Resources::Resource>(),
             sol::constructors<
             Resources::VertexShader(const std::string&)
             >()
             );
    
         vertexShader["Load"] = &Resources::VertexShader::Load;
         vertexShader["SendToPipeline"] = &Resources::VertexShader::SendToPipeline;
    
         vertexShader["SetId"] = &Resources::VertexShader::SetId;
    
         vertexShader["GetId"] = &Resources::VertexShader::GetId;
         vertexShader["GetSourceCode"] = &Resources::VertexShader::GetSourceCode;
         vertexShader["GetResourceType"] = &Resources::VertexShader::GetResourceType;
    
         // --- FragmentShader --- //
         auto fragmentShader = resourcesNamespace.new_usertype<Resources::FragmentShader>(
             "FragmentShader",
             sol::bases<Resources::Resource>(),
             sol::constructors<
             Resources::FragmentShader(const std::string&)
             >()
             );
    
         fragmentShader["Load"] = &Resources::FragmentShader::Load;
         fragmentShader["SendToPipeline"] = &Resources::FragmentShader::SendToPipeline;
    
         fragmentShader["SetId"] = &Resources::FragmentShader::SetId;
    
         fragmentShader["GetId"] = &Resources::FragmentShader::GetId;
         fragmentShader["GetSourceCode"] = &Resources::FragmentShader::GetSourceCode;
         fragmentShader["GetResourceType"] = &Resources::FragmentShader::GetResourceType;
    
         // --- ResourceManager --- //
         auto resourceManager = resourcesNamespace.new_usertype<Resources::ResourceManager>(
             "ResourceManager",
             sol::bases<Resources::Resource>(),
             sol::constructors<
             Resources::ResourceManager()
             >()
             );
    
         resourceManager["LoadFile"]   = &Resources::ResourceManager::LoadFile;
         resourceManager["UnloadFile"] = &Resources::ResourceManager::UnloadFile;
    
         resourceManager["GetFileResources"] = &Resources::ResourceManager::GetFileResources;
         
         resourceManager["CreateCubemap"]        = [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::Cubemap>(name, async); };
         resourceManager["CreateMaterial"]       = [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::Material>(name, async); };
         resourceManager["CreateMesh"]           = [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::Mesh>(name, async); };
         resourceManager["CreateRenderTexture"]  = [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::RenderTexture>(name, async); };
         resourceManager["CreateTexture"]        = [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::Texture>(name, async); };
         resourceManager["CreateVertexShader"]   = [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::VertexShader>(name, async); };
         resourceManager["CreateFragmentShader"] = [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::FragmentShader>(name, async); };
         resourceManager["CreateShaderProgram"]  = [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::ShaderProgram>(name, async); };
    
         resourceManager["GetCubemap"]        = &Resources::ResourceManager::Get<Resources::Cubemap>;
         resourceManager["GetMaterial"]       = &Resources::ResourceManager::Get<Resources::Material>;
         resourceManager["GetMesh"]           = &Resources::ResourceManager::Get<Resources::Mesh>;
         resourceManager["GetRenderTexture"]  = &Resources::ResourceManager::Get<Resources::RenderTexture>;
         resourceManager["GetTexture"]        = &Resources::ResourceManager::Get<Resources::Texture>;
         resourceManager["GetVertexShader"]   = &Resources::ResourceManager::Get<Resources::VertexShader>;
         resourceManager["GetFragmentShader"] = &Resources::ResourceManager::Get<Resources::FragmentShader>;
         resourceManager["GetShaderProgram"]  = &Resources::ResourceManager::Get<Resources::ShaderProgram>;
         
         resourceManager["Delete"] = &Resources::ResourceManager::Delete;
    
         resourceManager["IsFileLoaded"]         = &Resources::ResourceManager::IsFileLoaded;
         resourceManager["Exists"]               = &Resources::ResourceManager::Exists;
         resourceManager["InPipeline"]           = &Resources::ResourceManager::InPipeline;
         resourceManager["LoadDefaultResources"] = &Resources::ResourceManager::LoadDefaultResources;
    
         // --- ResourceManager --- //
         auto font = resourcesNamespace.new_usertype<Resources::Font>(
             "Font",
             sol::bases<Resources::Resource>(),
             sol::constructors<
             Resources::Font(const std::string&)
             >()
             );
    
         font["Load"] = &Resources::Font::Load;
         font["SendToPipeline"] = &Resources::Font::SendToPipeline;
    
         font["GetCharacter"] = &Resources::Font::GetCharacter;
    
         font["GetResourceType"] = &Resources::Font::GetResourceType;
    
         auto character = resourcesNamespace.new_usertype<Resources::Character>(
             "Character",
             sol::bases<Resources::Resource>(),
             sol::constructors<
             Resources::Character(),
             Resources::Character(uint, Maths::Vector2, Maths::Vector2, uint)
             >()
             );
    
         character["GetTextureID"] = &Resources::Character::GetTextureID;
         character["GetSize"]      = &Resources::Character::GetSize;
         character["GetBearing"]   = &Resources::Character::GetBearing;
         character["GetAdvance"]   = &Resources::Character::GetAdvance;
     }
    
     /// --- Scripts --- ///
     auto scriptsNamespace = state["Scripts"].get_or_create<sol::table>();
     {
         // --- ObjectScript --- //
         auto objectScript = scriptsNamespace.new_usertype<Scripts::ObjectScript>(
             "ObjectScript",
             sol::constructors<
             Scripts::ObjectScript()
             >()
             );
    
         objectScript["Start"]      = &Scripts::ObjectScript::Start;
         objectScript["LateStart"]  = &Scripts::ObjectScript::LateStart;
         objectScript["Update"]     = &Scripts::ObjectScript::Update;
         objectScript["LateUpdate"] = &Scripts::ObjectScript::LateUpdate;
    
         auto luaScript = scriptsNamespace.new_usertype<Scripts::LuaScript>(
             "LuaScript",
             sol::constructors<
             Scripts::LuaScript(const std::string&)
             >()
             );
    
         luaScript["GetState"] = &Scripts::LuaScript::GetState;
         luaScript["GetName"] = &Scripts::LuaScript::GetName;
    
         // --- ShaderScript --- //
         auto shaderScript = scriptsNamespace.new_usertype<Scripts::ShaderScript>(
             "ShaderScript",
             sol::constructors<
             Scripts::ShaderScript()
             >()
             );
    
         shaderScript["shader"]   = &Scripts::ShaderScript::shader;
         shaderScript["renderer"] = &Scripts::ShaderScript::renderer;
         shaderScript["engine"]   = &Scripts::ShaderScript::engine;
    
         // --- MeshShaderScript --- //
         auto meshShaderScript = scriptsNamespace.new_usertype<Scripts::MeshShaderScript>(
             "ShaderScript",
             sol::bases<Scripts::ShaderScript>(),
             sol::no_constructor
             );
    
         meshShaderScript["subMesh"]      = &Scripts::MeshShaderScript::subMesh;
         meshShaderScript["material"]     = &Scripts::MeshShaderScript::material;
         meshShaderScript["modelMat"]     = &Scripts::MeshShaderScript::modelMat;
         meshShaderScript["camera"]       = &Scripts::MeshShaderScript::camera;
         meshShaderScript["lightManager"] = &Scripts::MeshShaderScript::lightManager;
    
         // --- ScreenShaderScript --- //
         auto screenShaderScript = scriptsNamespace.new_usertype<Scripts::ScreenShaderScript>(
             "ScreenShaderScript",
             sol::bases<Scripts::ShaderScript>(),
             sol::no_constructor
             );
    
         screenShaderScript["textureID"]     = &Scripts::ScreenShaderScript::textureID;
         screenShaderScript["rectPos"]       = &Scripts::ScreenShaderScript::rectPos;
         screenShaderScript["rectSize"]      = &Scripts::ScreenShaderScript::rectSize;
         screenShaderScript["screenSize"]    = &Scripts::ScreenShaderScript::screenSize;
         screenShaderScript["postProcessor"] = &Scripts::ScreenShaderScript::postProcessor;
     }
    
     /// --- Core --- ///
     auto coreNamespace = state["Core"].get_or_create<sol::table>();
     {
         // --- WindowParams --- //
         auto windowParams = coreNamespace.new_usertype<Core::WindowParams>(
             "WindowParams",
             sol::no_constructor
             );
    
         windowParams["name"] = &Core::WindowParams::name;
         windowParams["width"] = &Core::WindowParams::width;
         windowParams["height"] = &Core::WindowParams::height;
         windowParams["posX"] = &Core::WindowParams::posX;
         windowParams["posY"] = &Core::WindowParams::posY;
         
         // --- WindowInputs --- //
         auto windowInputs = coreNamespace.new_usertype<Core::WindowInputs>(
             "WindowInputs",
             sol::no_constructor
             );
    
         windowInputs["dirMovement"]      = &Core::WindowInputs::movement;
         windowInputs["mousePos"]         = &Core::WindowInputs::mousePos;
         windowInputs["mouseDelta"]       = &Core::WindowInputs::mouseDelta;
         windowInputs["mouseLeftClick"]   = &Core::WindowInputs::mouseLeftClick;
         windowInputs["mouseMiddleClick"] = &Core::WindowInputs::mouseMiddleClick;
         windowInputs["mouseRightClick"]  = &Core::WindowInputs::mouseRightClick;
    
         // --- InputKeys --- //
         auto inputKeys = coreNamespace.new_usertype<Core::InputKeys>(
             "InputKeys",
             sol::constructors<
             Core::InputKeys()
             >()
             );
    
         inputKeys["exit"]      = &Core::InputKeys::exit;
         inputKeys["movement"]  = &Core::InputKeys::movement;
    
         // --- Window --- //
         auto window = coreNamespace.new_usertype<Core::Window>(
             "Window",
             sol::constructors<
             Core::Window(const Core::WindowParams&)
             >()
             );
    
         window["LockInputs"] = &Core::Window::LockInputs;
         window["LockHideCursor"] = &Core::Window::LockHideCursor;
         window["Update"] = &Core::Window::Update;
         window["SwapBuffers"] = &Core::Window::SwapBuffers;
         window["Close"] = &Core::Window::Close;
    
         window["SetName"] = &Core::Window::SetName;
         window["SetWidth"] = &Core::Window::SetWidth;
         window["SetHeight"] = &Core::Window::SetHeight;
         window["SetSize"] = &Core::Window::SetSize;
         window["SetPosX"] = &Core::Window::SetPosX;
         window["SetPosY"] = &Core::Window::SetPosY;
         window["SetPos"] = &Core::Window::SetPos;
         window["SetVsync"] = &Core::Window::SetVsync;
         window["SetExitKey"] = &Core::Window::SetExitKey;
         window["RemoveExitKey"] = &Core::Window::RemoveExitKey;
         window["SetViewportSize"] = &Core::Window::SetViewportSize;
    
         window["GetName"] = &Core::Window::GetName;
         window["GetWidth"] = &Core::Window::GetWidth;
         window["GetHeight"] = &Core::Window::GetHeight;
         window["GetSize"] = &Core::Window::GetSize;
         window["GetViewportSize"] = &Core::Window::GetViewportSize;
         window["GetAspect"] = &Core::Window::GetAspect;
         window["GetPosX"] = &Core::Window::GetPosX;
         window["GetPosY"] = &Core::Window::GetPosY;
         window["GetPos"] = &Core::Window::GetPos;
         window["GetInputs"] = &Core::Window::GetInputs;
         //window["GetWindowContext"] = &Core::Window::GetWindowContext;
         window["ShouldClose"] = &Core::Window::ShouldClose;
         
         // --- TimeManager --- //
         auto timeManager = coreNamespace.new_usertype<Core::TimeManager>(
              "TimeManager",
              sol::constructors<
              Core::TimeManager(const int&, const bool&)
              >()
              );
    
         timeManager["SetVsyncEnabled"] = &Core::TimeManager::SetVsyncEnabled;
         timeManager["SetTargetFPS"]    = &Core::TimeManager::SetTargetFPS;
    
         timeManager["IsVsyncEnabled"] = &Core::TimeManager::IsVsyncEnabled;
         timeManager["GetTargetFPS"]   = &Core::TimeManager::GetTargetFPS;
         timeManager["FPS"]            = &Core::TimeManager::FPS;
         timeManager["DeltaTime"]      = &Core::TimeManager::DeltaTime;
    
         timeManager["CanStartNextFrame"] = &Core::TimeManager::CanStartNextFrame;
         timeManager["NewFrame"]          = &Core::TimeManager::NewFrame;
    
         //state.set("_time", Core::Engine::Get()->GetTimeManager()); TODO: WAY TO BIND DATA
    
         // --- Engine --- //
         auto engine = coreNamespace.new_usertype<Core::Engine>(
              "Engine",
              sol::no_constructor
              );
    
         engine["Create"] = &Core::Engine::Create;
         engine["Get"] = &Core::Engine::Get;
         engine["Destroy"] = &Core::Engine::Destroy;
    
         engine["Init"] = &Core::Engine::Init;
         engine["ExecuteFrame"] = &Core::Engine::ExecuteFrame;
         engine["EndFrame"] = &Core::Engine::EndFrame;
         engine["Quit"] = &Core::Engine::Quit;
         engine["Release"] = &Core::Engine::Release;
    
         //engine["GetEventDispatcher"] = &Core::Engine::GetEventDispatcher;
         engine["GetLogger"] = &Core::Engine::GetLogger;
         engine["GetWindow"] = &Core::Engine::GetWindow;
         engine["GetTimeManager"] = &Core::Engine::GetTimeManager;
         //engine["GetThreadManager"] = &Core::Engine::GetThreadManager;
         engine["GetRenderer"] = &Core::Engine::GetRenderer;
         engine["GetResourceManager"] = &Core::Engine::GetResourceManager;
         engine["GetSceneManager"] = &Core::Engine::GetSceneManager;
         engine["GetPostProcessor"] = &Core::Engine::GetPostProcessor;
     }
    
     /// --- Debug --- ///
     auto debugNamespace = state["Debug"].get_or_create<sol::table>();
     {
         // --- LogType --- //
         debugNamespace.new_enum<Debug::LogType>("LogType",
             {
                 { "Info",  Debug::LogType::Info  },
                 { "Warning",  Debug::LogType::Warning  },
                 { "Error",  Debug::LogType::Error  },
             }
         );
         
         debugNamespace["LogTypeToStr"] = &Debug::LogTypeToStr;
         debugNamespace["Assertion"] = &Debug::Assertion;
    
         // --- Logger --- //
         auto logger = debugNamespace.new_usertype<Debug::Logger>(
             "Logger",
             sol::constructors<
             Debug::Logger(),
             Debug::Logger(const std::string&)
             >()
             );
    
         logger["SetLogFilename"] = &Debug::Logger::SetLogFilename;
         logger["SaveLogsToFile"] = &Debug::Logger::SaveLogsToFile;
         logger["LogToConsole"] = &Debug::Logger::LogToConsole;
         logger["LogToFile"] = &Debug::Logger::LogToFile;
         logger["LogToAll"] = &Debug::Logger::LogToAll;
    
         logger["GetLogs"] = &Debug::Logger::GetLogs;
     }
    
     /// --- Render --- ///
     auto renderNamespace = state["Render"].get_or_create<sol::table>();
     {
         // --- Light --- //
         auto light = renderNamespace.new_usertype<Render::Light>(
             "Light",
             sol::constructors<
             Render::Light(const uint&)
             >()
             );
    
         light["transform"] = &Render::Light::transform;
         
         light["ambient"] = &Render::Light::ambient;
         light["diffuse"] = &Render::Light::diffuse;
         light["specular"] = &Render::Light::specular;
         
         light["constant"] = &Render::Light::constant;
         light["linear"] = &Render::Light::linear;
         light["quadratic"] = &Render::Light::quadratic;
         
         light["outerCutoff"] = &Render::Light::outerCutoff;
         light["innerCutoff"] = &Render::Light::innerCutoff;
    
         light["GetId"] = &Render::Light::GetId;
         
         // --- LightManager --- //
         auto lightManager = renderNamespace.new_usertype<Render::LightManager>(
             "LightManager",
             sol::constructors<
             Render::LightManager()
             >()
             );
    
         lightManager["Create"] = &Render::LightManager::Create;
         lightManager["Get"] = &Render::LightManager::Get;
         //lightManager["GetLights"] = &Render::LightManager::GetLights;
         lightManager["Remove"] = sol::overload(
             static_cast<void (Render::LightManager::*)(const Render::Light*)>(&Render::LightManager::Remove),
             static_cast<void (Render::LightManager::*)(const uint&)>(&Render::LightManager::Remove)
         );
    
         // --- CameraParams --- //
         auto cameraParams = renderNamespace.new_usertype<Render::CameraParams>(
             "CameraParams",
             sol::constructors<
                 Render::CameraParams(),
                 Render::CameraParams(const unsigned int&, const unsigned int&),
                 Render::CameraParams(const unsigned int&, const unsigned int&, const float&, const float&, const float&)
             >()
         );
    
         cameraParams["width"] = &Render::CameraParams::width;
         cameraParams["height"] = &Render::CameraParams::height;
         
         cameraParams["near"] = &Render::CameraParams::near;
         cameraParams["far"] = &Render::CameraParams::far;
         cameraParams["fov"] = &Render::CameraParams::fov;
    
         cameraParams["aspect"] = &Render::CameraParams::aspect;
         
         // --- Camera --- //
         auto camera = renderNamespace.new_usertype<Render::Camera>(
             "Camera",
             sol::constructors<
             Render::Camera(const Render::CameraParams&, const size_t&),
             Render::Camera(const int&, const int&, const size_t&)
             >()
             );
    
         camera["transform"] = &Render::Camera::transform;
         
         camera["SetParams"]     = &Render::Camera::SetParams;
         camera["SetFullscreen"] = &Render::Camera::SetFullscreen;
         
         camera["GetParams"]         = &Render::Camera::GetParams;
         camera["GetProjectionMat"]  = &Render::Camera::GetProjectionMat;
         camera["GetViewMat"]        = &Render::Camera::GetViewMat;
         camera["GetId"]             = &Render::Camera::GetId;
    
         // --- CameraManager --- //
         auto cameraManager = renderNamespace.new_usertype<Render::CameraManager>(
             "CameraManager",
             sol::constructors<
             Render::CameraManager()
             >()
             );
    
         cameraManager["Create"] = &Render::CameraManager::Create;
         cameraManager["Get"]     = &Render::CameraManager::Get;
         cameraManager["Delete"] = sol::overload(
             static_cast<void (Render::CameraManager::*)(const Render::Camera*)>(&Render::CameraManager::Delete),
             static_cast<void (Render::CameraManager::*)(const size_t&)>(&Render::CameraManager::Delete)
         );
         
         cameraManager["UpdateEngineCamera"] = &Render::CameraManager::UpdateEngineCamera;
         cameraManager["SetSceneCamera"]     = &Render::CameraManager::SetSceneCamera;
         
         cameraManager["GetEngineCamera"] = &Render::CameraManager::GetEngineCamera;
         cameraManager["GetSceneCamera"]  = &Render::CameraManager::GetSceneCamera;
    
         // --- PostProcessor --- //
         auto postProcessor = renderNamespace.new_usertype<Render::PostProcessor>(
             "PostProcessor",
             sol::constructors<
             Render::PostProcessor()
             >()
             );
    
         postProcessor["BeginRender"]   = &Render::PostProcessor::BeginRender;
         postProcessor["EndRender"]     = &Render::PostProcessor::EndRender;
    
         postProcessor["AddShader"]         = &Render::PostProcessor::AddShader;
         postProcessor["AddShaderProgram"]  = &Render::PostProcessor::AddShaderProgram;
         //postProcessor["AddShaderPrograms"] = &Render::PostProcessor::AddShaderPrograms;
     }
}