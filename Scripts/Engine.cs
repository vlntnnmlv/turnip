using System.Numerics;
using System.Text.Json;
using Raylib_cs;

namespace Turnip;

// TODO: Resolve focus and hover - which element should count as focused?
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

public class Engine
{
    Vector2 m_Size;
    Node m_UIRoot;
    public Random RNG { get; }

    public Engine(int _Width, int _Height, string _Name)
    {
        RNG = new Random();
        RNG.Next();
        Raylib.SetRandomSeed((uint)RNG.Next());

        TryApplyConfig();

        Raylib.InitWindow(_Width, _Height, _Name);
        m_Size = new Vector2(_Width, _Height);

        m_UIRoot = new Node("root");
        m_UIRoot.Rect = new Rectangle(Vector2.Zero, m_Size);

        Event.OnMouseEvent += m_UIRoot.ProcessMouseEvent;
    }

    const string CONFIG_PATH = ".engine/engineConfig.json";

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
            Console.WriteLine("File not found.");
        }
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
            Node.ScheduleToRemove(node);

        Image bg = new Image(
            "bg",
            m_UIRoot,
            new ImageInfo { Texture = God.Texture },
            _Color: new Color(109, 146, 88)
        );

        Stack mainStack = new Stack(
            "main_stack",
            m_UIRoot,
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

        Image servicePanelImg = new Image(
            "servicePanelImg",
            servicePanel,
            new ImageInfo { Texture = Resources.LoadTexture("panel_simple"), Patch = new LRTB(16) }
        );

        Text lastMouseEvent = new Text(
            "lastMouseEvent",
            servicePanel,
            "",
            24,
            _Color: new Color(17, 39, 5)
        );

        Text fps = new Text(
            "fps",
            servicePanel,
            Raylib.GetFPS().ToString(),
            24,
            _Color: new Color(17, 39, 5)
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
