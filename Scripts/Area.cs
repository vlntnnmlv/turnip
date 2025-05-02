using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace basic_kafana;

public class Area : Node
{
    public Area(string id, Node parent, Texture2D texture, Vector2 position, Vector2 size)
        : base(id, parent, texture, position, size)
    {
    }

    public override Rectangle SourceRectangle
    {
        get
        {
            Rectangle rectangle = WorldRectangle;
            rectangle.Location = Point.Zero;

            return rectangle;
        }
    }

    public override Vector2 Origin => Vector2.Zero;
    public override SamplerState SamplerState => SamplerState.PointWrap;

}