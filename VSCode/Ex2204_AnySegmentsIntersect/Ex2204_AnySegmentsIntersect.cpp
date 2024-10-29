#include "../shared/draw_helper.h"

#include <set>

using namespace std;
using namespace cv;
using namespace hlab;

// 참고:
// - CLRS 3rd Edition p. 1025 ANY-SEGMENT-INTERSECT(S)
// - https://www.geeksforgeeks.org/given-a-set-of-line-segments-find-if-any-two-segments-intersect/?ref=lbp

// 조건
// - 수직선은 없음
// - 세 직선이 한 점에서 만나지 않음 (만난다면 두 직선만 한 점에서)

// 포인트
// - 바로 위나 바로 아래의 직선을 닿지 않고 더 위나 더 아래의 직선과 만날 수는 없다.
// - 주어진 선분들의 모든 쌍에 대해 교차 확인을 하는 것은 느리기 때문에, 정렬을 통해 가까운 선분들끼리만 비교
// - 새로 추가를 할 때 선분과 점을 비교하면서 위치를 결정
// - 개념은 그리 어렵지 않은데, 구현이 복잡

struct Segment; // Segment의 전방 선언(forward declaration)
// 참고: 전방선언
// - Segment을 "정의(define)"하기 전에 Segment라는 구조체가 있다는 것을 미리 알려주는(선언) 행위
// - 바로 아래 EndPoint에서는 Segment가 정의되기 전에 선언만 보고 사용하고 있음
// - 진짜 정의(definition)는 아래에 있음

struct EndPoint : public Point // 편의상 상속 사용
{
	Segment* segment = nullptr; // 어떤 선분에 속해있는지 포인터를 가지고 있음. Segment의 전방선언이 위에 있기 때문에 가능
	bool is_left = false;  // 선분에서 왼쪽점이면 true
	int order = 0; // 정렬 후 가시화/디버깅 용도
	bool is_event = false; // 이 점을 기준으로 다른 segment와 비교

	EndPoint(Point _p)
		: Point(_p), segment(nullptr), is_left(false), order(0), is_event(false)
	{}

	bool operator < (const EndPoint& p) const
	{
		if (x == p.x)
			return y < p.y;
		else
			return x < p.x;
	}
};

struct Segment // 위에 전방선언되었던 struct Segment의 진짜 정의
{
	EndPoint* left = nullptr;
	EndPoint* right = nullptr;
	string name; // 가시화/디버깅용

	bool intersect = false;   // 랜더링할때 사용

	Segment(EndPoint* p1, EndPoint* p2, string name)
		: left(p1), right(p2), intersect(false), name(name)
	{
		Reset();
	}

	void Reset() // 마우스 조작으로 변경되었을 때도 사용
	{
		if (*right < *left) swap(left, right); // 왼쪽점과 오른쪽점 순서가 바뀌었을 경우 교체

		left->is_left = true;
		right->is_left = false;

		left->segment = this;   // 점들이 어떤 선분에 속해있는지 포인터 저장
		right->segment = this;  // 점들이 어떤 선분에 속해있는지 포인터 저장

		intersect = false;
	}
};

int Direction(Point p0, Point p2, Point p1)
{
	return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

bool OnSegment(Point pi, Point pj, Point pk)
{
	if (std::min(pi.x, pj.x) <= pk.x && pk.x <= std::max(pi.x, pj.x) && min(pi.y, pj.y) <= pk.y && std::max(pi.y, pj.y)) return true;
	else return false;
}

bool SegmentsIntersect(Point p1, Point p2, Point p3, Point p4)
{
	int d1 = Direction(p3, p4, p1);
	int d2 = Direction(p3, p4, p2);
	int d3 = Direction(p1, p2, p3);
	int d4 = Direction(p1, p2, p4);

	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 and d4 > 0))) return true;
	else if (d1 == 0 && OnSegment(p3, p4, p1)) return true;
	else if (d2 == 0 && OnSegment(p3, p4, p2)) return true;
	else if (d3 == 0 && OnSegment(p1, p2, p3)) return true;
	else if (d4 == 0 && OnSegment(p1, p2, p4)) return true;
	else return false;
}

void DrawArrow(cv::Mat& image, Segment& segment, Scalar color)
{
	auto line_start = segment.left;
	auto line_end = segment.right;

	if (selected)
		cv::circle(image, *selected, 17, Scalar(0, 255, 0), 1, LINE_AA);

	cv::circle(image, *line_start, 5, color, -1, LINE_AA);
	cv::arrowedLine(image, *line_start, *line_end, color, 1, LINE_AA);

	cv::putText(image, to_string(line_start->order), *line_start, cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(30, 30, 30), 1, LINE_AA, true);
	cv::putText(image, to_string(line_end->order), *line_end, cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(30, 30, 30), 1, LINE_AA, true);
	cv::putText(image, segment.name, (*line_start + *line_end) / 2, cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(30, 30, 30), 1, LINE_AA, true);
}

