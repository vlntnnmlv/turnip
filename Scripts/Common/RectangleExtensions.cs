using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public static class RectangleExtensions
{
    public static Vector2 Center(this Rectangle _Rectangle)
    {
        return new Vector2(
            _Rectangle.X + _Rectangle.Width / 2,
            _Rectangle.Y + _Rectangle.Height / 2
        );
    }

    public static Rectangle Move(this Rectangle _Rectangle, Vector2 _Offset)
    {
        _Rectangle.X += _Offset.X;
        _Rectangle.Y += _Offset.Y;

        return _Rectangle;
    }

    public static Rectangle Shrink(this Rectangle _Rectangle, Vector2 _Offset)
    {
        _Rectangle.Width -= _Offset.X;
        _Rectangle.Height -= _Offset.Y;

        return _Rectangle;
    }

    public static Rectangle Expand(this Rectangle _Rectangle, Vector2 _Offset)
    {
        return _Rectangle.Shrink(-_Offset);
    }

    static float Lerp(float _A, float _B, float _Phase)
    {
        return _A + (_B - _A) * _Phase;
    }

    public static Rectangle Lerp(this Rectangle _This, Rectangle _Other, float _Phase)
    {
        return new Rectangle(
            LerpEpsilon(_This.X, _Other.X, _Phase, 2.0f),
            LerpEpsilon(_This.Y, _Other.Y, _Phase, 2.0f),
            LerpEpsilon(_This.Width, _Other.Width, _Phase, 2.0f),
            LerpEpsilon(_This.Height, _Other.Height, _Phase, 2.0f)
        );
    }

    public static bool Contains(this Rectangle _Rectangle, Vector2 _Vector)
    {
        return _Vector.X > _Rectangle.X
            && _Vector.X < _Rectangle.X + _Rectangle.Width
            && _Vector.Y > _Rectangle.Y
            && _Vector.Y < _Rectangle.Y + _Rectangle.Height;
    }

    static float LerpEpsilon(float _A, float _B, float _Phase, float _Epsilon)
    {
        if (Math.Abs(_B - _A) > _Epsilon)
            return Lerp(_A, _B, _Phase);

        return _A;
    }
}
