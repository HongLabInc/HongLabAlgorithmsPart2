#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

// 참고
// https://www.geeksforgeeks.org/window-sliding-technique/

int MaxSubSum(vector<int> arr, int k)
{
	int n = int(arr.size());

	return 0; // TODO
}

int main()
{
	vector<int> arr = { 1, 4, 2, 8, 2, 3, 1, 0, 9 };
	int k = 4; // 윈도우 크기 (배열 크기 이하)

	// 문제 설명을 위한 출력
	for (int i = 0; i < arr.size(); i++)
		cout << arr[i];
	cout << endl;
	for (int i = 0; i <= arr.size() - k; i++)
	{
		for (int j = 0; j < i; j++)
			cout << " ";
		int sum = 0;
		for (int j = i; j < i + k; j++)
		{
			sum += arr[j];
			cout << arr[j];
		}
		cout << " sum = " << sum << endl;
	}

	// 슬라이딩 윈도우 실행 결과 출력
	cout << MaxSubSum(arr, k);

	return 0;
}