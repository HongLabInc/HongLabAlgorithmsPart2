#pragma once

#include <queue>
#include <iomanip>

namespace hlab
{
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
		int num_vertices = 0;
		int num_edges = 0;
		vector<vector<DirectedEdge>> adj;

		EdgeWeightedDigraph()
		{}

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
		DijkstraShortestPaths()
		{}

		DijkstraShortestPaths(EdgeWeightedDigraph& _g, int start_vertex, int end_vertex, vector<double> heur)
		{
			Initialize(_g, start_vertex, end_vertex, heur);
		}

		void Initialize(EdgeWeightedDigraph& _g, int start_vertex, int _end_vertex, vector<double> heur)
		{
			g = _g;
			prev = vector<int>(_g.num_vertices, -1);
			dist = vector<double>(_g.num_vertices, numeric_limits<double>::infinity());
			visited = vector<bool>(_g.num_vertices, false);
			heuristics = heur;

			dist[start_vertex] = 0.0;
			pq = decltype(pq)(); // pq = priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>>(); // 초기화
			pq.push(pair<double, int>{ 0.0 + heuristics[start_vertex], start_vertex }); // 초기화만 여기서 하고 실제 진행은 Update() 호출할 때마다 한 단계씩 진행

			end_vertex = _end_vertex;
		}

		void Update() // 애니메이션 효과를 위해 한 번에 하나의 정점만 처리
		{
			if (visited[end_vertex] == true) return; // end_vertex까지 찾았을 경우 더 찾지 않음

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
					pq.push({ dist[w] + heuristics[w], w });
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

		deque<int> GetPath()
		{
			deque<int> path;
			path.push_front(end_vertex);
			int v = prev[end_vertex];
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
		int end_vertex = -1;

		priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
		EdgeWeightedDigraph g;
	};
}