#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

void display_progress(size_t bytes_copied, size_t total_size, double speed, duration<double> elapsed) {
    double percent = (double)bytes_copied / total_size * 100;
    int bar_width = 30;
    int pos = bar_width * percent / 100;

    cout << "\r[";
    for (int i = 0; i < bar_width; i++) {
        if (i < pos) {
            cout << "#";
        } else {
            cout << " ";
        }
    }
    cout << "] " << fixed << setprecision(2) << percent << "%";
    cout << " | Speed: " << speed / (1024 * 1024) << " MB/s";
    cout << " | ETA: " << (total_size - bytes_copied) / speed << " s";
    cout.flush();
}

void copy_chunk(int dest, void* src_map, off_t offset, size_t chunk_size) {
    pwrite(dest, (char*)src_map + offset, chunk_size, offset);
}

void copy_file(const char* input, const char* output, size_t block_size) {
    int src = open(input, O_RDONLY);
    if (src < 0) {
        cerr << "Error opening input file: " << strerror(errno) << endl;
        return;
    }

    int dest = open(output, O_WRONLY | O_CREAT | O_TRUNC | O_DIRECT, 0666);
    if (dest < 0) {
        cerr << "Error opening output file: " << strerror(errno) << endl;
        close(src);
        return;
    }

    struct stat st;
    if (fstat(src, &st) != 0) {
        cerr << "Error getting file size: " << strerror(errno) << endl;
        close(src);
        close(dest);
        return;
    }
    off_t total_size = st.st_size;

    posix_fadvise(src, 0, 0, POSIX_FADV_SEQUENTIAL);  // Optimize for sequential read

    void* src_map = mmap(nullptr, total_size, PROT_READ, MAP_SHARED, src, 0);
    if (src_map == MAP_FAILED) {
        cerr << "Error mapping input file: " << strerror(errno) << endl;
        close(src);
        close(dest);
        return;
    }

    size_t bytes_copied = 0;
    auto start_time = steady_clock::now();

    vector<thread> threads;
    for (size_t i = 0; i < total_size; i += block_size) {
        size_t chunk_size = min(block_size, total_size - i);
        threads.emplace_back(copy_chunk, dest, src_map, i, chunk_size);
        bytes_copied += chunk_size;

        auto now = steady_clock::now();
        duration<double> elapsed = now - start_time;
        double speed = bytes_copied / elapsed.count();
        display_progress(bytes_copied, total_size, speed, elapsed);
    }

    for (auto& t : threads) {
        t.join();
    }

    munmap(src_map, total_size);
    close(dest);
    close(src);
    cout << "\nCopy complete! " << bytes_copied << " bytes copied." << endl;
}

int main(int argc, char* argv[]) {

    const char* input = nullptr;
    const char* output = nullptr;
    size_t block_size = 0;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "from=", 5) == 0) {
            input = argv[i] + 5;
        } else if (strncmp(argv[i], "to=", 3) == 0) {
            output = argv[i] + 3;
        } else if (strncmp(argv[i], "bs=", 3) == 0) {
            block_size = stoul(argv[i] + 3);
        }
    }

    if (!input || !output || block_size == 0) {
        cerr << "Invalid arguments. Usage: " << argv[0] << " from=<input> to=<output> bs=<block_size>" << endl;
        return 1;
    }

    copy_file(input, output, block_size);
    return 0;
}
