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

    //Read header size (include sizes fon dynmically typed string fields
    size_t szHeader;
    char* hp = (char*) &szHeader;
    _fstream.seekg (0, _fstream.beg); //Start from beginning of file
    _fstream.read(hp, sizeof(size_t));

    //Now load entire header
    std::vector<char> buffer;
    buffer.resize(szHeader);
    _fstream.seekg (0, _fstream.beg); //Again start from beginning of file
    _fstream.read(&buffer[0], szHeader);

    _fstream.close();

    size_t bytes_read = header.deserialize(&buffer[0]);



    std::cout << "LOG FILE INFO\n";
    std::cout << "-------------\n";
    std::cout << "Log file :         " << log_file << "\n";
    std::cout << header << std::endl;
    std::ofstream of;
    of.open("encoding.txt", std::ios_base::out);
    of << header.dataModel;
    of.close();

    std::cout << "\nData Model details are stored to encoding.txt" << std::endl;
    exit(0);

}
