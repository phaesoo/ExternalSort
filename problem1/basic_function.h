#pragma once

#include "basic_definition.h"

string GetOriginalFileName()
{
    return ABS_FILE_DIR + FILE_NAME + FILE_EXT;
}

string GetDividedFileName(int index)
{
    return ABS_FILE_DIR + "DevidedFile\\" + FILE_NAME + "_" + to_string(index) + FILE_EXT; // 기존 파일명에 _index를 붙인 파일명 생성
}

string GetExternalSortName()
{
    return ABS_FILE_DIR + FILE_NAME_EXTSORT_RSLT + FILE_EXT; // 기존 파일명에 _index를 붙인 파일명 생성
}

string GetSizeSortFileName()
{
    return ABS_FILE_DIR + FILE_NAME_SIZESORT + FILE_EXT; // 기존 파일명에 _index를 붙인 파일명 생성
}

static vector<string> SeperateString(const string& text, char token)
{
    vector<string> result;

    string unit;
    size_t size = text.size();

    for (size_t i = 0; i < size; ++i)
    {
        if (text[i] == token)
        {
            result.push_back(unit);
            unit.clear();
        }
        else
        {
            unit.push_back(text[i]);
            
            if (i == size - 1) // Last loop
            {
                result.push_back(unit);
            }
        }
    }

    return result;
}

// Testcase만드려고
string GetTestcaseFileName()
{
    return ABS_FILE_DIR + "\\testcase\\problem1_c" + FILE_EXT;
}