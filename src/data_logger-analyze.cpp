#include "StreamHeader.hpp"
#include <fstream>

using namespace data_logger;

void usage(){
    std::cout << "USAGE:\n  data_logger-analyse LOG_FILE\n\n";
    std::cout << "Prints information about a log file." << std::endl;
}

int main(int argc, char** argv){
    if(argc < 2){
        std::cerr << "\nERROR: No log file was provided." << std::endl;
        usage();
        exit(1);
    }
    std::string log_file = argv[1];

    std::ifstream _fstream;
    errno = 0;
    _fstream.open(log_file, std::ios_base::in | std::ios_base::binary);
    if (errno != 0)
        throw std::system_error(errno, std::system_category());
    if (!_fstream){
        std::cerr << "IO_FAIL" <<std::endl;;
        exit(2);
    }

    StreamHeader header;
    _fstream.seekg(0, _fstream.beg);
    header.deserialize(_fstream);

    std::cout << "LOG FILE INFO\n";
    std::cout << "-------------\n";
    std::cout << "Log file  :        " << log_file << "\n";
    std::cout << header << std::endl;
    exit(0);
}
