using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public struct Collision
{
    public bool IsValid;
    public Box One;
    public Box Other;
    public Vector2 SPV;
    public Vector2 Normal;
    public Vector2 ContactPoint;

    public Collision(Box _One, Box _Other)
    {
        One = _One;
        Other = _Other;

        Rectangle collisionRect = Raylib.GetCollisionRec(One.Rectangle, Other.Rectangle);
        if (collisionRect.Size != Vector2.Zero)
        {
            IsValid = true;

            SPV = GetSPV(One.Rectangle, Other.Rectangle);
            Normal = -SPV.Normalized();
        }
    }

    // SPV = shortest separation vector
    // the resulted vector must be applied to the rect _One
    static Vector2 GetSPV(Rectangle _One, Rectangle _Other)
    {
        float dx1 = _Other.X + _Other.Width - _One.X; // from left
        float dx2 = _One.X + _One.Width - _Other.X; // from right
        float dy1 = _Other.Y + _Other.Height - _One.Y; // from top
        float dy2 = _One.Y + _One.Height - _Other.Y; // from bottom

        float spvX = dx1 < dx2 ? dx1 : -dx2;
        float spvY = dy1 < dy2 ? dy1 : -dy2;

        // Choose the axis with the smallest penetration
        if (Math.Abs(spvX) < Math.Abs(spvY))
            return new Vector2(spvX, 0);
        else
            return new Vector2(0, spvY);
    }
}