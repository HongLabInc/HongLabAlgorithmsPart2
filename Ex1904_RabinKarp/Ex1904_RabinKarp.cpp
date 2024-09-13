#include <string>
#include <vector>
#include <iostream>
using namespace std;

void RabinKarpMatcher(string pat, string txt, int d, int q)
{
	int M = int(pat.size());
	int N = int(txt.size());

	// 자주 사용되는 숫자를 미리 계산
	// 여기서는 (((128 % q) * 128 % q) * 128 % q) * 128 % q 
	int h = 1;
	for (int i = 0; i < M - 1; i++)
		h = (h * d) % q;

	int p = 0; // 찾으려는 패턴의 해시값
	int t = 0; // 대상 문자열의 앞부분 해시값
	for (int i = 0; i < M; i++)
	{
		//p = TODO:
		//t = TODO:
	}

	cout << "Pattern hash " << p << endl;

	for (int i = 0; i <= N - M; i++)
	{
		cout << string(i, ' ') + txt.substr(i, M) << " " << t << " at " << i << endl;

		if (p == t) // 해시값이 일치하면 한 글자씩 비교해서 확인
		{
			int j;
			for (j = 0; j < M; j++)
				if (txt[i + j] != pat[j])
					break;

			if (j == M)
				cout << "Pattern found at index " << i << endl;
			else
				cout << "Spurious hit " << i << endl;
		}

		if (i < N - M)
		{
			// TODO:
		}
	}
}

int main()
{
	string txt = "ABCDCDEDABABCDEBABCDEDA";
	string pat = "ABCD";

	int d = 128; // ASCII 코드의 가짓수

	// q는 d*q를 작게 만들 적당한 소수(prime number)
	int q = 997; // 이 숫자가 작으면 surous hit이 자주 발생, 해시 충돌과 비슷한 상황

	// 문제 설명을 위한 출력
	{
		int M = int(pat.size());
		int N = int(txt.size());
		cout << txt << endl;

		for (int i = 0; i <= N - M; i++)
		{
			// 해시값을 하나씩 더해서 계산
			int t = 0;
			for (int j = 0; j < M; j++)
				t = (d * t + txt[i + j]) % q;

			cout << string(i, ' ') + txt.substr(i, M) << " " << t << " at " << i << endl;
		}

		cout << endl;
	}

	RabinKarpMatcher(pat, txt, d, q);

	return 0;
}

