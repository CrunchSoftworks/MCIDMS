#pragma once
#include "Instance.hpp"

class DataModel {
public:
    static DataModel& GetInstance() {
        static DataModel instance;
        return instance;
    }

    InstancePtr GetWorkspace() const { return workspace; }
    InstancePtr GetPlayers() const { return players; }
    InstancePtr GetLighting() const { return lighting; }
    InstancePtr GetRunService() const { return runService; }

private:
    DataModel() {
        root = Instance::Create("DataModel");
        workspace = Instance::Create("Workspace");
        players = Instance::Create("Players");
        lighting = Instance::Create("Lighting");
        runService = Instance::Create("RunService");

        workspace->SetParent(root);
        players->SetParent(root);
        lighting->SetParent(root);
        runService->SetParent(root);
    }

    InstancePtr root;
    InstancePtr workspace;
    InstancePtr players;
    InstancePtr lighting;
    InstancePtr runService;
};