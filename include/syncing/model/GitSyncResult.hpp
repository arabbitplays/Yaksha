#ifndef DESKTOP_MANAGER_GITSYNCRESULT_HPP
#define DESKTOP_MANAGER_GITSYNCRESULT_HPP

enum class PullResult
{
    SUCCESS = 0,
    FAILED = 1,
};

enum class PushResult
{
    SUCCESS = 0,
    NOTHING_TO_COMMIT = 1,
    FAILED = 2,
};

struct GitSyncResult
{
    std::string name;
    PullResult pullResult;
    PushResult pushResult;
};

#endif //DESKTOP_MANAGER_GITSYNCRESULT_HPP