#include "loc.hpp"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <stdexcept>

loc::File::File(std::string_view s) noexcept
: name{s}
, blank{0}
, comment{0}
, code{0} {}

loc::loc(const std::vector<std::string>& v) noexcept {
    files.reserve(v.size());
    for(const auto& s : v)
        files.emplace_back(s);
}

void loc::validate_files() const {
    namespace fs = std::filesystem;
    for(const auto& f : files){
        if(!fs::exists(f.name)){
            throw std::runtime_error("ERROR: Cannot find file '" + f.name + "'");
        }else if(fs::is_directory(f.name)){
            throw std::runtime_error("ERROR: '" + f.name + "' is a directory");
        }else{
            auto pos = f.name.find_last_of('.');
            if(pos == std::string::npos)
                throw std::runtime_error("ERROR: '" + f.name + "' is not a supported file type");
            if(std::find(file_types.begin(), file_types.end(), f.name.substr(pos + 1)) == file_types.end())
                throw std::runtime_error("ERROR: '" + f.name + "' is not a supported file type");
        }
    }
}

void loc::pub_check() const {
    validate_files();
}

void loc::print_files() const noexcept {
    for(const auto& f : files){
        std::cout << f.name << ' ';
    }
    std::cout << std::endl;
}

void loc::count(){
    // Represents the end of the input stream
    std::istreambuf_iterator<char> eos;

    // Loop through the files
    for(auto& f : files){
        // Open the file
        std::ifstream fin(f.name);
        if(!fin.is_open())
            throw std::runtime_error("ERROR: Could not open '" + f.name + "'");

        bool is_code = false;
        bool is_comment = false;
        bool in_comment = false;
        bool in_multi_comment = false;
        bool in_string = false;
        bool prev_char_comment = false;
        // Loop through the file by character
        for(std::istreambuf_iterator<char> it(fin); it != eos; ++it){
            // Here for debugging
            char c = *it;

            if(c == '\n'){
                if(is_code){
                    ++f.code;
                }else if(is_comment){
                    ++f.comment;
                }else{
                    ++f.blank;
                }
                is_code = false;
                prev_char_comment = false;
                if(!in_multi_comment){
                    is_comment = false;
                    in_comment = false;
                }
                continue;
            }
            else if(std::isspace(c))
            continue;

            switch(c){
            case '/':
                if(!in_string && prev_char_comment){
                    prev_char_comment = false;
                    in_comment = !in_comment;
                    if(in_comment){
                        is_comment = true;
                    }else{
                        in_multi_comment = false;
                    }
                }else if(!in_string){
                    prev_char_comment = true;
                }
                break;

            case '*':
                if(!in_string && prev_char_comment){
                    prev_char_comment = false;
                    in_comment = true;
                    in_multi_comment = true;
                }else if(in_multi_comment){
                    prev_char_comment = true;
                }
                break;

            case '"':
                if(!in_comment){
                    in_string = !in_string;
                    is_code = true;
                }
                if(prev_char_comment) prev_char_comment = false;
                break;

            default:
                if(!in_comment) is_code = true;
                if(prev_char_comment) prev_char_comment = false;
                break;
            }
        }
        if(is_code){
            ++f.code;
        }else if(is_comment){
            ++f.comment;
        }else{
            ++f.blank;
        }
    }
}

void loc::output() const noexcept {
    output(std::cout);
}

void loc::output(std::ostream& out) const noexcept {
    out << "Name    Total    Code    Comment    Blank    \n"
        << "------------------------------------------\n";

    for(const auto& f : files){
        out << f.name << "    "
            << f.code + f.comment + f.blank << "    "
            << f.code << "    "
            << f.comment << "    "
            << f.blank << "\n\n";
    }
}