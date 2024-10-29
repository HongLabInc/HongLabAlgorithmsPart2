#include <iostream>
#include <string>
using namespace std;

// Sedgewick p. 761를 약간 변형하였습니다.
// NaiveStringMatch() 처럼 매번 다시 검색하는 방식인데 구조가 살짝 다릅니다.
// 뒤에 나오는 KMP 이해를 돕기 위한 것입니다.
void ExplicitBackupStringSearch(string& pat, string& txt)
{
	int M = int(pat.size());
	int N = int(txt.size());

	cout << txt << " " << pat << endl;
	for (int i = 0; i < N; i++)
		cout << i % 10;
	cout << endl;

	// i는 txt에 대한 인덱스, j는 pat에 대한 인덱스
	int i = 0;
	int j = 0;
	while (i < N)
	{
		// cout << i << " " << j << endl;

		if (txt.at(i) == pat.at(j)) // 글자가 일치하면 i와 j 함께 전진
		{
			// TODO: i 업데이트
			// TODO: j 업데이트
		}
		else // 그렇지 않으면 i와 j 둘 다 후퇴
		{
			// TODO: i 업데이트
			j = 0; // Brute-Force 방식입니다. 더 효율적인 방식은 뒤에 KMP에서 사용합니다.
		}

		if (j == M) // M 글자가 매칭 되었을 경우
		{
			for (int s = 0; s < i - M; s++)
				cout << " ";
			cout << pat << " " << i - M << endl;

			// i와 j 둘 다 다음 매칭을 위해 후퇴 

			// TODO: i 업데이트
			j = 0; // Brute-Force 방식입니다. 더 효율적인 방식은 뒤에 KMP에서 사용합니다.
		}

		break; // TODO: 무한루프 방지, 삭제
	}

	cout << endl;
}

int main()
{
	string txt1 = "acaabc";
	string pat1 = "aab";
	ExplicitBackupStringSearch(pat1, txt1);

	string txt2 = "AABAACAADAABAABA";
	string pat2 = "AABA";
	ExplicitBackupStringSearch(pat2, txt2);

	string txt3 = "ababababcababababab";
	string pat3 = "abab";
	ExplicitBackupStringSearch(pat3, txt3);

	return 0;
}
