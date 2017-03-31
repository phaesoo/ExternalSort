#include <algorithm>
#include <assert.h>

#include "PerfChecker.h"
#include <xutility>

const double MEGA = 1.0e+6;

bool PerfChecker::Start()
{
    if (run_flag_) { /*already running*/ assert(0); return false; }
    Initialize();
    run_flag_ = true;
    // start count
    if (!QueryPerformanceCounter(&dummy_)) { assert(0); return false; }
    check_.push_back(dummy_.QuadPart);
    return true;
}

bool PerfChecker::Check()
{
    if (!run_flag_) { /*do no running*/ assert(0); return false; }
    if (!QueryPerformanceCounter(&dummy_)) { assert(0); return false; }
    check_.push_back(dummy_.QuadPart);
    return true;
}

bool PerfChecker::End()
{
    if (!run_flag_) { /*do no running*/ assert(0); return false; }
    if (!QueryPerformanceCounter(&dummy_)) { assert(0); return false; }
    check_.push_back(dummy_.QuadPart);
    run_flag_ = false;
    return true;
}

bool PerfChecker::GetPerformTime(std::vector<double>& perform_time, bool accumulate /*= false*/)
{
    if (!CheckValidity()) { return false; }
    perform_time.clear();

    double accum_perftime = 0.0;
    for (unsigned int i = 1; i < check_.size(); ++i)
    {
        const double unit_perftime = CalcPerformTime(check_[i - 1], check_[i]);
        if (accumulate)
        {
            accum_perftime += unit_perftime;
        }
        perform_time.push_back(unit_perftime);
    }
    return true;
}

bool PerfChecker::GetTotalPerformTime(double& perform_time)
{
    if (!CheckValidity()) { return false; }
    perform_time = CalcPerformTime(*check_.begin(), *(--check_.end()));
    return true;
}

double PerfChecker::CalcPerformTime(const LONGLONG& start, const LONGLONG& end)
{
    if (!QueryPerformanceFrequency(&dummy_)) { assert(0); return 0.0; }
    const double us = (end - start) * MEGA / static_cast<double>(dummy_.QuadPart); // micro second
    return  us * GetUnitFactor();
}

double PerfChecker::GetUnitFactor()
{
    switch (timeunit_)
    {
    case PerfChecker::Second:      return 1.0e-6;
    case PerfChecker::MilliSecond: return 1.0e-3;
    case PerfChecker::MicroSecond: return 1.0;
    default: assert(0); break;
    }

    return 0.0;
}

bool PerfChecker::CheckValidity()
{
    if (run_flag_ || check_.empty()) { assert(0); return false; }
    return true;
}