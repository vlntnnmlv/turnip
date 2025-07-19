using System.IO;
using System.Numerics;
using System.Text.Json;
using System.Text.Json.Serialization;
using Raylib_cs;

namespace BasicKafana;

// TODO: Resolve focus and hover - which element should count as focused?
// TODO: Add new layout elements (grid, margin, clipping)
// TODO: Add 3D layer under UI
// TODO: Separate UI and GUI in engine
// TODO: Refactor serializing, and add loading scenes from .json

public class Engine
{
    Vector2 m_Size;
    Node m_UIRoot;

    public Engine(Vector2 _Size)
    {
        ReadConfig();

        m_Size = _Size;
        m_UIRoot = new Node("root");

        m_UIRoot.Rect = new Rectangle(Vector2.Zero, m_Size);

        Event.MouseEvent += m_UIRoot.ProcessMouseEvent;
    }

    const string PATH = ".engine/engineConfig.json";

    public void ReadConfig()
    {
        if (File.Exists(PATH))
        {
            string jsonString = File.ReadAllText(PATH);
            Dictionary<string, string> data = JsonSerializer.Deserialize<
                Dictionary<string, string>
            >(jsonString);

            foreach (string key in data.Keys)
            {
                string value = data[key];
                Console.WriteLine($"{key} : {data[key]}");
                switch (key)
                {
                    case "debugPhysics":
                        God.DebugPhysics = bool.Parse(value);
                        break;
                    case "debugUI":
                        God.DebugUI = bool.Parse(value);
                        break;
                    case "animateUIRate":
                        God.UIAnimationRate = float.Parse(value);
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            Console.WriteLine("File not found.");
        }
    }

    public void Serialize()
    {
        Serialization.Serialize(m_UIRoot);
    }

    public void Update(float _DeltaTime)
    {
        Event.ProcessMouse(
            Raylib.GetMousePosition(),
            Raylib.IsMouseButtonPressed(MouseButton.Left),
            Raylib.IsMouseButtonReleased(MouseButton.Left)
        );

        m_UIRoot.Traverse(_N =>
        {
            if (_N == null)
                return;

            _N.Update(_DeltaTime);
        });

        Animation.Update(_DeltaTime);

        m_UIRoot.Traverse(_N =>
        {
            if (_N == null)
                return;

            _N.Measure();
            _N.Arrange();

            // bool reallyHovered = _N.GetType() == typeof(Image) && _N.IsHovered;

            // Rectangle r = reallyHovered ? _N.Rect.Expand(Vector2.One * 10) : _N.Rect;
            // r = reallyHovered ? r.Move(Vector2.One * -5) : r;

            _N.RealRect = _N.RealRect.Lerp(_N.Rect, God.UIAnimationRate);

            _N.PlaceInWorld();
        });
    }

    public void Draw()
    {
        Raylib.BeginDrawing();
        Raylib.ClearBackground(Color.RayWhite);

        m_UIRoot.Traverse(_N =>
        {
            if (_N == null)
                return;

            _N.Draw();
        });

        if (God.DebugUI)
        {
            m_UIRoot.Traverse(_N =>
            {
                if (_N == null)
                    return;

                _N.DrawDebug();
            });
        }

        Raylib.EndDrawing();
    }

    public void CreateUI(Action<Node>? _CreateContent)
    {
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

        Node servicePanel = new Node(
            "service_panel",
            mainStack,
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

        Node scenePanel = new Node(
            "scene_panel",
            mainStack,
            new Size { AxisX = SizeType.FILL, AxisY = SizeType.FILL }
        );

        _CreateContent?.Invoke(scenePanel);
    }
}
