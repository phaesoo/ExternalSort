#include <windows.h>
#include <vector>
#include <assert.h>

class PerfChecker
{
public:
    enum TimeUnit : UINT
    {
        Second = 0,
        MilliSecond,
        MicroSecond,
    };

public:
    PerfChecker()
        : timeunit_(Second),
        run_flag_(false) { Initialize(); }
    PerfChecker(TimeUnit timeunit)
        : timeunit_(timeunit),
        run_flag_(false) { Initialize(); }

    ~PerfChecker() {}

    // Commands
    bool Start();
    bool Check();
    bool End();
    bool GetPerformTime(std::vector<double>& perform_time, bool accumulate = false);
    bool GetTotalPerformTime(double& perform_time);

    // Util
    void SetUnit(const TimeUnit unit) { timeunit_ = unit; }

private:
    // Calculate
    double CalcPerformTime(const LONGLONG& start, const LONGLONG& end);
    double GetUnitFactor();

    void Initialize() { check_.clear(); }
    bool CheckValidity();

private:
    // data
    std::vector<LONGLONG> check_;

    // setting
    TimeUnit timeunit_;
    bool run_flag_;

    // etc
    LARGE_INTEGER dummy_;
};
