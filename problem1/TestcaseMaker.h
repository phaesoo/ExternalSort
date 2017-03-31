#pragma once

#include "basic_function.h"

// 1-b 검증을 위해 정의한 100만개의 데이터를 한번에 메모리에 올려 정렬하는 함수
bool GeneralSort()
{
    ifstream inFile(GetOriginalFileName(), ifstream::binary);
    ofstream outFile(FILE_NAME_GENSORT_RSLT, ofstream::binary); // 결과파일 생성

    vector<uint16> dataList;

    string textline;
    while (getline(inFile, textline))
    {

        vector<string> textList = SeperateString(textline, DELIM); // delimiter를 기준으로 string분할
        size_t tsize = textList.size();
        if (tsize != STRING_BY_LINE)
        {
            // 줄당 이미 쓰기로한 데이터 양만큼 string이 넘어오지 않은경우 예외처리
            assert(0);
            break;
        }

        for (size_t t = 0; t < tsize; ++t)
        {
            try // stoi 비정상 동작시 예외 검사위해
            {
                dataList.push_back(static_cast<uint16>(stoi(textList[t]))); // 성능을 위해 push_back하지 않음
            }
            catch (...)
            {
                assert(0);
            }
        }
    }

    sort(dataList.begin(), dataList.end());
    for (int i = 1; i < dataList.size() + 1; ++i)
    {
        // 추후 getline을 활용할때 한줄이 너무 길어지면 문제에서 제시한 메모리를 넘을수도 있다는 가정하에
        // 1000개의 데이터씩 끊어서 기록
        if ((i % STRING_BY_LINE) == 0)
        {
            outFile << dataList[i - 1] << endl;
        }
        else
        {
            outFile << dataList[i - 1] << DELIM;
        }
    }

    return true;
}

// 1_c 검증을 위한 데이터 생성용 함수
void TestCase_problem1_c()
{
    ofstream outFile(GetTestcaseFileName(), ofstream::binary);

    int total = 1;

    // 문제에서 제시한 100만개의 데이터를 파일에 기록
    for (int i = 1; i < FULL_DATA_NUM + 1; ++i) // 개행지점을 쉽게 판별하기 위해 시작인덱스를 1로 지정
    {
        for (int num = 0; num < i; ++num)
        {
            // 추후 데이터 읽기시 getline을 활용할 것이므로 사전에 적절히 정의된 개수(STRING_BY_LINE)만큼씩 한줄씩 쓰기
            if ((total % STRING_BY_LINE) == 0)
            {
                outFile << i << endl;
                ++total;
            }
            else
            {
                outFile << i << DELIM;
                ++total;
            }
            if (total == FULL_DATA_NUM + 1) { break;  }
        }
        if (total == FULL_DATA_NUM + 1) { break; }
    }
}