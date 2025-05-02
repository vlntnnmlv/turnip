using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace basic_kafana;

public partial class Node : IPositionable, IRotatable, ITarget
{
    public static Node Root { get; private set; }

    // objects tree
    public string ID { get; private set; }
    public Node Parent { get; private set; }
    public List<Node> Children { get; private set; } = new();
    public bool Enabled { get; set; } = true;

    // rendering
    public Vector2 Position { get; set; }
    public Vector2 Size { get; set; }
    public float Rotation { get; set; }
    public Texture2D Texture { get; protected set; }

    // pathfinding
    public virtual Vector2 TargetPosition => Position;


    static Dictionary<string, Node> _objects = new();

    static public void RemoveObject(string id)
    {
        _objects.Remove(id);
    }

    static public Node GetObject(string id)
    {
        return _objects[id];
    }

    static public void IterateLinear(Action<Node> action)
    {
        foreach (Node obj in _objects.Values)
            action?.Invoke(obj);
    }

    static public void IterateTree(Action<Node> action)
    {
        LinkedList<Node> queue = new();
        List<Node> answer = new();

        queue.AddLast(Root);
        while (queue.Count != 0)
        {
            Node first = queue.First.Value;
            answer.Add(first);
            queue.RemoveFirst();
            foreach (Node child in first.Children)
            {
                queue.AddLast(child);
            }
        }

        foreach (Node obj in answer)
            action?.Invoke(obj);
    }

    bool _debug;
    Node _debugFrame;

    public Node(string id, Node parent, Texture2D texture, Vector2 position, Vector2 size)
    {
        ID = id;
        Position = position;
        Size = size;

        Texture = texture;

        _objects.Add(ID, this);

        if (parent == null)
            Root = this;
        else
            parent.LinkChild(this);
    }

    public void LinkChild(Node obj)
    {
        obj.Parent = this;
        Children.Add(obj);
    }

    public virtual void Update(GameTime gameTime)
    {
        ProcessEvent();
    }

    bool _hovered;

    void ProcessEvent()
    {
        MouseState mouseState = Mouse.GetState();

        Point mousePosition = mouseState.Position;

        _hovered = WorldRectangle.Contains(mousePosition);
    }

    public Rectangle LocalRectangle => new Rectangle(
        (int)Position.X,
        (int)Position.Y,
        (int)Size.X,
        (int)Size.Y
    );

    public Rectangle WorldRectangle => new Rectangle(
        LocalRectangle.X + (Parent == null ? 0 : (int)Parent.Position.X),
        LocalRectangle.Y + (Parent == null ? 0 : (int)Parent.Position.Y),
        LocalRectangle.Width,
        LocalRectangle.Height
    );

    public SizeType WidthType;
    public SizeType HeightType;
    public Margin Margin;

    public void Measure()
    {
        if (WidthType == SizeType.FILL)
        {
            Size = new Vector2(Parent.Size.X - (Margin.Right + Margin.Left), Size.Y);
            Position = new Vector2(Margin.Left, Position.Y);
        }

        if (HeightType == SizeType.FILL)
        {
            Size = new Vector2(Size.X, Parent.Size.Y - (Margin.Bottom + Margin.Top));
            Position = new Vector2(Position.X, Margin.Top);
        }
    }

    public virtual Rectangle SourceRectangle => new Rectangle(0, 0, Texture.Width, Texture.Height);
    public virtual Vector2 Origin => Vector2.Zero;
    public virtual SamplerState SamplerState => SamplerState.PointClamp;

    public virtual void Render(SpriteBatch spriteBatch)
    {
        // do nothing
        Measure();
    }

    public bool Is9Sliced { get; set; }

    public virtual void Draw(SpriteBatch spriteBatch)
    {
        if (!Enabled)
            return;
        
        Rectangle[] destinationRectangles;
        Rectangle[] sourceRectangles;
        if (Is9Sliced)
        {
            destinationRectangles = CreatePatches(WorldRectangle);
            sourceRectangles = CreatePatches(Texture.Bounds);
        }
        else
        {
            destinationRectangles = [WorldRectangle];
            sourceRectangles = [SourceRectangle];
        }

        spriteBatch.Begin(SpriteSortMode.Immediate, BlendState.AlphaBlend, SamplerState, null, null);
        for (int i = 0; i < sourceRectangles.Length; ++i)
        {
            spriteBatch.Draw(
                Texture,
                destinationRectangles[i],
                sourceRectangles[i],
                Color.White,
                Rotation,
                Origin * new Vector2(Texture.Width, Texture.Height),
                SpriteEffects.None,
                0.0f
            );
        }
        spriteBatch.End();
    }

    const int LEFT_PADDING = 2;
    const int RIGHT_PADDING = 2;
    const int TOP_PADDING = 2;
    const int BOTTOM_PADDING = 2;

    private Rectangle[] CreatePatches(Rectangle rectangle)
    {
        int x = rectangle.X;
        int y = rectangle.Y;
        int w = rectangle.Width;
        int h = rectangle.Height;
        int middleWidth = w - LEFT_PADDING - RIGHT_PADDING;
        int middleHeight = h - TOP_PADDING - BOTTOM_PADDING;
        int bottomY = y + h - BOTTOM_PADDING;
        int rightX = x + w - RIGHT_PADDING;
        int leftX = x + LEFT_PADDING;
        int topY = y + TOP_PADDING;
        Rectangle[] patches =
        {
            new Rectangle(x,      y,        LEFT_PADDING,  TOP_PADDING),      // top left
            new Rectangle(leftX,  y,        middleWidth,  TOP_PADDING),      // top middle
            new Rectangle(rightX, y,        RIGHT_PADDING, TOP_PADDING),      // top right
            new Rectangle(x,      topY,     LEFT_PADDING,  middleHeight),    // left middle
            new Rectangle(leftX,  topY,     middleWidth,  middleHeight),    // middle
            new Rectangle(rightX, topY,     RIGHT_PADDING, middleHeight),    // right middle
            new Rectangle(x,      bottomY,  LEFT_PADDING,  BOTTOM_PADDING),   // bottom left
            new Rectangle(leftX,  bottomY,  middleWidth,  BOTTOM_PADDING),   // bottom middle
            new Rectangle(rightX, bottomY,  RIGHT_PADDING, BOTTOM_PADDING)    // bottom right
        };
        return patches;
    }

    public void DrawDebug(SpriteBatch spriteBatch)
    {
        spriteBatch.Begin(SpriteSortMode.Immediate, BlendState.AlphaBlend, SamplerState, null, null);
        spriteBatch.Draw(
            AssetManager.GetTexture("debug.background"),
            WorldRectangle,
            null,
            Color.White,
            Rotation,
            Origin,
            SpriteEffects.None,
            0.0f
        );

        Rectangle[] destinationRectangles;
        Rectangle[] sourceRectangles;
        destinationRectangles = CreatePatches(WorldRectangle);
        sourceRectangles = CreatePatches(AssetManager.GetTexture("debug.frame").Bounds);

        for (int i = 0; i < sourceRectangles.Length; ++i)
        {
            spriteBatch.Draw(
                AssetManager.GetTexture("debug.frame"),
                destinationRectangles[i],
                sourceRectangles[i],
                Color.White,
                Rotation,
                Origin * new Vector2(Texture.Width, Texture.Height),
                SpriteEffects.None,
                0.0f
            );
        }

        spriteBatch.End();
    }
}