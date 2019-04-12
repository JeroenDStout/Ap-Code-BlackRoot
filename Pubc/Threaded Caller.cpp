/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <algorithm>
#include "BlackRoot/Pubc/Threaded Caller.h"

using namespace BlackRoot::Util;

    //  Setup
    // --------------------

ThreadedCaller::ThreadedCaller(Function f)
: InnerFunction(f)
{
}

ThreadedCaller::~ThreadedCaller()
{
}

    //  Thread management
    // --------------------

void ThreadedCaller::InternalLaunchThread()
{
    InnerThread * innerThread = new InnerThread;
    this->Threads.push_back(innerThread);

    innerThread->Thread = std::thread([&]{
        while (true) {
                // Wait for an event that requires us to wake up
            std::unique_lock<std::mutex> threadlk(this->MtThreads);
            if (this->RequestedCallCount == 0 && this->Threads.size() <= this->TargetThreadCount) {
                this->CvThreadsRequireAction.wait(threadlk);
            }

            while (true) {
                    // If there are too many threads, exit this thread
                if (this->Threads.size() > this->TargetThreadCount) {
                    this->Threads.erase(std::remove(this->Threads.begin(), this->Threads.end(), innerThread), this->Threads.end());
                    return;
                }
                
                    // If there are no more calls, go back to sleep
                if (this->RequestedCallCount == 0) {
                    break;
                }

                    // Decrease the request and call the function;
                    // we do this locked as many threads could be here
                this->RequestedCallCount -= 1;
                threadlk.unlock();
                this->InnerFunction();
                threadlk.lock();
            }
            
        }
    });
}

    //  Control
    // --------------------

void ThreadedCaller::SetMaxThreadCount(Count count)
{
    std::unique_lock<std::mutex> lk(this->MtThreads);
    
    this->TargetThreadCount = count;

        // If we have more threads than we need, notify them all
        // so they have a chance to quit
    if (this->Threads.size() > this->TargetThreadCount) {
        this->CvThreadsRequireAction.notify_all();
        return;
    }

        // If we have fewer threads than we need, keep adding
    while (this->Threads.size() < this->TargetThreadCount) {
        this->InternalLaunchThread();
    }
}

void ThreadedCaller::SetMaxThreadCountAndWait(Count count)
{
    this->SetMaxThreadCount(count);

        // Wait for the thread count to equal the target
    std::unique_lock<std::mutex> lk(this->MtThreads);
    this->CvThreadCountChange.wait(lk, [&]{ return this->Threads.size() == this->TargetThreadCount; });
}

void ThreadedCaller::EndAndWait()
{
    this->SetMaxThreadCountAndWait(0);
}

void ThreadedCaller::RequestCalls(Count count)
{
    std::unique_lock<std::mutex> threadlk(this->MtThreads);
    this->RequestedCallCount += count;
    threadlk.unlock();

    for (Count i = 0; i < count; i++) {
        this->CvThreadsRequireAction.notify_one();
    }
}