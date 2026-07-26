#ifndef DESKTOP_MANAGER_WORKSPACE_HPP
#define DESKTOP_MANAGER_WORKSPACE_HPP
#include <cstdint>

// physical_id and virtual_id are 0-indexed. Hyprland workspace ids are 1-indexed
struct Workspace
{
    uint32_t physical_id;
    uint32_t virtual_id;
};

#endif //DESKTOP_MANAGER_WORKSPACE_HPP
