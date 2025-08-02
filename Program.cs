using System.Numerics;
using Raylib_cs;

namespace Turnip;

class Program
{
    const int WINDOW_WIDTH = 1080;
    const int WINDOW_HEIGHT = 720;

    static float EaseInOutSine(float _Phase)
    {
        return -(MathF.Cos(MathF.PI * _Phase) - 1) / 2;
    }

    const string WINDOW_TITLE = "Turnip";

    const int NUM_BOXES = 1;
    const int PLAYER_SPEED = 300;

    static State m_State;
    static Space m_Space = new();

    static State CreateState(int _NumBoxes, bool _Walls = false)
    {
        State state = new State { };

        int numberOfWalls = _Walls ? 4 : 0;
        state.Boxes = new Box[_NumBoxes + 1 + numberOfWalls];

        // player controled box
        state.Boxes[0] = new Box()
        {
            Index = 0,
            ID = "Player",
            Position = new Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
            Size = Vector2.One * 50,
            Mass = 75,
        };

        // walls
        if (_Walls)
        {
            state.Boxes[1] = new Box()
            {
                Index = 1,
                ID = "WestWall",
                Position = new Vector2(-10, 0),
                Size = new Vector2(12, WINDOW_HEIGHT),
                IsStatic = true,
            };
            state.Boxes[2] = new Box()
            {
                Index = 2,
                ID = "NorthWall",
                Position = new Vector2(0, -10),
                Size = new Vector2(WINDOW_WIDTH, 12),
                IsStatic = true,
            };
            state.Boxes[3] = new Box()
            {
                Index = 3,
                ID = "EastWall",
                Position = new Vector2(WINDOW_WIDTH - 2, 0),
                Size = new Vector2(8, WINDOW_HEIGHT),
                IsStatic = true,
            };
            state.Boxes[4] = new Box()
            {
                Index = 4,
                ID = "SouthWall",
                Position = new Vector2(0, WINDOW_HEIGHT - 2),
                Size = new Vector2(WINDOW_WIDTH, 12),
                IsStatic = true,
            };
        }

        for (int i = 1 + numberOfWalls; i < state.Boxes.Length; i++)
        {
            state.Boxes[i].Index = i;
            state.Boxes[i].ID = $"Box{i}";
            state.Boxes[i].Size = new Vector2(
                Raylib.GetRandomValue(69, 169),
                Raylib.GetRandomValue(69, 169)
            );
            state.Boxes[i].Position = new Vector2(
                Raylib.GetRandomValue(0, WINDOW_WIDTH - (int)state.Boxes[i].Size.X),
                Raylib.GetRandomValue(0, WINDOW_HEIGHT - (int)state.Boxes[i].Size.Y)
            );
            state.Boxes[i].Rotation = 0.0f;
            state.Boxes[i].IsStatic = false; //Raylib.GetRandomValue(0, 100) < 20;
            state.Boxes[i].Mass = (float)Math.Sqrt(state.Boxes[i].Size.X * state.Boxes[i].Size.Y);
        }

        return state;
    }

    static State GetNextState(State _State, float _DeltaTime)
    {
        State newState = _State;

        // update from user input
        {
            // movement
            {
                Vector2 direction = Vector2.Zero;

                if (Raylib.IsKeyDown(KeyboardKey.W))
                    direction += new Vector2(0, -1);
                if (Raylib.IsKeyDown(KeyboardKey.A))
                    direction += new Vector2(-1, 0);
                if (Raylib.IsKeyDown(KeyboardKey.S))
                    direction += new Vector2(0, 1);
                if (Raylib.IsKeyDown(KeyboardKey.D))
                    direction += new Vector2(1, 0);

                Vector2 newPosition =
                    newState.Boxes[0].Position + direction.Normalized() * PLAYER_SPEED * _DeltaTime;

                newState.Boxes[0].Position = newPosition;

                if (direction != Vector2.Zero)
                    newState.Boxes[0].Velocity = Vector2.Zero;
            }

            // kick force
            {
                Vector2 mousePosition = Raylib.GetMousePosition();

                newState.ManualControl = mousePosition - newState.Boxes[0].Rectangle.Center();

                for (uint i = 1; i < _State.Boxes.Length; i++)
                {
                    if (
                        _State.Boxes[i].Rectangle.Contains(mousePosition)
                        && Raylib.IsKeyPressed(KeyboardKey.Space)
                    )
                    {
                        _State.Boxes[i].NetForce +=
                            newState.ManualControl.Normalized() * 5000 / _DeltaTime;
                    }
                }
            }
        }

        newState.Boxes = m_Space.Simulate(newState.Boxes, _DeltaTime);
        newState.Collisions = m_Space.GetCollisionsThisFrame();
        return newState;
    }

    static Engine? m_Engine;

