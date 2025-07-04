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
    const string WINDOW_TITLE = "Basic Kafana";

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
        state.Boxes[0] = new Box() { Index = 0, ID = "Player", Position = new Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), Size = Vector2.One * 50, Mass = 75 };

        // walls
        if (_Walls)
        {
            state.Boxes[1] = new Box() { Index = 1, ID = "WestWall", Position = new Vector2(-10, 0), Size = new Vector2(12, WINDOW_HEIGHT), IsStatic = true };
            state.Boxes[2] = new Box() { Index = 2, ID = "NorthWall", Position = new Vector2(0, -10), Size = new Vector2(WINDOW_WIDTH, 12), IsStatic = true };
            state.Boxes[3] = new Box() { Index = 3, ID = "EastWall", Position = new Vector2(WINDOW_WIDTH - 2, 0), Size = new Vector2(8, WINDOW_HEIGHT), IsStatic = true };
            state.Boxes[4] = new Box() { Index = 4, ID = "SouthWall", Position = new Vector2(0, WINDOW_HEIGHT - 2), Size = new Vector2(WINDOW_WIDTH, 12), IsStatic = true };
        }

        for (int i = 1 + numberOfWalls; i < state.Boxes.Length; i++)
        {
            state.Boxes[i].Index = i;
            state.Boxes[i].ID = $"Box{i}";
            state.Boxes[i].Size = new Vector2(Raylib.GetRandomValue(69, 169), Raylib.GetRandomValue(69, 169));
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

                Vector2 newPosition = newState.Boxes[0].Position + direction.Normalized() * PLAYER_SPEED * _DeltaTime;

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
                    if (_State.Boxes[i].Rectangle.Contains(mousePosition) && Raylib.IsKeyPressed(KeyboardKey.Space))
                    {
                        _State.Boxes[i].NetForce += newState.ManualControl.Normalized() * 5000 / _DeltaTime;
                    }
                }
            }
        }

        newState.Boxes = m_Space.Simulate(newState.Boxes, _DeltaTime);
        newState.Collisions = m_Space.GetCollisionsThisFrame();
        return newState;
    }

    // TODO: Figure our wth is this?
    // STAThread is required if you deploy using NativeAOT on Windows - See https://github.com/raylib-cs/raylib-cs/issues/301
    [STAThread]
    public static void Main()
    {
        Raylib.InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

        Random random = new Random();
        random.Next();

        Raylib.SetRandomSeed((uint)random.Next());

        Node uiTreeRoot = new("root", new Meristem(new Rectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)));

        Node first = new("first", new Meristem(new Rectangle(0, 0, 300, 300)));
        Node second = new("second", new Meristem(new Rectangle(400, 0, 300, 300)));
        Node third = new("third", new Meristem(new Rectangle(0, 400, 100, 100)));

        uiTreeRoot.LinkChild(first);
        uiTreeRoot.LinkChild(second);
        uiTreeRoot.LinkChild(third);

        PetalInfo petalInfo = new PetalInfo
        {
            Texture = Raylib.LoadTexture("frame.png"),
            Patch = new Rectangle(0, 0, 0, 0)
        };

        Node a = new("a", new Petal(new Rectangle(0, 0, 10, 10), petalInfo));
        Node b = new("b", new Petal(new Rectangle(10, 10, 20, 20), petalInfo));
        Node c = new("c", new Petal(new Rectangle(0, 0, 50, 50), petalInfo));

        first.LinkChild(a);
        first.LinkChild(b);
        third.LinkChild(c);

        // m_State = CreateState(NUM_BOXES);

        while (!Raylib.WindowShouldClose())
        {
            // if (Raylib.IsKeyPressed(KeyboardKey.R))
            // m_State = CreateState(NUM_BOXES);

            Raylib.BeginDrawing();
            Raylib.ClearBackground(Color.RayWhite);

            // Renderer.DrawState(m_State);
            uiTreeRoot.Traverse(
                _N =>
                {
                    if (_N == null || _N.Meristem == null)
                        return;

                    _N.Meristem.Draw(_N);
                }
            );

            Raylib.EndDrawing();

            // m_State = GetNextState(m_State, Raylib.GetFrameTime());
        }

        Raylib.CloseWindow();
    }
}