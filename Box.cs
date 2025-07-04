
using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public struct Box
{
    public float HP;
    public string ID;
    public int Index;
    public Vector2 Size;
    public Vector2 Position;
    public Vector2 Velocity;
    public Vector2 Acceleration;
    public Vector2 NetForce;
    public float Rotation;
    public float AngularVelocity;
    public bool IsStatic;
    public bool HadCollisionsThisFrame;
    public float Mass;
    public Material Material;

    public Vector2 Direction => Velocity.Normalized();
    public Rectangle Rectangle => new Rectangle(Position, Size);
    public Shape Collider => new Shape { Points = [Rectangle.Position, new Vector2(Rectangle.Position.X, Rectangle.Position.Y + Rectangle.Height), new Vector2(Rectangle.Position.X + Rectangle.Width, Rectangle.Position.Y + Rectangle.Height)] };// Shape.Rectangle(Rectangle);
};