#pragma once

#include "basic_function.h"

void SizeSort();

// 문제 1-b 풀이 함수입니다.
void SolveProblem1_c()
{
    SizeSort();
}

// 정렬된 100만개의 데이터를 개수가 가장 많은 데이터부터 정렬하는 알고리즘
// 추가규칙 : 개수가 같은 경우 데이터숫자가 적은 데이터부터 정렬
void SizeSort()
{
    ifstream inFile(GetExternalSortName(), ifstream::binary);

    // geline할때 local 변수로 정의해도 되지만
    // 미리 공간 예약해 놈으로서 성능향상 도모
    vector<string> textList;
    textList.reserve(STRING_BY_LINE);

    uint16 prevdata = 0; // 이전 데이터
    int dataNum = 0; // 각 데이터별 개수

    // 데이터(uint16), 개수(int)를 순차적으로 저장할 Counter 변수
    // 추후 sort 알고리즘을 사용하여 개수가 많은 순서로 재정렬할 예정
    // 문제에 제시된 메모리 사이즈 내에서 충분히 저장 가능
    numeric_limits<uint16> limit;
    vector < pair<uint16, int> > dataCounter(limit.max() + 1);

    string textline;
    while (getline(inFile, textline))
    {
        textList = SeperateString(textline, DELIM); // delimiter를 기준으로 string분할
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
                uint16 data = static_cast<uint16>(stoi(textList[t]));

                if (dataNum == 0) // 최초 진입시에만
                {
                    prevdata = data;
                    dataNum = 1;
                }
                else if (prevdata == data) // 이전과 같은 데이터일 경우 개수 + 1
                {
                    ++dataNum;
                }
                else // 이전과 다른데이터가 들어온 경우
                {
                    dataCounter[prevdata].first  = prevdata; // 이전데이터를 넣어주고
                    dataCounter[prevdata].second = dataNum; // 이전데이터 개수를 넣어주고
                    dataNum = 1; // 데이터 개수를 초기화하고
                    prevdata = data; // 이전데이터를 현재데이터로 갱신한다.
                }
            }
            catch (...)
            {
                assert(0);
            }
        }
    }

    if (dataNum > 0) // 반복문에서 넣지못한 데이터가 남아있을경우(마지막 데이터) 값을 넣어준다.
    {
        dataCounter[prevdata] = make_pair(prevdata, dataNum);
    }

    // 정렬을 수행한다
    // 정렬기준 1. 개수가 많은순서
    // 정렬기준 2. 개수가 같은경우 작은 데이터 먼저
    sort(dataCounter.begin(), dataCounter.end(), [](const pair<uint16, int>& left, const pair<uint16, int>& right)->bool {
        if (left.second > right.second) // 가장 우선적으로 개수 많은 순으로 정렬
        {
            return true;
        }
        else if (left.second == right.second) // 개수가 같을경우 숫자가 작은 값을 먼저 배치
        {
            return left.first < right.first;
        }
        else
        {
            return false;
        }
    });

    ofstream outFile(GetSizeSortFileName(), ofstream::binary); // 결과파일

    int dataIndex = 1;
    for (auto itr = dataCounter.begin(); itr != dataCounter.end(); ++itr)
    {
        uint16 data = itr->first;
        int dataCount = itr->second;
        assert(dataCount > 0);

        for (int i = 0; i < dataCount; ++i) // 각 데이터의 개수만큼씩 결과파일에 적어준다.
        {
            // 추후 getline을 활용할때 한줄이 너무 길어지면 문제에서 제시한 메모리를 넘을수도 있다는 가정하에
            // 1000개의 데이터씩 끊어서 기록
            if ((dataIndex % STRING_BY_LINE) == 0)
            {
                outFile << data << endl;
            }
            else
            {
                outFile << data << DELIM;
            }

            ++dataIndex;
        }
    }

    assert(dataIndex == FULL_DATA_NUM + 1); // 예외처리
}