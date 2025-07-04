using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public static class RectangleExtensions
{
    public static Vector2 Center(this Rectangle _Rectangle)
    {
        return new Vector2(_Rectangle.X + _Rectangle.Width / 2, _Rectangle.Y + _Rectangle.Height / 2);
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


    public static bool Contains(this Rectangle _Rectangle, Vector2 _Vector)
    {
        return _Vector.X > _Rectangle.X && _Vector.X < _Rectangle.X + _Rectangle.Width
            && _Vector.Y > _Rectangle.Y && _Vector.Y < _Rectangle.Y + _Rectangle.Height;
    }

}