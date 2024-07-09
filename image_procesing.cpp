#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

struct Pixel {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Image {
    int width;
    int height;
    std::vector<Pixel> pixels;
};

Image loadImage(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Failed to open file " << filename << std::endl;
        return Image();
    }

    Image img;
    file.read(reinterpret_cast<char*>(&img.width), sizeof(img.width));
    file.read(reinterpret_cast<char*>(&img.height), sizeof(img.height));

    img.pixels.resize(img.width * img.height);
    file.read(reinterpret_cast<char*>(img.pixels.data()), img.width * img.height * sizeof(Pixel));

    file.close();
    return img;
}

bool saveImage(const Image& img, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Failed to create file " << filename << std::endl;
        return false;
    }

    file.write(reinterpret_cast<const char*>(&img.width), sizeof(img.width));
    file.write(reinterpret_cast<const char*>(&img.height), sizeof(img.height));
    file.write(reinterpret_cast<const char*>(img.pixels.data()), img.width * img.height * sizeof(Pixel));

    file.close();
    return true;
}

Image grayscale(const Image& img) {
    Image grayImg;
    grayImg.width = img.width;
    grayImg.height = img.height;
    grayImg.pixels.resize(img.pixels.size());

    for (size_t i = 0; i < img.pixels.size(); ++i) {
        unsigned char gray = static_cast<unsigned char>((img.pixels[i].red + img.pixels[i].green + img.pixels[i].blue) / 3);
        grayImg.pixels[i].red = gray;
        grayImg.pixels[i].green = gray;
        grayImg.pixels[i].blue = gray;
    }

    return grayImg;
}

Image blur(const Image& img) {
    Image blurredImg;
    blurredImg.width = img.width;
    blurredImg.height = img.height;
    blurredImg.pixels.resize(img.pixels.size());

    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            int redSum = 0, greenSum = 0, blueSum = 0, count = 0;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < img.width && ny >= 0 && ny < img.height) {
                        int idx = ny * img.width + nx;
                        redSum += img.pixels[idx].red;
                        greenSum += img.pixels[idx].green;
                        blueSum += img.pixels[idx].blue;
                        ++count;
                    }
                }
            }

            int idx = y * img.width + x;
            blurredImg.pixels[idx].red = static_cast<unsigned char>(redSum / count);
            blurredImg.pixels[idx].green = static_cast<unsigned char>(greenSum / count);
            blurredImg.pixels[idx].blue = static_cast<unsigned char>(blueSum / count);
        }
    }

    return blurredImg;
}

Image sharpen(const Image& img) {
    Image sharpenedImg;
    sharpenedImg.width = img.width;
    sharpenedImg.height = img.height;
    sharpenedImg.pixels.resize(img.pixels.size());

    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            int redSum = 0, greenSum = 0, blueSum = 0, count = 0;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < img.width && ny >= 0 && ny < img.height) {
                        int idx = ny * img.width + nx;
                        if (dx == 0 && dy == 0) {
                            redSum += 9 * img.pixels[idx].red;
                            greenSum += 9 * img.pixels[idx].green;
                            blueSum += 9 * img.pixels[idx].blue;
                        } else {
                            redSum -= img.pixels[idx].red;
                            greenSum -= img.pixels[idx].green;
                            blueSum -= img.pixels[idx].blue;
                        }
                        ++count;
                    }
                }
            }

            int idx = y * img.width + x;
            sharpenedImg.pixels[idx].red = static_cast<unsigned char>(std::min(std::max(redSum / 9 + img.pixels[idx].red, 0), 255));
            sharpenedImg.pixels[idx].green = static_cast<unsigned char>(std::min(std::max(greenSum / 9 + img.pixels[idx].green, 0), 255));
            sharpenedImg.pixels[idx].blue = static_cast<unsigned char>(std::min(std::max(blueSum / 9 + img.pixels[idx].blue, 0), 255));
        }
    }

    return sharpenedImg;
}

Image adjustBrightness(const Image& img, double factor) {
    Image adjustedImg;
    adjustedImg.width = img.width;
    adjustedImg.height = img.height;
    adjustedImg.pixels.resize(img.pixels.size());

    for (size_t i = 0; i < img.pixels.size(); ++i) {
        adjustedImg.pixels[i].red = static_cast<unsigned char>(std::min(std::max(img.pixels[i].red * factor, 0.0), 255.0));
        adjustedImg.pixels[i].green = static_cast<unsigned char>(std::min(std::max(img.pixels[i].green * factor, 0.0), 255.0));
        adjustedImg.pixels[i].blue = static_cast<unsigned char>(std::min(std::max(img.pixels[i].blue * factor, 0.0), 255.0));
    }

    return adjustedImg;
}

