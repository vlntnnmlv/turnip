using System.Numerics;

namespace BasicKafana;

public struct State
{
    public Box[] Boxes;
    public Collision[] Collisions;
    public Vector2 ManualControl;
};