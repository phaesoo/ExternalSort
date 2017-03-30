

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
	return "./hexDec_" + to_string(index) + ".bin"; // 기존 파일명에 _index를 붙인 파일명 생성
}

// problem a
void GenerateFile_hexDec()
{
	ofstream outFile("./hexDec.bin", std::ifstream::binary);

	mt19937 engine(static_cast<unsigned int>(time(NULL))); // 균일한 난수생성을위해 mt19937 사용
	numeric_limits<uint16> limit; // unsigned __int16 데이터 형의 최대값을 알아오기위해
	uniform_int_distribution<unsigned __int16> dist(0, limit.max()); // 범위 점의
	auto rand_generator = bind(dist, engine); // 난수발생기 생성

	// 문제에서 제시한 1,000,000개의 데이터 파일에 기록
	for (int i = 1; i < c_fullsize + 1; ++i) // 1000개씩 쉽게 끊기 위해 시작 index를 1로
	{
		// 추후 getline을 활용할때 한줄이 너무 길어지면 문제에서 제시한 메모리를 넘을수도 있다는 가정하에
		// 1000개의 데이터씩 끊어서 기록
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

// 100만개의 hexDec.bin파일을 읽어 정렬된 데이터 10만개씩의 파일 10개를 생성하는 함수
bool DivideFileWithSort()
{
	ifstream inFile("./hexDec.bin", ifstream::binary);

	string textline;

	int ratio = c_sizemax / c_stringsize; // 개별 파일당 데이터 사이즈에서 줄당 데이터 개수를 나눈만큼 loop돌것

	// 10개의 파일에 대한 loop 수행
		for (int file = 0; file < 10; ++file)
		{
			string fileName = GetDividedFileName(file);
			ofstream outFile(fileName, ifstream::binary);

			vector<uint16> dataList(c_sizemax); // 10만개짜리 배열데이터 생성, 성능을 위해 미리 사이즈 잡음 <- 문제에서 주어진 10만개의 데이터 메모리 조건

			int dataIndex = 0;
			for (int r = 0; r < ratio; ++r)
			{
				if (dataIndex >= c_sizemax) { assert(0); return false; } // 예외처리

				getline(inFile, textline); // 한줄씩 읽어들임

				vector<string> textList = TextUtil::SeperateString(textline, delimiter); // delimiter를 기준으로 string분할
				long tsize = textList.size();
				if (tsize != c_stringsize)
				{
					// 줄당 이미 쓰기로한 데이터 양만큼 string이 넘어오지 않은경우 예외처리
					assert(0);
					break;
				}

				for (int t = 0; t < tsize; ++t)
				{
					try // stoi 비정상 동작시 예외 검사위해
					{
						dataList[dataIndex] = static_cast<uint16>(stoi(textList[t])); // 성능을 위해 push_back하지 않음
					}
					catch (...)
					{
						assert(0);
						dataList[dataIndex] = 0; // 예외발생시 0값을 채워줌
					}
					++dataIndex;
				}
			}

			if (dataIndex != c_sizemax) { assert(0); return false; } // 예외처리

			sort(dataList.begin(), dataList.end()); // 데이터 sort수행

			for (int i = 1; i < c_sizemax + 1; ++i) // 1000개씩 쉽게 끊기 위해 시작 index를 1로
			{
				// 추후 getline을 활용할때 한줄이 너무 길어지면 문제에서 제시한 메모리를 넘을수도 있다는 가정하에
				// 1000개의 데이터씩 끊어서 기록
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
	ofstream outFile("./hexDec_Sorted.bin", std::ifstream::binary); // 결과파일 생성

	auto _ReleaseHeapMemory = [](vector<ifstream*>& fileList) -> bool // Heap 메모리 초기화함수 람다함수로 정의
	{
		size_t sz = fileList.size();
		if (sz != 10) { assert(0); return false; }

		for (size_t i = 0; i < sz; ++i) // 힙메모리 초기화
		{
			if (fileList[i] == nullptr) { assert(0); }
			else { delete fileList[i]; }
		}

		return true;
	};

	int sortDataNum = 0;
	vector<uint16> dataBuffer;

	int fileNum = 10;
	vector<ifstream*> fileList(fileNum); // 파일을 읽어들여 vector에 배열로 포인터를 저장
	for (int i = 0; i < fileNum; ++i)
	{
		fileList[i] = new ifstream(GetDividedFileName(i), ifstream::binary);
	}

	int data_per_file = (c_sizemax - c_buffersize) / 10; // 파일당 읽는 데이터 개수는 (memory에 올리는 데이터 개수 - buffer할당 데이터 개수) / 파일개수, 효율성을위해 딱떨어지게 조정했음
	if (data_per_file % 10 != 0) { assert(0); } // 예외처리
	if (data_per_file % c_stringsize != 0) { assert(0); } // 효율성을 위해 파일 한줄에 써지는 데이터 개수의 배수로 맞춤, 예외처리

	vector < deque<uint16> > dataFile(fileNum); // <-- 결론적으로 파일당 data_per_file개의 데이터를가진 10개의 배열(data_per_file * 10)이 생성됨, deque를 이용하는 이유는 pop_front를 위해서
	//for (int i = 0; i < fileNum; ++i)
	//{
	//	dataFile.emplace_back(deque<uint16>(data_per_file));
	//}

	while (sortDataNum < c_fullsize) // 100만개의 데이터가 모두 정렬된 경우 탈출
	{
		for (int i = 0; i < fileNum; ++i)
		{
			if (sortDataNum > 0) // 최초 진입시에는 아님
			{
				// 데이터가 비워져 있는 경우에만 메모리 사이즈 할당후 읽어옴
				if (!dataFile[i].empty()) { continue; }

				//dataFile[i].resize(data_per_file);
			}

			string textline;
			int dataIndex = 0;
			while (dataIndex < data_per_file && getline(*(fileList[i]), textline))
			{
				vector<string> textList = TextUtil::SeperateString(textline, delimiter); // delimiter를 기준으로 string분할
				size_t tsize = textList.size();
				if (tsize != c_stringsize)
				{
					// 줄당 이미 쓰기로한 데이터 양만큼 string이 넘어오지 않은경우 예외처리
					assert(0);
					break;
				}

				for (size_t t = 0; t < tsize; ++t)
				{
					try // stoi 비정상 동작시 예외 검사위해
					{
						dataFile[i].push_back(static_cast<uint16>(stoi(textList[t]))); // 성능을 위해 push_back하지 않음
					}
					catch (...)
					{
						assert(0);
						dataFile[i][dataIndex] = 0; // 예외발생시 0값을 채워줌
					}

					++dataIndex;
				}
			}
		}

		while (dataBuffer.size() < c_buffersize)
		{
			int min_index = 0;
			int min_val = 999999; // 임의의 max값
			for (int index = 0; index < fileNum; ++index)
			{
				if (dataFile[index][0] < min_val)
				{
					min_val = dataFile[index][0];
					min_index = index;
				}
			}

			dataBuffer.push_back(min_val); // Buffer에 값넣어줌

			dataFile[min_index].pop_front();
			if (dataFile[min_index].empty()) { break; }
		}

		if (dataBuffer.size() == c_buffersize)
		{
			for (int i = 1; i < c_buffersize + 1; ++i) // 1000개씩 쉽게 끊기 위해 시작 index를 1로
			{
				// 추후 getline을 활용할때 한줄이 너무 길어지면 문제에서 제시한 메모리를 넘을수도 있다는 가정하에
				// 1000개의 데이터씩 끊어서 기록
				if ((i % c_stringsize) == 0)
				{
					outFile << dataBuffer[i - 1] << endl;
				}
				else
				{
					outFile << dataBuffer[i - 1] << delimiter;
				}
			}

			dataBuffer.clear(); // 파일에 쓰고 버퍼를 비워줌
			sortDataNum += c_buffersize; // 써진 결과개수 더해줌
		}

	}

	_ReleaseHeapMemory(fileList);

	return true;
}

bool GenSort()
{
	ifstream inFile("./hexDec.bin", ifstream::binary);
	ofstream outFile("./hexDec_GenSort.bin", std::ifstream::binary); // 결과파일 생성

	vector<uint16> dataList;

	string textline;
	while (getline(inFile, textline))
	{

		vector<string> textList = TextUtil::SeperateString(textline, delimiter); // delimiter를 기준으로 string분할
		size_t tsize = textList.size();
		if (tsize != c_stringsize)
		{
			// 줄당 이미 쓰기로한 데이터 양만큼 string이 넘어오지 않은경우 예외처리
			assert(0);
			break;
		}

		for (size_t t = 0; t < tsize; ++t)
		{
			try // stoi 비정상 동작시 예외 검사위해
			{
				dataList.push_back(static_cast<uint16>(stoi(textList[t]))); // 성능을 위해 push_back하지 않음
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
		// 추후 getline을 활용할때 한줄이 너무 길어지면 문제에서 제시한 메모리를 넘을수도 있다는 가정하에
		// 1000개의 데이터씩 끊어서 기록
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