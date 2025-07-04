#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <set>
#include <cstring> // strcmp()
using namespace std;

class SuffixArray
{
public:
	string txt;
	vector<int> sa;

	SuffixArray(string txt)
		: txt(txt)
	{
		BuildSuffixArray();
	}

	void BuildSuffixArray()
	{
		sa.resize(txt.size()); // suffix array

		for (int i = 0; i < txt.size(); i++)
			sa[i] = i;

		cout << "Before sorting" << endl;
		for (int i = 0; i < sa.size(); i++)
			cout << setw(3) << i << " : " << setw(3) << sa[i] << " : " << txt.substr(sa[i], sa.size() - sa[i]) << endl;
		cout << endl;

		sort(sa.begin(), sa.end(),
			[&](int a, int b) {
				return false; // TODO: strcmp() 사용
			}
		);

		cout << "After sorting" << endl;
		for (int i = 0; i < sa.size(); i++)
			cout << setw(3) << i << " : " << setw(3) << sa[i] << " : " << txt.substr(sa[i], sa.size() - sa[i]) << endl;
		cout << endl << endl;
	}

	void Search(string pat)
	{
		for (int i = 0; i < txt.size(); i++)
			cout << i % 10;
		cout << endl << txt << endl;

		int l = 0, r = int(sa.size() - 1);
		while (l <= r)
		{
			int mid = l + (r - l) / 2;

			int compare = strncmp(&txt.c_str()[sa[mid]], pat.c_str(), pat.size()); // 힌트로 사용하세요

			// std::strcmp https://en.cppreference.com/w/cpp/string/byte/strcmp

			if (compare == 0)
			{
				// 매칭되는 순서대로 출력하기 위해 set에 저장해서 정렬했다가 출력
				set<int> matched;
				matched.insert(sa[mid]);

				// TODO: 

				for (int index : matched)
					cout << string(index, ' ') << pat << " " << index << endl;
				cout << endl;

				return;
			}

			// TODO: mid 변경
			return; // TODO: 제거
		}

		cout << "Not found" << endl;
	}
};

int main()
{
	//  C/C++ 문자열 복습 (string::c_str(), strcmp(), strncmp() 3가지)
	//{
	//	string str1("abc");
	//	string str2("abx");

	//	// 1. string::c_str()
	//	const char* c_str1 = str1.c_str(); // c언어 스타일의 문자열 (마지막에 널캐릭터 포함)
	//	const char* c_str2 = str2.c_str(); // c언어 스타일의 문자열 (마지막에 널캐릭터 포함)

	//	cout << c_str1 << endl; // "abc"
	//	cout << c_str2 << endl; // "abx"

	//	// 2. strcmp(c_str1, c_str2): 두 문자열의 길이와 글자가 같을 경우 0, str1이 앞설 경우 -1, str2가 앞설 경우 1 반환
	//	cout << strcmp(str1.c_str(), str2.c_str()) << endl; // -1
	//	cout << strcmp("ab", "abc") << endl; // -1
	//	cout << strcmp("abc", "ab") << endl; // 1
	//	cout << strcmp("ab", "ab") << endl;  // 0
	// 
	// // 3. strncmp(c_str1, c_str2, n): strcmp를 글자 n개만 수행
	// // cout << strncmp("abc", "ab", 2) << endl; // 0, strcmp()에서는 1
	//}

	{
		SuffixArray s("abcdefg");
	}

	{
		SuffixArray s("gfedcba");
	}

	{
		SuffixArray s("banana");
		s.Search("na");
	}

	{
		SuffixArray s("aaaaaaaaaa");
		s.Search("aaa");
	}

	{
		SuffixArray s("abababcabababca");
		s.Search("abababca");
	}

	{
		SuffixArray s("AABAACAADAABAABA");
		s.Search("AABA");
	}

	{
		SuffixArray s("ababababcababababab");
		s.Search("abab");
	}

	return 0;
}
