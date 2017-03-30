

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

#include "TextUtil.h"

const int FULL_DATA_NUM = 1000000; // ��ü ������ ������ 100����
const int MEMORY_DATA_NUM = 100000; // �޸𸮿� �ѹ��� �ø��� �ִ� ������ ���� 10����
const int FILE_NUM = 10; // �� ���ҵ� ���� ����
const int STRING_BY_LINE = 100; // ���ٴ� ������ ���ڿ� ����
const int BUFFER_SIZE = 10000; // MergeSort�ÿ� ���� ������ ������

const char delimiter = ','; // ���Ͽ� ��ϵ� ������ ��������

typedef unsigned __int16 uint16;
using namespace std;

string GetDividedFileName(int index)
{
	return "./hexDec_" + to_string(index) + ".bin"; // ���� ���ϸ� _index�� ���� ���ϸ� ����
}

// problem a
void GenerateFile_hexDec()
{
	ofstream outFile("./hexDec.bin", std::ifstream::binary);

	mt19937 engine(static_cast<unsigned int>(time(NULL))); // ������ �������������� mt19937 ���
	numeric_limits<uint16> limit; // unsigned __int16 ������ ���� �ִ밪�� �˾ƿ�������
	uniform_int_distribution<unsigned __int16> dist(0, limit.max()); // ���� ����
	auto rand_generator = bind(dist, engine); // �����߻��� ����

	// �������� ������ 1,000,000���� ������ ���Ͽ� ���
	for (int i = 1; i < FULL_DATA_NUM + 1; ++i) // ���������� ���� Ȯ���ϱ� ���� ���� index�� 1��
	{
		// ���� ������ �б�ÿ� getline�� Ȱ���� ���̹Ƿ� ������ ������ ���ǵ� ������ŭ�� ���پ� ����
		if ((i % STRING_BY_LINE) == 0)
		{
			outFile << rand_generator() << endl;

		}
		else
		{
			outFile << rand_generator() << delimiter;
		}
	}
}

