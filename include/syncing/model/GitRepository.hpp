#ifndef DESKTOP_MANAGER_GITREPOSITORY_HPP
#define DESKTOP_MANAGER_GITREPOSITORY_HPP
#include <filesystem>

namespace fs = std::filesystem;
struct GitRepository
{
    GitRepository(fs::path const& path) : path(path)
    {
        name = path.filename();
    }
    std::string name;
    fs::path path;
};

typedef std::shared_ptr<GitRepository> GitRepositoryHandle;

#endif //DESKTOP_MANAGER_GITREPOSITORY_HPP