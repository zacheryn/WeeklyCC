#ifndef LOC_HPP
#define LOC_HPP

#include <string>
#include <string_view>
#include <vector>
#include <array>

using namespace std::literals;

// Used to count the number of lines of code in the specified file(s)
class loc {
private:

    // Contains useful info for the file
    struct File {
        const std::string name;
        size_t blank;
        size_t comment;
        size_t code;

        File(std::string_view s) noexcept;
    };

    // A list of the provided files
    std::vector<File> files;

    // Accepted file types
    static constexpr std::array file_types = {
        "cpp"sv,
        "hpp"sv,
        "c"sv,
        "h"sv
    };

    // Checks that all specified files exist,
    // are not a directory, and are a valid file type
    void validate_files() const;

public:
    loc() = delete;

    loc(const std::vector<std::string>& v) noexcept;

    // Count and track the number of each type of lines for each file
    void count();

    void pub_check() const;

    void print_files() const noexcept;

    // Outputs the results to the specified output stream
    void output(std::ostream& out) const noexcept;

    // Outputs the results to stdout
    void output() const noexcept;
};

#endif