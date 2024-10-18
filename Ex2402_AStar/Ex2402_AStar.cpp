#include "../shared/draw_helper.h"

using namespace std;
using namespace cv;
using namespace hlab;

#include <iostream>
#include <list>
#include <vector>
#include <limits>
#include <queue>
#include <iomanip>

using namespace std;

class DirectedEdge
{
public:
	int v;
	int w;
	double weight;

	DirectedEdge(int _v, int _w, double _weight)
		: v(_v), w(_w), weight(_weight)
	{}

	double Weight() { return weight; }
	int From() { return v; }
	int To() { return w; }
};

class EdgeWeightedDigraph
{
public:
	int num_vertices;
	int num_edges;
	vector<vector<DirectedEdge>> adj;

	EdgeWeightedDigraph(int _num_vertices)
		: num_vertices(_num_vertices), num_edges(0), adj(num_vertices)
	{}

	void AddEdge(DirectedEdge e)
	{
		adj[e.From()].push_back(e);
		num_edges += 1;
	}

	vector<DirectedEdge>& Adj(int v) { return adj[v]; }
};

class DijkstraShortestPaths
{
public:
	DijkstraShortestPaths(EdgeWeightedDigraph& _g, int s, vector<double> heur)
		:
		g(_g),
		prev(_g.num_vertices, -1),
		dist(_g.num_vertices, numeric_limits<double>::infinity()),
		visited(_g.num_vertices, false),
		heuristics(heur)
	{
		dist[s] = 0.0;

		pq.push(pair<double, int>{ dist[s], s }); // 초기화만 여기서 하고 Update() 반복해야 함
		// TODO: heuristics 사용
	}

	void Update() // 애니메이션 효과를 위해 한 번에 하나의 정점만 처리
	{
		if (visited[visited.size() - 1] == true) return; // 찾았을 경우 더 찾지 않음

		if (!pq.empty())
		{
			int v = pq.top().second;
			pq.pop();
			if (visited[v]) return;
			visited[v] = true;
			Relax(g, v);
		}
	}

	void Relax(EdgeWeightedDigraph& g, int v)
	{
		for (DirectedEdge& e : g.Adj(v))
		{
			int w = e.To();
			if (visited[w]) continue;
			double new_dist = dist[v] + e.Weight();
			if (dist[w] > new_dist)
			{
				dist[w] = new_dist;
				prev[w] = e.From();
				pq.push({ dist[w], w });
				// TODO: heuristics 사용
			}
		}
		// PrintDist(dist);
	}

	void PrintIndex(vector<double>& dist)
	{
		cout << "Vertex: ";
		for (int i = 0; i < dist.size(); i++)
			cout << setw(6) << i;
		cout << endl;
	}

	void PrintDist(vector<double>& dist)
	{
		cout << "Dist  : ";
		for (int i = 0; i < dist.size(); i++)
			cout << setw(6) << dist[i];
		cout << endl;
	}

	void PrintPaths()
	{
		for (int i = 0; i < prev.size(); i++)
		{
			deque<int> path;
			path.push_front(i);
			int v = prev[i];
			while (v != -1)
			{
				path.push_front(v);
				v = prev[v];
			}

			for (auto v : path) {
				cout << v;
				if (v != path.back())
					cout << " -> ";
			}
			cout << endl;
		}
	}

	deque<int> GetPath(int end)
	{
		deque<int> path;
		path.push_front(end);
		int v = prev[end];
		while (v != -1)
		{
			path.push_front(v);
			v = prev[v];
		}

		//for (auto v : path) {
		//	cout << v;
		//	if (v != path.back())
		//		cout << " -> ";
		//}
		//cout << endl;

		return path;
	}

public:
	vector<int> prev;     // 최단 경로 기록
	vector<double> dist;  // 거리 기록
	vector<bool> visited; // 방문했는지 기록
	vector<double> heuristics;

	priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
	EdgeWeightedDigraph& g;
};

struct Rectangle
{
	int x, y, width, height;

	bool IsInside(Point p)
	{
		if (p.x < x) return false;
		if (p.x >= x + width) return false;
		if (p.y < y) return false;
		if (p.y >= y + height) return false;
		return true;
	}
};

int main(int argc, char** argv)
{
	hlab::initialize(1280, 960);

	vector<Point> points;

	int s = 32;

	Rectangle obstacle1{ 300, 500, 600, 100 };
	Rectangle obstacle2{ 800, 300, 100, 400 };

	for (int j = 0; j < image.rows; j += s)
		for (int i = 0; i < image.cols; i += s)
		{
			if (obstacle1.IsInside(Point(i, j))) continue; // 물체 안으로는 들어갈 수 없음
			if (obstacle2.IsInside(Point(i, j))) continue;

			points.push_back(Point(i, j));
		}

	EdgeWeightedDigraph g(int(points.size()));

	vector<double> heur(points.size());

	for (int i = 0; i < points.size(); i++)
	{
		// Euclidean distance가 의외로 잘 안됩니다.
		// heur[i] = cv::norm(points[i] - points.back());

		// Manhattan distance
		auto diff = points[i] - points.back(); // points.back()이 목적지
		heur[i] = (abs(diff.x) + abs(diff.y)) * 1; // 뒤에 곱해지는 숫자에 따라서 결과가 달라집니다.
		//heur[i] = max(abs(diff.x), abs(diff.y)) * 10; // // Chebyshev distance

		for (int j = 0; j < points.size(); j++)
		{
			if (i == j) continue; // 자기 자신에 대해서는 간선 추가 X

			double dist = cv::norm(points[j] - points[i]);

			if (dist <= double(s) * 1.42 + 1e-6) // 실수 끼리 비교할 때는 1e-6 같이 작은 숫자를 더해서 수치 오류를 방지합니다.
			{
				g.AddEdge(DirectedEdge(i, j, dist));
			}
		}
	}

	DijkstraShortestPaths d(g, 0, heur);

	while (true)
	{
		d.Update();
		auto path = d.GetPath(int(points.size()) - 1);

		hlab::preframe();

		for (int i = 0; i < points.size(); i++)
		{
			Point p = points[i];

			if (d.visited[i])
				cv::circle(image, p, 1, Scalar(0, 0, 200), -1, LINE_AA);
			else
				cv::circle(image, p, 1, Scalar(200, 200, 200), -1, LINE_AA);
		}

		for (int i = 1; i < path.size(); i++) // 1에서 시작
		{
			cv::line(image, points[path[i - 1]], points[path[i]], Scalar(0, 0, 0), 1, LINE_AA);
		}

		if (hlab::postframe()) break;
	}

	return 0;
}
