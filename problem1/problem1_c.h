#pragma once

#include "basic_function.h"

void SizeSort();

// ���� 1-b Ǯ�� �Լ��Դϴ�.
void SolveProblem1_c()
{
    SizeSort();
}

// ���ĵ� 100������ �����͸� ������ ���� ���� �����ͺ��� �����ϴ� �˰���
// �߰���Ģ : ������ ���� ��� �����ͼ��ڰ� ���� �����ͺ��� ����
void SizeSort()
{
    ifstream inFile(GetExternalSortName(), ifstream::binary);

    // geline�Ҷ� local ������ �����ص� ������
    // �̸� ���� ������ �����μ� ������� ����
    vector<string> textList;
    textList.reserve(STRING_BY_LINE);

    uint16 prevdata = 0; // ���� ������
    int dataNum = 0; // �� �����ͺ� ����

    // ������(uint16), ����(int)�� ���������� ������ Counter ����
    // ���� sort �˰����� ����Ͽ� ������ ���� ������ �������� ����
    // ������ ���õ� �޸� ������ ������ ����� ���� ����
    numeric_limits<uint16> limit;
    vector < pair<uint16, int> > dataCounter(limit.max() + 1);

    string textline;
    while (getline(inFile, textline))
    {
        textList = SeperateString(textline, DELIM); // delimiter�� �������� string����
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
                uint16 data = static_cast<uint16>(stoi(textList[t]));

                if (dataNum == 0) // ���� ���Խÿ���
                {
                    prevdata = data;
                    dataNum = 1;
                }
                else if (prevdata == data) // ������ ���� �������� ��� ���� + 1
                {
                    ++dataNum;
                }
                else // ������ �ٸ������Ͱ� ���� ���
                {
                    dataCounter[prevdata].first  = prevdata; // ���������͸� �־��ְ�
                    dataCounter[prevdata].second = dataNum; // ���������� ������ �־��ְ�
                    dataNum = 1; // ������ ������ �ʱ�ȭ�ϰ�
                    prevdata = data; // ���������͸� ���絥���ͷ� �����Ѵ�.
                }
            }
            catch (...)
            {
                assert(0);
            }
        }
    }

    if (dataNum > 0) // �ݺ������� �������� �����Ͱ� �����������(������ ������) ���� �־��ش�.
    {
        dataCounter[prevdata] = make_pair(prevdata, dataNum);
    }

    // ������ �����Ѵ�
    // ���ı��� 1. ������ ��������
    // ���ı��� 2. ������ ������� ���� ������ ����
    sort(dataCounter.begin(), dataCounter.end(), [](const pair<uint16, int>& left, const pair<uint16, int>& right)->bool {
        if (left.second > right.second) // ���� �켱������ ���� ���� ������ ����
        {
            return true;
        }
        else if (left.second == right.second) // ������ ������� ���ڰ� ���� ���� ���� ��ġ
        {
            return left.first < right.first;
        }
        else
        {
            return false;
        }
    });

    ofstream outFile(GetSizeSortFileName(), ofstream::binary); // �������

    int dataIndex = 1;
    for (auto itr = dataCounter.begin(); itr != dataCounter.end(); ++itr)
    {
        uint16 data = itr->first;
        int dataCount = itr->second;
        assert(dataCount > 0);

        for (int i = 0; i < dataCount; ++i) // �� �������� ������ŭ�� ������Ͽ� �����ش�.
        {
            // ���� getline�� Ȱ���Ҷ� ������ �ʹ� ������� �������� ������ �޸𸮸� �������� �ִٴ� �����Ͽ�
            // 1000���� �����;� ��� ���
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

    assert(dataIndex == FULL_DATA_NUM + 1); // ����ó��
}