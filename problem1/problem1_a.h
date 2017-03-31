#pragma once

#include "basic_function.h"

void GenerateFile_hexDec();

// ���� 1-a Ǯ�� �Լ��Դϴ�.
void SolveProblem1_a()
{
    // 100������ random���� �����Ͽ� ���Ϸ� �����ϴ� �Լ�
    GenerateFile_hexDec();
}

void GenerateFile_hexDec()
{
    ofstream outFile(GetOriginalFileName(), ofstream::binary);

    // ������ �������������� rand ��� mt19937 ���
    // 0 ~ unsigned __int16 max�� ���̿��� ���� �߻���(rand_generator)�� ������
    mt19937 engine(static_cast<unsigned int>(time(NULL)));
    numeric_limits<uint16> limit;
    uniform_int_distribution<unsigned __int16> dist(0, limit.max());
    auto rand_generator = bind(dist, engine);

    // �������� ������ 100������ �����͸� ���Ͽ� ���
    for (int i = 1; i < FULL_DATA_NUM + 1; ++i) // ���������� ���� �Ǻ��ϱ� ���� �����ε����� 1�� ����
    {
        // ���� ������ �б�� getline�� Ȱ���� ���̹Ƿ� ������ ������ ���ǵ� ����(STRING_BY_LINE)��ŭ�� ���پ� ����
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