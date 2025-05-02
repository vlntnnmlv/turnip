using System;
using Microsoft.Xna.Framework;

namespace basic_kafana;

public interface IMovementController
{
    public Vector2 Direction { get; }
    public Vector2 GetDirection();
    public float GetRotation();
}