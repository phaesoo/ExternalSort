#pragma once

// Problem1�� Ǯ������ include�ϴ� standard �Լ�/ ����� ���� Ŭ����/ ����� ���� ���� ���Ǹ� ���� �̰��� �ѹ��� ����
// ���� �ǹ������� �̷��� �ѹ��� �־ ������� ������ ����Ǯ�� ���Ǹ� ���� �̷��� ������

// ����Ǯ�� ���Ǽ��� ���� ���⿡ �׳� �� include��Ŵ
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

typedef unsigned __int16 uint16; // �������� ���õ� 16bit unsigned integer typedef
using namespace std;

// Define specific numbers
const int FULL_DATA_NUM   = 1000000; // ��ü ������ ������ 100����
const int MEMORY_DATA_NUM = 100000; // �޸𸮿� �ѹ��� �ø��� �ִ� ������ ���� 10����
const int FILE_NUM        = 10; // �� ���ҵ� ���� ����
const int STRING_BY_LINE  = 100; // ���ٴ� ������ ���ڿ� ����
const int OUT_BUFFER_SIZE = 10000; // MergeSort�ÿ� ���� out  ������ ������

// Define specific strings(about file)
const string ABS_FILE_DIR = []()-> const string // Program ���۽ÿ� �����θ� �˾ƿͼ� ��� ����
{
    char the_path[256];
    _getcwd(the_path, 256);

    string strpath(the_path);
    strpath.resize(strpath.find("ExternalSort"));

    return strpath + "ExternalSort\\Result\\";
}();

const string FILE_NAME              = "hexDec"; // ���ϸ�
const string FILE_NAME_GENSORT_RSLT = "hexDecSort_Gen"; // ������ ���� �Ϲ����� �����
const string FILE_NAME_EXTSORT_RSLT = "hexDecSort"; // ������ ���� �ܺ����� �����
const string FILE_NAME_SIZESORT     = "hexDecSizeSort"; // ������ ũ������� ������ ���
const string FILE_EXT               = ".bin"; // Ȯ����

const char DELIM = ','; // ���Ͽ� ��ϵ� ������ ��������