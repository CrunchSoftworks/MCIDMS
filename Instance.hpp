#pragma once
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <variant>
#include <stdexcept>

class Instance;
using InstancePtr = std::shared_ptr<Instance>;
using WeakInstancePtr = std::weak_ptr<Instance>;
using PropertyValue = std::variant<int, float, double, std::string, bool>;
using SignalCallback = std::function<void(const PropertyValue&, const PropertyValue&)>;

class Instance : public std::enable_shared_from_this<Instance> {
public:
    static InstancePtr Create(const std::string& className);

    // Core instance methods
    std::string GetClassName() const { return className; }
    std::string GetName() const { return name; }
    void SetName(const std::string& newName) { name = newName; }

    // Parent-child relationship
    InstancePtr GetParent() const;
    void SetParent(InstancePtr newParent);
    std::vector<InstancePtr> GetChildren() const;
    InstancePtr FindFirstChild(const std::string& childName) const;

    // Property system
    void SetProperty(const std::string& name, const PropertyValue& value);
    PropertyValue GetProperty(const std::string& name) const;
    bool HasProperty(const std::string& name) const;

    // Signal system
    size_t Connect(const std::string& propertyName, SignalCallback callback);
    void Disconnect(const std::string& propertyName, size_t connectionId);

protected:
    explicit Instance(const std::string& className);
    virtual ~Instance() = default;

private:
    std::string className;
    std::string name;
    WeakInstancePtr parent;
    std::vector<InstancePtr> children;

    std::unordered_map<std::string, PropertyValue> properties;
    std::unordered_map<std::string,
        std::unordered_map<size_t, SignalCallback>> signals;
    size_t nextSignalId = 0;
};