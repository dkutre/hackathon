#include <iostream>
#include "Server.h"

int main(int argc, char* argv[])
{
    try {
        std::string port;
        int opt = 0;
        while ((opt = getopt(argc, argv, "p:")) != -1) {
            switch (opt) {
                case 'p':
                    port = std::string(optarg);
                    if (port.empty())
                        port = "8000";
                default:
                    break;
            }
        }

        boost::filesystem::path cur_dir = boost::filesystem::current_path();
        std::size_t number_threads = 2;
        std::cout << "threads: " << number_threads << "\n root_dir: " << cur_dir.string() << std::endl;
        Server s("127.0.0.1", port, cur_dir.string(), number_threads);
        s.run();
    } catch (std::exception &e) {
        std::cerr << "exception: " << e.what() << "\n";
    }
    return 0;
}