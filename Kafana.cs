#define DEBUG_LAYOUT

using System;
using System.Collections;
using System.Threading.Tasks;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace basic_kafana;

public class Kafana : Game
{
    private GraphicsDeviceManager _graphics;
    private SpriteBatch _spriteBatch;
    // private CharacterManager _characterManager;

    public int Width => _graphics.PreferredBackBufferWidth;
    public int Height => _graphics.PreferredBackBufferHeight;

    public Kafana()
    {
        _graphics = new GraphicsDeviceManager(this);
        GraphicsDevice.DiscardColor = Color.Transparent;

        Content.RootDirectory = "Content";
        IsMouseVisible = true;
    }

    protected override void LoadContent()
    {
        _graphics.PreferredBackBufferWidth = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width / 2;
        _graphics.PreferredBackBufferHeight = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height / 2;
        _graphics.ApplyChanges();

        _spriteBatch = new SpriteBatch(GraphicsDevice);

        RandomManager.Init();
        AssetManager.Init(this);
        TextureManager.Init(this);
    }

    protected override void Initialize()
    {
        base.Initialize();

        // root
        new Area("root", null, AssetManager.GetTexture("background"), Vector2.Zero, new Vector2(Width, Height));
        // Object grass = new Area("grass", Object.Root, AssetManager.GetTexture("grass"), new Vector2(Width / 2, Height / 2), new Vector2(Width / 2, Height / 2));
        // Area tile = new Area("tile", grass, AssetManager.GetTexture("tile"), Vector2.Zero, new Vector2(Width / 4, Height / 4));
        // Object player = new Object("player", grass, AssetManager.GetTexture("player"), Vector2.Zero, Vector2.One * 64);
        // player.Rotation = 3.14f / 4.0f;

        // background objects
        // Texture2D tileTexture = AssetManager.GetTexture("tile");
        // float tileHeight = MathF.Floor(0.5f * Height / tileTexture.Height) * tileTexture.Height;
        // new Area("tile", Object.Root, tileTexture, new Vector2(0, 0), new Vector2(Width, tileHeight));
        // new Area("grass", Object.Root, AssetManager.GetTexture("grass"), new Vector2(0, tileHeight), new Vector2(Width, Height - tileHeight));
        new Text("title", Node.Root, "BASIC KAFANA", AssetManager.GetFont("kafana"),new Vector2(Width, Height) / 2);

        // tmp
        // Object flll = new Object("dialog_fill", Object.Root, AssetManager.GetTexture("dialog_fill"), new Vector2(50, 100), new Vector2(500, 300));
        _frame = new Node("dialog_frame", Node.Root, AssetManager.GetTexture("dialog_frame"), new Vector2(50, 100), new Vector2(500, 300));
        _frame.Is9Sliced = true;
        
        Node tmp = new Node("tmp", _frame, AssetManager.GetTexture("dialog_fill"), Vector2.Zero, Vector2.Zero);
        tmp.WidthType = SizeType.FILL;
        tmp.HeightType = SizeType.FILL;
        tmp.Margin = new Margin(2, 2, 2, 2);

        // initialize managers
        // _characterManager = new CharacterManager(this);
        // _characterManager.CreateManualCharacter(new Vector2(Width / 2, Height / 2));
    }
    Node _frame;

    protected override void Update(GameTime gameTime)
    {
        if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
            Exit();

        // if (Keyboard.GetState().IsKeyDown(Keys.P))
        //     _frame.Size += Vector2.One * 10 * (float)gameTime.ElapsedGameTime.TotalSeconds;

        // if (Keyboard.GetState().IsKeyDown(Keys.O))
        //     _frame.Size -= Vector2.One * 10 * (float)gameTime.ElapsedGameTime.TotalSeconds;

        // MouseState mouseState = Mouse.GetState();

        // if (_frame.WorldRectangle.Intersects(
        //         new Rectangle(
        //             mouseState.Position.X - 50,
        //             mouseState.Position.Y - 50,
        //             mouseState.Position.X + 50,
        //             mouseState.Position.Y + 50
        //             )
        //         )
        //         && mouseState.LeftButton == ButtonState.Pressed
        //     )
        // {
        //     _frame.Size = mouseState.Position.ToVector2() - _frame.Position;
        // }

        // TODO: Add your update logic here
        // _characterManager.Update(gameTime);

        Node.IterateLinear(o => o.Update(gameTime));

        base.Update(gameTime);
    }

    protected override void Draw(GameTime gameTime)
    {
        GraphicsDevice.Clear(Color.AntiqueWhite);

        // TODO: Add your drawing code here
        Node.IterateTree(o => o.Render(_spriteBatch));
        Node.IterateTree(o => o.Draw(_spriteBatch));

        #if DEBUG_LAYOUT
        Node.IterateTree(o => o.DrawDebug(_spriteBatch));
        #endif

        base.Draw(gameTime);
    }
}
