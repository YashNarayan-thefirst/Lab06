#include <iostream>
#include <cmath>
#include "Image.h"       
#include "ConsoleGfx.h"  
using namespace std;

unsigned char *scaledImage(unsigned char *imageData, const int order) {
    auto img = Image(imageData);
    const int height = img.getHeight();
    const int width = img.getWidth();
    int adjustedOrder = order;
    
    while ((width * pow(2, adjustedOrder) > 256) || (height * pow(2, adjustedOrder) > 256) ||
           (width * pow(2, adjustedOrder) < 1) || (height * pow(2, adjustedOrder) < 1)) {
        ((width * pow(2, adjustedOrder) > 256) || (height * pow(2, adjustedOrder) > 256)) ? adjustedOrder-- : adjustedOrder++;
    }

    int sfactor = pow(2, adjustedOrder);
    int sheight = height * sfactor;
    int swidth = width * sfactor;
    int sarea = sheight * swidth;

    auto *sdata = new unsigned char[sarea + 2];
    sdata[0] = static_cast<unsigned char>(swidth);
    sdata[1] = static_cast<unsigned char>(sheight);

    // Scale image data
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < sfactor; k++) {
                for (int l = 0; l < sfactor; l++) {
                    sdata[sfactor * i + k + (sfactor * j + l) * swidth] = img.getPixelData()[i + j * width + 2];
                }
            }
        }
    }
    return sdata;
}

int main() {
    ConsoleGfx *cg = ConsoleGfx::getInstance();
    auto image = Image(nullptr);
    printf("Welcome to the Image Scaler!\n\nDisplaying Spectrum Image:\n");
    cg->displayImage(cg->testRainbow);
    int choice = -1;
    string input;
    int order;

    while (choice) {
        printf("\nScaler Menu\n-----------\n0. Exit\n1. Load File\n2. Load Test Image\n3. Display Image\n4. Enlarge Image\n5. Shrink Image\n6. Show Image Properties\n");
        cout << endl <<"Select a Menu Option: ";
        cin >> choice;

        if (choice == 0) {
            break;
        }

        switch (choice) {
            case 1:{
                printf("Enter name of file to load: ");
                try {
                    cin >> input;
                    printf("\n");
                    image.setImageData(cg->loadFile(input));
                    printf("File loaded.\n");
                } catch (const exception &e) {
                    printf("\nError: could not load file.\n");
                }
                break;
            }
            case 2:{
                image.setImageData(cg->testImage);
                cout << "Test image data loaded." << endl;
                break;
            }
            case 3:{
                try {
                    cg->displayImage(image.getPixelData());
                } catch (const exception &e) {
                    printf("Error: no image loaded.\n");
                }
                break;
            }
            case 4:{
                printf("Enter orders of magnitude for enlargement: ");
                cin >> order;
                image.setImageData(scaledImage(image.getImageData(), order));
                printf("Image enlarged!\n");
                break;
            }
            case 5:{
                printf("Enter orders of magnitude for reduction: ");
                cin >> order;
                image.setImageData(scaledImage(image.getImageData(), order * -1));
                printf("Image reduced!\n");
                break;
            }
            case 6:{
                try {
                    printf("Image Dimensions: (%d,%d)", image.getWidth(), image.getHeight());
                } catch (const exception &e) {
                    printf("Error: no image loaded\n");
                }
                break;
            }
            default:{
                printf("Unrecognized Menu Selection!");
                break;
            }
        }
    }
    return 0;
}
