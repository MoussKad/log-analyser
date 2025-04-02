#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sys/inotify.h>
#include <unistd.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define EVENT_BUF_LEN (1024 * (EVENT_SIZE + 16))

void monitor_logs(const std::string& log_file, const std::regex& filter_regex, bool save_logs) {
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

    std::ofstream output_file;
    if (save_logs) {
        output_file.open("filtered_logs.txt", std::ios::app);
        if (!output_file) {
            std::cerr << "Erreur : Impossible d'ouvrir filtered_logs.txt" << std::endl;
            return;
        }
    }

    char buffer[EVENT_BUF_LEN];
    while (true) {
        int length = read(fd, buffer, EVENT_BUF_LEN);
        if (length < 0) {
            perror("read");
            break;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (std::regex_search(line, filter_regex)) {
                std::cout << line << std::endl;
                if (save_logs) {
                    output_file << line << std::endl;
                }
            }
        }
    }

    if (save_logs) {
        output_file.close();
    }

    inotify_rm_watch(fd, wd);
    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <fichier_log> <regex> [--save]" << std::endl;
        return 1;
    }

    std::string log_file = argv[1];
    std::string filter_pattern = argv[2];
    bool save_logs = (argc > 3 && std::string(argv[3]) == "--save");

    try {
        std::regex filter_regex(filter_pattern);
        std::cout << "Surveillance du fichier : " << log_file << std::endl;
        std::cout << "Filtrage avec : \"" << filter_pattern << "\"" << std::endl;
        if (save_logs) {
            std::cout << "Les logs filtrés seront enregistrés dans 'filtered_logs.txt'" << std::endl;
        }

        monitor_logs(log_file, filter_regex, save_logs);
    } catch (std::regex_error& e) {
        std::cerr << "Erreur : Expression régulière invalide !" << std::endl;
        return 1;
    }

    return 0;
}
