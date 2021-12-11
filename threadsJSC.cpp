#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <thread>
#include <vector>

void suma(double *v, int init, int end, double &result);

int main(int argc, char **argv) {
  int N = std::atoi(argv[1]);
  int NTH = std::atoi(argv[2]);
  std::vector<double> res(NTH, 0.0);
  double *data = new double[N]{0.0};
  std::iota(data, data + N, 1.0);
  std::vector<std::thread> ths;
  int localsize = N / NTH;
  if (N % NTH == 0) {
    for (int i = 0; i < NTH; i++) {
      int th_min_global_idx =
	(N / NTH) * (i); // minimum global index for thread i
      int th_max_global_idx =
	(N / NTH) * (i+1); // maximum global index for thread i.
      ths.push_back(std::thread(suma, data, th_min_global_idx,
                                th_max_global_idx, std::ref(res[i])));
    }
  } else {
    for (int i = 0; i < NTH; i++) {
      int th_min_global_idxs =
          (N / NTH) * i; // minimum global index for thread i
      int th_max_global_idxs =
          (N / NTH) * (i + 1); // maximum global index for thread i.
      ths.push_back(std::thread(suma, data, th_min_global_idxs,
                                th_max_global_idxs, std::ref(res[i])));
    }
  }
  auto start = std::chrono::steady_clock::now();
  for (auto &th : ths) {
    th.join();
  }
  std::chrono::duration<double> elapsed =
      std::chrono::steady_clock::now() - start;

  std::cout << NTH << "\t" << elapsed.count() << "\n";
  std::clog << "accum = " << std::accumulate(res.begin(), res.end(), 0.0)
            << std::endl;
  delete[] data;
  return 0;
}

void suma(double *v, int init, int end, double &result) {
  result = std::accumulate(v + init, v + end, 0.0);
}
