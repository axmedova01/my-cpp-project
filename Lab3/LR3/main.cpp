#include <QCoreApplication>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const double MAX = 1e20;

void printSqrMat(double **mat, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << mat[i][j] << " ";
        }
        cout << endl;
    }
}

void bfs(int start, double **g, int n, double* res) { // breadth first search
    vector <bool> used;
        for (int i=0; i < n-1; i++){
            used.push_back(false);
        }
    queue<int> q; // создаем очередь

    fill(res, res + n, MAX); // заполняем некоторые значение по умолчанию (зачинаем заполнять с res, заканчиваем res + n, заполняем MAX)
    res[start] = 0;
    q.push(start); // добавляем в очередь start
    while (!q.empty()) { // пока очередь не пуста
        int v = q.front(); //  v = первому элементу
        q.pop(); // удаляем первый элемент
        used[v] = true;
        // обновляем длину, если она короче:
        for (int i = 0; i < n; ++i)
            if (g[v][i]) {
                res[i] = min(res[i], res[v] + g[v][i]);
                if (!used[i]) {
                    q.push(i);
                }
            }
        }
    }

double getMaxThread(int from, int to, double **g, int n) {
    double* d = new double[n];
    int path[n];
    int head;
    int threadRes = 0;

    bfs(from, g, n, d);
    // Пока найден путь
    while (d[to] < MAX) {
        head = to;
        double min_weight = MAX;

        // Восстанавливаем путь
        while (head != from) {
            for (int prev = 0; prev < n; ++prev) {
                if (prev == head || !g[prev][head]) continue;
                if (d[head] - d[prev] == g[prev][head]) {
                    path[head] = prev;
                    // Одновременно c этим выбираем самое маленькое ребро
                    min_weight = min(min_weight, (double)g[prev][head]);
                    head = prev;
                    break;
                }
            }
        }

        // Обновляем граф.
        head = to;
        while (head != from) {
            int prev = path[head];
            g[prev][head] -= min_weight;
            g[head][prev] += min_weight;
            head = prev;
        }

        threadRes += min_weight;
        bfs(from, g, n, d);
    }
    delete [] d;
    return threadRes;
}

int main() {
    srand(time(NULL));
    int n;
    cout <<"enter the number of vertices n:";
    cin >> n;

    double** g = new double*[n];
    for (int i = 0; i < n; ++i) {
        g[i] = new double[n]{};
    }

    for (int i = 0; i < n - 1; ++i) {
        for (int j = 1; i + j < n; ++j) {
            g[i][i+j] = rand() % 10;
            g[i+j][i] = g[i][i+j];
        }
    }

    printSqrMat(g, n);
    cout << getMaxThread(0, n-1, g, n) << endl;
}
