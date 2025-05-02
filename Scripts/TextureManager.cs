using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

public static class TextureManager
{
    private static Game _game;

    public static void Init(Game game)
    {
        _game = game;
    }

    public static RenderTarget2D GetRenderTarget(Vector2 size)
    {
        RenderTarget2D target = new RenderTarget2D(_game.GraphicsDevice, (int)size.X, (int)size.Y);
        return target;
    }

    public static void SetRenderTarget(RenderTarget2D target)
    {
        _game.GraphicsDevice.SetRenderTarget(target);
    }

    public static void ResetRenderTarget()
    {
        _game.GraphicsDevice.SetRenderTarget(null);
    }
}