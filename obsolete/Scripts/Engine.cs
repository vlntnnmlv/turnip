using System.Numerics;
using System.Text.Json;
using Raylib_cs;

namespace Turnip;

// TODO: Resolve focus and hover - which element should count as focused?

// TODO: Hover element is recalclated every mouse event.
//       It changes all the way from the root to the leaf node, because of this state of the pressed buttons is lost.
//       It shouldnt change until the traversal is finished.

// TODO: Add node removal mechanism
// TODO: Add mechanism to bind values to text: actual binding? update actions? ...
// TODO: Use shaders (optimize? - now it's 60 fps with shader/ 2500 fps without)
// TODO: Add new layout elements (grid, margin, clipping)
// TODO: Recalculate layoyt only if needed
// TODO: Change arrange and measure logic so they change the node itself, not it's children
// TODO: Add 3D layer under UI
// TODO: Separate UI and GUI in engine
// TODO: Refactor serializing, and add loading scenes from .json
// TODO: Add render modifiers, which don't affect layout calculations
// TODO: Add text fitting
// TODO: Figure why Observer pattern can be usefull in UI systems?

public class Engine
{
    const string CONFIG_PATH = ".engine/engineConfig.json";

    Vector2 m_Size;
    Node m_UIRoot;

    public Random? RNG { get; private set; }

    public Engine(int _WindowWidth, int _WindowHeight, string _WindowTitle)
    {
        m_Size = new Vector2(_WindowWidth, _WindowHeight);

        TryApplyConfig();

        CreateWindow(_WindowTitle);

        InitUIRoot();
        if (m_UIRoot == null)
            throw new NullReferenceException("UI Root failed to initialized");

        InitRandom();
    }

    static void TryApplyConfig()
    {
        if (File.Exists(CONFIG_PATH))
        {
            string jsonString = File.ReadAllText(CONFIG_PATH);
            God? god = JsonSerializer.Deserialize<God>(jsonString);

            if (god != null)
            {
                God.SetInstance(god);
            }
        }
        else
        {
            Console.WriteLine($"Engine config file not found at {CONFIG_PATH}");
        }
    }

    void CreateWindow(string _Title)
    {
        Raylib.SetConfigFlags(ConfigFlags.ResizableWindow); // | ConfigFlags.HighDpiWindow);
        Raylib.InitWindow((int)m_Size.X, (int)m_Size.Y, _Title);
    }

    void InitUIRoot()
    {
        m_UIRoot = new Node("root");
        m_UIRoot.Rect = new Rectangle(Vector2.Zero, m_Size);

        Event.OnMouseEvent += m_UIRoot.ProcessMouseEvent;
    }

    void InitRandom()
    {
        RNG = new Random();
        RNG.Next();
        Raylib.SetRandomSeed((uint)RNG.Next());
    }

    public void SerializeUI()
    {
        Serialization.Serialize(m_UIRoot);
    }

    public void DeserializeUI()
    {
        CreateUI(Serialization.Deserialize());
    }

    public void Update(float _DeltaTime)
    {
        Node.RemoveScheduled();

        Event.ProcessMouse(
            Raylib.GetMousePosition(),
            Raylib.IsMouseButtonPressed(MouseButton.Left),
            Raylib.IsMouseButtonReleased(MouseButton.Left)
        );

        Animation.Update(_DeltaTime);

        m_UIRoot.Traverse(_N =>
        {
            _N?.Update(_DeltaTime);
        });

        m_UIRoot.Traverse(_N =>
        {
            _N?.ProcessLayout(_DeltaTime);
        });
    }

    public void Draw()
    {
        Raylib.BeginDrawing();
        Raylib.ClearBackground(Color.RayWhite);

        m_UIRoot.Traverse(_N =>
        {
            _N?.Draw();
        });

        if (God.Instance.DebugUI)
        {
            m_UIRoot.Traverse(_N =>
            {
                _N?.DrawDebug();
            });
        }

        Raylib.EndDrawing();
    }

    public void CreateUI(Action<Node>? _CreateContent)
    {
        foreach (Node node in m_UIRoot.Children)
            node.Remove();

        Frame bg = new Frame("bg", m_UIRoot);

        Stack mainStack = new Stack(
            "main_stack",
            bg,
            Stack.StackType.HORIZONTAL,
            Stack.ContentType.CENTER
        );
        mainStack.Padding = new LRTB(5);

        Stack servicePanel = new Stack(
            "service_panel",
            mainStack,
            Stack.StackType.VERTICAL,
            Stack.ContentType.CENTER,
            new Size
            {
                AxisX = SizeType.START,
                Width = 400,
                AxisY = SizeType.FILL,
            }
        );
        servicePanel.Padding = new LRTB(16);

        Frame servicePanelImg = new Frame("servicePanelImg", servicePanel);
        Stack serviceStack = new Stack(
            "stack",
            servicePanelImg,
            Stack.StackType.VERTICAL,
            Stack.ContentType.CENTER
        );
        serviceStack.Padding = new LRTB(0, 0, 50, 50);
        Text lastMouseEvent = new Text(
            "lastMouseEvent",
            serviceStack,
            "NONE",
            24,
            _Color: Color.Black
        );

        Text fps = new Text(
            "fps",
            serviceStack,
            Raylib.GetFPS().ToString(),
            24,
            _Color: Color.Black
        );

        Switch debugUISwitch = new Switch(
            "debugUISwitch",
            serviceStack,
            _V => God.Instance.DebugUI = _V,
            string.Empty,
            new Size
            {
                AxisX = SizeType.CENTER,
                AxisY = SizeType.CENTER,
                Width = 64,
                Height = 32,
            }
        );

        Event.OnMouseEvent += (_E) =>
        {
            lastMouseEvent.SText = _E.Type.ToString();
            fps.SText = Raylib.GetFPS().ToString();
        };

        Node scenePanel = new Node(
            "scene_panel",
            mainStack,
            new Size { AxisX = SizeType.FILL, AxisY = SizeType.FILL }
        );

        _CreateContent?.Invoke(scenePanel);
    }
}
