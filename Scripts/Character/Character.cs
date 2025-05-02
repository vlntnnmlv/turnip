using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace basic_kafana;

public class Character : Node, IAgent
{
    int _movementSpeed;
    int _rotationSpeed;
    IMovementController _directionController;
    CharacterInfo _info;
    Text _nameTag;


    public override Vector2 Origin => Vector2.One / 2;
    public Character(
            string id,
            Node parent,
            CharacterInfo info,
            Texture2D texture,
            Vector2 position,
            IMovementController directionController,
            int movementSpeed = 50,
            int rotationSpeed = 10
        ) : base(id, parent, texture, position, Vector2.One * 32)
    {
        _info = info;
        _directionController = directionController;
        _movementSpeed = movementSpeed;
        _rotationSpeed = rotationSpeed;

        _nameTag = new Text($"name_{ID}", this, _info.Name, AssetManager.GetFont("kafana"), new Vector2(0, -32));
    }

    public override void Update(GameTime gameTime)
    {
        base.Update(gameTime);

        _nameTag.Enabled = WorldRectangle.Contains(Mouse.GetState().Position.ToVector2());

        Vector2 direction = _directionController.GetDirection();
        Position += direction * _movementSpeed * (float)gameTime.ElapsedGameTime.TotalSeconds;

        float targetRotation = _directionController.GetRotation();
        Rotation = LerpAngle(Rotation, targetRotation, _rotationSpeed * (float)gameTime.ElapsedGameTime.TotalSeconds);
    }

    public static float LerpAngle(float start, float end, float amount)
    {
        float difference = MathHelper.WrapAngle(end - start);

        return start + difference * amount;
    }
}