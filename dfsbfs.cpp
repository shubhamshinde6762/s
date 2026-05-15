#include <bits/stdc++.h>
#include <omp.h>
using namespace std;

class Graph {
    int v;
    vector<vector<int>> adj;

public:
    Graph(int v)
    {
        this->v = v;
        adj.resize(v);
    }

    void addEdge(int u, int v)
    {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int node) 
    {
        vector<int> visited(v, false);
        dfsUtil(node, visited);
    }

    void dfsParallel(int node)
    {
        vector<int> visited(v, 0);
        visited[node] = 1;

        #pragma omp parallel
        {
            #pragma omp single
            {
                #pragma omp task shared(visited)
                dfsUtilParallel(node, visited);
            }
        }
    }

    void bfs(int node)
    {
        vector<int> visited(v, 0);
        bfsUtil(node, visited);
    }

    void bfsParallel(int node) 
    {
        vector<int> visited(v, 0);
        bfsUtilParallel(node, visited);
    }

private:
    void dfsUtil(int node, vector<int>& visited)
    {
        visited[node] = 1;

        for (auto nbr: adj[node])
        {
            if (!visited[nbr])
            {
                visited[nbr] = 1;
                dfsUtil(nbr, visited);
            }
        }
    }

    void dfsUtilParallel(int node, vector<int>& visited)
    {
        for (auto nbr : adj[node])
        {
            if (!visited[nbr])
            {
                bool spawn_task = false;    

                #pragma omp critical
                {
                    if (!visited[nbr])
                    {
                        visited[nbr] = 1;
                        spawn_task = true;
                    }
                }

                if (spawn_task)
                {
                    #pragma omp task shared(visited)
                    dfsUtilParallel(nbr, visited);
                }
            }
        }

        #pragma omp taskwait
    }

    void bfsUtil(int node, vector<int>& visited)
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
                if (!visited[nbr])
                {
                    visited[nbr] = 1;
                    q.push(nbr);
                }
            }
        }
    }

    void bfsUtilParallel(int node, vector<int>& visited) 
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
                int node = current[i];

                for (int nbr : adj[node])
                {
                    if (!visited[nbr])
                    {
                        #pragma omp critical
                        {
                            if (!visited[nbr])
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
    int v = 20000;
    Graph *obj = new Graph(v);

    srand(42);

    for (int i = 0; i < v; i++)
    {
        for (int j = 0; j < 150; j++)
        {
            int nbr = rand() % v;
            if (nbr != i)
            {
                obj->addEdge(i, nbr);
            }
        }
    }

    double t1, t2;
    t1 = omp_get_wtime();
    obj->dfsParallel(0);
    t2 = omp_get_wtime();  
    cout << "Time taken by parallel DFS: " << t2 - t1 << " seconds" << endl;

    t1 = omp_get_wtime();
    obj->bfsParallel(0);
    t2 = omp_get_wtime();
    cout << "Time taken by parallel BFS: " << t2 - t1 << " seconds" << endl;

    t1 = omp_get_wtime();
    obj->dfs(0);
    t2 = omp_get_wtime();
    cout << "Time taken by sequential DFS: " << t2 - t1 << " seconds" << endl;

    t1 = omp_get_wtime();
    obj->bfs(0);
    t2 = omp_get_wtime();
    cout << "Time taken by sequential BFS: " << t2 - t1 << " seconds" << endl;

    // cout <<  _OPENMP << endl;

    return 0;
}