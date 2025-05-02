using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace basic_kafana;

public class Ingredient : Node
{
    public Ingredient(string id, Texture2D texture, Vector2 position, Vector2 size)
        : base(id, Node.Root, texture, position, size)
    {
    }

    public float CollectionProgress { get; set; } = 0.0f;
    const float COLLECTION_SPEED = 5f;

    public override void Update(GameTime gameTime)
    {
        base.Update(gameTime);

        if ((GetObject("manualCharacter").Position - Position).Length() < 16f)
        {
            CollectionProgress += (float)gameTime.ElapsedGameTime.TotalSeconds * COLLECTION_SPEED;
        }

        if (CollectionProgress > 1.0f)
            RemoveObject(ID);
    }
}