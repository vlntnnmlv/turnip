using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace basic_kafana;

public class MovementControllerManual : IMovementController
{
    public Vector2 Direction { get; private set; }

    private IPositionable _positionable;

    public void Init(IPositionable positionable)
    {
        _positionable = positionable;
    }

    public Vector2 GetDirection()
    {
        Vector2 direction = Vector2.Zero;
        KeyboardState kstate = Keyboard.GetState();
        if (kstate.IsKeyDown(Keys.W))
            direction.Y -= 1;
        if (kstate.IsKeyDown(Keys.A))
            direction.X -= 1;
        if (kstate.IsKeyDown(Keys.S))
            direction.Y += 1;
        if (kstate.IsKeyDown(Keys.D))
            direction.X += 1;

        if (direction.Length() != 0)
            direction.Normalize();

        Direction = direction;
        return Direction;
    }

    public float GetRotation()
    {
        MouseState ms = Mouse.GetState();
        Vector2 lookAt = new Vector2(ms.X - _positionable.Position.X, ms.Y - _positionable.Position.Y);

        // texture is oriented upwards, so up direction is no rotation
        return MathF.Atan2(lookAt.Y , lookAt.X) + MathF.PI / 2;
    }
}
