using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.Collections.Generic;

using basic_kafana;

public class Desk : Node
{
    LinkedList<Character> _visitorsQueue = new();
    Node _defaultTarget;

    public override Vector2 TargetPosition => _visitorsQueue.Count == 0 ? _defaultTarget.Position : _visitorsQueue.Last.Value.Position;

    public Desk(string id, Texture2D texture, Vector2 position, Vector2 size)
        : base(id, Node.Root, texture, position, size)
    {
        _defaultTarget = new Node("defaultTarget", Node.Root, AssetManager.GetTexture("point"), Position + new Vector2(0, 32), Vector2.One * 8);
    }

    public void AddVisitorToQueue(Character visitor)
    {
        _visitorsQueue.AddLast(visitor);
    }
}