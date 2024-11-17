#include "Instance.hpp"

Instance::Instance(const std::string& className)
    : className(className), name(className) {}

InstancePtr Instance::Create(const std::string& className) {
    return std::shared_ptr<Instance>(new Instance(className));
}

InstancePtr Instance::GetParent() const {
    return parent.lock();
}

void Instance::SetParent(InstancePtr newParent) {
    auto currentParent = parent.lock();
    if (currentParent) {
        // Remove from current parent's children
        auto& parentChildren = currentParent->children;
        parentChildren.erase(
            std::remove_if(parentChildren.begin(), parentChildren.end(),
                [this](const InstancePtr& child) {
                    return child.get() == this;
                }
            ),
            parentChildren.end()
        );
    }

    if (newParent) {
        parent = newParent;
        newParent->children.push_back(shared_from_this());
    }
    else {
        parent.reset();
    }
}

std::vector<InstancePtr> Instance::GetChildren() const {
    return children;
}

InstancePtr Instance::FindFirstChild(const std::string& childName) const {
    auto it = std::find_if(children.begin(), children.end(),
        [&childName](const InstancePtr& child) {
            return child->GetName() == childName;
        });
    return it != children.end() ? *it : nullptr;
}

void Instance::SetProperty(const std::string& name, const PropertyValue& value) {
    auto oldValue = properties[name];
    properties[name] = value;

    // Trigger signals
    auto signalIt = signals.find(name);
    if (signalIt != signals.end()) {
        for (const auto& [_, callback] : signalIt->second) {
            callback(value, oldValue);
        }
    }
}

PropertyValue Instance::GetProperty(const std::string& name) const {
    auto it = properties.find(name);
    if (it != properties.end()) {
        return it->second;
    }
    throw std::runtime_error("Property not found: " + name);
}

bool Instance::HasProperty(const std::string& name) const {
    return properties.find(name) != properties.end();
}

size_t Instance::Connect(const std::string& propertyName, SignalCallback callback) {
    size_t signalId = nextSignalId++;
    signals[propertyName][signalId] = callback;
    return signalId;
}

void Instance::Disconnect(const std::string& propertyName, size_t connectionId) {
    auto signalIt = signals.find(propertyName);
    if (signalIt != signals.end()) {
        signalIt->second.erase(connectionId);
    }
}