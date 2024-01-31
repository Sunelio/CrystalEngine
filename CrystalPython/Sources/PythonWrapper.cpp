#pragma warning(disable :4114)
#include "PythonOpaque.h"

#include <../Externals/Includes/python/Python.h>
#include <../Externals/Includes/pybind/pybind11.h>
#include <../Externals/Includes/pybind/operators.h>
#include <../Externals/Includes/pybind/stl.h>

using namespace Maths;
namespace py = pybind11;

PYBIND11_MODULE(CrystalPython, m)
{
    m.doc() = "Python bindings used for scripting in CrystalEngine.";

    // --- Arithmetic --- //
    m.attr("PI") = PI;

    //
    m.def("roundInt", &roundInt, "Rounds the given value to the nearest int.",                                  py::arg("val"));
    m.def("floorInt", &floorInt, "Rounds down the given value.",                                                py::arg("val"));
    m.def("ceilInt",  &ceilInt,  "Rounds up the given value.",                                                  py::arg("val"));
    m.def("sqpow",    &sqpow,    "Returns the sqare power of the given value.",                                 py::arg("val"));
    m.def("signof",   &signof,   "Returns 1 if the given value is positive or null, and -1 if it is negative.", py::arg("val"));

    // Degrees/Radians conversions.
    m.def("degToRad", &degToRad, "Converts the given angle from degrees to radians.", py::arg("angle"));
    m.def("radToDeg", &radToDeg, "Converts the given angle from radians to degrees.", py::arg("angle"));

    // Clamp.
    m.def("clamp",      &clamp,      "Clamps the given value to be superior or equal to the minimum value and inferior or equal to the maximum value.", py::arg("value"), py::arg("minimum"), py::arg("maximum"));
    m.def("clampUnder", &clampUnder, "Clamps the given value to be inferior or equal to the maximum value.", py::arg("value"), py::arg("maximum"));
    m.def("clampAbove", &clampAbove, "Clamps the given value to be superior or equal to the minimum value.", py::arg("value"), py::arg("minimum"));

    // Lerp.
    m.def("lerp",    &lerp,    "Compute linear interpolation between start and end for the parameter val (if 0 <= val <= 1: start <= return <= end).", py::arg("value"), py::arg("start"), py::arg("end"));
    m.def("getLerp", &getLerp, "Compute the linear interpolation factor that returns val when lerping between start and end.",                         py::arg("value"), py::arg("start"), py::arg("end"));
    m.def("remap",   &remap,    "Remaps the given value from one range to another.", py::arg("value"), py::arg("inputStart"), py::arg("inputEnd"), py::arg("outputStart"), py::arg("outputEnd"));

    // Powers of 2.
    m.def("isPowerOf2",       &isPowerOf2,       "Returns True if the given value is a power of 2.",                       py::arg("value"));
    m.def("getPowerOf2Under", &getPowerOf2Under, "Returns the closest power of 2 that is inferior or equal to the value.", py::arg("value"));
    m.def("getPowerOf2Above", &getPowerOf2Above, "Returns the closest power of 2 that is superior or equal to the value.", py::arg("value"));

    // --- Color --- //
    py::class_<RGBA>(m, "RGBA")
        .def(py::init<float>())
        .def(py::init<float, float, float, float>())
        .def(py::init<RGB, float>())

        .def_readwrite("r", &RGBA::r)
        .def_readwrite("g", &RGBA::g)
        .def_readwrite("b", &RGBA::b)
        .def_readwrite("a", &RGBA::a)
        .def("toRGB", &RGBA::toRGB, "Creates and returns an RGB color from this RGBA color.")

        .def(py::self + py::self)
        .def(py::self + float())

        .def(py::self += py::self)
        .def(py::self += float())

        .def(py::self - py::self)
        .def(py::self - float())

        .def(py::self -= py::self)
        .def(py::self -= float())

        .def(py::self * py::self)
        .def(py::self * float())
        .def(py::self *= py::self)
        .def(py::self *= float())

        .def(py::self / py::self)
        .def(py::self / float())
        .def(py::self /= py::self)
        .def(py::self /= float())

        .def(py::self == py::self)
        .def(py::self != py::self);

    py::class_<RGB>(m, "RGB")
        .def(py::init<float>())
        .def(py::init<float, float, float>())
        .def(py::init<RGBA>())

        .def_readwrite("r", &RGB::r)
        .def_readwrite("g", &RGB::g)
        .def_readwrite("b", &RGB::b)
        .def("toRGBA", &RGB::toRGBA, "Creates and returns an RGBA color from this RGB color.");

    py::class_<HSVA>(m, "HSVA")
        .def(py::init<float>())
        .def(py::init<float, float, float, float>())
        .def(py::init<HSVA>())

        .def_readwrite("h", &HSVA::h)
        .def_readwrite("s", &HSVA::s)
        .def_readwrite("v", &HSVA::v)

        .def_readwrite("a", &HSVA::a)
        .def("toHSV", &HSVA::toHSV, "Creates and returns an HSV color from this HSVA color.");

    py::class_<HSV>(m, "HSV")
        .def(py::init<float, float, float>())

        .def_readwrite("h", &HSV::h)
        .def_readwrite("s", &HSV::s)
        .def_readwrite("v", &HSV::v)

        .def("toHSVA", &HSV::toHSVA, "Creates and returns an HSVA color from this HSV color.");

    m.def("ColorGetHue", &ColorGetHue, "Returns the given RGB color's hue.",              py::arg("rgb"));
    m.def("ColorLerp",   &ColorLerp,   "Linear interpolation between two given colors.",  py::arg("val"), py::arg("start"), py::arg("end"));
    m.def("BlendHSV",    &BlendHSV,    "Blend between two HSV colors.",                   py::arg("col0"), py::arg("col1"));
    m.def("RGBAtoHSVA",  &RGBAtoHSVA,  "Convert an RGBA color (0 <= rgba <= 1) to HSVA.", py::arg("color"));
    m.def("HSVAtoRGBA",  &HSVAtoRGBA,  "Convert an HSVA color to RGBA.",                  py::arg("hsva"));
    m.def("ColorShift",  &ColorShift,  "Shifts the hue of the given color.",              py::arg("color"), py::arg("hue"));

    // --- Vector2 --- //
    py::class_<Vector2>(m, "Vector2")
        .def(py::init<>())
        .def(py::init<float>())
        .def(py::init<float, float>())
        .def(py::init<Vector2, Vector2>())
        .def(py::init<float, float, bool>())

        // Properties.
        .def_readwrite("x", &Vector2::x)
        .def_readwrite("y", &Vector2::y)

        // Comparison.
        .def(py::self == py::self)
        .def(py::self != py::self)

        // Addition.
        .def(py::self + py::self)
        .def(py::self + float())
        .def(py::self += py::self)
        .def(py::self += float())

        // Substraction.
        .def(py::self - py::self)
        .def(py::self - float())
        .def(py::self -= py::self)
        .def(py::self -= float())
        .def(-py::self)

        // Multiplication.
        .def(py::self * py::self)
        .def(py::self * float())
        .def(py::self *= py::self)
        .def(py::self *= float())

        // Division.
        .def(py::self / py::self)
        .def(py::self / float())
        .def(py::self /= py::self)
        .def(py::self /= float())

        // Dot and cross products.
        .def("Dot",   &Vector2::Dot,   "Computes dot product with another vector.")
        .def("Cross", &Vector2::Cross, "Computes cross product with another vector.")

        // Length.
        .def("GetLength", &Vector2::GetLength, "Returns the vector's length.")
        .def("SetLength", &Vector2::SetLength, "Modifies the vector's length.", py::arg("length"))

        // Normalizing.
        .def("Normalize",     &Vector2::Normalize,     "Normalizes the vector.")
        .def("GetNormalized", &Vector2::GetNormalized, "Returns a normalized copy of the vector.")

        // Negation.
        .def("Negate",     &Vector2::Negate,     "Negates the vector's coordinates.")
        .def("GetNegated", &Vector2::GetNegated, "Returns a negated copy of the vector.")

        // Copysign.
        .def("CopySign",      &Vector2::CopySign,      "Copies the signs from the source vector to the destination vector.",                       py::arg("source"))
        .def("GetCopiedSign", &Vector2::GetCopiedSign, "Returns a new vector with the source vector's signs and the destination vector's values.", py::arg("source"))

        // Angle.
        .def("GetAngle",                  [](Vector2& self){ return self.GetAngle(); },                                  "Returns the angle (in radians) of the given vector.")
        .def("GetAngleWithVector2",       [](Vector2& self, const Vector2& other){ return self.GetAngle(other); },       "Returns the angle (in radians) between two vectors.",        py::arg("v"))
        .def("GetAngleSignedWithVector2", [](Vector2& self, const Vector2& other){ return self.GetAngleSigned(other); }, "Returns the signed angle (in radians) between two vectors.", py::arg("v"))

        // Rotation.
        .def("Rotate",        &Vector2::Rotate,        "Rotates the given vector by the given angle (in radians).",                                                 py::arg("angle"))
        .def("RotateAsPoint", &Vector2::RotateAsPoint, "Rotates the vector (interpreted as a point) around the given pivot point by the given angle (in radians).", py::arg("pivot"), py::arg("angle"))

        .def("GetNormal", &Vector2::GetNormal, "Returns the normal the vector.")

        .def("GetDistanceFromPoint", &Vector2::GetDistanceFromPoint, "Returns the normal the vector.", py::arg("p"))

        // String cast.
        .def("__repr__", &Vector2::ToString, "Returns the vector's contents as a string.", py::arg("precision") = 3)

        // Point 2 lerp.
        .def_static("Lerp", &Vector2::Lerp, "Calculates linear interpolation for a value from a start point to an end point.", py::arg("val"), py::arg("start"), py::arg("dest"));

    // --- Vector3 --- //
    py::class_<Vector3>(m, "Vector3")
        .def(py::init<>())
        .def(py::init<float>())
        .def(py::init<float, float, float>())
        .def(py::init<Vector3, Vector3>())
        .def(py::init<Vector3, float>())

        // Properties.
        .def_readwrite("x", &Vector3::x)
        .def_readwrite("y", &Vector3::y)
        .def_readwrite("z", &Vector3::z)

        // Comparison.
        .def(py::self == py::self)
        .def(py::self != py::self)

        // Addition.
        .def(py::self + py::self)
        .def(py::self + float())
        .def(py::self += py::self)
        .def(py::self += float())

        // Substraction.
        .def(py::self - py::self)
        .def(py::self - float())
        .def(py::self -= py::self)
        .def(py::self -= float())
        .def(-py::self)

        // Multiplication.
        .def(py::self * py::self)
        .def(py::self * float())
        .def(py::self *= py::self)
        .def(py::self *= float())

        // Division.
        .def(py::self / py::self)
        .def(py::self / float())
        .def(py::self /= py::self)
        .def(py::self /= float())

        // Length.
        .def("GetLength", &Vector3::GetLength, "Returns the vector's length.")
        .def("SetLength", &Vector3::SetLength, "Modifies the vector's length.", py::arg("length"))

        // Normalizing.
        .def("Normalize",     &Vector3::Normalize,     "Normalizes the vector.")
        .def("GetNormalized", &Vector3::GetNormalized, "Returns a normalized copy of the vector.")

        // Negation.
        .def("Negate",     &Vector3::Negate,     "Negates the vector's coordinates.")
        .def("GetNegated", &Vector3::GetNegated, "Returns a negated copy of the vector.")

        // Copysign.
        .def("copySign",      &Vector3::CopySign,      "Copies the signs from the source vector to the destination vector.",                       py::arg("source"))
        .def("getCopiedSign", &Vector3::GetCopiedSign, "Returns a new vector with the source vector's signs and the destination vector's values.", py::arg("source"))

        // Distance from point.
        .def("GetDistanceFromPoint", &Vector3::GetDistanceFromPoint, "Interprets the vector as a point and returns the distance to another point.", py::arg("point"))

        // Conversions.
        .def("ToSphericalCoords", &Vector3::ToSphericalCoords, "Returns this point in spherical coords (x: longitude, y: latitude, z: length).")
        .def("ToVector4",         &Vector3::ToVector4,         "Creates a Vector4 from this vector.")
        .def("ToString",          &Vector3::ToString,          "Returns the vector's contents as a string.", py::arg("precision") = 3)

        .def_static("Lerp", &Vector3::Lerp, "Calculates linear interpolation for a value from a start point to an end point.", py::arg("start"), py::arg("dest"), py::arg("val"));

    // --- Vector4 --- //
    py::class_<Vector4>(m, "Vector4")
        .def(py::init<>())
        .def(py::init<float, float, float, float>())
        //.def(py::init<Vector3, float>())

        // Properties.
        .def_readwrite("x", &Vector4::x)
        .def_readwrite("y", &Vector4::y)
        .def_readwrite("z", &Vector4::z)
        .def_readwrite("w", &Vector4::w)

        // Comparison.
        .def(py::self == py::self)
        .def(py::self != py::self)

        // Homogenizing.
        .def("Homogenize",     &Vector4::Homogenize,     "Homogenizes the vector by dividing it by its w component.")
        .def("GetHomogenized", &Vector4::GetHomogenized, "Returns a homogenized copy of the vector.")

        // Vector4 cast.
        .def("ToVector3", &Vector4::ToVector3, "Creates a Vector3 from this vector.")

        // String cast.
        .def("__repr__", &Vector4::ToString, "Returns the vector's contents as a string.", py::arg("precision") = 3);

    // --- Matrix --- //
    py::class_<Mat2>(m, "Mat2")
        .def(py::init<bool>())
        .def(py::init<float, float, float, float>())

        .def("__getitem__", [](const Mat2& mat, const int& idx) { return mat.m[idx]; })

        .def(py::self + py::self)
        .def(py::self + float())
        .def(py::self += py::self)
        .def(py::self += float())

        .def(py::self - py::self)
        .def(py::self - float())
        .def(py::self -= py::self)
        .def(py::self -= float())

        .def(py::self * py::self)
        .def(py::self * float())
        .def(py::self *= py::self)
        .def(py::self *= float())

        .def(py::self / float())
        .def("Pow", &Mat2::Pow)

        .def("GetRows",        &Mat2::GetRows,        "Returns the number of rows in the matrix.")
        .def("GetColumns",     &Mat2::GetColumns,     "Returns the number of columns in the matrix.")
        .def("GetMatrixValue", &Mat2::GetMatrixValue, "Returns the element at the given indices.", py::arg("y"), py::arg("x"))
        .def("IsSquare",       &Mat2::IsSquare,       "Returns true if the matrix is square, false if not.")
        .def("IsIdentity",     &Mat2::IsIdentity,     "Returns true if the matrix is identity.")
        .def("IsSymmetrical",  &Mat2::IsSymmetrical,  "Returns true if the matrix is symmetrical.")

        .def("Det2", &Mat2::Det2, "Returns the determinant of the matrix.")
        .def("Inv2", &Mat2::Inv2, "Returns the inverse of the matrix.")

        .def("Transpose",     &Mat2::Transpose,     "Transposes the matrix.")
        .def("GetTransposed", &Mat2::GetTransposed, "Returns a transposed copy of this matrix.")

        .def("__repr__", &Mat2::ToString, "Returns matrix contents as string.");

    py::class_<Mat3>(m, "Mat3")
        .def(py::init<bool>())
        .def(py::init<float, float, float,
            float, float, float,
            float, float, float>())

        .def("__getitem__", [](const Mat3& mat, const int& idx) { return mat.m[idx]; })

        .def(py::self + py::self)
        .def(py::self + float())
        .def(py::self += py::self)
        .def(py::self += float())

        .def(py::self - py::self)
        .def(py::self - float())
        .def(py::self -= py::self)
        .def(py::self -= float())

        .def(py::self * py::self)
        .def(py::self * float())
        .def(py::self *= py::self)
        .def(py::self *= float())

        .def(py::self / float())
        .def("Pow", &Mat3::Pow)

        .def("GetRows",        &Mat3::GetRows,        "Returns the number of rows in the matrix.")
        .def("GetColumns",     &Mat3::GetColumns,     "Returns the number of columns in the matrix.")
        .def("GetMatrixValue", &Mat3::GetMatrixValue, "Returns the element at the given indices.", py::arg("y"), py::arg("x"))
        .def("IsSquare",       &Mat3::IsSquare,       "Returns true if the matrix is square, false if not.")
        .def("IsIdentity",     &Mat3::IsIdentity,     "Returns true if the matrix is identity.")
        .def("IsSymmetrical",  &Mat3::IsSymmetrical,  "Returns true if the matrix is symmetrical.")

        .def("Det3", &Mat3::Det3, "Returns the determinant of the matrix.")
        .def("Inv3", &Mat3::Inv3, "Returns the inverse of the matrix.")

        .def("Transpose",     &Mat3::Transpose,     "Transposes the matrix.")
        .def("GetTransposed", &Mat3::GetTransposed, "Returns a transposed copy of this matrix.")

        .def("__repr__", &Mat3::ToString, "Returns matrix contents as string.");

        // .def_static("Mat4ToMat3", &Mat3::Mat4ToMat3, "", py::arg("mat"));

    py::class_<Matrix<4, 4>>(m, "Mat4")
        .def(py::init<bool>())
        .def(py::init<Vector3>())
        .def(py::init<AngleAxis>())
        .def(py::init<Quaternion>())
        .def(py::init<float, float, float, float,
                      float, float, float, float,
                      float, float, float, float,
                      float, float, float, float>())
        .def(py::init<Mat2, Mat2, Mat2, Mat2>())

        .def("__getitem__", [](const Matrix<4, 4>& mat, const int& idx) { return mat.m[idx]; })

        .def(py::self + py::self)
        .def(py::self + float())
        .def(py::self += py::self)
        .def(py::self += float())

        .def(py::self - py::self)
        .def(py::self - float())
        .def(py::self -= py::self)
        .def(py::self -= float())

        .def(py::self * py::self)
        .def(py::self * float())
        .def(py::self *= py::self)
        .def(py::self *= float())

        .def(py::self / float())
        .def("Pow", &Mat4::Pow, "Returns this matrix raised to the given power.", py::arg("power"))

        .def("GetRows",        &Mat4::GetRows,        "Returns the number of rows in the matrix.")
        .def("GetColumns",     &Mat4::GetColumns,     "Returns the number of columns in the matrix.")
        .def("GetMatrixValue", &Mat4::GetMatrixValue, "Returns the element at the given indices.", py::arg("y"), py::arg("x"))
        .def("IsSquare",       &Mat4::IsSquare,       "Returns true if the matrix is square, false if not.")
        .def("IsIdentity",     &Mat4::IsIdentity,     "Returns true if the matrix is identity.")
        .def("IsSymmetrical",  &Mat4::IsSymmetrical,  "Returns true if the matrix is symmetrical.")

        .def("Det4", &Mat4::Det4, "Returns the determinant of the matrix.")
        .def("Inv4", &Mat4::Inv4, "Returns the inverse of the matrix.")

        .def("Transpose",     &Mat4::Transpose,     "Transposes the matrix.")
        .def("GetTransposed", &Mat4::GetTransposed, "Returns a transposed copy of this matrix.")

        .def("RemoveScale",     &Mat4::RemoveScale,     "Removes scaling form this matrix.")
        .def("GetWithoutScale", &Mat4::GetWithoutScale, "Returns a copy of this matrix without any scaling.")
    
        .def("ToPosition",      &Mat4::ToPosition,   "Extracts a position from a transformation matrix.")
        .def("ToQuaternion",    &Mat4::ToQuaternion, "Extracts a quaternion rotation from a transformation matrix.")
        .def("ToAngleAxis",     &Mat4::ToAngleAxis,  "Extracts a angle axis rotation from a transformation matrix.")
        .def("ToScale",         &Mat4::ToScale,      "Extracts the scale from a transformation matrix.")
        .def("__repr__",        &Mat4::ToString,     "Returns matrix contents as string.")

        .def_static("FromTranslation", &Mat4::FromTranslation, "Matrix from translation.",                                  py::arg("translation"))
        .def_static("FromPitch",       &Mat4::FromPitch,       "Matrix that rotates around the X axis by the given angle.", py::arg("scale"))
        .def_static("FromRoll",        &Mat4::FromRoll,        "Matrix that rotates around the Y axis by the given angle.", py::arg("angle"))
        .def_static("FromYaw",         &Mat4::FromYaw,         "Matrix that rotates around the Z axis by the given angle.", py::arg("angle"))
        .def_static("FromEuler",       &Mat4::FromEuler,       "Matrix from euler angles (pitch, roll, yaw).",              py::arg("angle"))
        .def_static("FromAngleAxis",   &Mat4::FromAngleAxis,   "Matrix from angle-axis.", py::arg("angles"))
        .def_static("FromQuaternion",  &Mat4::FromQuaternion,  "Matrix from quaternion.", py::arg("quaternion"))
        .def_static("FromScale",       &Mat4::FromScale,       "Matrix from scale.",      py::arg("scale"))
        .def_static("FromTransform",   &Mat4::FromTransform,   "Matrix from transform.",  py::arg("pos"), py::arg("rot"), py::arg("scale"), py::arg("reveverse") = false);
        // .def_static("Mat3ToMat4", &Mat4::Mat3ToMat4, "", py::arg("mat"));

    // --- Transform --- //
    py::class_<Transform>(m, "Transform")
        .def(py::init<bool>())
        .def(py::init<Vector3, Quaternion, Vector3, bool>())

        // Position.
        .def("GetPosition",      &Transform::GetPosition,      "Returns the transform's position as a Vector3.")
        .def("GetWorldPosition", &Transform::GetWorldPosition, "Modifies the transform's position.")
        .def("SetPosition",      &Transform::SetPosition,      "Modifies the transform's position.", py::arg("position"))
        .def("Move",             &Transform::Move,             "Moves the transform's position by the given Vector3.", py::arg("movement"))

        // Forward.
        .def("Forward",    &Transform::Forward,    "Returns the transform's forward direction.")
        .def("Up",         &Transform::Up,         "Returns the transform's upwards direction.")
        .def("Right",      &Transform::Right,      "Returns the transform's right direction.")
        .def("SetForward", &Transform::SetForward, "Rotates the transform so that it points in the given direction.", py::arg("forward"))

        // Rotation.
        .def("GetRotation", &Transform::GetRotation, "Returns the transform's quaternion rotation.")
        .def("GetEulerRot", &Transform::GetEulerRot, "Returns the transform's euler angles rotation.")
        .def("SetRotation", &Transform::SetRotation, "Modifies the transform's quaternion rotation.",    py::arg("rotation"))
        .def("SetEulerRot", &Transform::SetEulerRot, "Modifies the transform's euler angles rotation.",  py::arg("euler"))
        .def("Rotate",      &Transform::Rotate,      "Rotates the transform by the given quaternion.",   py::arg("rotation"))
        .def("RotateEuler", &Transform::RotateEuler, "Rotates the transform by the given euler angles.", py::arg("euler"))

        // Scale.
        .def("GetScale",        &Transform::GetScale,        "Returns the transform's scale.")
        .def("GetUniformScale", &Transform::GetUniformScale, "Returns a Vector3 with all its components set to the scale's highest component.")
        .def("SetScale",        &Transform::SetScale,        "Modifies the transform's scale.", py::arg("scale"))

        .def("IsCamera",    &Transform::IsCamera,    "Returns true if the transform is a camera transform, false if it is an object transform.")
        .def("SetIsCamera", &Transform::SetIsCamera, "Modifies the transform's type (object transform or camera transform).", py::arg("isCamera"))

        .def("SetPosRot", &Transform::SetPosRot, "Modifies the transform's position and rotation.", py::arg("position"), py::arg("rotation"))
        .def("SetValues", &Transform::SetValues, "Modifies all of the transform's values.",         py::arg("position"), py::arg("rotation"), py::arg("scale"))

        .def("GetLocalMat",  &Transform::GetLocalMat,  "Returns the transform's local matrix.")
        .def("GetViewMat",   &Transform::GetViewMat,   "Returns the transform's view matrix (identity if not camera).")
        .def("GetParentMat", &Transform::GetParentMat, "Returns the transform's parent matrix.")
        .def("GetWorldMat",  &Transform::GetWorldMat,  "Returns the transform's world matrix (local * parent).")
        .def("SetParentMat", &Transform::SetParentMat, "Modifies the transform's parent matrix.", py::arg("mat"))

        .def_static("Lerp", &Transform::Lerp, "Linearly interpolates between start and dest.", py::arg("start"), py::arg("dest"), py::arg("val"), py::arg("useSlerp"));

    // --- Quaternion --- //
    py::class_<Quaternion>(m, "Quaternion")
        .def(py::init<>())
        .def(py::init<float>())
        .def(py::init<float, float, float, float>())
        .def(py::init<Vector3>())
        .def(py::init<AngleAxis>())
        .def(py::init<Mat4>())

        .def_static("FromPitch",     &Quaternion::FromPitch, py::arg("angle"))
        .def_static("FromRoll",      &Quaternion::FromRoll, py::arg("angle"))
        .def_static("FromYaw",       &Quaternion::FromYaw, py::arg("angle"))
        .def_static("FromEuler",     &Quaternion::FromEuler, py::arg("angle"))
        .def_static("FromAngleAxis", &Quaternion::FromAngleAxis, py::arg("angleAxis"))
        .def_static("FromMatrix",    &Quaternion::FromMatrix, py::arg("matrix"))

        .def(py::self == py::self)
        .def(py::self == float())

        .def(py::self != py::self)
        .def(py::self != float())
    
        .def(py::self + py::self)
        .def(py::self + float())

        .def(-py::self)

        .def(py::self - py::self)
        .def(py::self - float())
        
        .def(py::self * py::self)
        .def(py::self * float())

        .def(py::self / py::self)
        .def(py::self / float())
    
        .def(py::self += py::self)
        .def(py::self += float())

        .def(py::self -= py::self)
        .def(py::self -= float())

        .def(py::self *= py::self)
        .def(py::self *= float())

        .def(py::self /= py::self)
        .def(py::self /= float())

        .def("Dot", &Quaternion::Dot)

        .def("GetModulus",    &Quaternion::GetModulus)
        .def("Normalize",     &Quaternion::Normalize)
        .def("GetNormalized", &Quaternion::GetNormalized)
        .def("Conjugate",     &Quaternion::Conjugate)
        .def("GetConjugate",  &Quaternion::GetConjugate)
        .def("Inverse",       &Quaternion::Inverse)
        .def("GetInverse",    &Quaternion::GetInverse)
        .def("RotateQuat",    &Quaternion::RotateQuat, py::arg("q"))
        .def("RotateVec",     &Quaternion::RotateVec, py::arg("v"))

        .def_static("Lerp",  &Quaternion::Lerp, py::arg("start"), py::arg("dest"), py::arg("val"))
        .def_static("NLerp", &Quaternion::NLerp, py::arg("start"), py::arg("dest"), py::arg("val"))
        .def_static("SLerp", &Quaternion::SLerp, py::arg("start"), py::arg("dest"), py::arg("val"), py::arg("useShortestPath") = true)

        .def("ToAngleAxis", &Quaternion::ToAngleAxis)
        .def("ToMatrix",    &Quaternion::ToMatrix)
        .def("ToEuler",     &Quaternion::ToEuler)
        .def("ToString",    &Quaternion::ToString, py::arg("precision"));

    // --- AngleAxis --- //
    py::class_<AngleAxis>(m, "AngleAxis")
        .def(py::init<>())
        .def(py::init<float, Vector3>())
        .def(py::init<Quaternion>())
        .def(py::init<Mat4>())

        .def_readwrite("angle", &AngleAxis::angle)
        .def_readwrite("axis",  &AngleAxis::axis)

        .def_static("FromQuaternion", &AngleAxis::FromQuaternion, py::arg("quaternion"))
        .def_static("FromMatrix",     &AngleAxis::FromMatrix, py::arg("matrix"))

        .def("ToQuaternion", &AngleAxis::ToQuaternion)
        .def("ToMatrix",     &AngleAxis::ToMatrix)
        .def("ToString",     &AngleAxis::ToString);
    
    // --- Debug --- //
    m.def("DebugLogInfo", [](const char* message, const char* file, const char* function, const long& line) { Debug::Logger::LogToConsole(Debug::LogType::Info, message, file, function, line); },
        "Send an info log containing the given message.", py::arg("message"), py::arg("file") = "Python file", py::arg("function") = '\0', py::arg("line") = -1);

    m.def("DebugLogWarning", [](const char* message, const char* file, const char* function, const long& line) { Debug::Logger::LogToConsole(Debug::LogType::Warning, message, file, function, line); },
        "Send a warning log containing the given message.", py::arg("message"), py::arg("file") = "Python file", py::arg("function") = '\0', py::arg("line") = -1);

    m.def("DebugLogError", [](const char* message, const char* file, const char* function, const long& line) { Debug::Logger::LogToConsole(Debug::LogType::Error, message, file, function, line); },
        "Send an error log containing the given message.", py::arg("message"), py::arg("file") = "Python file", py::arg("function") = '\0', py::arg("line") = -1);
#pragma region Vertex.h
    // --- Vertex --- //
    py::class_<Vertex>(m, "Vertex")
        .def(py::init<>())

        .def_readwrite("pos", &Vertex::pos)
        .def_readwrite("uv", &Vertex::uv)
        .def_readwrite("normal", &Vertex::normal)

        .def(py::self == py::self);

    // --- TangentVertex --- //
    py::class_<TangentVertex>(m, "TangentVertex")
        .def(py::init<>())

        .def_readwrite("pos", &TangentVertex::pos)
        .def_readwrite("uv", &TangentVertex::uv)
        .def_readwrite("normal", &TangentVertex::normal)
        .def_readwrite("tangent", &TangentVertex::tangent)
        .def_readwrite("bitangent", &TangentVertex::bitangent)

        .def(py::self == py::self);

    // --- AnimatedVertex --- //
    py::class_<AnimatedVertex, TangentVertex>(m, "AnimatedVertex")
        .def(py::init<>());

    // .def_readwrite("boneIDs", &AnimatedVertex::boneIDs)
    // .def_readwrite("weights", &AnimatedVertex::weights)
    // .attr("MAX_BONE_WEIGHTS") = MAX_BONE_WEIGHTS;

    // --- VertexIndices --- //
    py::class_<VertexIndices>(m, "VertexIndices")
        .def(py::init<>())

        .def_readwrite("pos", &VertexIndices::pos)
        .def_readwrite("uv", &VertexIndices::uv)
        .def_readwrite("normal", &VertexIndices::normal)

        .def(py::self == py::self);
#pragma endregion

    // --- ResourceType --- //
    // py::enum_<Resources::ResourceType>(m, "ResourceType")
    //     .value("Undefined", Resources::ResourceType::Undefined)
    //     .value("Texture", Resources::ResourceType::Texture)
    //     .value("RenderTexture", Resources::ResourceType::RenderTexture)
    //     .value("Cubemap", Resources::ResourceType::Cubemap)
    //     .value("Material", Resources::ResourceType::Material)
    //     .value("Mesh", Resources::ResourceType::Mesh)
    //     .value("VertexShader", Resources::ResourceType::VertexShader)
    //     .value("FragmentShader", Resources::ResourceType::FragmentShader)
    //     .value("ShaderProgram", Resources::ResourceType::ShaderProgram)
    //     .value("Font", Resources::ResourceType::Font)
    //     .export_values();

    m.def("ResourceTypeToString", &Resources::ResourceTypeToString, "", py::arg("type"));

    // --- Resource --- //
    py::class_<Resources::Resource>(m, "Resource")
        .def("GetName",           &Resources::Resource::GetName,           "Returns the resource's name.")
        .def("GetType",           &Resources::Resource::GetType,           "Returns the resource's type.")
        .def("IsLoaded",          &Resources::Resource::IsLoaded,          "Returns True if the resource is loaded.")
        .def("WasSentToPipeline", &Resources::Resource::WasSentToPipeline, "Returns True if the resource has been sent to the pipeline.")
        .def("IsUsed",            &Resources::Resource::IsUsed,            "Returns True if the resource still has a use, False if it can be unloaded.");

    // --- Texture --- //
    py::class_<Resources::Texture, Resources::Resource>(m, "Texture")
        .def(py::init<std::string>())

        .def("Load",                 &Resources::Texture::Load,                 "Loads the texture.")
        .def("SendToPipeline",       &Resources::Texture::SendToPipeline,       "Sends the texture to GPU memory.")
        .def("SetFlippedVertically", &Resources::Texture::SetFlippedVertically, "Toggles vertical flip of the texture (triggers a total reload).", py::arg("flipped"))

        .def("GetId",       &Resources::Texture::GetId,       "Returns the texture's pipeline ID.")
        .def("GetWidth",    &Resources::Texture::GetWidth,    "Returns the texture's width.")
        .def("GetHeight",   &Resources::Texture::GetHeight,   "Returns the texture's height.")
        .def("GetData",     &Resources::Texture::GetData,     "Returns the texture's pixel data.")
        .def("GetChannels", &Resources::Texture::GetChannels, "Returns the number of color channels in the texture.")

        .def("GetResourceType", &Resources::Texture::GetResourceType, "Returns the resource type of this class.");

    // --- RenderTexture --- //
    py::class_<Resources::RenderTexture, Resources::Resource>(m, "RenderTexture")
        .def(py::init<std::string>())

        .def_readwrite("clearColor", &Resources::RenderTexture::clearColor)

        .def("Load",           &Resources::RenderTexture::Load,           "Loads the render texture.")
        .def("SendToPipeline", &Resources::RenderTexture::SendToPipeline, "Sends the render texture to GPU memory.")

        .def("BeginRender", &Resources::RenderTexture::BeginRender, "Starts rendering to the rendertexture.")
        .def("EndRender",   &Resources::RenderTexture::EndRender,   "Stops rendering to the rendertexture (defaults back to rendering to screen).")

        .def("SetWidth",        &Resources::RenderTexture::SetWidth,  "Modifies the rendertexture's width.",  py::arg("width"))
        .def("SetHeight",       &Resources::RenderTexture::SetHeight, "Modifies the rendertexture's height.", py::arg("height"))
        .def("SetSize",         &Resources::RenderTexture::SetSize,   "Modifies the rendertexture's size.",   py::arg("width"), py::arg("height"))

        .def("GetWidth",        &Resources::RenderTexture::GetWidth,        "Returns the rendertexture's width.")
        .def("GetHeight",       &Resources::RenderTexture::GetHeight,       "Returns the rendertexture's height.")
        .def("GetSize",         &Resources::RenderTexture::GetSize,         "Returns the render texture's size.")
        .def("GetPipelineData", &Resources::RenderTexture::GetPipelineData, "Returns the render texture's pipeline data.")

        .def("GetResourceType", &Resources::RenderTexture::GetResourceType, "Returns the resource type of this class.");

#pragma region ShaderScript.h
// --- ShaderScript --- //
    py::class_<Scripts::ShaderScript>(m, "ShaderScript")
        .def(py::init<>())

        .def_readwrite("shader",   &Scripts::ShaderScript::shader)
        .def_readwrite("renderer", &Scripts::ShaderScript::renderer)
        .def_readwrite("engine",   &Scripts::ShaderScript::engine);

    // --- ShaderScript --- //
    py::class_<Scripts::MeshShaderScript, Scripts::ShaderScript>(m, "MeshShaderScript")
        .def(py::init<>())

        .def_readwrite("subMesh",      &Scripts::MeshShaderScript::subMesh)
        .def_readwrite("material",     &Scripts::MeshShaderScript::material)
        .def_readwrite("modelMat",     &Scripts::MeshShaderScript::modelMat)
        .def_readwrite("camera",       &Scripts::MeshShaderScript::camera)
        .def_readwrite("lightManager", &Scripts::MeshShaderScript::lightManager);

    // --- ShaderScript --- //
    py::class_<Scripts::ScreenShaderScript, Scripts::ShaderScript>(m, "UiShaderScript")
        .def(py::init<>())

        .def_readwrite("textureID",     &Scripts::ScreenShaderScript::textureID)
        .def_readwrite("rectPos",       &Scripts::ScreenShaderScript::rectPos)
        .def_readwrite("rectSize",      &Scripts::ScreenShaderScript::rectSize)
        .def_readwrite("screenSize",    &Scripts::ScreenShaderScript::screenSize)
        .def_readwrite("postProcessor", &Scripts::ScreenShaderScript::postProcessor);
#pragma endregion

    // --- Engine --- //
    py::class_<Core::Engine>(m, "Engine")
        .def_static("Get", &Core::Engine::Get, "", py::return_value_policy::reference)

        .def("Quit", &Core::Engine::Quit, "Quits the engine and closes the window.")

        .def("GetEventDispatcher", &Core::Engine::GetEventDispatcher, "", py::return_value_policy::reference)
        .def("GetLogger",          &Core::Engine::GetLogger,          "", py::return_value_policy::reference)
        .def("GetWindow",          &Core::Engine::GetWindow,          "", py::return_value_policy::reference)
        .def("GetTimeManager",     &Core::Engine::GetTimeManager,     "", py::return_value_policy::reference)
        .def("GetThreadManager",   &Core::Engine::GetThreadManager,   "", py::return_value_policy::reference)
        .def("GetRenderer",        &Core::Engine::GetRenderer,        "", py::return_value_policy::reference)
        .def("GetResourceManager", &Core::Engine::GetResourceManager, "", py::return_value_policy::reference)
        .def("GetSceneManager",    &Core::Engine::GetSceneManager,    "", py::return_value_policy::reference)
        .def("GetPostProcessor",   &Core::Engine::GetPostProcessor,   "", py::return_value_policy::reference);

    // --- SceneState --- //
    py::enum_<Scenes::SceneState>(m, "SceneState")
        .value("Unloaded", Scenes::SceneState::Unloaded)
        .value("Loaded",   Scenes::SceneState::Loaded)
        .value("Playing",  Scenes::SceneState::Playing)
        .value("Paused",   Scenes::SceneState::Paused)
        .export_values();

    // --- Scene --- //
    py::class_<Scenes::Scene>(m, "Scene")
        .def(py::init<std::string>())

        .def("Save",   &Scenes::Scene::Save,   "Save this scene to the file corresponding to its name.")
        .def("Load",   &Scenes::Scene::Load,   "Loads this scene using its name.", py::arg("async") = true)
        .def("Unload", &Scenes::Scene::Unload, "Unloads this scene from memory.")
        .def("Reset",  &Scenes::Scene::Reset,  "Resets all game objects to their default transform.")

        .def("Start",  &Scenes::Scene::Start,  "Starts play mode and calls every game object's Start method.")
        .def("Pause",  &Scenes::Scene::Pause,  "Pauses play mode.")
        .def("Resume", &Scenes::Scene::Resume, "Resumes play mode after it has been paused.")
        .def("Stop",   &Scenes::Scene::Stop,   "Stops play mode and resets the scene.") // TODO: This doesn't link!

        .def("IsLoaded",  &Scenes::Scene::IsLoaded,  "Returns True if the scene is loaded.")
        .def("IsPlaying", &Scenes::Scene::IsPlaying, "Returns True if the scene is playing.")
        .def("IsPaused",  &Scenes::Scene::IsPaused,  "Returns True if the scene is paused.")
        .def("GetState",  &Scenes::Scene::GetState,  "Returns this scene's name.")
        .def("GetName",   &Scenes::Scene::GetName,   "Returns this scene's state.")
        .def("GetRenderTexture", &Scenes::Scene::GetRenderTexture, "Returns this scene's render texture.")
        .def("GetCameraManager", &Scenes::Scene::GetCameraManager, "Returns this scene's camera manager.")
        .def("GetLightManager",  &Scenes::Scene::GetLightManager,  "Returns this scene's light manager.")
        .def("GetSceneGraph",    &Scenes::Scene::GetSceneGraph,    "Returns this scene's scene graph.");

    // --- SceneManager --- //
    py::class_<Scenes::SceneManager>(m, "SceneManager")
        .def(py::init<>())

        .def("Exists", &Scenes::SceneManager::Exists, "Returns true if a scene with the given name has been created.", py::arg("name"))
        .def("Loaded", &Scenes::SceneManager::Loaded, "Returns true if a scene with the given name is loaded.",        py::arg("name"))
        .def("Create", &Scenes::SceneManager::Create, "Creates a new scene with the given name and returns it.",       py::arg("name"), py::return_value_policy::reference)
        .def("Delete", &Scenes::SceneManager::Delete, "Deletes the scene with the given name if it exists.",           py::arg("name"))

        .def("Get",               &Scenes::SceneManager::Get,              "Returns the scene with the given name if it exists.",                  py::arg("name"), py::return_value_policy::reference)
        .def("GetActiveScene",    &Scenes::SceneManager::GetActiveScene,   "Returns the currently active scene.",                                                   py::return_value_policy::reference)
        .def("SetActiveScene",    &Scenes::SceneManager::SetActiveScene,   "Changes the currently active scene to the scene with the given name.", py::arg("name"), py::arg("shouldStart"))
        .def("StartActiveScene",  &Scenes::SceneManager::StartActiveScene, "Starts the currently active scene.");
    
    // --- Renderer --- //
    py::class_<Render::Renderer>(m, "Renderer")
        .def(py::init<>())
    
        .def("CreateShaderBuffer",         &Render::Renderer::CreateShaderBuffer,         "Creates a shader buffer and returns it.")
        .def("SetShaderStorageBufferData", &Render::Renderer::SetShaderStorageBufferData, "Modifies the data stored in the given buffer to the given data.", py::arg("id"), py::arg("size"), py::arg("data"))
        .def("SetArrayBufferData",         &Render::Renderer::SetShaderArrayBufferData,   "Modifies the data stored in the given buffer to the given data.", py::arg("id"), py::arg("size"), py::arg("data"))
        .def("DeleteShaderBuffer",         &Render::Renderer::DeleteShaderBuffer,         "Deletes the given buffer from GPU memory.")

        .def("UseShaderProgram",          &Render::Renderer::UseShaderProgram,          "Starts using the given shader program.", py::arg("shaderProgram"))
        .def("SetShaderSampler",          &Render::Renderer::SetShaderSampler,          "Sets shader texture sampler variable.", py::arg("shaderProgram"), py::arg("name"), py::arg("location"), py::arg("textureID"))
        .def("GetShaderVariableLocation", &Render::Renderer::GetShaderVariableLocation, "Returns the location of the given variable in the given shader program.", py::arg("shaderProgram"), py::arg("name"))

        .def("SetShaderBool", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const bool& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderBoolByLocation", [](Render::Renderer& self, const int& location, const bool& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderInt", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const int& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderIntByLocation", [](Render::Renderer& self, const int& location, const int& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderUint", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const uint& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderUintByLocation", [](Render::Renderer& self, const int& location, const uint& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderFloat", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const float& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderFloatByLocation", [](Render::Renderer& self, const int& location, const float& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderVector2", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Vector2& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderVector2ByLocation", [](Render::Renderer& self, const int& location, const Vector2& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderVector3", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Vector3& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderVector3ByLocation", [](Render::Renderer& self, const int& location, const Vector3& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderVector4", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Vector4& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderVector4ByLocation", [](Render::Renderer& self, const int& location, const Vector4& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderRGB", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const RGB& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderRGBByLocation", [](Render::Renderer& self, const int& location, const RGB& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderRGBA", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const RGBA& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderRGBAByLocation", [](Render::Renderer& self, const int& location, const RGBA& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat2", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Mat2& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat2ByLocation", [](Render::Renderer& self, const int& location, const Mat2& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat3", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Mat3& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat3ByLocation", [](Render::Renderer& self, const int& location, const Mat3& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat4", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Mat4& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat4ByLocation", [](Render::Renderer& self, const int& location, const Mat4& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat2x3", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Matrix<2,3> & value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat2x3yLocation", [](Render::Renderer& self, const int& location, const Matrix<2,3>& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat3x2", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Matrix<3,2>& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat3x2Location", [](Render::Renderer& self, const int& location, const Matrix<3,2>& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat2x4", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Matrix<2,4>& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat2x4Location", [](Render::Renderer& self, const int& location, const Matrix<2,4>& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat4x2", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Matrix<4,2>& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat4x2Location", [](Render::Renderer& self, const int& location, const Matrix<4,2>& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat3x4", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Matrix<3,4>& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat3x4Location", [](Render::Renderer& self, const int& location, const Matrix<3,4>& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))

        .def("SetShaderMat4x3", [](Render::Renderer& self, const Resources::ShaderProgram* shaderProgram, const char* name, const Matrix<4,3>& value) { self.SetShaderVariable(shaderProgram, name, value); }, py::arg("shaderProgram"), py::arg("name"), py::arg("value"))
        .def("SetShaderMat4x3Location", [](Render::Renderer& self, const int& location, const Matrix<4,3>& value) { self.SetShaderVariable(location, value); }, py::arg("location"), py::arg("value"))
    
        .def("BindRenderTexture",        &Render::Renderer::BindRenderTexture,        "Starts using the given render texture to draw objects.", py::arg("renderTexture"))
        .def("UnbindRenderTexture",      &Render::Renderer::UnbindRenderTexture,      "Starts drawing objects directly to the screen texture.")
        .def("ClearBindedRenderTexture", &Render::Renderer::ClearBindedRenderTexture, "Fills the currently binded render texture with the given color.", py::arg("color"), py::arg("onlyStencil"))

        .def("DrawSubMesh",   &Render::Renderer::DrawSubMesh,   "Renders the given sub-mesh using the specified model transform matrix, camera and the light manager.",           py::arg("subMesh"), py::arg("modelMat"), py::arg("camera"), py::arg("lightManager"), py::arg("objectId"))
        .def("DrawInstanced", &Render::Renderer::DrawInstanced, "Renders the given instanced sub-mesh using the specified model transform matrix, camera and the light manager.", py::arg("subMesh"), py::arg("instanceCount"), py::arg("modelMat"), py::arg("camera"), py::arg("lightManager"), py::arg("objectId"))
        .def("DrawCubemap",   &Render::Renderer::DrawCubemap,   "", py::arg("subMesh"), py::arg("cubemap"), py::arg("modelMat"), py::arg("camera"), py::arg("lightManager"), py::arg("objectId"))
    
        .def("DrawScreenTexture",       [](Render::Renderer& self, const Resources::Texture*       texture, const Resources::ShaderProgram* shaderProgram){ self.DrawScreenTexture(texture, shaderProgram); }, "Render the given texture on the screen.", py::arg("texture"),       py::arg("shaderProgram") = nullptr)
        .def("DrawScreenRendertexture", [](Render::Renderer& self, const Resources::RenderTexture* texture, const Resources::ShaderProgram* shaderProgram){ self.DrawScreenTexture(texture, shaderProgram); }, "Render the given texture on the screen.", py::arg("renderTexture"), py::arg("shaderProgram") = nullptr)
        .def("DrawText",      &Render::Renderer::DrawText,      "", py::arg("text"),    py::arg("font"), py::arg("pos"), py::arg("scale"),   py::arg("color"), py::arg("id"), "Renders the given text with the given font.")
        .def("DrawTexture2D", &Render::Renderer::DrawTexture2D, "", py::arg("texture"), py::arg("size"), py::arg("pos"), py::arg("texSize"), py::arg("id"), "Draws the given texture on the screen.")
        .def("DrawLine2D",    [](Render::Renderer& self, const Vector2& start, const Vector2& end, const RGB& startColor, const RGB& endColor){ self.DrawLine(start, end, startColor, endColor); }, "Draws a 2D line.", py::arg("start"), py::arg("end"), py::arg("startColor"), py::arg("endColor"))
        .def("DrawLine3D",    [](Render::Renderer& self, const Vector3& start, const Vector3& end, const RGB& startColor, const RGB& endColor, const Render::Camera* camera){ self.DrawLine(start, end, startColor, endColor, camera); }, "Draws a 3D line.", py::arg("start"), py::arg("end"), py::arg("startColor"), py::arg("endColor"), py::arg("camera"))

        .def("GetTextureSamplerId",    &Render::Renderer::GetTextureSamplerId,    "Returns the ID of the texture sampler (necessary to sample textures within shaders).")
        .def("RenderObjectsToStencil", &Render::Renderer::RenderObjectsToStencil, "Enables drawing object IDs to a texture to make it possible to click on viewport objects.",   py::arg("enable"))
        .def("RenderObjectsToStencil", &Render::Renderer::RenderWireframe,        "Starts rendering everything in wireframe mode when set to true, to revert, set it to false.", py::arg("enable"))
        .def("PickObjectFromViewport", &Render::Renderer::PickObjectFromViewport, "Returns the ID of the clicked object, 0 if no object is clicked and negative values if the gizmo is clicked.", py::arg("pickPos"), py::arg("viewportTexture"), py::arg("viewportSize"));

    #pragma region Font.h
    py::class_<Resources::Character>(m, "Character")
        .def(py::init<>())
        .def(py::init<uint, Vector2, Vector2, uint>())

        .def("GetTextureID", &Resources::Character::GetTextureID, "Returns this text character's texture ID.")
        .def("GetSize",      &Resources::Character::GetSize,      "Returns this text character's size.")
        .def("GetBearing",   &Resources::Character::GetBearing,   "Returns this text character's bearing.")
        .def("GetAdvance",   &Resources::Character::GetAdvance,   "Returns this text character's advance.");

    py::class_<Resources::Font, Resources::Resource>(m, "Font")
        .def(py::init<std::string>())

        .def("Load",           &Resources::Font::Load,           "Loads this font.")
        .def("SendToPipeline", &Resources::Font::SendToPipeline, "Sends this font to the pipeline.")

        .def("GetCharacter", &Resources::Font::GetCharacter, "Returns the text character corresponding to the given char.", py::arg("character"))

        .def_static("GetResourceType", &Resources::Font::GetResourceType, "Returns the resource type of this class.");
    #pragma endregion

    #pragma region Cubemap.h
    // --- CubemapSides --- //
    py::enum_<Resources::CubemapSide>(m, "CubemapSide")
        .value("Right", Resources::CubemapSide::Right)
        .value("Left", Resources::CubemapSide::Left)
        .value("Top", Resources::CubemapSide::Top)
        .value("Bottom", Resources::CubemapSide::Bottom)
        .value("Front", Resources::CubemapSide::Front)
        .value("Back", Resources::CubemapSide::Back)
        .export_values();
 
    // --- CubemapSides --- //
    py::class_<Resources::Cubemap, Resources::Resource>(m, "Cubemap")
        .def(py::init<std::string>())

        .def("Load",           &Resources::Cubemap::Load,           "Loads the cubemap.")
        .def("SendToPipeline", &Resources::Cubemap::SendToPipeline, "Sends the cubemap to GPU memory.")

        .def("SetCubemapSide", &Resources::Cubemap::SetCubemapSide, "Sets the cubemap's texture on the given side.", py::arg("side"), py::arg("name"))

        .def_static("GetResourceType", &Resources::Cubemap::GetResourceType, "Returns the resource type of this class.");
    #pragma endregion

    // --- Material --- //
    py::class_<Resources::Material, Resources::Resource>(m, "Material")
        .def(py::init<std::string>())

        .def_readwrite("ambient",      &Resources::Material::ambient)
        .def_readwrite("diffuse",      &Resources::Material::diffuse)
        .def_readwrite("specular",     &Resources::Material::specular)
        .def_readwrite("emission",     &Resources::Material::emission)
        .def_readwrite("shininess",    &Resources::Material::shininess)
        .def_readwrite("transparency", &Resources::Material::transparency)

        .def_readwrite("ambientTexture",  &Resources::Material::ambientTexture)
        .def_readwrite("diffuseTexture",  &Resources::Material::diffuseTexture)
        .def_readwrite("specularTexture", &Resources::Material::specularTexture)
        .def_readwrite("emissionTexture", &Resources::Material::emissionTexture)
        .def_readwrite("shininessMap",    &Resources::Material::shininessMap)
        .def_readwrite("alphaMap",        &Resources::Material::alphaMap)
        .def_readwrite("normalMap",       &Resources::Material::normalMap)

        .def("Load",           &Resources::Material::Load,           "Loads the material.")
        .def("SendToPipeline", &Resources::Material::SendToPipeline, "Sends the material to GPU memory.")

        .def("SetParams", &Resources::Material::SetParams, "Sets the material's parameters to the given ones.",
            py::arg("ambient"), py::arg("diffuse"), py::arg("specular"), py::arg("emission"), py::arg("shininess"), py::arg("transparency"))

        .def_static("GetResourceType", &Resources::Material::GetResourceType, "Returns the resource type of this class.");

    #pragma region Mesh.h
    // --- Mesh --- //
    py::class_<Resources::Mesh, Resources::Resource>(m, "Mesh")
        .def(py::init<std::string>())

        .def_readwrite("subMeshes", &Resources::Mesh::subMeshes)

        .def("Load",           &Resources::Mesh::Load,           "Returns true if all sub-meshes are loaded.")
        .def("SendToPipeline", &Resources::Mesh::SendToPipeline, "Returns true if all sub-meshes have been sent to GPU memory.")

        .def("AreAllSubMeshesLoaded",   &Resources::Mesh::AreSubMeshesLoaded,          "Returns True if all of the sub-meshes of this mesh are loaded.")
        .def("AreAllSubMeshesInOpenGL", &Resources::Mesh::WereSubMeshesSentToPipeline, "Returns True if all of the sub-meshes of this mesh have been sent to OpenGL.")

        .def("GetVertexCount",  &Resources::Mesh::GetVertexCount,  "Returns this mesh's vertex count.")
        .def("GetSubMeshCount", &Resources::Mesh::GetSubMeshCount, "Returns this mesh's sub-mesh count.")
        .def("GetAnimator",     &Resources::Mesh::GetAnimator,     "Returns this mesh's animator.", py::return_value_policy::reference)
        .def("GetSkeleton",     &Resources::Mesh::GetSkeleton,     "Returns this mesh's skeleton.", py::return_value_policy::reference)

        .def("GetResourceType", &Resources::Mesh::GetResourceType, "Returns the resource type of this class.");
    #pragma endregion

    #pragma region PipelineData.h
    // --- SubMeshPipelineData ---- //
    py::class_<Utils::SubMeshPipelineData>(m, "SubMeshPipelineData")
        .def_readwrite("VBO", &Utils::SubMeshPipelineData::VBO)
        .def_readwrite("EBO", &Utils::SubMeshPipelineData::EBO)
        .def_readwrite("VAO", &Utils::SubMeshPipelineData::VAO)
        .def_readwrite("instanceBO", &Utils::SubMeshPipelineData::instanceBO)

        .def("IsEmpty",     &Utils::SubMeshPipelineData::IsEmpty)
        .def("IsInstanced", &Utils::SubMeshPipelineData::IsInstanced);

    // --- RenderTexturePipelineData ---- //
    py::class_<Utils::RenderTexturePipelineData>(m, "RenderTexturePipelineData")
        .def_readwrite("FBO", &Utils::RenderTexturePipelineData::FBO)
        .def_readwrite("RBO", &Utils::RenderTexturePipelineData::RBO)
        .def_readwrite("ID",  &Utils::RenderTexturePipelineData::ID)

        .def("IsEmpty", &Utils::RenderTexturePipelineData::IsEmpty);

    // --- ScreenRectPipelineData ---- //
    py::class_<Utils::ScreenRectPipelineData>(m, "ScreenRectPipelineData")
        .def_readwrite("VAO", &Utils::ScreenRectPipelineData::VAO)
        .def_readwrite("VBO", &Utils::ScreenRectPipelineData::VBO)

        .def("IsEmpty", &Utils::ScreenRectPipelineData::IsEmpty);
    #pragma endregion

    #pragma region SubMesh.h
    // --- SubMeshType ---- //
    py::enum_<Resources::SubMeshType>(m, "SubMeshType")
        .value("Static",   Resources::SubMeshType::Static)
        .value("Animated", Resources::SubMeshType::Animated)
        .export_values();

    // --- SubMesh ---- //
    py::class_<Resources::SubMesh>(m, "SubMesh")
        .def("GetName",                   &Resources::SubMesh::GetName,                   "Returns this sub-mesh's name.")
        .def("GetType",                   &Resources::SubMesh::GetType,                   "Returns this sub-mesh's type.")
        .def("GetVertexCount",            &Resources::SubMesh::GetVertexCount,            "Returns this sub-mesh's vertex count.")
        .def("GetPipelineData",           &Resources::SubMesh::GetPipelineData,           "Returns this sub-mesh's pipeline data.")
        .def("IsLoaded",                  &Resources::SubMesh::IsLoaded,                  "Returns True if this sub-mesh is loaded.")
        .def("WasSentToPipeline",         &Resources::SubMesh::WasSentToPipeline,         "Returns True if this sub-mesh was sent to the pipeline.")
        .def("GetMaterial",               &Resources::SubMesh::GetMaterial,               "Returns this sub-mesh's material.")
        .def("GetShaderProgram",          &Resources::SubMesh::GetShaderProgram,          "Returns this sub-mesh's shader program.")
        .def("GetInstancedShaderProgram", &Resources::SubMesh::GetInstancedShaderProgram, "Returns this sub-mesh's instanced shader program.")

        .def("SetShaderProgram", &Resources::SubMesh::SetShaderProgram, "Sets this sub-mesh's shader program to the given one.", py::arg("shaderProgram"))
        .def("SetMaterial",      &Resources::SubMesh::SetMaterial,      "Sets this sub-mesh's material to the given one.", py::arg("material"));

    // --- StaticSubMesh ---- //
    py::class_<Resources::StaticSubMesh, Resources::SubMesh>(m, "StaticSubMesh")
        .def(py::init<std::string>())

        .def("GetVertices", &Resources::StaticSubMesh::GetVertices, "Returns this sub-mesh's vertices.")
        .def("GetIndices",  &Resources::StaticSubMesh::GetIndices,  "Returns this sub-mesh's indices.");

    // --- AnimatedSubMesh ---- //
    py::class_<Resources::AnimatedSubMesh, Resources::SubMesh>(m, "AnimatedSubMesh")
        .def(py::init<std::string>())

        .def("GetVertices", &Resources::AnimatedSubMesh::GetVertices, "Returns this sub-mesh's vertices.")
        .def("GetIndices",  &Resources::AnimatedSubMesh::GetIndices,  "Returns this sub-mesh's indices.")
        .def("GetBoneMatricesBuffer", &Resources::AnimatedSubMesh::GetBoneMatricesBuffer, "Returns this sub-mesh's bone matrices buffer ID.");
    #pragma endregion

    #pragma region Shader.h
    // --- VertexShader --- //
    py::class_<Resources::VertexShader, Resources::Resource>(m, "VertexShader")
        .def(py::init<std::string>())

        .def("Load",           &Resources::VertexShader::Load,           "Loads the vertex shader source code.")
        .def("SendToPipeline", &Resources::VertexShader::SendToPipeline, "Compiles and sends the vertex shader to GPU memory.")

        .def("GetId",           &Resources::VertexShader::GetId,         "Returns the vertex shader's ID.")
        .def("GetSourceCode",   &Resources::VertexShader::GetSourceCode, "Returns the vertex shader's source code as a string.")
    
        .def_static("GetResourceType", &Resources::VertexShader::GetResourceType, "Returns the resource type of this class.");

    // --- FragmentShader --- //
    py::class_<Resources::FragmentShader, Resources::Resource>(m, "FragmentShader")
        .def(py::init<std::string>())

        .def("Load",           &Resources::FragmentShader::Load,           "Loads the fragment shader source code.")
        .def("SendToPipeline", &Resources::FragmentShader::SendToPipeline, "Compiles and sends the fragment shader to GPU memory.")

        .def("GetId",         &Resources::FragmentShader::GetId,         "Returns the fragment shader's ID.")
        .def("GetSourceCode", &Resources::FragmentShader::GetSourceCode, "Returns the fragment shader's source code as a string.")
    
        .def_static("GetResourceType", &Resources::FragmentShader::GetResourceType, "Returns the resource type of this class.");

    // --- ShaderProgram --- //
    py::class_<Resources::ShaderProgram, Resources::Resource>(m, "ShaderProgram")
        .def(py::init<std::string>())

        .def("AttatchShader", &Resources::ShaderProgram::AttachShader, "Attaches the given shader to be linked with the shader program.", py::arg("shader"))

        .def("Load",           &Resources::ShaderProgram::Load,           "Loads the shader program if all attached shaders have been loaded.")
        .def("SendToPipeline", &Resources::ShaderProgram::SendToPipeline, "Links and sends the shader program to GPU memory is all attached shaders have been sent to GPU memory.")

        .def("SetScript", &Resources::ShaderProgram::SetScript, "Sets the shader's script to the given one.", py::arg("script"))
    
        .def("GetId",              &Resources::ShaderProgram::GetId,              "Returns the shader program's ID.")
        .def("GetAttachedShaders", &Resources::ShaderProgram::GetAttachedShaders, "Returns all of the shaders attached to the shader program.")
        .def("GetShaderScript",    &Resources::ShaderProgram::GetShaderScript,    "Returns the shader's script so that it can be populated and ran.")
    
        .def_static("GetResourceType", &Resources::ShaderProgram::GetResourceType, "Returns the resource type of this class.");
    #pragma endregion

    // --- TimeManager --- //
    py::class_<Core::TimeManager>(m, "TimeManager")
        .def(py::init<int, bool>())

        .def("SetVsyncEnabled", &Core::TimeManager::SetVsyncEnabled, "Enables/disables vertical synchronization.", py::arg("vsync"))
        .def("SetTargetFPS",    &Core::TimeManager::SetTargetFPS,    "Modifies the maximum frames per second.",    py::arg("targetFPS"))

        .def("IsVsyncEnabled", &Core::TimeManager::IsVsyncEnabled, "Returns True if vertical synchronization is enabled.")
        .def("GetTargetFPS",   &Core::TimeManager::GetTargetFPS,   "Returns the engine's FPS cap.")
        .def("FPS",            &Core::TimeManager::FPS,            "Returns the engine's current FPS.")
        .def("DeltaTime",      &Core::TimeManager::DeltaTime,      "Returns the engine's current delta time.");

    // --- ResourceManager --- //
    py::class_<Resources::ResourceManager>(m, "ResourceManager")
        .def("LoadFile",   [](Resources::ResourceManager& self, const std::string& filename, const bool& async) { self.LoadFile(filename, async); }, "Loads the given file if it is supported and creates new resources according to its contents.", py::arg("filename"), py::arg("async") = true)
        .def("UnloadFile", [](Resources::ResourceManager& self, const std::string& filename) { self.UnloadFile(filename); }, "Unloads the resources that have been created by the given file.", py::arg("filename"))
        .def("GetFileResources", [](Resources::ResourceManager& self, const std::string& filename) { self.GetFileResources(filename); }, "Returns a vector containing all resources that have been created by the given file.", py::arg("filename"))

        .def("CreateCubemap",        [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::Cubemap       >(name, async); }, "Creates a cubemap with the given name, loads it and returns it.",             py::arg("name"), py::arg("async"), py::return_value_policy::reference)
        .def("CreateMaterial",       [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::Material      >(name, async); }, "Creates a new material with the given name, loads it and returns it.",        py::arg("name"), py::arg("async"), py::return_value_policy::reference)
        .def("CreateMesh",           [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::Mesh          >(name, async); }, "Creates a new mesh with the given name, loads it and returns it.",            py::arg("name"), py::arg("async"), py::return_value_policy::reference)
        .def("CreateRenderTexture",  [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::RenderTexture >(name, async); }, "Creates a render texture with the given name, loads it and returns it.",      py::arg("name"), py::arg("async"), py::return_value_policy::reference)
        .def("CreateTexture",        [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::Texture       >(name, async); }, "Creates a new texture with the given name, loads it and returns it.",         py::arg("name"), py::arg("async"), py::return_value_policy::reference)
        .def("CreateVertexShader",   [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::VertexShader  >(name, async); }, "Creates a new vertex shader with the given name, loads it and returns it.",   py::arg("name"), py::arg("async"), py::return_value_policy::reference)
        .def("CreateFragmentShader", [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::FragmentShader>(name, async); }, "Creates a new fragment shader with the given name, loads it and returns it.", py::arg("name"), py::arg("async"), py::return_value_policy::reference)
        .def("CreateShaderProgram",  [](Resources::ResourceManager& self, const std::string& name, const bool& async) { return self.Create<Resources::ShaderProgram >(name, async); }, "Creates a new shader program with the given name, loads it and returns it.",  py::arg("name"), py::arg("async"), py::return_value_policy::reference)
    
        .def("GetCubemap",        &Resources::ResourceManager::Get<Resources::Cubemap       >, "Returns a cubemap with the given name.",         py::arg("name"), py::arg("noWarning") = false, py::return_value_policy::reference)
        .def("GetMaterial",       &Resources::ResourceManager::Get<Resources::Material      >, "Returns a material with the given name.",        py::arg("name"), py::arg("noWarning") = false, py::return_value_policy::reference)
        .def("GetMesh",           &Resources::ResourceManager::Get<Resources::Mesh          >, "Returns a mesh with the given name.",            py::arg("name"), py::arg("noWarning") = false, py::return_value_policy::reference)
        .def("GetRenderTexture",  &Resources::ResourceManager::Get<Resources::RenderTexture >, "Returns a render texture with the given name.",  py::arg("name"), py::arg("noWarning") = false, py::return_value_policy::reference)
        .def("GetTexture",        &Resources::ResourceManager::Get<Resources::Texture       >, "Returns a texture with the given name.",         py::arg("name"), py::arg("noWarning") = false, py::return_value_policy::reference)
        .def("GetVertexShader",   &Resources::ResourceManager::Get<Resources::VertexShader  >, "Returns a vertex shader with the given name.",   py::arg("name"), py::arg("noWarning") = false, py::return_value_policy::reference)
        .def("GetFragmentShader", &Resources::ResourceManager::Get<Resources::FragmentShader>, "Returns a fragment shader with the given name.", py::arg("name"), py::arg("noWarning") = false, py::return_value_policy::reference)
        .def("GetShaderProgram",  &Resources::ResourceManager::Get<Resources::ShaderProgram >, "Returns a shader program with the given name.",  py::arg("name"), py::arg("noWarning") = false, py::return_value_policy::reference)

        .def("Delete", &Resources::ResourceManager::Delete, "Deletes the resource with the given name.", py::arg("name"))

        .def("IsFileLoaded", &Resources::ResourceManager::IsFileLoaded, "Returns true if a file with the given filename is loaded.",                   py::arg("filename"))
        .def("Exists",       &Resources::ResourceManager::Exists,       "Returns true if a resource with the given name has been created.",            py::arg("name"))
        .def("InPipeline",   &Resources::ResourceManager::InPipeline,   "Returns true if a resource with the given name has been sent to GPU memory.", py::arg("name"));

    // --- Light --- //
    py::class_<Render::Light>(m, "Light")
        .def(py::init<int>())

        .def_readwrite("transform",   &Render::Light::transform)
        .def_readwrite("ambient",     &Render::Light::ambient)
        .def_readwrite("diffuse",     &Render::Light::diffuse)
        .def_readwrite("specular",    &Render::Light::specular)
        .def_readwrite("constant",    &Render::Light::constant)
        .def_readwrite("linear",      &Render::Light::linear)
        .def_readwrite("quadratic",   &Render::Light::quadratic)
        .def_readwrite("outerCutoff", &Render::Light::outerCutoff)
        .def_readwrite("innerCutoff", &Render::Light::innerCutoff)

        .def("GetId", &Render::Light::GetId, "Returns the light's id.");

    // --- LightManager --- //
    py::class_<Render::LightManager>(m, "LightManager")
        .def(py::init<>())

        .def("Create", &Render::LightManager::Create, "Creates and returns a new light.", py::return_value_policy::reference)
        .def("Get",    [](Render::LightManager& self, const int& id) { self.Get(id); }, "Returns the light with the given id.", py::arg("id"), py::return_value_policy::reference)
        //.def("GetLights", &Render::LightManager::GetLights, "", py::return_value_policy::reference);
        .def("RemoveById", [](Render::LightManager& self, const int& id) { self.Remove(id); }, "Removes the light with the given id.", py::arg("id"))
        .def("Remove",     [](Render::LightManager& self, const Render::Light* light) { self.Remove(light); }, "Removes the given light object from list.", py::arg("light"))
        .attr("MAX_LIGHTS") = &Render::LightManager::MAX_LIGHTS;

    // --- CameraParams --- //
    py::class_<Render::CameraParams>(m, "CameraParams")
        .def(py::init<unsigned int, unsigned int>())
        .def(py::init<unsigned int, unsigned int, float, float, float>())

        .def_readwrite("width",  &Render::CameraParams::width)
        .def_readwrite("height", &Render::CameraParams::height)
        .def_readwrite("near",   &Render::CameraParams::near)
        .def_readwrite("far",    &Render::CameraParams::far)
        .def_readwrite("fov",    &Render::CameraParams::fov)
        .def_readonly("aspect",  &Render::CameraParams::aspect);

    // --- Camera --- //
    py::class_<Render::Camera>(m, "Camera")
        .def(py::init<Render::CameraParams, size_t>())
        .def(py::init<int, int, size_t>())

        .def_readwrite("transform", &Render::Camera::transform)

        .def("SetParams",     &Render::Camera::SetParams,     "Modifies the camera's parameters to the given ones.", py::arg("params"))
        .def("SetFullscreen", &Render::Camera::SetFullscreen, "Toggles camera Fullscreen (if true, the camera's size matches the window size).", py::arg("fullscreen"))

        .def("GetParams",        &Render::Camera::GetParams,        "Returns the camera's parameters.")
        .def("GetProjectionMat", &Render::Camera::GetProjectionMat, "Returns the camera's projection matrix (4x4).")
        .def("GetViewMat",       &Render::Camera::GetViewMat,       "Returns the camera's view matrix (4x4).")
        .def("GetId",            &Render::Camera::GetId,            "Returns the camera's id.");

    // --- CameraManager --- //
    py::class_<Render::CameraManager>(m, "CameraManager")
        .def(py::init<>())

        .def("Create",     &Render::CameraManager::Create, "Creates a new camera from the given parameters.", py::arg("cameraParams"), py::return_value_policy::reference)
        .def("Get",        &Render::CameraManager::Get,    "Returns the camera with the given ID.", py::arg("index"), py::return_value_policy::reference)
        .def("DeleteById", [](Render::CameraManager& self, const int& index) { self.Delete(index); }, "Deletes the camera with the given ID.", py::arg("index"))
        .def("Delete",     [](Render::CameraManager& self, const Render::Camera* camera) { self.Delete(camera); }, "Deletes the given camera.", py::arg("camera"))

        .def("SetSceneCamera",  &Render::CameraManager::SetSceneCamera,  "Changes which camera is used to render the scene in play mode.",    py::arg("camera"))
        .def("GetEngineCamera", &Render::CameraManager::GetEngineCamera, "Returns the camera from which the scene is rendered in edit mode.", py::return_value_policy::reference)
        .def("GetSceneCamera",  &Render::CameraManager::GetSceneCamera,  "Returns the camera from which the scene is rendered in play mode.", py::return_value_policy::reference);

    // --- PostProcessor --- //
    py::class_<Render::PostProcessor>(m, "PostProcessor")
        .def(py::init<>())

        .def("BeginRender", &Render::PostProcessor::BeginRender, "Begins rendering on the given render texture.",        py::arg("renderTexture"))
        .def("EndRender",   &Render::PostProcessor::EndRender,   "Applies post-processing to the given render texture.", py::arg("renderTexture"))

        .def("AddShader",        &Render::PostProcessor::AddShader,        "Adds the given shader to the post-processing shaders.", py::arg("shader"), py::arg("script") = nullptr)
        .def("AddShaderProgram", &Render::PostProcessor::AddShaderProgram, "Adds the given shader to the post-processing shaders.", py::arg("shaderProgram"));

    // --- ObjectTag --- //
    py::class_<Scenes::ObjectTag>(m, "ObjectTag")
        .def(py::init<std::string>())

        .def_readonly("name",    &Scenes::ObjectTag::name)
        .def_readwrite("tagged", &Scenes::ObjectTag::tagged)

        .def("Tag",   &Scenes::ObjectTag::Tag,   "Gives this tag to the given game object.",     py::arg("gameObject"))
        .def("Untag", &Scenes::ObjectTag::Untag, "Removes this tag from the given game object.", py::arg("gameObject"));
    
    #pragma region GameObject.h
    // --- GameObjectType --- //
    py::enum_<Scenes::GameObjectType>(m, "GameObjectType")
        .value("Empty",     Scenes::GameObjectType::Empty)
        .value("Model",     Scenes::GameObjectType::Model)
        .value("Instanced", Scenes::GameObjectType::Instanced)
        .value("Camera",    Scenes::GameObjectType::Camera)
        .value("Cubemap",   Scenes::GameObjectType::Cubemap)
        .value("Canvas",    Scenes::GameObjectType::Canvas)
        .value("Light",     Scenes::GameObjectType::Light)
        .export_values();

    // --- GameObject --- //
    py::class_<Scenes::GameObject>(m, "GameObject")
        .def(py::init<Scenes::Scene*, size_t, std::string, Scenes::GameObject*>())

        .def_readwrite("name",      &Scenes::GameObject::name)
        .def_readwrite("transform", &Scenes::GameObject::transform)
        //.def_readwrite("rigidBody", &Scenes::GameObject::rigidBody)

        .def("AddScript", &Scenes::GameObject::AddScript, "Adds the given script to this game object's scrpits.", py::arg("script"))
        .def("GetScript", &Scenes::GameObject::GetScript, "Returns the script at the given index.", py::arg("id"), py::return_value_policy::reference)
        .def("RemoveScriptById", [](Scenes::GameObject& self, const int& id) { self.RemoveScript(id); },                     "Deletes the given script and removes it from this game object's scripts.", py::arg("id"))
        .def("RemoveScript",     [](Scenes::GameObject& self, Scripts::ObjectScript* script) { self.RemoveScript(script); }, "Deletes the script at the given index and removes it from this game object's scripts.", py::arg("script"))

        .def("AddChild",    &Scenes::GameObject::AddChild,    "Adds the given game object to this game object's children.",            py::arg("child"))
        .def("RemoveChild", &Scenes::GameObject::RemoveChild, "Removes the child with the given id from this game object's children.", py::arg("childId"))
        .def("SetParent",   &Scenes::GameObject::SetParent,   "Sets this game object's parent to the given game object.",              py::arg("newParent"))

        .def("GetTags",   &Scenes::GameObject::GetTags,   "Gets all tags of game object")
        .def("AddTag",    &Scenes::GameObject::AddTag,    "Adds the given tag to game object",    py::arg("tag"))
        .def("RemoveTag", &Scenes::GameObject::RemoveTag, "Removes the given tag to game object", py::arg("tag"))
        .def("ClearTags", &Scenes::GameObject::ClearTags, "Removes all tags to game object")

        .def("GetId",            &Scenes::GameObject::GetId,       "Returns this game object's ID.")
        .def("GetType",          &Scenes::GameObject::GetType,     "Returns this game object's type.")
        .def("GetParent",        &Scenes::GameObject::GetParent,   "Returns this game object's parent.", py::return_value_policy::reference)
        .def("GetChildren",      &Scenes::GameObject::GetChildren, "Returns a vector of all of this game object's children.")
        .def("FindInChildrenId", [](Scenes::GameObject& self, const size_t& id){ return self.FindInChildren(id); },          "Searches recursively in children to find and return a game object with the given ID.",   py::arg("searchId"),   py::return_value_policy::reference)
        .def("FindInChildren",   [](Scenes::GameObject& self, const std::string& name){ return self.FindInChildren(name); }, "Searched recursively in children to find and return a game object with the given name.", py::arg("searchName"), py::return_value_policy::reference);

    // --- ModelObject --- //
    py::class_<Scenes::ModelObject, Scenes::GameObject>(m, "ModelObject")
        .def(py::init<Scenes::Scene*, size_t, std::string, Resources::Mesh*, Scenes::GameObject*>())

        .def_readwrite("mesh", &Scenes::ModelObject::mesh)

        .def("Draw", &Scenes::ModelObject::Draw, "Renders this model object with the given camera and light manager.", py::arg("renderer"), py::arg("camera"), py::arg("lightManager"));

    // --- InstancedObject --- //
    py::class_<Scenes::InstancedObject, Scenes::GameObject>(m, "InstancedObject")
        .def(py::init<Scenes::Scene*, size_t, std::string, Resources::Mesh*, int, Scenes::GameObject*>())

        .def_readwrite("instanceCount", &Scenes::InstancedObject::instanceCount)
        .def_readwrite("mesh",          &Scenes::InstancedObject::mesh)
        .def_readwrite("transforms",    &Scenes::InstancedObject::transforms)

        .def("Draw", &Scenes::InstancedObject::Draw, "Renders this instanced object with the given camera and light manager.", py::arg("renderer"), py::arg("camera"), py::arg("lightManager"));

    // --- CameraObject --- //
    py::class_<Scenes::CameraObject, Scenes::GameObject>(m, "CameraObject")
        .def(py::init<Scenes::Scene*, size_t, std::string, Render::Camera*, Scenes::GameObject*>())

        .def_readwrite("camera", &Scenes::CameraObject::camera);

    // --- CubemapObject --- //
    py::class_<Scenes::CubemapObject, Scenes::GameObject>(m, "CubemapObject")
        .def(py::init<Scenes::Scene*, size_t, std::string, Resources::Cubemap*, Scenes::GameObject*>())

        .def_readwrite("mesh", &Scenes::CubemapObject::mesh)
        .def_readwrite("cubemap", &Scenes::CubemapObject::cubemap)

        .def("Draw", &Scenes::CubemapObject::Draw, "Renders this cubemap object with the given camera and light manager.", py::arg("renderer"), py::arg("camera"), py::arg("lightManager"));
  
    // --- CanvasObject --- //
    py::class_<Scenes::CanvasObject, Scenes::GameObject>(m, "CanvasObject")
        .def(py::init<Scenes::Scene*, size_t, std::string, Scenes::GameObject*>())

        .def("AddEmptyWidget",     &Scenes::CanvasObject::AddEmptyWidget,  "Creates a new empty widget with the given name.",  py::arg("widgetName"), py::arg("widgetParent") = py::none(), py::return_value_policy::reference)
        .def("AddTextWidget",      &Scenes::CanvasObject::AddTextWidget,   "Creates a new text widget with the given name.",   py::arg("widgetName"), py::arg("widgetParent") = py::none(), py::return_value_policy::reference)
        .def("AddButtonWidget",    &Scenes::CanvasObject::AddButtonWidget, "Creates a new button widget with the given name.", py::arg("widgetName"), py::arg("widgetParent") = py::none(), py::return_value_policy::reference)
        .def("RemoveWidget",       [](Scenes::CanvasObject& self, const Interface::Widget* widget){ self.RemoveWidget(widget); }, "Removes the given widget.",             py::arg("widget"))
        .def("RemoveWidgetWithId", [](Scenes::CanvasObject& self, const size_t& widgetId){ self.RemoveWidget(widgetId); },        "Removes the widget with the given ID.", py::arg("widgetId"))

        .def("FindWidget",       [](Scenes::CanvasObject& self, const const std::string& name) { return self.FindWidget(name);     }, py::arg("widgetName"), py::return_value_policy::reference)
        .def("FindWidgetWithId", [](Scenes::CanvasObject& self, const size_t& widgetId)        { return self.FindWidget(widgetId); }, py::arg("widgetId"),   py::return_value_policy::reference)
        .def("GetRoot",    &Scenes::CanvasObject::GetRoot, py::return_value_policy::reference)

        .def("Update", &Scenes::CanvasObject::Update, py::arg("inputs"))
        .def("Draw",   &Scenes::CanvasObject::Draw, py::arg("renderer"));
    #pragma endregion
        
    #pragma region Widget.h
    // --- WidgetType --- //
    py::enum_<Interface::WidgetType>(m, "WidgetType")
        .value("Empty",  Interface::WidgetType::Empty)
        .value("Text",   Interface::WidgetType::Text)
        .value("Button", Interface::WidgetType::Button)
        .export_values();

    // --- Widget --- //
    py::class_<Interface::Widget>(m, "Widget")
        // .def(py::init<Scenes::CanvasObject*, size_t, std::string, uint, Interface::WidgetAnchor, Interface::WidgetType>())
        .def_readwrite("name",     &Interface::Widget::name)
        .def_readwrite("position", &Interface::Widget::position)
        .def_readwrite("scale",    &Interface::Widget::scale)
        .def_readwrite("rotation", &Interface::Widget::rotation)

        .def("AddChild",    &Interface::Widget::AddChild,    "Adds the given Widget to this widget children.",                py::arg("child"))
        .def("RemoveChild", &Interface::Widget::RemoveChild, "Removes the child at the given id from this widgets children.", py::arg("childId"))
        .def("SetParent",   &Interface::Widget::SetParent,   "Sets this widgets parent to the given widget.",                 py::arg("parent"))

        .def("GetType",   &Interface::Widget::GetType,   "Returns the widget's name.")
        .def("GetId",     &Interface::Widget::GetId,     "Returns the widget's ID.")
        .def("GetParent", &Interface::Widget::GetParent, "Returns the widget's parent.")
        .def("GetLayer",  &Interface::Widget::GetLayer,  "Returns the widget's layer.")
        
        .def("GetChildren",      &Interface::Widget::GetChildren, "Returns an array of all the widget's children.")
        .def("FindInChildren",   [](Interface::Widget& self, const std::string& name){ return self.FindInChildren(name); }, "Recursively searches for a widget with the given name.")
        .def("FindInChildrenId", [](Interface::Widget& self, const size_t&      id)  { return self.FindInChildren(id);   }, "Recursively searches for a widget with the given ID.");

    // --- ButtonWidget --- //
    py::class_<Interface::ButtonWidget, Interface::Widget>(m, "ButtonWidget")
        // .def(py::init<Scenes::CanvasObject*, size_t, std::string, uint, Interface::WidgetAnchor>())

        .def_readwrite("buttonTexture", &Interface::ButtonWidget::buttonTexture)
        .def_readwrite("hoverTexture", &Interface::ButtonWidget::hoverTexture)
        .def_readwrite("pressTexture", &Interface::ButtonWidget::pressTexture)
        .def_readwrite("textureSize", &Interface::ButtonWidget::textureSize)

        .def("Draw", &Interface::ButtonWidget::Draw, "", py::arg("renderer"));

    // --- TextWidget --- //
    py::class_<Interface::TextWidget, Interface::Widget>(m, "TextWidget")
        // .def(py::init<Scenes::CanvasObject*, size_t, std::string, uint, Interface::WidgetAnchor>())

        .def_readwrite("text", &Interface::TextWidget::text)
        .def_readwrite("font", &Interface::TextWidget::font)
        .def_readwrite("color", &Interface::TextWidget::color)

        .def("Draw", &Interface::TextWidget::Draw, "", py::arg("renderer"));
    #pragma endregion

    // --- SceneGraph --- //
    py::class_<Scenes::SceneGraph>(m, "SceneGraph")
        .def(py::init<Scenes::Scene*>())

        .def("AddEmpty",     &Scenes::SceneGraph::AddEmpty,     "Adds a new empty object to the scene and returns it.",    py::arg("name"),                                            py::arg("parent") = py::none(), py::return_value_policy::reference)
        .def("AddModel",     &Scenes::SceneGraph::AddModel,     "Adds a new model to the scene and returns it.",           py::arg("name"), py::arg("mesh"),                           py::arg("parent") = py::none(), py::return_value_policy::reference)
        .def("AddInstanced", &Scenes::SceneGraph::AddInstanced, "Adds a new instanced model to the scene and returns it.", py::arg("name"), py::arg("mesh"), py::arg("instanceCount"), py::arg("parent") = py::none(), py::return_value_policy::reference)
        .def("AddLight",     &Scenes::SceneGraph::AddLight,     "Adds a new light to the scene and returns it.",           py::arg("name"), py::arg("light"),                          py::arg("parent") = py::none(), py::return_value_policy::reference)
        .def("AddCamera",    &Scenes::SceneGraph::AddCamera,    "Adds a new camera light to the scene and returns it.",    py::arg("name"), py::arg("camera"),                         py::arg("parent") = py::none(), py::return_value_policy::reference)
        .def("AddCubemap",   &Scenes::SceneGraph::AddCubemap,   "Adds a new cubemap to the scene and returns it.",         py::arg("name"), py::arg("cubemap"),                        py::arg("parent") = py::none(), py::return_value_policy::reference)
        .def("AddCanvas",    &Scenes::SceneGraph::AddCanvas,    "Adds a new canvas to the scene and returns it.",          py::arg("name"),                                            py::arg("parent") = py::none(), py::return_value_policy::reference)

        .def("CreateTag", &Scenes::SceneGraph::CreateTag, "Creates a new tag with the given name and returns it.", py::arg("name"), py::return_value_policy::reference)

        .def("DrawAll", &Scenes::SceneGraph::DrawAll, "Renders all game objects with the given camera and light manager.", py::arg("renderer"), py::arg("camera"), py::arg("lightManager"))
    
        .def("Find",        [](Scenes::SceneGraph& self, const std::string& name){ return self.Find(name); }, "Tries to find a game object with the given name.", py::arg("searchName"), py::return_value_policy::reference)
        .def("FindWithId",  [](Scenes::SceneGraph& self, const size_t& id){ return self.Find(id); }, "Tries to find a game object with the given ID.", py::arg("searchId"), py::return_value_policy::reference)
        .def("FindWithTag", &Scenes::SceneGraph::FindWithTag, "Tries to find a game object with the given name and tag.", py::arg("searchName"), py::arg("tag"))

        .def("GetRoot", &Scenes::SceneGraph::GetRoot, "Returns the root game object.", py::return_value_policy::reference);

    #pragma region ObjectScript.h PythonScript.h
    // --- Object Script --- //
    py::class_<Scripts::ObjectScript>(m, "ObjectScript")
        .def(py::init<>())

        // .def_readwrite("object",    &Scripts::ObjectScript::object)
        // .def_readwrite("transform", &Scripts::ObjectScript::transform)
        // .def_readwrite("time",      &Scripts::ObjectScript::time)
        // .def_readwrite("scene",     &Scripts::ObjectScript::scene)
        // .def_readwrite("engine",    &Scripts::ObjectScript::engine)
        .def_readwrite("inputs",    &Scripts::ObjectScript::inputs)

        .def("Start",      &Scripts::ObjectScript::Start,      "Function called when the scene is in play mode, or when the object is added to the scene while in play mode.")
        .def("LateStart",  &Scripts::ObjectScript::LateStart,  "Similar to the Start function, but is called after all other objects' Start functions are done.")
        .def("Update",     &Scripts::ObjectScript::Update,     "Function called every frame when the scene is in play mode.")
        .def("LateUpdate", &Scripts::ObjectScript::LateUpdate, "Similar to the Update function, but is called after all other objects' Update functions are done.");
    
    // --- PythonScript --- //
    py::class_<Scripts::PythonScript, Scripts::ObjectScript>(m, "PythonScript")
        .def(py::init<std::string>())

        .def("GetObject", &Scripts::PythonScript::GetObject)
        .def("GetName", &Scripts::PythonScript::GetName);
    #pragma endregion

    #pragma region Window.h
    // --- WindowInputs --- //
    py::class_<Core::WindowInputs>(m, "WindowInputs")
        .def_readonly("movement", &Core::WindowInputs::movement)
        .def_readonly("mousePos", &Core::WindowInputs::mousePos)
        .def_readonly("mouseDelta", &Core::WindowInputs::mouseDelta)
        .def_readonly("mouseLeftClick", &Core::WindowInputs::mouseLeftClick)
        .def_readonly("mouseMiddleClick", &Core::WindowInputs::mouseMiddleClick)
        .def_readonly("mouseRightClick", &Core::WindowInputs::mouseRightClick);

    // --- WindowInputs --- //
        py::class_<Core::InputKeys>(m, "InputKeys")
            .def(py::init<>())

            .def_readonly("exit", &Core::InputKeys::exit)
            .def_readonly("movement", &Core::InputKeys::movement);

    // --- WindowParams --- //
    py::class_<Core::WindowParams>(m, "WindowParams")
        .def(py::init<>())

        .def_readonly("name", &Core::WindowParams::name)
        .def_readonly("width", &Core::WindowParams::width)
        .def_readonly("height", &Core::WindowParams::height)
        .def_readonly("posX", &Core::WindowParams::posX)
        .def_readonly("posY", &Core::WindowParams::posY);

    // --- Window --- //
    py::class_<Core::Window>(m, "Window")
        .def(py::init<Core::WindowParams>())

        .def("LockInputs",     &Core::Window::LockInputs,     "Prevents any inputs from registering until turned back on.", py::arg("locked"))
        .def("LockHideCursor", &Core::Window::LockHideCursor, "Hides the mouse cursor and locks it in place.",              py::arg("locked"))

        .def("SetName",       &Core::Window::SetName,       "Sets the window's name.",           py::arg("windowName"))
        .def("SetWidth",      &Core::Window::SetWidth,      "Sets the window's width.",          py::arg("windowWidth"))
        .def("SetHeight",     &Core::Window::SetHeight,     "Sets the window's height.",         py::arg("windowHeight"))
        .def("SetSize",       &Core::Window::SetSize,       "Sets the window's size.",           py::arg("windowWidth"), py::arg("windowHeight"))
        .def("SetPosX",       &Core::Window::SetPosX,       "Sets the window's X position.",     py::arg("windowPosX"))
        .def("SetPosY",       &Core::Window::SetPosY,       "Sets the window's Y position.",     py::arg("windowPosY"))
        .def("SetPos",        &Core::Window::SetPos,        "Sets the window's position.",       py::arg("windowPosX"), py::arg("windowPosY"))
        .def("SetVsync",      &Core::Window::SetVsync,      "Toggles vertical synchronization.", py::arg("windowVsync"))
        .def("SetExitKey",    &Core::Window::SetExitKey,    "Sets the window's exit key.",       py::arg("windowExitKey"))
        .def("RemoveExitKey", &Core::Window::RemoveExitKey, "Removes the window's exit key.")

        .def("GetName",     &Core::Window::GetName,     "Returns the window's name.")
        .def("GetWidth",    &Core::Window::GetWidth,    "Returns the window's width.")
        .def("GetHeight",   &Core::Window::GetHeight,   "Returns the window's height.")
        .def("GetSize",     &Core::Window::GetSize,     "Returns the window's size.")
        .def("GetAspect",   &Core::Window::GetAspect,   "Returns the window's aspect ratio.")
        .def("GetPosX",     &Core::Window::GetPosX,     "Returns the window's X position.")
        .def("GetPosY",     &Core::Window::GetPosY,     "Returns the window's Y position.")
        .def("GetPos",      &Core::Window::GetPos,      "Returns the window's position.")
        .def("GetInputs",   &Core::Window::GetInputs,   "Returns the window's inputs.")
        .def("ShouldClose", &Core::Window::ShouldClose, "Returns True if the window sould close.");
    #pragma endregion
}