#include "../../../include/dashboard/widgets/SyncWidget.hpp"
#include <terminal_renderer/nodes/border_node/BorderNode.hpp>

#include "syncing/SyncService.hpp"
#include "terminal_renderer/builder/SceneBuilder.hpp"

SyncWidget::SyncWidget(const std::shared_ptr<SyncService>& sync_service) : sync_service(sync_service)
{
    text_node = SceneBuilder::text({.flow_mode = TextFlowMode::STATIC}).build();
    root = SceneBuilder::roundedBorder(1, 1, {.scaling_mode = STATIC}).setChild(text_node).build();
}

void SyncWidget::onStart()
{
    sync_thread = std::thread(&SyncWidget::runSync, this);
}

void SyncWidget::runSync()
{
    sync_results = sync_service->syncGitRepositories();
    sync_results.push_back(sync_service->syncConfigFiles());
    sync_done.store(true);
}

void SyncWidget::onUpdate()
{
    text_node->clearTextSegments();
    text_node->appendTextSegment("Repository Sync:\n");

    if (sync_done.load())
    {
        writeResults();
    }
    else
    {
        writeLoading();
        curr_frame = (curr_frame + 1) % spinner_frames.size();
    }
}

void SyncWidget::writeLoading()
{
    std::vector<std::string> repo_names = sync_service->getSyncedRepositoryNames();
    for (const auto& name : repo_names)
    {
        addLoadingSegment();
        text_node->appendTextSegment(" " + name + "\n");
    }
}

void SyncWidget::addLoadingSegment()
{
    std::string spinner = spinner_frames.at(curr_frame);
    text_node->appendTextSegment(spinner);
}

void SyncWidget::writeResults()
{
    for (const auto& result : sync_results)
    {
        addMarkerSegment(result);
        text_node->appendTextSegment(" " + result.name + "\n");
    }
}

void SyncWidget::addMarkerSegment(GitSyncResult result)
{
    bool success = result.pullResult == PullResult::SUCCESS && result.pushResult != PushResult::FAILED;
    if (success)
    {
        text_node->appendTextSegment(SUCCESS_MARKER, StandardColor::create(GREEN));
    }
    else
    {
        text_node->appendTextSegment(FAILURE_MARKER, StandardColor::create(RED));
    }
}
