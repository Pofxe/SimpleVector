#pragma once

#include <chrono>
#include <iostream>
#include <string>

using namespace std;

#define PROFILE_CONCAT_INTERNAL(X, Y) X##Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profileGuard, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)

class LogDuration
{
public:

    using Clock = chrono::steady_clock;

    LogDuration(const std::string& id) : id_(id) {}

    ~LogDuration()
    {
        using namespace chrono;
        using namespace literals;

        const auto end_time = Clock::now();
        const auto dur = end_time - start_time_;
        cerr << id_ << ": "s << duration_cast<milliseconds>(dur).count() << " ms"s << endl;
    }

private:
    const string id_;
    const Clock::time_point start_time_ = Clock::now();
};