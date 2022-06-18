#include <cstdlib>
#include <iostream>

#include "image.hpp"
#include "pngfile.hpp"
#include "resizer.hpp"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Usage:\n\t" << argv[0]
                  << " <input_filepath> <output_filepath> <output_width> <output_height>"
                  << std::endl;
        return 1;
    }

    const char* input_filepath = argv[1];
    const char* output_filepath = argv[2];

    auto width = std::atoi(argv[3]);
    auto height = std::atoi(argv[4]);

    if (width <= 0 || height <= 0) {
        std::cerr << " [-] width and height must be equal to or greater than 1." << std::endl;
        return 1;
    }

    auto input_file = PNGFile::load_from_file(input_filepath);
    if (!input_file) {
        std::cerr << " [-] couldn't load input file." << std::endl;
        return 1;
    }

    auto output_file = Resizer::resize(*input_file, width, height);
    if (!output_file) {
        std::cerr << " [-] couldn't generate output image." << std::endl;
        return 1;
    }

    std::cout << " [!] input image size: " << input_file->width() << ':' << input_file->height() << std::endl;
    std::cout << " [!] requested image size: " << width << ':' << height << std::endl;
    std::cout << " [!] output image size: " << output_file->width() << ':' << output_file->height() << std::endl;

    if (!output_file->save_as(output_filepath)) {
        std::cerr << " [-] couldn't save output file." << std::endl;
        return 1;
    }

    std::cout << " [+] saved output file." << std::endl;

    return 0;
}