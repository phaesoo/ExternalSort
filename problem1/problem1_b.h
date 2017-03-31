#pragma once

#include "basic_function.h"

bool DivideFileWithSort();
bool ExternalMergeSort();

// 문제 1-b 풀이 함수입니다.
void SolveProblem1_b()
{
    // 100만개의 hexDec.bin파일을 읽어 데이터 10만개씩의 정렬된 파일 10개를 생성하는 함수
    if (!DivideFileWithSort()) { assert(0); }

    // External sort를 실행하는 함수
    if (!ExternalMergeSort()) { assert(0); }
}

bool DivideFileWithSort()
{
    ifstream inFile(GetOriginalFileName(), ifstream::binary);

    const int ratio = MEMORY_DATA_NUM / STRING_BY_LINE; // 개별 파일당 데이터 사이즈에서 줄당 데이터 개수를 나눈만큼 loop돌것

    for (int file = 0; file < FILE_NUM; ++file)
    {
        string fileName = GetDividedFileName(file);
        ofstream outFile(fileName, ofstream::binary);

        // 10만개짜리 배열데이터 생성, 성능을 위해 사전에 데이터 사이즈를 할당함
        // 문제에서 주어진 10만개의 데이터 메모리 조건 사용
        vector<uint16> dataList(MEMORY_DATA_NUM);

        int dataIndex = 0;
        for (int r = 0; r < ratio; ++r)
        {
            if (dataIndex >= MEMORY_DATA_NUM) { assert(0); return false; } // 예외처리

            string textline;
            getline(inFile, textline); // 한줄씩 읽어들임

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
                    dataList[dataIndex] = static_cast<uint16>(stoi(textList[t]));
                }
                catch (...)
                {
                    assert(0);
                    dataList[dataIndex] = 0; // 예외발생시 0값을 채워줌
                }
                ++dataIndex;
            }
        }

        if (dataIndex != MEMORY_DATA_NUM) { assert(0); return false; } // 예외처리

        sort(dataList.begin(), dataList.end()); // 10만개의 데이터 sort수행

        for (int i = 1; i < MEMORY_DATA_NUM + 1; ++i) // 개행지점을 쉽게 판별하기 위해 시작인덱스를 1로 지정
        {
            // 추후 데이터 읽기시 getline을 활용할 것이므로 사전에 적절히 정의된 개수(STRING_BY_LINE)만큼씩 한줄씩 쓰기
            if ((i % STRING_BY_LINE) == 0)
            {
                outFile << dataList[i - 1] << endl;
            }
            else
            {
                outFile << dataList[i - 1] << DELIM;
            }
        }
    }

    return true;
}

