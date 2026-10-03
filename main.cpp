#include <iostream>
#include <cstring>
#include <fstream>

struct Options {
    bool lines = false;
    bool words = false;
    bool alnum = false;
    bool bytes = false;
};

struct Stats {
    uint64_t lines = 0;
    uint64_t words = 0;
    uint64_t alnum = 0;
    uint64_t bytes = 0;
};

bool IsLongOption(const char* n) {
    return n[0] == '-' && n[1] == '-' && n[1] != '\0';
}

bool IsOption(const char* n) {
    return n[0] == '-' && n[1] != '\0';
}

bool IsSpace(unsigned char n) {
    return ((n == ' ') || (n == '\n') || (n == '\t') || (n == '\r') || (n == '\v') || (n == '\f'));
}

bool IsAlnum(unsigned char n) {
    return (n >= '0' && n <= '9') || (n >= 'a' && n <= 'z') || (n >= 'A' && n <= 'Z');
}

void parseShortOptions(const char* n, Options& options) {
    for (int i = 1; n[i] != '\0'; i++) {
        if (n[i] == 'l') {
            options.lines = true;
        } else if (n[i] == 'w') {
            options.words = true;
        } else if (n[i] == 'a') {
            options.alnum = true;
        } else if (n[i] == 'c') {
            options.bytes = true;
        }
    }
}


void parseOptions(const char* n, Options& options) {
    if (std::strcmp(n, "--lines") == 0) {
        options.lines = true;
    } else if (std::strcmp(n, "--words") == 0) {
        options.words = true;
    } else if (std::strcmp(n, "--alnum") == 0) {
        options.alnum = true;
    } else if (std::strcmp(n, "--bytes") == 0) {
        options.bytes = true;
    } else {
        parseShortOptions(n, options);
    }
}

bool countFile(const char* filename, Stats& stats) {

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }

    char n;
    bool Insideword = false;
    bool IsfileNotEmpty = false;
    bool LastHasNewLine = false;

    while (file.get(n)) {

        IsfileNotEmpty = true;

        stats.bytes++;

        if (n == '\n') {
            stats.lines++;
            LastHasNewLine = true;
        } else LastHasNewLine = false;

        

        if (IsAlnum(n)) {
            stats.alnum++;
        }
        
        if (IsSpace(n)) {
            Insideword = false;
        } else if (Insideword == false) {
            stats.words++;
            Insideword = true;
        }
    }

    if (IsfileNotEmpty && LastHasNewLine == false) {
        stats.lines++;
    }

    return true;
}

void PrintResult(const Options& options, const Stats& stats, const char* filename) {

    if (options.lines) {
        std::cout << stats.lines << ' ';
    }

    if (options.words) {
        std::cout << stats.words << ' ';
    }

    if (options.alnum) {
        std::cout << stats.alnum << ' ';
    }

    if (options.bytes) {
        std::cout << stats.bytes << ' ';
    }

    std::cout << filename << '\n';
}


int main(int argc, char** argv) {

    Options options;

    // parsing options
    for (int i = 1; i < argc; i++) {
        if (IsOption(argv[i])) {
            parseOptions(argv[i], options);

            // check if it's option or filename
            // parseOption
        }
    }

    if (options.lines == false && options.words == false && options.bytes == false && options.alnum == false) {
        options.lines = true;
        options.words = true;
        options.bytes = true;
    }

    // parsing files
    for (int i = 1; i < argc; i++) {
        if (IsOption(argv[i])) {
            continue;
        }

        Stats stats;

        if ((countFile(argv[i], stats)) == false) {
            std::cerr << "File is empty!" << '\n';
            return 1;
        }

        PrintResult(options, stats, argv[i]);

        



        // check if it's option or filename
            // parseFile
    }
}