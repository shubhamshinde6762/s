#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Graph
{
    vector<vector<int>> adj;
    int n;

public:
    Graph(int n) : n(n)
    {
        adj.resize(n);
    }

    void addEdge(int u, int v)
    {
        if (u < 0 || v < 0 || u >= n || v >= n)
            return;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfsSequential(int start)
    {
        vector<int> visited(n);
        dfsSequentialHelper(start, visited);
    }

    void bfsSequqntial(int start)
    {
        vector<int> visited(n);
        bfsSequentialHelper(start, visited);
    }

    void dfsParallel(int start)
    {
        vector<int> visited(n);
        visited[start] = 1;

        #pragma omp parallel
        {
            #pragma omp single
            {
                #pragma omp task shared(visited)
                dfsParallelHelper(start, visited);
            }
        }
    }

    void bfsParallel(int start)
    {
        vector<int> visited(n);
        bfsParallelHelper(start, visited);
    }

private:
    void dfsSequentialHelper(int node, vector<int> &visited)
    {
        visited[node] = 1;

        for (auto &nbr : adj[node])
        {
            if (visited[nbr] != 1)
            {
                dfsSequentialHelper(nbr, visited);
            }
        }
    }

    void bfsSequentialHelper(int node, vector<int> &visited)
    {
        queue<int> q;
        q.push(node);

        visited[node] = 1;

        while (!q.empty())
        {
            auto frontNode = q.front();
            q.pop();

            for (auto nbr : adj[frontNode])
            {
                if (visited[nbr] != 1)
                {
                    q.push(nbr);
                    visited[nbr] = 1;
                }
            }
        }
    }

    void dfsParallelHelper(int node, vector<int> &visited)
    {
        for (auto &nbr : adj[node])
        {
            if (visited[nbr] != 0)
            {
                bool spawnTask = 0;

                #pragma omp critical
                {
                    if (visited[nbr] != 1)
                    {
                        visited[nbr] = 1;
                        spawnTask = 1;
                    }
                }

                if (spawnTask)
                {
                    #pragma omp task shared(visited)
                    dfsParallelHelper(nbr, visited);
                }
            }
        }
    }

    void bfsParallelHelper(int node, vector<int> &visited)
    {
        vector<int> current;
        current.push_back(node);

        visited[node] = 1;

        while(!current.empty())
        {
            vector<int> next;
            int size = current.size();

            #pragma omp parallel for
            for (int i = 0; i < size; i++)
            {
                auto node = current[i];

                for (auto &nbr : adj[node])
                {
                    if (visited[nbr] != 1)
                    {
                        #pragma omp critical
                        {
                            if (visited[nbr] != 1)
                            {
                                visited[nbr] = 1;
                                next.push_back(nbr);
                            }
                        }
                    }
                }
            }

            current = next;
        }
    }
};

int main()
{
    int n = 20000;
    
    Graph *g = new Graph(n);

    for (int i = 0; i < n; i++)
    {
        for (int temp = 0; temp < 150; temp++)
        {
            int j = rand() % n;
            if (i != j)
                g->addEdge(i, j);
        }
    }


    double t1, t2;
    t1 = omp_get_wtime();
    g->dfsSequential(0);
    t2 = omp_get_wtime();
    cout << "dfs seq " << t2 - t1 << "\n"; 

    t1 = omp_get_wtime();
    g->dfsParallel(0);
    t2 = omp_get_wtime();
    cout << "dfs parallel" << t2 - t1 << "\n"; 

    t1 = omp_get_wtime();
    g->bfsSequqntial(0);
    t2 = omp_get_wtime();
    cout << "bfs seq " << t2 - t1 << "\n"; 

    t1 = omp_get_wtime();
    g->bfsParallel(0);
    t2 = omp_get_wtime();
    cout << "bfs parallel " << t2 - t1 << "\n"; 
}