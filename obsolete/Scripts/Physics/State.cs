using System.Numerics;

namespace Turnip;

public struct State
{
    public Box[] Boxes;
    public Collision[] Collisions;
    public Vector2 ManualControl;
};
