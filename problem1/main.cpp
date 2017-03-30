

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

const int c_fullsize = 1000000;
const int c_sizemax = 100000;
const int c_stringsize = 1000;
const int c_buffersize = 10000;
const char delimiter = ',';
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
	for (int i = 1; i < c_fullsize + 1; ++i) // 1000���� ���� ���� ���� ���� index�� 1��
	{
		// ���� getline�� Ȱ���Ҷ� ������ �ʹ� ������� �������� ������ �޸𸮸� �������� �ִٴ� �����Ͽ�
		// 1000���� �����;� ��� ���
		if ((i % c_stringsize) == 0)
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

	string textline;

	int ratio = c_sizemax / c_stringsize; // ���� ���ϴ� ������ ������� �ٴ� ������ ������ ������ŭ loop����

	// 10���� ���Ͽ� ���� loop ����
		for (int file = 0; file < 10; ++file)
		{
			string fileName = GetDividedFileName(file);
			ofstream outFile(fileName, ifstream::binary);

			vector<uint16> dataList(c_sizemax); // 10����¥�� �迭������ ����, ������ ���� �̸� ������ ���� <- �������� �־��� 10������ ������ �޸� ����

			int dataIndex = 0;
			for (int r = 0; r < ratio; ++r)
			{
				if (dataIndex >= c_sizemax) { assert(0); return false; } // ����ó��

				getline(inFile, textline); // ���پ� �о����

				vector<string> textList = TextUtil::SeperateString(textline, delimiter); // delimiter�� �������� string����
				long tsize = textList.size();
				if (tsize != c_stringsize)
				{
					// �ٴ� �̹� ������� ������ �縸ŭ string�� �Ѿ���� ������� ����ó��
					assert(0);
					break;
				}

				for (int t = 0; t < tsize; ++t)
				{
					try // stoi ������ ���۽� ���� �˻�����
					{
						dataList[dataIndex] = static_cast<uint16>(stoi(textList[t])); // ������ ���� push_back���� ����
					}
					catch (...)
					{
						assert(0);
						dataList[dataIndex] = 0; // ���ܹ߻��� 0���� ä����
					}
					++dataIndex;
				}
			}

			if (dataIndex != c_sizemax) { assert(0); return false; } // ����ó��

			sort(dataList.begin(), dataList.end()); // ������ sort����

			for (int i = 1; i < c_sizemax + 1; ++i) // 1000���� ���� ���� ���� ���� index�� 1��
			{
				// ���� getline�� Ȱ���Ҷ� ������ �ʹ� ������� �������� ������ �޸𸮸� �������� �ִٴ� �����Ͽ�
				// 1000���� �����;� ��� ���
				if ((i % c_stringsize) == 0)
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

	auto _ReleaseHeapMemory = [](vector<ifstream*>& fileList) -> bool // Heap �޸� �ʱ�ȭ�Լ� �����Լ��� ����
	{
		size_t sz = fileList.size();
		if (sz != 10) { assert(0); return false; }

		for (size_t i = 0; i < sz; ++i) // ���޸� �ʱ�ȭ
		{
			if (fileList[i] == nullptr) { assert(0); }
			else { delete fileList[i]; }
		}

		return true;
	};

	int sortDataNum = 0;
	vector<uint16> dataBuffer;

	int fileNum = 10;
	vector<ifstream*> fileList(fileNum); // ������ �о�鿩 vector�� �迭�� �����͸� ����
	for (int i = 0; i < fileNum; ++i)
	{
		fileList[i] = new ifstream(GetDividedFileName(i), ifstream::binary);
	}

	int data_per_file = (c_sizemax - c_buffersize) / 10; // ���ϴ� �д� ������ ������ (memory�� �ø��� ������ ���� - buffer�Ҵ� ������ ����) / ���ϰ���, ȿ���������� ���������� ��������
	if (data_per_file % 10 != 0) { assert(0); } // ����ó��
	if (data_per_file % c_stringsize != 0) { assert(0); } // ȿ������ ���� ���� ���ٿ� ������ ������ ������ ����� ����, ����ó��

	vector < deque<uint16> > dataFile(fileNum); // <-- ��������� ���ϴ� data_per_file���� �����͸����� 10���� �迭(data_per_file * 10)�� ������, deque�� �̿��ϴ� ������ pop_front�� ���ؼ�
	//for (int i = 0; i < fileNum; ++i)
	//{
	//	dataFile.emplace_back(deque<uint16>(data_per_file));
	//}

	while (sortDataNum < c_fullsize) // 100������ �����Ͱ� ��� ���ĵ� ��� Ż��
	{
		for (int i = 0; i < fileNum; ++i)
		{
			if (sortDataNum > 0) // ���� ���Խÿ��� �ƴ�
			{
				// �����Ͱ� ����� �ִ� ��쿡�� �޸� ������ �Ҵ��� �о��
				if (!dataFile[i].empty()) { continue; }

				//dataFile[i].resize(data_per_file);
			}

			string textline;
			int dataIndex = 0;
			while (dataIndex < data_per_file && getline(*(fileList[i]), textline))
			{
				vector<string> textList = TextUtil::SeperateString(textline, delimiter); // delimiter�� �������� string����
				size_t tsize = textList.size();
				if (tsize != c_stringsize)
				{
					// �ٴ� �̹� ������� ������ �縸ŭ string�� �Ѿ���� ������� ����ó��
					assert(0);
					break;
				}

				for (size_t t = 0; t < tsize; ++t)
				{
					try // stoi ������ ���۽� ���� �˻�����
					{
						dataFile[i].push_back(static_cast<uint16>(stoi(textList[t]))); // ������ ���� push_back���� ����
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

		while (dataBuffer.size() < c_buffersize)
		{
			int min_index = 0;
			int min_val = 999999; // ������ max��
			for (int index = 0; index < fileNum; ++index)
			{
				if (dataFile[index][0] < min_val)
				{
					min_val = dataFile[index][0];
					min_index = index;
				}
			}

			dataBuffer.push_back(min_val); // Buffer�� ���־���

			dataFile[min_index].pop_front();
			if (dataFile[min_index].empty()) { break; }
		}

		if (dataBuffer.size() == c_buffersize)
		{
			for (int i = 1; i < c_buffersize + 1; ++i) // 1000���� ���� ���� ���� ���� index�� 1��
			{
				// ���� getline�� Ȱ���Ҷ� ������ �ʹ� ������� �������� ������ �޸𸮸� �������� �ִٴ� �����Ͽ�
				// 1000���� �����;� ��� ���
				if ((i % c_stringsize) == 0)
				{
					outFile << dataBuffer[i - 1] << endl;
				}
				else
				{
					outFile << dataBuffer[i - 1] << delimiter;
				}
			}

			dataBuffer.clear(); // ���Ͽ� ���� ���۸� �����
			sortDataNum += c_buffersize; // ���� ������� ������
		}

	}

	_ReleaseHeapMemory(fileList);

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
		if (tsize != c_stringsize)
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
		if ((i % c_stringsize) == 0)
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

	//GenSort();
	MergeSort();

	vector<uint16> dummy(c_sizemax);


	return 0;
}