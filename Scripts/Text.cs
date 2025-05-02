using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace basic_kafana;

public class Text : Node
{
    private string _text;
    private SpriteFont _font;
    private bool _textRendered;

    public Text(string id, Node parent, string text, SpriteFont font, Vector2 position)
        : base(id, parent, null, position, font.MeasureString(text))
    {
        _text = text;
        _font = font;

        Texture = TextureManager.GetRenderTarget(_font.MeasureString(_text));
    }

    public override void Render(SpriteBatch spriteBatch)
    {
        if (!_textRendered)
        {
            spriteBatch.Begin();
            TextureManager.SetRenderTarget(Texture as RenderTarget2D);
            spriteBatch.DrawString(_font, _text, Vector2.Zero, Color.Black, 0.0f, Vector2.Zero, 1.0f, SpriteEffects.None, 0.0f);
            spriteBatch.End();

            _textRendered = true;

            TextureManager.ResetRenderTarget();
        }
    }
}