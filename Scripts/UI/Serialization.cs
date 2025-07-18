using System.IO;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace BasicKafana;

public static class Serialization
{
    const string DIRECTORY_PATH = ".serialization";
    const string JSON_PATH = "ui.json";

    public static void CheckSerializationRoot()
    {
        if (!Directory.Exists(DIRECTORY_PATH))
            Directory.CreateDirectory(DIRECTORY_PATH);
    }

    public static void Serialize(Node _Root)
    {
        CheckSerializationRoot();

        string json = JsonSerializer.Serialize(
            _Root,
            new JsonSerializerOptions
            {
                WriteIndented = true,
                TypeInfoResolver = new SerializeResolver(),
            }
        );

        string filePath = Path.Combine(DIRECTORY_PATH, JSON_PATH);
        File.WriteAllText(filePath, json);
    }
}
