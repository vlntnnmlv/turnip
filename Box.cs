
using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public struct Box
{
    public string ID;
    public Vector2 Size;
    public Vector2 Position;
    public Vector2 Velocity;
    public Vector2 Acceleration;
    public Vector2 NetForce;
    public bool IsStatic;
    public bool HadCollisionsThisFrame;
    public float Mass;

    public Vector2 Direction => Velocity.Normalized();
    public Rectangle Rectangle => new Rectangle(Position, Size);
};