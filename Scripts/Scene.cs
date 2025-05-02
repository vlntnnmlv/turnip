using System.Collections.Generic;

namespace basic_kafana;

public class Scene
{
    public static string _currentSceneID;
    public static Dictionary<string, Scene> _scenes = new();
    public static void SetCurrentScene(string sceneID)
    {
        _currentSceneID = sceneID;
    }

    public string ID { get; private set; }
    public List<Node> _nodes = new();

    public Scene(string id)
    {
        ID = id;

        _scenes[id] = this;
    }

    public void RegisterNode(Node obj)
    {
        _nodes.Add(obj);
    }

    public void Update()
    {
        
    }

    public void Draw()
    {

    }
}