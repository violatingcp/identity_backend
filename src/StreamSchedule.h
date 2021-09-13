#ifndef StreamSchedule_h
#define StreamSchedule_h

#include <memory>
#include <string>
#include <vector>

#include "Framework/ProductRegistry.h"
#include "Framework/WaitingTaskHolder.h"
#include "plugin-Validation/CountValidatorSimple.cc"

namespace edmplugin {
  class PluginManager;
}

namespace edm {
  class EventSetup;
  class Source;
  class Worker;

  // Schedule of modules per stream (concurrent event)
  class StreamSchedule {
  public:
    // copy ProductRegistry per stream
    explicit StreamSchedule(ProductRegistry reg,
                            edmplugin::PluginManager& pluginManager,
                            Source* source,
                            EventSetup const* eventSetup,
                            int streamId,
                            std::vector<std::string> const& path);
    ~StreamSchedule();
    StreamSchedule(StreamSchedule const&) = delete;
    StreamSchedule& operator=(StreamSchedule const&) = delete;
    StreamSchedule(StreamSchedule&&);
    StreamSchedule& operator=(StreamSchedule&&);

    void runToCompletionAsync(WaitingTaskHolder h);

    void endJob();
    void processOneEvent2(WaitingTask *h);
    uint32_t processOneEvent(WaitingTaskHolder h);
    //uint32_t processOneEvent(WaitingTaskHolder h);
    void processOneEventAsync(WaitingTaskHolder h);
    CountValidatorSimple* fOutput;
    
  private:

    ProductRegistry registry_;
    Source* source_;
    EventSetup const* eventSetup_;
    std::vector<std::unique_ptr<Worker>> path_;
    int streamId_;
  };
}  // namespace edm

#endif
