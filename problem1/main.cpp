#include "problem1_a.h"
#include "problem1_b.h"
#include "problem1_c.h"
#include "PerfChecker.h"

#include "TestcaseMaker.h"


void Testcase()
{
    TestCase_problem1_c();
}

int main()
{
    Testcase();

    PerfChecker pf(PerfChecker::MilliSecond); // 밀리세컨드 단위로 성능 체크

    // Problem 1-a
    cout << "<----- Start to solve problem 1-a ----->" << endl;
    pf.Start();
    SolveProblem1_a();
    pf.End();

    double time = 0.0;
    if (!pf.GetTotalPerformTime(time)) { assert(0); }

    cout << "Finish to solve!" << endl
        << "Result file path : " << GetOriginalFileName() << endl
        << "Performance time : " << time << " (ms)" << endl << endl;

    // Problem 1-b
    cout << "<----- Start to solve problem 1-b ----->" << endl;
    pf.Start();
    SolveProblem1_b();
    pf.End();
    
    if (!pf.GetTotalPerformTime(time)) { assert(0); }

    cout << "Finish to solve!" << endl
        << "Result file path : " << GetExternalSortName() << endl
        << "Performance time : " << time << " (ms)" << endl << endl;

    // Problem 1-c
    cout << "<----- Start to solve problem 1-c ----->" << endl;
    pf.Start();
    SolveProblem1_c();
    pf.End();

    if (!pf.GetTotalPerformTime(time)) { assert(0); }

    cout << "Finish to solve!" << endl
        << "Result file path : " << GetSizeSortFileName() << endl
        << "Performance time : " << time << " (ms)" << endl << endl;

	return 0;
}