int main(int argc, char** argv)
{
	hlab::initialize(1280, 960);

	vector<EndPoint> points = { Point(1, 5), Point(4, 5), Point(2, 5), Point(10, 1),
								Point(3, 2), Point(10, 3), Point(6, 4), Point(9, 4),
								Point(7, 1), Point(8, 1) };

	for (auto& p : points) // 화면 가운데로 이동
	{
		p *= 100;
		p.x += 50;
		p.y += 200;
	}

	vector<Segment> segments = { {&points[0], &points[1], "A"}, {&points[2], &points[3], "B"}, {&points[4], &points[5], "C"},
						  {&points[6], &points[7], "D"},	{&points[8], &points[9], "E"} };

	std::vector<EndPoint*> point_pointers; // 점들의 포인터만 저장, 정렬할 때 사용, 정렬해도 메모리 주소가 바뀌지 않도록

	for (auto& p : points)
		point_pointers.push_back(&p);

	for (auto& p : points)
		movable_points.push_back(&p);

	hlab::update_flag = true; // 처음에 한 번은 마우스 이벤트와 상관 없이 업데이트

	while (true)
	{
		hlab::preframe();

		if (selected)
			cv::circle(image, *selected, 17, Scalar(0, 255, 0), 1, LINE_AA);

		if (hlab::update_flag) // 마우스 드래그가 새로 발생하면 업데이트
		{
			for (Segment& s : segments) s.Reset();

			std::sort(point_pointers.begin(), point_pointers.end(), [](EndPoint* p1, EndPoint* p2) {return *p1 < *p2; });

			for (int i = 0; i < point_pointers.size(); i++)
				point_pointers[i]->order = i;

			hlab::update_flag = false;

			auto cmp = [](Segment* s1, Segment* s2) -> bool
				{
					if (s1 == s2) return false; // !comp(a, b) && !comp(b, a)일 경우 a == b라고 판단하기 때문
					// 참고: https://en.cppreference.com/w/cpp/container/set

					assert(s1->left->is_event + s1->right->is_event + s2->left->is_event + s2->right->is_event == 1);

					//cout << "Compare " << s1->name << " " << s2->name << " ";

					if (s1->left->is_event)
					{
						//cout << (Direction(*s2->left, *s2->right, *s1->left) <= 0) << endl;

						return Direction(*s2->left, *s2->right, *s1->left) > 0;
					}
					else if (s1->right->is_event)
					{
						//cout << (Direction(*s2->left, *s2->right, *s1->right) <= 0) << endl;

						return Direction(*s2->left, *s2->right, *s1->right) > 0;
					}
					else if (s2->left->is_event)
					{
						//cout << (Direction(*s1->left, *s1->right, *s2->left) > 0) << endl;

						return Direction(*s1->left, *s1->right, *s2->left) <= 0;
					}
					else // if (s2->right->is_event)
					{
						//cout << (Direction(*s1->left, *s1->right, *s2->right) > 0) << endl;

						return Direction(*s1->left, *s1->right, *s2->right) <= 0;
					}

					// Direction() == 0인 경우는 여기서 고려할 필요 없음
					// 교차했다면 여기까지 올 필요가 없기 때문
				};

			std::set<Segment*, decltype(cmp)> T(cmp); // segment의 포인터를 저장하는 레드블랙트리
			// T에 insert를할때 새로운 left와 기존의 segment의 cross product를 이용해서 비교를 구현

			auto Above = [&](decltype(T.begin()) itr) -> Segment*
				{
					if (itr == T.end()) return nullptr;

					auto temp = std::next(itr);
					if (temp == T.end())
						return nullptr;
					else
						return *temp;
				};

			auto Below = [&](decltype(T.begin()) itr) -> Segment*
				{
					if (itr == T.begin()) return nullptr;
					auto temp = std::prev(itr);
					if (itr == T.end()) return nullptr;
					return *temp;
				};

			for (EndPoint* p : point_pointers)
			{
				//cout << p->order << endl;
				//for (auto& s : T)
				//	cout << s->name << " ";  // <- set안에 어떤 순서로 저장되어 있는지 확인 가능, 먼저 출력되는 선분이 아래 방향
				//cout << endl;

				if (p->is_left)
				{
					p->segment->left->is_event = true;

					T.insert(p->segment);
					auto itr = T.find(p->segment);
					Segment* above = Above(itr);
					Segment* below = Below(itr);

					//if (TODO: )
					//{
					//	above->intersect = true;
					//	p->segment->intersect = true;
					//	p->segment->left->is_event = false;
					//	break;
					//}

					//if (TODO: )
					//{
					//	below->intersect = true;
					//	p->segment->intersect = true;
					//	p->segment->left->is_event = false;
					//	break;
					//}

					p->segment->left->is_event = false;
				}
				else
				{
					break; // TODO: 삭제, 런타임 오류 방지용

					p->segment->right->is_event = true;

					//assert(T.count(p->segment) > 0);

					auto itr = T.find(p->segment); // left에 의해 segment가 이미 T에 들어 있어야 함

					assert(itr != T.end());

					Segment* above = Above(itr);
					Segment* below = Below(itr);

					//if (TODO: )
					//{
					//	above->intersect = true;
					//	below->intersect = true;
					//	p->segment->right->is_event = false;
					//	break;
					//}

					//cout << "erase " << (*itr)->name << endl;
					T.erase(itr);

					p->segment->right->is_event = false;
				}
			}
		}

		for (Segment& s : segments)
		{
			if (s.intersect)
				DrawArrow(image, s, kRed);
			else
				DrawArrow(image, s, kBlack);
		}

		if (hlab::postframe()) break;
	}

	return 0;
}