// 100������ hexDec.bin������ �о� ���ĵ� ������ 10�������� ���� 10���� �����ϴ� �Լ�
bool DivideFileWithSort()
{
	ifstream inFile("./hexDec.bin", ifstream::binary);

	int ratio = MEMORY_DATA_NUM / STRING_BY_LINE; // ���� ���ϴ� ������ ������� �ٴ� ������ ������ ������ŭ loop����

	// 10���� ���Ͽ� ���� loop ����
	for (int file = 0; file < FILE_NUM; ++file)
	{
		string fileName = GetDividedFileName(file);
		ofstream outFile(fileName, ifstream::binary);

		// 10����¥�� �迭������ ����, ������ ���� ������ ������ ����� �Ҵ�
		// �������� �־��� 10������ ������ �޸� ���� ���
		vector<uint16> dataList(MEMORY_DATA_NUM); 

		int dataIndex = 0;
		for (int r = 0; r < ratio; ++r)
		{
			if (dataIndex >= MEMORY_DATA_NUM) { assert(0); return false; } // ����ó��

			string textline;
			getline(inFile, textline); // ���پ� �о����

			vector<string> textList = TextUtil::SeperateString(textline, delimiter); // delimiter�� �������� string����
			long tsize = textList.size();
			if (tsize != STRING_BY_LINE)
			{
				// �ٴ� �̹� ������� ������ �縸ŭ string�� �Ѿ���� ������� ����ó��
				assert(0);
				break;
			}

			for (int t = 0; t < tsize; ++t)
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

		sort(dataList.begin(), dataList.end()); // ������ sort����

		for (int i = 1; i < MEMORY_DATA_NUM + 1; ++i) // ���������� ���� Ȯ���ϱ� ���� ���� index�� 1��
		{
			// ���� ������ �б�ÿ� getline�� Ȱ���� ���̹Ƿ� ������ ������ ���ǵ� ������ŭ�� ���پ� ����
			if ((i % STRING_BY_LINE) == 0)
			{
				outFile << dataList[i - 1] << endl;
			}
			else
			{
				outFile << dataList[i - 1] << delimiter;
			}
		}
	}
}

bool MergeSort()
{
	ofstream outFile("./hexDec_Sorted.bin", std::ifstream::binary); // ������� ����

	vector<uint16> dataBuffer;

	vector<ifstream*> fileList(FILE_NUM); // ������ �о�鿩 vector�� �迭�� �����͸� ����
	for (int i = 0; i < FILE_NUM; ++i)
	{
		fileList[i] = new ifstream(GetDividedFileName(i), ifstream::binary);
	}

	int data_per_file = (MEMORY_DATA_NUM - BUFFER_SIZE) / FILE_NUM; // ���ϴ� �д� ������ ������ (memory�� �ø��� ������ ���� - buffer�Ҵ� ������ ����) / ���ϰ���, ȿ���������� ���������� ��������
	if (data_per_file % FILE_NUM != 0) { assert(0); } // ����ó��
	if (data_per_file % STRING_BY_LINE != 0) { assert(0); } // ȿ������ ���� ���� ���ٿ� ������ ������ ������ ����� ����, ����ó��

	vector < deque<uint16> > dataFile; // <-- ��������� ���ϴ� data_per_file���� �����͸����� 10���� �迭(data_per_file * 10)�� ������, deque�� �̿��ϴ� ������ pop_front�� ���ؼ�
	for (int i = 0; i < FILE_NUM; ++i)
	{
		dataFile.emplace_back(deque<uint16>(data_per_file));
	}

	int sortedDataNum = 0; // ���ĵ� ������ ����
	while (sortedDataNum < FULL_DATA_NUM) // 100������ �����Ͱ� ��� ���ĵ� ��� Ż��
	{
		for (int i = 0; i < FILE_NUM; ++i)
		{
			if (sortedDataNum > 0) // ���� ���Խÿ��� �ƴ�
			{
				// �����Ͱ� ����� �ִ� ��쿡�� �޸� ������ �Ҵ��� �о��
				if (!dataFile[i].empty()) { continue; }

				dataFile[i].resize(data_per_file);
			}

			string textline;
			int dataIndex = 0;
			while (dataIndex < data_per_file)
			{
				getline(*(fileList[i]), textline); // ���پ� �о��

				vector<string> textList = TextUtil::SeperateString(textline, delimiter); // delimiter�� �������� string����
				size_t tsize = textList.size();
				if (tsize == 0)
				{
					dataFile[i].resize(dataIndex);
					break;
				}

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
						dataFile[i][dataIndex] = static_cast<uint16>(stoi(textList[t])); // ������ ���� push_back���� ����
					}
					catch (...)
					{
						assert(0);
						dataFile[i][dataIndex] = 0; // ���ܹ߻��� 0���� ä����
					}

					++dataIndex;
				}
			}
		}

		while (dataBuffer.size() < BUFFER_SIZE)
		{
			int min_index = 0;
			int min_val = 999999; // ������ max��
			for (int index = 0; index < FILE_NUM; ++index)
			{
				if (dataFile[index].empty()) { continue; } // Page �����Ͱ� ������� continue

				if (dataFile[index].front() < min_val) // �� Page�� ù��° �����͵��� ��
				{
					min_val = dataFile[index].front();
					min_index = index;
				}
			}

			dataBuffer.push_back(min_val); // Buffer�� ���־���

			dataFile[min_index].pop_front();
			if (dataFile[min_index].empty()) { break; }
		}

		if (dataBuffer.size() == BUFFER_SIZE)
		{
			for (int i = 1; i < BUFFER_SIZE + 1; ++i) // ���������� ���� Ȯ���ϱ� ���� ���� index�� 1��
			{
				// ���� ������ �б�ÿ� getline�� Ȱ���� ���̹Ƿ� ������ ������ ���ǵ� ������ŭ�� ���پ� ����
				if ((i % STRING_BY_LINE) == 0)
				{
					outFile << dataBuffer[i - 1] << endl;
				}
				else
				{
					outFile << dataBuffer[i - 1] << delimiter;
				}
			}

			dataBuffer.clear(); // ���Ͽ� ���� ���۸� �����
			sortedDataNum += BUFFER_SIZE; // ���ĵ� ��� ���� ����
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

bool GenSort()
{
	ifstream inFile("./hexDec.bin", ifstream::binary);
	ofstream outFile("./hexDec_GenSort.bin", std::ifstream::binary); // ������� ����

	vector<uint16> dataList;

	string textline;
	while (getline(inFile, textline))
	{

		vector<string> textList = TextUtil::SeperateString(textline, delimiter); // delimiter�� �������� string����
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
			outFile << dataList[i - 1] << delimiter;
		}
	}

	return true;
}

int main()
{
	//GenerateFile_hexDec();

	//DivideFileWithSort();

	GenSort();
	MergeSort();

	vector<uint16> dummy(MEMORY_DATA_NUM);


	return 0;
}