    static void CreateUI1(Node _Root)
    {
        _Root.Padding = new LRTB(10, 10, 10, 10);

        ImageInfo bgImageInfo = new ImageInfo { Texture = God.Texture };
        Image bg = new Image("bg", _Root, bgImageInfo, _Color: new Color(0.5f, 0.5f, 0.5f));

        Stack stack = new Stack(
            "stack",
            _Root,
            Stack.StackType.HORIZONTAL,
            Stack.ContentType.START
        );
        stack.Spacing = 20;

        Stack stackLeft = new Stack(
            "stackLeft",
            stack,
            Stack.StackType.HORIZONTAL,
            Stack.ContentType.END
        );
        stackLeft.Spacing = 10;

        Stack nodeCenter = new Stack(
            "stackCenter",
            stack,
            Stack.StackType.VERTICAL,
            Stack.ContentType.CENTER
        );
        nodeCenter.Spacing = 8;

        Stack stackRight = new Stack(
            "stackRight",
            stack,
            Stack.StackType.VERTICAL,
            Stack.ContentType.START
        );
        stackRight.Spacing = 10;

        ImageInfo petalInfo = new ImageInfo
        {
            Texture = Resources.LoadTexture("frame_simple"),
            Patch = new LRTB(5),
        };

        Node img2 = new Image(
            "image",
            stackLeft,
            petalInfo,
            new Size { AxisY = SizeType.CENTER, Height = 250 }
        );
        Node img3 = new Image("image", stackLeft, petalInfo);
        Node img4 = new Image("image", stackLeft, petalInfo);

        Node img8 = new Image("image", stackRight, petalInfo);

        void ShowEvent(Vector2 _V, string _Msg)
        {
            if (_Msg == "MOVED")
                return;

            Text t = new Text(
                "t",
                stackRight,
                _Msg,
                12,
                new Size
                {
                    AxisX = SizeType.FILL,
                    AxisY = SizeType.END,
                    Height = 40,
                },
                Color.Black
            );

            Animation animation = new Animation(
                1.5f,
                (_P) =>
                {
                    t.Color = new Color(t.Color.R, t.Color.G, t.Color.B, 1 - _P);
                },
                () => t.Remove()
            );
            animation.Start();
        }

        Event.OnMouseEvent += (_I) => ShowEvent(_I.Position, _I.Type.ToString());

        Animation animation = new Animation(
            20,
            (_P) =>
            {
                img2.Size.Height =
                    50 + MathF.Abs(MathF.Sin(EaseInOutSine(_P) * 10 * MathF.PI)) * 200;
            },
            null,
            true
        );
        animation.Start();
    }

    static void CreateUI2(Node _Root)
    {
        Stack horizontal = new Stack(
            "stackH",
            _Root,
            Stack.StackType.HORIZONTAL,
            Stack.ContentType.CENTER
        );
        horizontal.Spacing = 10;
        horizontal.Padding = new LRTB(10);
        ButtonInfo btnInfo = new ButtonInfo
        {
            Action = null,
            Normal = new ImageInfo
            {
                Texture = Resources.LoadTexture("button"),
                Patch = new LRTB(7),
            },
            Pressed = new ImageInfo
            {
                Texture = Resources.LoadTexture("button_pressed"),
                Patch = new LRTB(9),
            },
            Hovered = new ImageInfo
            {
                Texture = Resources.LoadTexture("button_hovered"),
                Patch = new LRTB(7),
            },
        };

        for (int i = 0; i < 6; i++)
        {
            Stack verical = new Stack(
                "stackV",
                horizontal,
                Stack.StackType.VERTICAL,
                Stack.ContentType.CENTER
            );
            verical.Spacing = 10;
            for (int j = 0; j < 6; j++)
            {
                Button btn = new Button($"btn{i}{j}", verical, btnInfo);
                Text t = new Text($"t{i}{j}", btn, "0", 16, _Color: Color.Black);
                t.IgnoreEvents = true;

                btn.Action = () =>
                {
                    t.SText = (int.Parse(t.SText) + 1).ToString();
                    if (int.Parse(t.SText) > 3)
                        btn.Remove();
                };
            }
        }
    }

    static void CreateUI3(Node _Root)
    {
        _Root.Padding = new LRTB(50);

        ButtonInfo btnInfo = new ButtonInfo
        {
            Action = null,
            Normal = new ImageInfo
            {
                Texture = Resources.LoadTexture("button"),
                Patch = new LRTB(16),
            },
            Pressed = new ImageInfo
            {
                Texture = Resources.LoadTexture("button_pressed"),
                Patch = new LRTB(16),
            },
            Hovered = new ImageInfo
            {
                Texture = Resources.LoadTexture("button_hovered"),
                Patch = new LRTB(16),
            },
        };
        new Button("t", _Root, btnInfo);
    }

    // TODO: Figure our wth is this?
    // STAThread is required if you deploy using NativeAOT on Windows - See https://github.com/raylib-cs/raylib-cs/issues/301
    [STAThread]
    public static void Main()
    {
        // TODO:
        //
        // new engine();
        //
        // engine.run()
        //

        m_Engine = new Engine(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
        m_Engine.CreateUI(CreateUI3);

        while (!Raylib.WindowShouldClose())
        {
            m_DeltaTime = Raylib.GetFrameTime();

            if (Raylib.IsKeyPressed(KeyboardKey.S) && Raylib.IsKeyDown(KeyboardKey.LeftControl))
            {
                m_Engine.SerializeUI();
            }

            if (Raylib.IsKeyPressed(KeyboardKey.O) && Raylib.IsKeyDown(KeyboardKey.LeftControl))
            {
                m_Engine.DeserializeUI();
            }

            m_Engine.Update(m_DeltaTime);
            m_Engine.Draw();
        }

        Raylib.CloseWindow();
    }

    static float m_DeltaTime;
}
