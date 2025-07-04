using Raylib_cs;
using System.Numerics;

namespace BasicKafana;

public class Meristem
{
    public Meristem(Rectangle _Rect)
    {
        Rect = _Rect;
    }

    public Rectangle WorldRect { get; set; }

    public virtual void Draw(Node _Node)
    {
        if (_Node.Parent == null)
            WorldRect = Rect;
        else
            WorldRect = Rect.Move(_Node.Parent.Meristem.Rect.Position);

        float spacing = 20;
        Rectangle padding = new Rectangle(20, 20, 20, 20);

        if (_Node.Parent?.ID == "vstack")
        {
            int childrenNumber = _Node.Parent.Children.Count;

            Rectangle parentRect = _Node.Parent.Meristem.Rect.Shrink(new Vector2(padding.X + padding.Width, padding.Y + padding.Height));
            float height = (parentRect.Height - spacing * (childrenNumber - 1)) / childrenNumber;

            WorldRect = new Rectangle(parentRect.X + padding.X, parentRect.Y + padding.Y + _Node.Order * height + spacing * _Node.Order, parentRect.Width, height);
        }

        if (_Node.Parent?.ID == "hstack")
        {
            int childrenNumber = _Node.Parent.Children.Count;

            Rectangle parentRect = _Node.Parent.Meristem.Rect.Shrink(new Vector2(padding.X + padding.Width, padding.Y + padding.Height));
            float width = (parentRect.Width - spacing * (childrenNumber - 1)) / childrenNumber;

            WorldRect = new Rectangle(parentRect.X + padding.X + _Node.Order * width + spacing * _Node.Order, parentRect.Y + padding.Y, width, parentRect.Height);
        }
    }

    public void DrawDebug(Node _Node)
    {
        Raylib.DrawRectangleLinesEx(WorldRect, 1, Color.Red);

        int fontSize = 8;

        Vector2 textSize = Raylib.MeasureTextEx(Garden.Font, _Node.ID, fontSize, 0.0f);
        Raylib.DrawTextEx(Garden.Font, _Node.ID, WorldRect.Center() - textSize / 2, fontSize, 5, Color.Red);
    }

    public Rectangle Rect;
}