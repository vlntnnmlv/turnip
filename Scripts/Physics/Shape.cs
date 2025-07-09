using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public struct Shape
{
    public Vector2[] Points;

    public Vector2[] Edges
    {
        get
        {
            Vector2[] edges = new Vector2[Points.Length];
            for (int i = 0; i < Points.Length; i++)
            {
                int j = (i + 1) % Points.Length;
                edges[i] = Points[j] - Points[i];
            }

            return edges;
        }
    }

    public Vector2 Center => Points.Aggregate((_P1, _P2) => _P1 + _P2) / Points.Length;
    public readonly bool IsRectangle => Points[0].X == Points[1].X && Points[2].X == Points[3].X
                                        && Points[0].Y == Points[3].Y && Points[1].Y == Points[2].Y;

    public static Shape Rectangle(Rectangle _Rectangle)
    {
        return new Shape()
        {
            Points = [
                new Vector2(_Rectangle.X, _Rectangle.Y),
                new Vector2(_Rectangle.X, _Rectangle.Y + _Rectangle.Height),
                new Vector2(_Rectangle.X + _Rectangle.Width, _Rectangle.Y + _Rectangle.Height),
                new Vector2(_Rectangle.X + _Rectangle.Width, _Rectangle.Y),
            ]
        };
    }  
}