using System.Reflection;
using System.Text.Json;
using System.Text.Json.Serialization.Metadata;

namespace Turnip;

public class SerializeResolver : IJsonTypeInfoResolver
{
    static DefaultJsonTypeInfoResolver m_DefaultResolver = new DefaultJsonTypeInfoResolver();

    public JsonTypeInfo GetTypeInfo(Type _Type, JsonSerializerOptions _Options)
    {
        if (_Type.IsPrimitive || _Type == typeof(string))
            return m_DefaultResolver.GetTypeInfo(_Type, _Options);

        Console.WriteLine($"{_Type}");

        JsonTypeInfo jsonTypeInfo = JsonTypeInfo.CreateJsonTypeInfo(_Type, _Options);
        MemberInfo[] members =
        [
            .. _Type
                .GetMembers(BindingFlags.Instance | BindingFlags.Public | BindingFlags.NonPublic)
                .Where(_M => _M.GetCustomAttribute<SerializeAttribute>() != null),
        ];

        foreach (MemberInfo member in members)
        {
            Type memberType;
            Func<object, object?> getter;
            Action<object, object?>? setter;

            if (member is PropertyInfo property)
            {
                if (!property.CanRead)
                    continue;

                memberType = property.PropertyType;
                getter = property.GetValue;
                setter = property.CanWrite ? property.SetValue : null;
            }
            else if (member is FieldInfo field)
            {
                memberType = field.FieldType;
                getter = field.GetValue;
                setter = field.SetValue;
            }
            else
            {
                continue;
            }

            JsonPropertyInfo propertyInfo = jsonTypeInfo.CreateJsonPropertyInfo(
                memberType,
                member.Name
            );
            propertyInfo.Get = getter;
            propertyInfo.Set = setter;

            jsonTypeInfo.Properties.Add(propertyInfo);
        }

        return jsonTypeInfo;
    }
}
