using System.IO;
using System.Numerics;
using System.Text.Json;
using System.Text.Json.Serialization;
using Raylib_cs;

namespace BasicKafana;

public class Engine
{
    Vector2 m_Size;
    Node m_UIRoot;

    public Engine(Vector2 _Size)
    {
        ReadConfig();

        m_Size = _Size;
        m_UIRoot = new Node("root");
        m_UIRoot.Rect = new Rectangle(0, 0, m_Size);

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

    public void CreateUI(Action<Node> _CreateContent)
    {
        _CreateContent(m_UIRoot);
    }
}
