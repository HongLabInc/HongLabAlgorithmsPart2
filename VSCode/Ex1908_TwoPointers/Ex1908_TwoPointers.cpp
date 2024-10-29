#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

// 참고: 
// https://www.geeksforgeeks.org/two-pointers-technique/
// https://leetcode.com/problems/two-sum/description/

int main()
{
	vector<int> arr = { 1, 2, 5, 7, 8, 9, 10, 11, 12 };

	int target = 16;

	sort(arr.begin(), arr.end()); // 정렬을 하고 시작합니다.

	int i = 0;                    // 한 인덱스는 맨 앞에서 시작합니다.
	int j = int(arr.size()) - 1;  // 다른 인덱스는 맨 뒤에서 시작합니다.

	// 인덱스도 특정 메모리 위치를 가리킨다는 관점에서 포인터로 볼 수 있습니다.

	while (i < j)
	{
		// cout << i << " " << j << endl;

		if (arr[i] + arr[j] == target)
		{
			cout << arr[i] << " + " << arr[j] << " = " << target << endl;
			return 0;
		}
		//else if (arr[i] + arr[j] < target)
		//	TODO:
		//else // if (arr[i] + arr[j] > target)
		//	TODO:

		break; // 무한 반복 방지 (TODO: 삭제하세요)
	}

	return 0;
}
