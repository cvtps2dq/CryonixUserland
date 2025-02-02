#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>

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

void copy_file(const char* input, const char* output, size_t block_size) {
    int src = open(input, O_RDONLY);
    if (src < 0) {
        cerr << "Error opening input file: " << strerror(errno) << endl;
        return;
    }

    int dest = open(output, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dest < 0) {
        cerr << "Error opening output file: " << strerror(errno) << endl;
        close(src);
        return;
    }

    struct stat st;
    fstat(src, &st);
    off_t total_size = st.st_size;

    posix_fadvise(src, 0, 0, POSIX_FADV_SEQUENTIAL);  // Hint kernel for sequential read

    char* buffer = new char[block_size];
    size_t bytes_copied = 0;
    size_t blocks_copied = 0;

    auto start_time = steady_clock::now();
    ssize_t bytes_read;
    while ((bytes_read = read(src, buffer, block_size)) > 0) {
        ssize_t bytes_written = write(dest, buffer, bytes_read);
        if (bytes_written < 0) {
            cerr << "Error writing to file: " << strerror(errno) << endl;
            break;
        }
        bytes_copied += bytes_written;
        blocks_copied++;

        auto now = steady_clock::now();
        duration<double> elapsed = now - start_time;
        double speed = bytes_copied / elapsed.count();
        display_progress(bytes_copied, total_size, speed, elapsed);
    }

    delete[] buffer;

    fsync(dest);  // Sync once at the end for efficiency
    close(dest);
    close(src);
    cout << "\nCopy complete! " << bytes_copied << " bytes copied in " << blocks_copied << " blocks." << endl;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " from=<input> to=<output> bs=<block_size>" << endl;
        return 1;
    }

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
