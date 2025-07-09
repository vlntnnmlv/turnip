using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public struct Projection
{
    public float Min;
    public float Max;

    public static bool Overlaps(Projection _One, Projection _Other)
    {
        return _One.Min < _Other.Max || _One.Max > _Other.Min;
    }

    public static float GetOverlap(Projection _One, Projection _Other)
    {
        if (_One.Max > _Other.Min && _One.Max < _Other.Max)
            return _One.Max - _Other.Min;

        if (_Other.Max > _One.Min && _Other.Max < _One.Max)
            return _Other.Max - _One.Min;

        return 0.0f;
    }
}

// TODO: SAT doesnt work! fix.
public struct Collision
{
    public bool IsValid;

    public Shape One;
    public Shape Other;
    public int IndexOne;
    public int IndexOther;
    public Vector2 MTVDirection;
    public float MTVMagnitude;

    // public Collision(Shape _One, Shape _Other)
    // {
    //     One = _One;
    //     Other = _Other;

    //     Rectangle collisionRect = Raylib.GetCollisionRec(One.Rectangle, Other.Rectangle);
    //     if (collisionRect.Size != Vector2.Zero)
    //     {
    //         IsValid = true;

    //         MTV = GetMTV(One.Rectangle, Other.Rectangle);
    //         Normal = -SPV.Normalized();
    //     }
    // }

    static Vector2 GetMTV(Rectangle _One, Rectangle _Other)
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

    public static Collision SAT(Shape _One, Shape _Other, int _IndexOne, int _IndexOther)
    {
        Collision result = new Collision
        {
            IsValid = false,
            MTVDirection = Vector2.Zero,
            MTVMagnitude = 0,
        };

        Vector2[] oneAxes = [.. _One.Edges.Select(_E => _E.Normal())];
        Vector2[] otherAxes = [.. _Other.Edges.Select(_E => _E.Normal())];
        Vector2[] allAxes = [.. oneAxes, .. otherAxes];

        Vector2 mtvDirection = Vector2.Zero;
        float mtvMagnitude = float.MaxValue;

        foreach (Vector2 axis in allAxes)
        {
            Projection oneProjection = GetProjection(axis, _One);
            Projection otherProjection = GetProjection(axis, _Other);

            float overlap = Projection.GetOverlap(oneProjection, otherProjection);
            if (overlap < 0.5f)
            {
                return result;
            }
            else
            {
                if (overlap >= mtvMagnitude)
                    continue;

                mtvDirection = axis;
                mtvMagnitude = overlap;
            }
        }

        result.IsValid = true;
        result.MTVDirection = mtvDirection;
        result.MTVMagnitude = mtvMagnitude;
        result.IndexOne = _IndexOne;
        result.IndexOther = _IndexOther;
        Console.WriteLine($"d {result.MTVDirection}, m {result.MTVMagnitude}");
        return result;
    }

    public static Projection GetProjection(Vector2 _Axis, Shape _Shape)
    {
        float min = Vector2.Dot(_Axis, _Shape.Points[0]);
        float max = min;

        for (int i = 0; i < _Shape.Points.Length; i++)
        {
            float p = Vector2.Dot(_Shape.Points[i], _Axis);
            if (p < min) min = p;
            if (p > max) max = p;
        }

        return new Projection { Min = min, Max = max };
    }

}