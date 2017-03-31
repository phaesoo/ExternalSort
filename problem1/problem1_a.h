#pragma once

#include "basic_function.h"

void GenerateFile_hexDec();

// 문제 1-a 풀이 함수입니다.
void SolveProblem1_a()
{
    // 100만개의 random값을 생성하여 파일로 저장하는 함수
    GenerateFile_hexDec();
}

void GenerateFile_hexDec()
{
    ofstream outFile(GetOriginalFileName(), ofstream::binary);

    // 균일한 난수생성을위해 rand 대신 mt19937 사용
    // 0 ~ unsigned __int16 max값 사이에서 난수 발생기(rand_generator)를 정의함
    mt19937 engine(static_cast<unsigned int>(time(NULL)));
    numeric_limits<uint16> limit;
    uniform_int_distribution<unsigned __int16> dist(0, limit.max());
    auto rand_generator = bind(dist, engine);

    // 문제에서 제시한 100만개의 데이터를 파일에 기록
    for (int i = 1; i < FULL_DATA_NUM + 1; ++i) // 개행지점을 쉽게 판별하기 위해 시작인덱스를 1로 지정
    {
        // 추후 데이터 읽기시 getline을 활용할 것이므로 사전에 적절히 정의된 개수(STRING_BY_LINE)만큼씩 한줄씩 쓰기
        if ((i % STRING_BY_LINE) == 0)
        {
            outFile << rand_generator() << endl;

        }
        else
        {
            outFile << rand_generator() << DELIM;
        }
    }
}