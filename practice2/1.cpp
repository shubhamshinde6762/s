#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Graph {
    vector<vector<int>> adj;
    int n;

public:
    Graph(int n)
    {
        adj.resize(n);
        this->n = n;
    }

    void dfs(int node)
    {
        vector<int> visited(n);
        dfs(node, visited);
    }

    void dfs(int node, vector<int> &visited)
    {
        visited[node] = 1;

        for (auto &nbr : adj[node])
        {
            if (visited[nbr] != 1)
            {
                visited[nbr] = 1;
                dfs(nbr, visited);
            }
        }
    }

    void dfsParallel(int node)
    {
        vector<int> visited(n);
        visited[node] = 1;
        
        #pragma omp parallel
        {
            #pragma omp single
            {
                #pragma omp task shared(visited)
                dfsParallel(node, visited);
            }
        }
    }

    void dfsParallel(int node, vector<int> &visited)
    {
        for (auto &nbr : adj[node])
        {
            if (visited[nbr] != 1)
            {
                bool isSpawn = 0;

                #pragma omp critical
                {
                    if (visited[nbr] != 1)
                    {
                        visited[nbr] = 1;
                        isSpawn = true;
                    }
                }

                if (isSpawn)
                {
                    #pragma omp task shared(visited)
                    dfsParallel(nbr, visited);
                }
            }
        }

        #pragma omp taskwait
    }

    void bfs(int node)
    {
        vector<int> visited(n);
        bfs(node, visited);
    }

    void bfs(int node, vector<int>& visited)
    {
        queue<int> q;
        q.push(node);
        visited[node] = 1;

        while(!q.empty())
        {
            auto front = q.front();
            q.pop();

            for (auto &nbr : adj[front])
            {
                if (visited[nbr] != 1)
                {
                    visited[nbr] = 1;
                    q.push(nbr);
                }
            }
        }
    }

    void bfsParallel(int node)
    {
        vector<int> visited(n);
        bfsParallel(node, visited);
    }

    void bfsParallel(int node, vector<int> &visited)
    {
        vector<int> current;
        current.push_back(node);
        visited[node] = 1;

        while(!current.empty())
        {
            vector<int> next;

            #pragma omp parallel for 
            for (int i = 0; i < current.size(); i++)
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

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
};

int main()
{
    int n = 20000;

    Graph *g = new Graph(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < 150; j++)
        {
            int x = rand() % n;
            if (i != x)
                g->addEdge(i, x);
        }
    }

    double t1, t2;
    t1 = omp_get_wtime();
    g->dfs(0);
    t2 = omp_get_wtime();
    cout << "dfs s " << t2 - t1 << endl;

    t1 = omp_get_wtime();
    g->dfsParallel(0);
    t2 = omp_get_wtime();
    cout << "dfs p " << t2 - t1 << endl;

    t1 = omp_get_wtime();
    g->bfs(0);
    t2 = omp_get_wtime();
    cout << "bfs s " << t2 - t1 << endl;

    t1 = omp_get_wtime();
    g->bfsParallel(0);
    t2 = omp_get_wtime();
    cout << "bfs p " << t2 - t1 << endl;
}