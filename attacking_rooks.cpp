#include <iostream>

using namespace std;

#include <vector>
#include <deque>

namespace dinics {

constexpr int noparent = -1;

/**
 * Algoritmo de Dinitz (llamado usualmente dinics).  Observaciones:
 * - los nodos se numeran de 0 a n-1.  En caso en que se numeren de 1 a n, simplemente generar un flowgraph con n+1 vertices
 *   sin conectar nada al vertice 0.
 *
 * COMPLEJIDAD
 * - Este algoritmo tiene complejidad O(n^2m), lo cual es mas que suficiente para competencias (salvo push relabel para grafos densos).
 * - En el caso en que todas las capacidades sean 1, cuesta O(min(m^1/2, n^2/3)m) lo cual es O(n^{2/3}m) para caso denso O(m^{3/2}) para ralos .
 * - Cuando el grafo es bipartito cuesta O(sqrt(n)m), lo cual matchea con el que se usa en competencia (de Hopcroft y Karp) y es
 * mejor que el de Berge.
 *
 * NOTAS:
 * Esta implementacion es basica, separada en subtareas, sin mezclar nada para mostrar una implementacion casi directa.
 * (ver dinics-fast.cpp)
 */
class flowgraph {
    struct edge {
        long long from,         //vertice del que parte la arista; solo por simetria y para simplificar, se puede sacar
                  to,           //vertice al que llega la arista
                  capacity,     //capacidad de la arista
                  flow,         //flujo de la arista, el residual es capacity - flow
                  reverse;      //indice a la arista reversa en el grafo residual
    };
    template<class T>
    using digraph = vector<vector<T>>;

    digraph<edge> residual;

    inline edge& reverse(const edge& e) {
        return residual[e.to][e.reverse];
    }


    /**
     * Computa el grafo de niveles usando BFS a partir del residual.
     * Retorna un grafo donde por cada vertice se tienen los indices de los vecinos
     * que pertenecen al level graph en el grafo residual.
     */
    digraph<int> find_level_graph(int source) {
        digraph<int> level_graph(residual.size());
        vector<int> level(residual.size(), noparent);
        deque<int> q(1, source);
        level[source] = 0;

        while(not q.empty()) {
            auto v = q.front(); q.pop_front();
            for(int idx = 0; idx < residual[v].size(); ++idx) {
                auto& e = residual[v][idx];
                if(e.flow == e.capacity) continue;
                if(level[e.to] == noparent) {
                    q.push_back(e.to);
                    level[e.to] = level[v] + 1;
                }
                if(level[e.to] > level[v]) level_graph[v].push_back(idx);
            }
        }
        return level_graph;
    }

    /**
     * Aplica DFS para encontrar un augementing path mientras se busca el blocking flow.
     * Retorna el valor del augmenting path, que es valido cuando dicho valor es mayor a 0.
     * En parent e index deja anotada la arista con la que se llega a cada vertice hasta sink
     * Anula los dead-ends del level_graph cuando los encuentra.
     */
    long long find_augmenting_path(digraph<int>* level_graph, int from, int sink, vector<int>* parent, vector<int>* index) {
        while(not level_graph->at(from).empty()) {
            auto& e = residual[from][level_graph->at(from).back()];
            auto success = e.capacity-e.flow;
            if(success > 0 && e.to != sink) success = min(success, find_augmenting_path(level_graph, e.to, sink, parent, index));
            if(success == 0) {
                //arista saturada! o dead end!
                level_graph->at(from).pop_back();
                continue;
            }
            //camino encontrado.  Guardamos la informacion y retornamos el flujo
            parent->at(e.to) = e.from;
            index->at(e.to) = level_graph->at(from).back();
            return min(success, e.capacity - e.flow);
        }
        //no habia augmenting path
        return 0;
    }

    /**
     * Busca iterativamente los augmenting paths, aumentandolos hasta tener un blocking flow.  Retorna
     * el valor del flujo aumentado.
     * Requiere: que ninguna arista este en el level graph, ya que se calcula en esta etapa.
     */
    long long find_blocking_flow(int source, int sink) {
        auto level_graph = find_level_graph(source);

        vector<int> parent(residual.size(), noparent);
        vector<int> index(residual.size(), 0);
        long long ans=0, volume;

        while((volume = find_augmenting_path(&level_graph, source, sink, &parent, &index)) > 0) {
            for(int to = sink; parent[to] != noparent; to = parent[to]) {
                auto& e = residual[parent[to]][index[to]];
                e.flow += volume;
                reverse(e).flow -= volume;
            }
            ans += volume;
        }
        return ans;
    }


public:
    flowgraph(int n) {
        residual.assign(n, vector<edge>());
    }

    void add_edge(int from, int to, long long capacity) {
        if(capacity <= 0) return;
        auto idxfrom = (int)residual[from].size(), idxto = (int)residual[to].size();
        residual[from].push_back(edge{from, to, capacity, 0, idxto});
        residual[to].push_back(edge{to, from, 0, 0, idxfrom});
    }

    /**
     * Carga en this el flujo maximo de source a sink.  Notar que this podria
     * tener un flujo precargado y lo modifica para tener el flujo maximo.
     * Retorna todo el flujo que se pudo agregar.
     */
    long long maxflow(int source, int sink) {
        long long res = 0, step;
        while((step = find_blocking_flow(source, sink)) > 0) {
            res += step;
        }
        return res;
    }

    void print(ostream& out) {
        for(int f = 0; f < residual.size(); ++f) {
            cout << f << ": ";
            for(auto e : residual[f]) {
                auto& rev = reverse(e);
                out << "(" << e.from << "," << e.to << "," << e.capacity << "," << e.flow << ") {"
                    << rev.from << "," << rev.to << ","  << rev.capacity << "," << rev.flow << "}  -  ";
            }
            cout << endl;
        }
    }
};
}

ostream& operator<<(ostream& os, dinics::flowgraph G) {
    G.print(os);
    return os;
}

typedef vector<int> vi;

int main() {
    ios::sync_with_stdio(0); cin.tie(0);

    string line;
    char cell;
    while (true) {
        cin >> line;
        if (line.empty()) break;

        int n = stoi(line);

        vector<vector<bool>> board(n, vector<bool>(n));
        vi rows_cap(n); vi cols_cap(n);

        // Build matrix
        for (int r = 0; r < n; ++r) // per row
            for (int c = 0; c < n; ++c) { // per col
                cin >> cell;
                board[r][c] = cell != 'X';
            }

        // Get info for building graph
        bool has_space;
        for (int r = 0; r < n; ++r)
            for (int c = 0; c < n; ++c) {
                has_space = board[r][c];
                if (!has_space) {
                    if (r > 0 && r < n-1 && board[r-1][c]) cols_cap[c]++;
                    if (c > 0 && c < n-1 && board[r][c-1]) rows_cap[r]++;
                } else {
                    if (cols_cap[c] == 0) cols_cap[c]++;
                    if (rows_cap[r] == 0) rows_cap[r]++;
                }
            }

        // Build graph with the info
        int cells = n*2+2; // rows, cols and source/sink
        dinics::flowgraph graph = dinics::flowgraph(cells);
        for (int r = 0; r < n; ++r) {
            graph.add_edge(cells-2, r, rows_cap[r]); // add from source to row
            for (int c = 0; c < n; ++c) { // add from row to columns
                if (board[r][c]) graph.add_edge(r, c+n, 1);
            }
        }
        for (int c = 0; c < n; ++c)
            graph.add_edge(cells-1, c+n, cols_cap[c]); // add from column to sink
        long long max_flow = graph.maxflow(cells-2, cells-1);
    }
}