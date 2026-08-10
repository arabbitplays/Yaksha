#ifndef YAKSHA_SYNCWIDGET_HPP
#define YAKSHA_SYNCWIDGET_HPP
#include <atomic>
#include <thread>

#include "syncing/SyncService.hpp"
#include "terminal_renderer/nodes/text_node/TextNode.hpp"
#include "terminal_renderer/widgets/Widget.hpp"

using namespace TerminalRenderer;

class SyncWidget : public Widget
{
public:
    explicit SyncWidget(const std::shared_ptr<SyncService>& sync_service);
    ~SyncWidget() override = default;

    void onStart() override;
    void runSync();
    void onUpdate() override;
    void writeLoading();
    void addLoadingSegment();
    void writeResults();
    void addMarkerSegment(GitSyncResult);

private:
    std::string getPushMarker(PushResult push_result);
    std::string getPullMarker(PullResult pull_result);

    std::shared_ptr<SyncService> sync_service;

    std::vector<GitSyncResult> sync_results;
    std::shared_ptr<TextNode> text_node;

    std::atomic<bool> sync_done;
    std::thread sync_thread;

    std::vector<std::string> spinner_frames = {"✹", "✸", "✷", "✶", "✷", "✸", "✹", "✺"};
    uint32_t curr_frame;

    static constexpr std::string SUCCESS_MARKER = "✓";
    static constexpr std::string FAILURE_MARKER = "⤫";
};


#endif //YAKSHA_SYNCWIDGET_HPP