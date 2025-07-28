using System.Reflection;
using System.Text.Json;
using System.Text.Json.Serialization;

namespace Turnip;

public static class Serialization
{
    const string DIRECTORY_PATH = ".serialization";
    const string JSON_PATH = "ui.json";

    public static void CheckSerializationRoot()
    {
        if (!Directory.Exists(DIRECTORY_PATH))
            Directory.CreateDirectory(DIRECTORY_PATH);
    }

    static object? GetMemberValue(object _Object, MemberInfo _MemberInfo)
    {
        if (_MemberInfo is PropertyInfo propertyInfo)
        {
            return propertyInfo.GetValue(_Object);
        }

        if (_MemberInfo is FieldInfo fieldInfo)
        {
            return fieldInfo.GetValue(_Object);
        }

        return null;
    }

    public static void Serialize(Node _Root)
    {
        CheckSerializationRoot();

        Dictionary<ulong, Dictionary<string, object?>> data = new();

        _Root.Traverse(_N =>
        {
            data[_N.UID] = SerializeNode(_N);
        });

        string json = JsonSerializer.Serialize(data);

        string filePath = Path.Combine(DIRECTORY_PATH, JSON_PATH);
        File.WriteAllText(filePath, json);
    }

    static Dictionary<string, object?> SerializeNode(Node _Node)
    {
        Dictionary<string, object?> data = new();

        MemberInfo[] members =
        [
            .. _Node
                .GetType()
                .GetMembers(BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic)
                .Where(_M => _M.GetCustomAttribute<SerializeAttribute>() != null),
        ];

        foreach (MemberInfo memberInfo in members)
        {
            data[memberInfo.Name] = SetializeMember(GetMemberValue(_Node, memberInfo));
        }

        List<ulong> childrenUIDs = new();
        foreach (Node child in _Node.Children)
        {
            childrenUIDs.Add(child.UID);
        }
        data["children"] = childrenUIDs;

        return data;
    }

    static object? SetializeMember(object? _Member) // Dictionary<string, object?>
    {
        if (_Member == null)
            return null;

        if (_Member.GetType().IsPrimitive || _Member.GetType() == typeof(string))
            return _Member;

        Dictionary<string, object?> data = new();

        MemberInfo[] members =
        [
            .. _Member
                .GetType()
                .GetMembers(BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic)
                .Where(_M => _M.GetCustomAttribute<SerializeAttribute>() != null),
        ];

        foreach (MemberInfo memberInfo in members)
        {
            data[memberInfo.Name] = GetMemberValue(_Member, memberInfo);
        }

        return data;
    }

    public static Action<Node>? Deserialize()
    {
        CheckSerializationRoot();

        string filePath = Path.Combine(DIRECTORY_PATH, JSON_PATH);
        if (!File.Exists(filePath))
        {
            return null;
        }

        string jsonString = File.ReadAllText(filePath);

        return _N =>
        {
            Node? node = JsonSerializer.Deserialize<Node>(jsonString);
            if (node != null)
            {
                _N.LinkChild(node);
            }
        };
    }
}
