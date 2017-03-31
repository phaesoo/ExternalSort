#pragma once

// Problem1을 풀기위해 include하는 standard 함수/ 사용자 정의 클래스/ 사용자 정의 값을 편의를 위해 이곳에 한번에 모음
// 실제 실무에서는 이렇게 한번에 넣어서 사용하지 않지만 문제풀이 편의를 위해 이렇게 정의함

// 문제풀이 편의성을 위해 여기에 그냥 다 include시킴
#include <vector>
#include <deque>
#include <fstream>
#include <random>
#include <ctime>
#include <limits>
#include <functional>
#include <string>
#include <assert.h>
#include <algorithm>
#include <set>
#include <iostream>
#include <direct.h>

typedef unsigned __int16 uint16; // 문제에서 제시된 16bit unsigned integer typedef
using namespace std;

// Define specific numbers
const int FULL_DATA_NUM   = 1000000; // 전체 데이터 사이즈 100만개
const int MEMORY_DATA_NUM = 100000; // 메모리에 한번에 올릴수 있는 데이터 개수 10만개
const int FILE_NUM        = 10; // 총 분할될 파일 개수
const int STRING_BY_LINE  = 100; // 한줄당 쓰여질 문자열 개수
const int OUT_BUFFER_SIZE = 10000; // MergeSort시에 사용될 out  버퍼의 사이즈

// Define specific strings(about file)
const string ABS_FILE_DIR = []()-> const string // Program 동작시에 절대경로를 알아와서 경로 세팅
{
    char the_path[256];
    _getcwd(the_path, 256);

    string strpath(the_path);
    strpath.resize(strpath.find("ExternalSort"));

    return strpath + "ExternalSort\\Result\\";
}();

const string FILE_NAME              = "hexDec"; // 파일명
const string FILE_NAME_GENSORT_RSLT = "hexDecSort_Gen"; // 검증을 위한 일반정렬 결과명
const string FILE_NAME_EXTSORT_RSLT = "hexDecSort"; // 문제를 위한 외부정렬 결과명
const string FILE_NAME_SIZESORT     = "hexDecSizeSort"; // 사이즈 크기순으로 정렬한 결과
const string FILE_EXT               = ".bin"; // 확장자

const char DELIM = ','; // 파일에 기록될 데이터 구분인자