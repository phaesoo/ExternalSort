#pragma once

#include "basic_function.h"

// 1-b ������ ���� ������ 100������ �����͸� �ѹ��� �޸𸮿� �÷� �����ϴ� �Լ�
bool GeneralSort()
{
    ifstream inFile(GetOriginalFileName(), ifstream::binary);
    ofstream outFile(FILE_NAME_GENSORT_RSLT, ofstream::binary); // ������� ����

    vector<uint16> dataList;

    string textline;
    while (getline(inFile, textline))
    {

        vector<string> textList = SeperateString(textline, DELIM); // delimiter�� �������� string����
        size_t tsize = textList.size();
        if (tsize != STRING_BY_LINE)
        {
            // �ٴ� �̹� ������� ������ �縸ŭ string�� �Ѿ���� ������� ����ó��
            assert(0);
            break;
        }

        for (size_t t = 0; t < tsize; ++t)
        {
            try // stoi ������ ���۽� ���� �˻�����
            {
                dataList.push_back(static_cast<uint16>(stoi(textList[t]))); // ������ ���� push_back���� ����
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
        // ���� getline�� Ȱ���Ҷ� ������ �ʹ� ������� �������� ������ �޸𸮸� �������� �ִٴ� �����Ͽ�
        // 1000���� �����;� ��� ���
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

// 1_c ������ ���� ������ ������ �Լ�
void TestCase_problem1_c()
{
    ofstream outFile(GetTestcaseFileName(), ofstream::binary);

    int total = 1;

    // �������� ������ 100������ �����͸� ���Ͽ� ���
    for (int i = 1; i < FULL_DATA_NUM + 1; ++i) // ���������� ���� �Ǻ��ϱ� ���� �����ε����� 1�� ����
    {
        for (int num = 0; num < i; ++num)
        {
            // ���� ������ �б�� getline�� Ȱ���� ���̹Ƿ� ������ ������ ���ǵ� ����(STRING_BY_LINE)��ŭ�� ���پ� ����
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