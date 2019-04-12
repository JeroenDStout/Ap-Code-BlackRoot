/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#pragma once

#include <functional>
#include <thread>
#include <atomic>
#include <vector>
#include <condition_variable>

#include "BlackRoot/Pubc/Sys Thread.h"

namespace BlackRoot {
namespace Util {

    
    class ThreadedCaller { 
    public:
        using Function  = std::function<void()>;
        using Count     = std::size_t;
        using Priority  = BlackRoot::System::ThreadPriority::Type;

    protected:
        struct InnerThread {
            std::thread     Thread;
        };

        Function                    InnerFunction;
        std::vector<InnerThread*>   Threads;

        std::mutex                  MtThreads;
        std::condition_variable     CvThreadCountChange, CvThreadsRequireAction;
        
        Count                       TargetThreadCount;
        std::atomic<Count>          RequestedCallCount;

        void InternalLaunchThread();
    public:
        ThreadedCaller(Function f);
        ~ThreadedCaller();

        void SetMaxThreadCount(Count);
        void SetMaxThreadCountAndWait(Count);

        void RequestCalls(Count amount);

        void SetPriority(Priority);

        void EndAndWait();
    };

}
}