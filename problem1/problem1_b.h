#pragma once

#include "basic_function.h"

bool DivideFileWithSort();
bool ExternalMergeSort();

// ���� 1-b Ǯ�� �Լ��Դϴ�.
void SolveProblem1_b()
{
    // 100������ hexDec.bin������ �о� ������ 10�������� ���ĵ� ���� 10���� �����ϴ� �Լ�
    if (!DivideFileWithSort()) { assert(0); }

    // External sort�� �����ϴ� �Լ�
    if (!ExternalMergeSort()) { assert(0); }
}

bool DivideFileWithSort()
{
    ifstream inFile(GetOriginalFileName(), ifstream::binary);

    const int ratio = MEMORY_DATA_NUM / STRING_BY_LINE; // ���� ���ϴ� ������ ������� �ٴ� ������ ������ ������ŭ loop����

    for (int file = 0; file < FILE_NUM; ++file)
    {
        string fileName = GetDividedFileName(file);
        ofstream outFile(fileName, ofstream::binary);

        // 10����¥�� �迭������ ����, ������ ���� ������ ������ ����� �Ҵ���
        // �������� �־��� 10������ ������ �޸� ���� ���
        vector<uint16> dataList(MEMORY_DATA_NUM);

        int dataIndex = 0;
        for (int r = 0; r < ratio; ++r)
        {
            if (dataIndex >= MEMORY_DATA_NUM) { assert(0); return false; } // ����ó��

            string textline;
            getline(inFile, textline); // ���پ� �о����

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
                    dataList[dataIndex] = static_cast<uint16>(stoi(textList[t]));
                }
                catch (...)
                {
                    assert(0);
                    dataList[dataIndex] = 0; // ���ܹ߻��� 0���� ä����
                }
                ++dataIndex;
            }
        }

        if (dataIndex != MEMORY_DATA_NUM) { assert(0); return false; } // ����ó��

        sort(dataList.begin(), dataList.end()); // 10������ ������ sort����

        for (int i = 1; i < MEMORY_DATA_NUM + 1; ++i) // ���������� ���� �Ǻ��ϱ� ���� �����ε����� 1�� ����
        {
            // ���� ������ �б�� getline�� Ȱ���� ���̹Ƿ� ������ ������ ���ǵ� ����(STRING_BY_LINE)��ŭ�� ���پ� ����
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
    ofstream outFile(GetExternalSortName(), ofstream::binary); // ������� ����

    // �� ���ĵǾ��ִ� 10���� ������ �о� vector�� �����ͷ� �����س��� ����
    vector<ifstream*> fileList(FILE_NUM);
    for (int i = 0; i < FILE_NUM; ++i)
    {
        fileList[i] = new ifstream(GetDividedFileName(i), ifstream::binary);
    }

    // �� 10���� ���Ͽ��� �о�ͼ� ������ input buffer�� ������ ���
    // �޸� ����� �ִ�ȭ �ϱ� ���� �������� ������ ó����
    int inBufferSize = (MEMORY_DATA_NUM - OUT_BUFFER_SIZE) / FILE_NUM;
    // ����ó��
    if (inBufferSize % FILE_NUM != 0) { assert(0); }
    if (inBufferSize % STRING_BY_LINE != 0) { assert(0); }

    // Input buffer ����
    // deque�� �̿��� ������ �������� ���Ľ� �� �յ����͸� pop_front�Ͽ� ������ ����
    // emplace_back�� �̿��Ͽ� push_back���� ���� ��� ����(R-value)
    vector < deque<uint16> > inBuffer;
    for (int i = 0; i < FILE_NUM; ++i)
    {
        inBuffer.emplace_back(deque<uint16>(inBufferSize));
    }

    // geline�Ҷ� local ������ �����ص� ������
    // �̸� ���� ������ �����μ� ������� ����
    vector<string> textList;
    textList.reserve(STRING_BY_LINE);

    // ExternalMergeSort ������ ���� Output Buffer����
    // �̸� buffer����� reserve�س������ν� ���ʿ��� �޸� ���Ҵ��� ����(�������)
    vector<uint16> outBuffer;
    outBuffer.reserve(OUT_BUFFER_SIZE);

    int sortedDataNum = 0; // ���ĵ� ������ ����
    while (sortedDataNum < FULL_DATA_NUM) // 100������ �����Ͱ� ��� ���ĵ� ��� Ż��
    {
        for (int i = 0; i < FILE_NUM; ++i)
        {
            if (sortedDataNum > 0) // ���� ���Խÿ��� ������ �ؽ�Ʈ�� �о�;� �ϹǷ�
            {
                // Input buffer�� ���� �����Ͱ� �ִ°�� �����͸� �ٽ� �о���� �ʾƵ� ��
                if (!inBuffer[i].empty()) { continue; }

                inBuffer[i].resize(inBufferSize);
            }

            string textline;
            int dataIndex = 0;
            while (dataIndex < inBufferSize)
            {
                getline(*(fileList[i]), textline); // ���پ� �о��

                textList = SeperateString(textline, DELIM); // delimiter�� �������� string����
                size_t tsize = textList.size();
                if (tsize == 0)
                {
                    inBuffer[i].resize(dataIndex);
                    break;
                }

                if (tsize != STRING_BY_LINE) // ����ó��
                {
                    assert(0);
                    break;
                }

                for (size_t t = 0; t < tsize; ++t)
                {
                    try // stoi ������ ���۽� ���� �˻�����
                    {
                        inBuffer[i][dataIndex] = static_cast<uint16>(stoi(textList[t])); // ������ ���� push_back���� ����
                    }
                    catch (...)
                    {
                        assert(0);
                        inBuffer[i][dataIndex] = 0; // ���ܹ߻��� 0���� ä����
                    }

                    ++dataIndex;
                }
            }
        }

        while (outBuffer.size() < OUT_BUFFER_SIZE) // Output Buffer�� ��� ä���� ��� Ż��
        {
            int min_index = 0;
            int min_val = 999999; // �ּҰ� �Ǻ��� ���� �ӽ� �ʱ� max�� (uint max������ ū����)
            for (int index = 0; index < FILE_NUM; ++index)
            {
                if (inBuffer[index].empty()) { continue; } // Page �����Ͱ� ������� continue

                if (inBuffer[index].front() < min_val) // �� Page�� ù��° �����͵��� ��
                {
                    min_val = inBuffer[index].front();
                    min_index = index;
                }
            }

            outBuffer.push_back(min_val); // Buffer�� ���־���

            inBuffer[min_index].pop_front(); // �ּҰ��� �ִ� Input buffer�� ���� ������

            if (inBuffer[min_index].empty()) { break; } // Input buffer���� ���� �Ѱ� ������ ���� input buffer�� ��� ������ٸ� Ż��
        }

        if (outBuffer.size() == OUT_BUFFER_SIZE) // Output buffer�� ������� ������Ͽ� ����� �� output buffer�� �ʱ�ȭ����
        {
            for (int i = 1; i < OUT_BUFFER_SIZE + 1; ++i) // ���������� ���� �Ǻ��ϱ� ���� �����ε����� 1�� ����
            {
                // ���� ������ �б�� getline�� Ȱ���� ���̹Ƿ� ������ ������ ���ǵ� ����(STRING_BY_LINE)��ŭ�� ���پ� ����
                if ((i % STRING_BY_LINE) == 0)
                {
                    outFile << outBuffer[i - 1] << endl;
                }
                else
                {
                    outFile << outBuffer[i - 1] << DELIM;
                }
            }

            outBuffer.clear(); // Output buffer �ʱ�ȭ
            sortedDataNum += OUT_BUFFER_SIZE; // ���ĵ� ��� ���� ����
        }
        else // Output buffer�� ���� �� ���� �ʾ����Ƿ� loop �����
        {
            continue;
        }
    }

    // ���޸� �ʱ�ȭ
    size_t sz = fileList.size();
    if (sz != FILE_NUM) { assert(0); return false; }

    for (size_t i = 0; i < sz; ++i) // ���޸� �ʱ�ȭ
    {
        if (fileList[i] == nullptr) { assert(0); continue; }

        delete fileList[i];
        fileList[i] = nullptr;
    }

    return true;
}

