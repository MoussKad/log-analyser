#include <iostream>
#include <fstream>
#include <string>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_SIZE  (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

void monitor_logs(const std::string& log_file, const std::string& filter) {
    int fd = inotify_init();
    if (fd < 0) {
        perror("inotify_init");
        return;
    }

    int wd = inotify_add_watch(fd, log_file.c_str(), IN_MODIFY);
    if (wd < 0) {
        perror("inotify_add_watch");
        return;
    }

    std::ifstream file(log_file);
    file.seekg(0, std::ios::end); // Aller à la fin du fichier pour ne lire que les nouvelles lignes

    char buffer[EVENT_BUF_LEN];
    while (true) {
        int length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("read");
            break;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (filter.empty() || line.find(filter) != std::string::npos) {
                std::cout << line << std::endl;
            }
        }
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <fichier_log> [mot-clé]" << std::endl;
        return 1;
    }

    std::string log_file = argv[1];
    std::string filter = (argc > 2) ? argv[2] : "";

    std::cout << "Surveillance du fichier : " << log_file << std::endl;
    if (!filter.empty()) {
        std::cout << "Filtrage activé : \"" << filter << "\"" << std::endl;
    }

    monitor_logs(log_file, filter);
    return 0;
}
