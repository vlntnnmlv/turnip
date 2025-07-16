using System.Numerics;
using Raylib_cs;

namespace BasicKafana;

public static class Globals
{
    public const int WINDOW_WIDTH = 1080;
    public const int WINDOW_HEIGHT = 720;
}

class Program
{
    const int WINDOW_WIDTH = 1080;
    const int WINDOW_HEIGHT = 720;
    const string WINDOW_TITLE = "BasicKafana Kafana";

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

    // TODO: Figure our wth is this?
    // STAThread is required if you deploy using NativeAOT on Windows - See https://github.com/raylib-cs/raylib-cs/issues/301
    [STAThread]
    public static void Main()
    {
        Raylib.InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

        Random random = new Random();
        random.Next();

        Raylib.SetRandomSeed((uint)random.Next());

        m_Engine = new Engine(new Vector2(WINDOW_WIDTH, WINDOW_HEIGHT));
        m_Engine.CreateUI(_Root =>
        {
            _Root.Padding = new LRTB(10, 10, 10, 10);

            Stack stack = new Stack("stack", Stack.StackType.HORIZONTAL, Stack.ContentType.START);
            stack.Spacing = 20;
            _Root.LinkChild(stack);

            Stack stackLeft = new Stack(
                "stackLeft",
                Stack.StackType.HORIZONTAL,
                Stack.ContentType.END
            );
            stackLeft.Spacing = 10;
            stack.LinkChild(stackLeft);

            Stack nodeCenter = new Stack(
                "stackCenter",
                Stack.StackType.VERTICAL,
                Stack.ContentType.CENTER
            );
            nodeCenter.Spacing = 8;
            stack.LinkChild(nodeCenter);

            Text mouseState = new Text(
                "mouseState",
                Event.IsMouseDown.ToString(),
                Color.Black,
                24,
                new Size { AxisY = SizeType.CENTER, Height = 60 }
            );
            nodeCenter.LinkChild(mouseState);
            Text mousePos = new Text(
                "mousePos",
                Event.MousePosition.ToString(),
                Color.Black,
                24,
                new Size { AxisY = SizeType.CENTER, Height = 60 }
            );
            nodeCenter.LinkChild(mousePos);

            Stack stackRight = new Stack(
                "stackRight",
                Stack.StackType.VERTICAL,
                Stack.ContentType.START
            );
            stackRight.Spacing = 10;
            stack.LinkChild(stackRight);

            ImageInfo petalInfo = new ImageInfo
            {
                Texture = Raylib.LoadTexture("Resources/Textures/frame_smooth_rect.png"),
                Patch = new LRTB(16),
            };

            Node img2 = new Image(
                "image",
                petalInfo,
                new Size { AxisY = SizeType.CENTER, Height = 250 }
            );
            Node img3 = new Image("image", petalInfo);
            Node img4 = new Image("image", petalInfo);
            stackLeft.LinkChild(img2);
            stackLeft.LinkChild(img3);
            stackLeft.LinkChild(img4);

            // Node img5 = new Image("image", petalInfo);
            // Node img6 = new Image("image", petalInfo);
            // Node img7 = new Image("image", petalInfo);
            Node img8 = new Image("image", petalInfo);
            // stackRight.LinkChild(img5);
            // stackRight.LinkChild(img6);
            // stackRight.LinkChild(img7);
            stackRight.LinkChild(img8);

            void ShowEvent(Vector2 _V, string _Msg)
            {
                if (_Msg == "MOVED")
                    return;

                Text t = new Text(
                    "t",
                    _Msg,
                    Color.Black,
                    12,
                    new Size
                    {
                        AxisX = SizeType.FILL,
                        AxisY = SizeType.END,
                        Height = 40,
                    }
                );

                Animation animation = new Animation(
                    1.5f,
                    (_P) =>
                    {
                        t.Color = new Color((float)_P, (float)_P, (float)_P);
                    },
                    () => t.Remove()
                );
                animation.Start();

                stackRight.LinkChild(t);
            }

            Event.MouseEvent += (_I) => ShowEvent(_I.Position, _I.Type.ToString());

            void OnClick()
            {
                Node tip = new Image(
                    "tip",
                    petalInfo,
                    new Size
                    {
                        AxisX = SizeType.ABSOLUTE,
                        AxisY = SizeType.ABSOLUTE,
                        X = WINDOW_WIDTH / 2 - 50,
                        Y = WINDOW_HEIGHT / 2 - 30,
                        Width = 100,
                        Height = 60,
                    }
                );
                void OnTipClick()
                {
                    tip.Remove();
                }
                _Root.LinkChild(tip);
                Button tipbutton = new Button("tupbutton", OnTipClick);
                tip.LinkChild(tipbutton);
            }

            Button button = new Button(
                "button",
                OnClick,
                new Size
                {
                    AxisX = SizeType.START,
                    Width = 50,
                    AxisY = SizeType.END,
                    Height = 40,
                }
            );
            img8.LinkChild(button);

            Animation animation = new Animation(
                20,
                (_P) =>
                {
                    img2.Size.Height = 50 + MathF.Abs(MathF.Sin(_P * 10 * MathF.PI)) * 100;
                },
                null,
                true
            );
            animation.Start();
        });

        // m_State = CreateState(NUM_BOXES);

        // List<Action> lateActions = new();

        while (!Raylib.WindowShouldClose())
        {
            m_DeltaTime = Raylib.GetFrameTime();

            if (Raylib.IsKeyPressed(KeyboardKey.S) && Raylib.IsKeyDown(KeyboardKey.LeftControl))
            {
                m_Engine.Serialize();
            }

            // lateActions.Clear();

            // m_State = CreateState(NUM_BOXES);
            // Renderer.DrawState(m_State);

            m_Engine.Update(m_DeltaTime);
            m_Engine.Draw();

            // m_State = GetNextState(m_State, Raylib.GetFrameTime());

            // foreach (Action action in lateActions)
            //     action?.Invoke();
        }

        Raylib.CloseWindow();
    }

    static float m_DeltaTime;
}
