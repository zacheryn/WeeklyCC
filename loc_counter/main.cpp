#include <boost/program_options.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "loc.hpp"

int main(int argc, char *argv[]){
    // Simplify namespace
    namespace po = boost::program_options;

    // Set up flags
    po::options_description desc("Allowed Options");
    desc.add_options()
    ("help", "produce help message")
    ("input-file", po::value<std::vector<std::string>>(), "input file (a loose file also counts)");

    // Allow user to add input files without using -input-file flag
    po::positional_options_description p;
    p.add("input-file", -1);

    // Parse the command line to grab the flags
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);

    // Display help message and return
    if(vm.count("help")){
        std::cout << desc << std::endl;
        return 1;
    }

    // Prompt the user to specify files and return
    if(!vm.count("input-file")){
        std::cerr << "ERROR: Please specify files when running\n"
                  << "Example: ./loc.exe <filename1> ... <filenameN>"
                  << std::endl;

        return 1;
    }



    loc l(vm["input-file"].as<std::vector<std::string>>());
    l.count();
    l.output();
}