using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public static class Vector2Extensions
{
    public static Vector2 Normalized(this Vector2 _Vector)
    {
        return _Vector == Vector2.Zero ? Vector2.Zero : Vector2.Normalize(_Vector);
    }

    public static float Magnitude(this Vector2 _Vector)
    {
        return Vector2.Distance(Vector2.Zero, _Vector);
    }

    public static Vector2 Reversed(this Vector2 _Vector, bool _X)
    {
        Vector2 reverseVector = new Vector2(_X ? -1 : 1, _X ? 1 : -1);
        return _Vector * reverseVector;
    }

    public static Vector2 Normal(this Vector2 _Vector)
    {
        return new Vector2(-_Vector.Y, _Vector.X).Normalized();
    }
}