Image adjustContrast(const Image& img, double factor) {
    Image adjustedImg;
    adjustedImg.width = img.width;
    adjustedImg.height = img.height;
    adjustedImg.pixels.resize(img.pixels.size());

    for (size_t i = 0; i < img.pixels.size(); ++i) {
        adjustedImg.pixels[i].red = static_cast<unsigned char>(std::min(std::max((img.pixels[i].red - 127.5) * factor + 127.5, 0.0), 255.0));
        adjustedImg.pixels[i].green = static_cast<unsigned char>(std::min(std::max((img.pixels[i].green - 127.5) * factor + 127.5, 0.0), 255.0));
        adjustedImg.pixels[i].blue = static_cast<unsigned char>(std::min(std::max((img.pixels[i].blue - 127.5) * factor + 127.5, 0.0), 255.0));
    }

    return adjustedImg;
}

Image cropImage(const Image& img, int x, int y, int width, int height) {
    Image croppedImg;
    croppedImg.width = width;
    croppedImg.height = height;
    croppedImg.pixels.resize(width * height);

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            croppedImg.pixels[j * width + i] = img.pixels[(y + j) * img.width + (x + i)];
        }
    }

    return croppedImg;
}

Image resizeImage(const Image& img, int newWidth, int newHeight) {
    Image resizedImg;
    resizedImg.width = newWidth;
    resizedImg.height = newHeight;
    resizedImg.pixels.resize(newWidth * newHeight);

    double xRatio = static_cast<double>(img.width - 1) / newWidth;
    double yRatio = static_cast<double>(img.height - 1) / newHeight;

    for (int j = 0; j < newHeight; ++j) {
        for (int i = 0; i < newWidth; ++i) {
            int x = static_cast<int>(xRatio * i);
            int y = static_cast<int>(yRatio * j);
            resizedImg.pixels[j * newWidth + i] = img.pixels[y * img.width + x];
        }
    }

    return resizedImg;
}

void printMenu() {
    std::cout << "Image Processing Application Menu:" << std::endl;
    std::cout << "1. Load Image" << std::endl;
    std::cout << "2. Display Image" << std::endl;
    std::cout << "3. Convert to Grayscale" << std::endl;
    std::cout << "4. Apply Blur Filter" << std::endl;
    std::cout << "5. Apply Sharpen Filter" << std::endl;
    std::cout << "6. Adjust Brightness" << std::endl;
    std::cout << "7. Adjust Contrast" << std::endl;
    std::cout << "8. Crop Image" << std::endl;
    std::cout << "9. Resize Image" << std::endl;
    std::cout << "10. Save Image" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your choice: ";
}

int main() {
    Image currentImage;
    bool imageLoaded = false;

    int choice;
    do {
        printMenu();
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::string filename;
                std::cout << "Enter filename to load: ";
                std::cin >> filename;
                currentImage = loadImage(filename);
                if (!currentImage.pixels.empty()) {
                    std::cout << "Image loaded successfully." << std::endl;
                    imageLoaded = true;
                }
                break;
            }
            case 2: {
                if (imageLoaded) {
                    std::cout << "Displaying image..." << std::endl;
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 3: {
                if (imageLoaded) {
                    currentImage = grayscale(currentImage);
                    std::cout << "Image converted to grayscale." << std::endl;
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 4: {
                if (imageLoaded) {
                    currentImage = blur(currentImage);
                    std::cout << "Blur filter applied to image." << std::endl;
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 5: {
                if (imageLoaded) {
                    currentImage = sharpen(currentImage);
                    std::cout << "Sharpen filter applied to image." << std::endl;
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 6: {
                if (imageLoaded) {
                    double brightnessFactor;
                    std::cout << "Enter brightness factor (0.0 - 2.0): ";
                    std::cin >> brightnessFactor;
                    currentImage = adjustBrightness(currentImage, brightnessFactor);
                    std::cout << "Brightness adjusted." << std::endl;
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 7: {
                if (imageLoaded) {
                    double contrastFactor;
                    std::cout << "Enter contrast factor (0.1 - 10.0): ";
                    std::cin >> contrastFactor;
                    currentImage = adjustContrast(currentImage, contrastFactor);
                    std::cout << "Contrast adjusted." << std::endl;
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 8: {
                if (imageLoaded) {
                    int x, y, cropWidth, cropHeight;
                    std::cout << "Enter x, y, width, height for cropping: ";
                    std::cin >> x >> y >> cropWidth >> cropHeight;
                    currentImage = cropImage(currentImage, x, y, cropWidth, cropHeight);
                    std::cout << "Image cropped." << std::endl;
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 9: {
                if (imageLoaded) {
                    int newWidth, newHeight;
                    std::cout << "Enter new width and height for resizing: ";
                    std::cin >> newWidth >> newHeight;
                    currentImage = resizeImage(currentImage, newWidth, newHeight);
                    std::cout << "Image resized." << std::endl;
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 10: {
                if (imageLoaded) {
                    std::string filename;
                    std::cout << "Enter filename to save: ";
                    std::cin >> filename;
                    if (saveImage(currentImage, filename)) {
                        std::cout << "Image saved successfully." << std::endl;
                    } else {
                        std::cerr << "Failed to save image." << std::endl;
                    }
                } else {
                    std::cout << "No image loaded." << std::endl;
                }
                break;
            }
            case 0:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }

    } while (choice != 0);

    return 0;
}