bool ExternalMergeSort()
{
    ofstream outFile(GetExternalSortName(), ofstream::binary); // 결과파일 생성

    // 각 정렬되어있는 10개의 파일을 읽어 vector에 포인터로 저장해놓고 접근
    vector<ifstream*> fileList(FILE_NUM);
    for (int i = 0; i < FILE_NUM; ++i)
    {
        fileList[i] = new ifstream(GetDividedFileName(i), ifstream::binary);
    }

    // 각 10개의 파일에서 읽어와서 저장할 input buffer의 사이즈 계산
    // 메모리 사용을 최대화 하기 위해 나머지가 없도록 처리함
    int inBufferSize = (MEMORY_DATA_NUM - OUT_BUFFER_SIZE) / FILE_NUM;
    // 예외처리
    if (inBufferSize % FILE_NUM != 0) { assert(0); }
    if (inBufferSize % STRING_BY_LINE != 0) { assert(0); }

    // Input buffer 선언
    // deque를 이용한 이유는 오름차순 정렬시 맨 앞데이터를 pop_front하여 빼오기 위해
    // emplace_back을 이용하여 push_back보다 성능 향상 도모(R-value)
    vector < deque<uint16> > inBuffer;
    for (int i = 0; i < FILE_NUM; ++i)
    {
        inBuffer.emplace_back(deque<uint16>(inBufferSize));
    }

    // geline할때 local 변수로 정의해도 되지만
    // 미리 공간 예약해 놈으로서 성능향상 도모
    vector<string> textList;
    textList.reserve(STRING_BY_LINE);

    // ExternalMergeSort 수행을 위한 Output Buffer선언
    // 미리 buffer사이즈를 reserve해놓음으로써 불필요한 메모리 재할당을 막음(성능향상)
    vector<uint16> outBuffer;
    outBuffer.reserve(OUT_BUFFER_SIZE);

    int sortedDataNum = 0; // 정렬된 데이터 개수
    while (sortedDataNum < FULL_DATA_NUM) // 100만개의 데이터가 모두 정렬된 경우 탈출
    {
        for (int i = 0; i < FILE_NUM; ++i)
        {
            if (sortedDataNum > 0) // 최초 진입시에는 무조건 텍스트를 읽어와야 하므로
            {
                // Input buffer에 아직 데이터가 있는경우 데이터를 다시 읽어오지 않아도 됨
                if (!inBuffer[i].empty()) { continue; }

                inBuffer[i].resize(inBufferSize);
            }

            string textline;
            int dataIndex = 0;
            while (dataIndex < inBufferSize)
            {
                getline(*(fileList[i]), textline); // 한줄씩 읽어옴

                textList = SeperateString(textline, DELIM); // delimiter를 기준으로 string분할
                size_t tsize = textList.size();
                if (tsize == 0)
                {
                    inBuffer[i].resize(dataIndex);
                    break;
                }

                if (tsize != STRING_BY_LINE) // 예외처리
                {
                    assert(0);
                    break;
                }

                for (size_t t = 0; t < tsize; ++t)
                {
                    try // stoi 비정상 동작시 예외 검사위해
                    {
                        inBuffer[i][dataIndex] = static_cast<uint16>(stoi(textList[t])); // 성능을 위해 push_back하지 않음
                    }
                    catch (...)
                    {
                        assert(0);
                        inBuffer[i][dataIndex] = 0; // 예외발생시 0값을 채워줌
                    }

                    ++dataIndex;
                }
            }
        }

        while (outBuffer.size() < OUT_BUFFER_SIZE) // Output Buffer가 모두 채워진 경우 탈출
        {
            int min_index = 0;
            int min_val = 999999; // 최소값 판별을 위한 임시 초기 max값 (uint max값보다 큰값임)
            for (int index = 0; index < FILE_NUM; ++index)
            {
                if (inBuffer[index].empty()) { continue; } // Page 데이터가 없을경우 continue

                if (inBuffer[index].front() < min_val) // 각 Page의 첫번째 데이터들을 비교
                {
                    min_val = inBuffer[index].front();
                    min_index = index;
                }
            }

            outBuffer.push_back(min_val); // Buffer에 값넣어줌

            inBuffer[min_index].pop_front(); // 최소값이 있는 Input buffer의 값을 제거함

            if (inBuffer[min_index].empty()) { break; } // Input buffer에서 값을 한개 제거한 이후 input buffer가 모두 비워졌다면 탈출
        }

        if (outBuffer.size() == OUT_BUFFER_SIZE) // Output buffer가 꽉찬경우 결과파일에 기록한 후 output buffer를 초기화해줌
        {
            for (int i = 1; i < OUT_BUFFER_SIZE + 1; ++i) // 개행지점을 쉽게 판별하기 위해 시작인덱스를 1로 지정
            {
                // 추후 데이터 읽기시 getline을 활용할 것이므로 사전에 적절히 정의된 개수(STRING_BY_LINE)만큼씩 한줄씩 쓰기
                if ((i % STRING_BY_LINE) == 0)
                {
                    outFile << outBuffer[i - 1] << endl;
                }
                else
                {
                    outFile << outBuffer[i - 1] << DELIM;
                }
            }

            outBuffer.clear(); // Output buffer 초기화
            sortedDataNum += OUT_BUFFER_SIZE; // 정렬된 결과 개수 갱신
        }
        else // Output buffer가 아직 다 차지 않았으므로 loop 재실행
        {
            continue;
        }
    }

    // 힙메모리 초기화
    size_t sz = fileList.size();
    if (sz != FILE_NUM) { assert(0); return false; }

    for (size_t i = 0; i < sz; ++i) // 힙메모리 초기화
    {
        if (fileList[i] == nullptr) { assert(0); continue; }

        delete fileList[i];
        fileList[i] = nullptr;
    }

    return true;
}

