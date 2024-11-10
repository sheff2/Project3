#include <fstream>
#include <iostream>
#include <vector>


struct __attribute__((__packed__)) Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};


struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};


int example1() {
    Header carheader;
    std::ifstream carfile("input/layer1.tga", std::ios::binary);
    carfile.read(reinterpret_cast<char*>(&carheader), sizeof(Header));
    std::vector<Pixel> carpixels(carheader.width * carheader.height);
    carfile.read(reinterpret_cast<char*>(carpixels.data()), carpixels.size() * sizeof(Pixel));

    Header patternheader;
    std::ifstream patternfile("input/pattern1.tga", std::ios::binary);
    patternfile.read(reinterpret_cast<char*>(&patternheader), sizeof(Header));
    std::vector<Pixel> patternpixels(patternheader.width * patternheader.height);
    patternfile.read(reinterpret_cast<char*>(patternpixels.data()), patternpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;

    for(int i = 0; i < carheader.width * carheader.height; i++) {
        Pixel temp;
        temp.red = ((carpixels[i].red * patternpixels[i].red) / 255.0f) + 0.5f;
        temp.green = ((carpixels[i].green * patternpixels[i].green) / 255.0f) + 0.5f;
        temp.blue = ((carpixels[i].blue * patternpixels[i].blue) / 255.0f) + 0.5f;
        endresult.push_back(temp);

    }


    std::ofstream outFile("output/part1.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&carheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(endresult.data()), endresult.size() * sizeof(Pixel));
    outFile.close();
}

int example2() {
    Header carheader;
    std::ifstream carfile("input/car.tga", std::ios::binary);
    carfile.read(reinterpret_cast<char*>(&carheader), sizeof(Header));
    std::vector<Pixel> carpixels(carheader.width * carheader.height);
    carfile.read(reinterpret_cast<char*>(carpixels.data()), carpixels.size() * sizeof(Pixel));

    Header patternheader;
    std::ifstream patternfile("input/layer2.tga", std::ios::binary);
    patternfile.read(reinterpret_cast<char*>(&patternheader), sizeof(Header));
    std::vector<Pixel> patternpixels(patternheader.width * patternheader.height);
    patternfile.read(reinterpret_cast<char*>(patternpixels.data()), patternpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;
    //car - pattern
    for(int i = 0; i < carheader.width * carheader.height; i++) {
        Pixel temp;
        double red =static_cast<double>(carpixels[i].red) - static_cast<double>(patternpixels[i].red);
        double green = static_cast<double>(carpixels[i].green) - static_cast<double>(patternpixels[i].green);
        double blue = static_cast<double>(carpixels[i].blue) - static_cast<double>(patternpixels[i].blue);
        if(red < 0) {
            red = 0;
        }

        if(green < 0) {
            green = 0;
        }

        if(blue < 0) {
            blue = 0;
        }

        temp.red = static_cast<unsigned char>(red);
        temp.green = static_cast<unsigned char>(green);
        temp.blue = static_cast<unsigned char>(blue);


        endresult.push_back(temp);

    }


    std::ofstream outFile("output/part2.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&carheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(endresult.data()), endresult.size() * sizeof(Pixel));
    outFile.close();

}

int example3() {
    Header layerheader;
    std::ifstream layerfile("input/layer1.tga", std::ios::binary);
    layerfile.read(reinterpret_cast<char*>(&layerheader), sizeof(Header));
    std::vector<Pixel> layerpixels(layerheader.width * layerheader.height);
    layerfile.read(reinterpret_cast<char*>(layerpixels.data()), layerpixels.size() * sizeof(Pixel));

    Header patternheader;
    std::ifstream patternfile("input/pattern2.tga", std::ios::binary);
    patternfile.read(reinterpret_cast<char*>(&patternheader), sizeof(Header));
    std::vector<Pixel> patternpixels(patternheader.width * patternheader.height);
    patternfile.read(reinterpret_cast<char*>(patternpixels.data()), patternpixels.size() * sizeof(Pixel));

    Header textheader;
    std::ifstream textfile("input/text.tga", std::ios::binary);
    textfile.read(reinterpret_cast<char*>(&textheader), sizeof(Header));
    std::vector<Pixel> textpixels(textheader.width * textheader.height);
    textfile.read(reinterpret_cast<char*>(textpixels.data()), textpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;

    for(int i = 0; i < layerheader.width * layerheader.height; i++) {
        Pixel temp;
        temp.red = ((layerpixels[i].red * patternpixels[i].red) / 255.0f) + 0.5f;
        temp.green = ((layerpixels[i].green * patternpixels[i].green) / 255.0f) + 0.5f;
        temp.blue = ((layerpixels[i].blue * patternpixels[i].blue) / 255.0f) + 0.5f;
        endresult.push_back(temp);
    }

    std::vector<Pixel> finalresult;
    for(int i = 0; i < layerheader.width * layerheader.height; i++) {
        Pixel temp;
        //screen blend formula
        //(255 - image1[i].green) * (255 - image2[i].green) / 255)
        //1−(1−A)×(1−B)
        float firstred = endresult[i].red / 255.0;
        float secondred = textpixels[i].red / 255.0;

        float firstblue = endresult[i].blue / 255.0;
        float secondblue = textpixels[i].blue / 255.0;

        float firstgreen = endresult[i].green / 255.0;
        float secondgreen = textpixels[i].green / 255.0;

        float red = 1.0 - (1.0 - firstred) * (1.0 - secondred);
        float green = 1.0 - (1.0 - firstgreen) * (1.0 - secondgreen);
        float blue = 1.0 - (1.0 - firstblue) * (1.0 - secondblue);

        if(red < 0) {
            red = 0;
        }

        if(green < 0) {
            green = 0;
        }

        if(blue < 0) {
            blue = 0;
        }

        temp.red = static_cast<unsigned char>(red * 255 + 0.5f);
        temp.green = static_cast<unsigned char>(green * 255 + 0.5f);
        temp.blue = static_cast<unsigned char>(blue * 255 + 0.5f);


        finalresult.push_back(temp);

    }




    std::ofstream outFile("output/part3.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&layerheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(finalresult.data()), finalresult.size() * sizeof(Pixel));
    outFile.close();
}

int example4() {
    Header layerheader;
    std::ifstream layerfile("input/layer2.tga", std::ios::binary);
    layerfile.read(reinterpret_cast<char*>(&layerheader), sizeof(Header));
    std::vector<Pixel> layerpixels(layerheader.width * layerheader.height);
    layerfile.read(reinterpret_cast<char*>(layerpixels.data()), layerpixels.size() * sizeof(Pixel));

    Header patternheader;
    std::ifstream patternfile("input/circles.tga", std::ios::binary);
    patternfile.read(reinterpret_cast<char*>(&patternheader), sizeof(Header));
    std::vector<Pixel> patternpixels(patternheader.width * patternheader.height);
    patternfile.read(reinterpret_cast<char*>(patternpixels.data()), patternpixels.size() * sizeof(Pixel));

    Header textheader;
    std::ifstream textfile("input/pattern2.tga", std::ios::binary);
    textfile.read(reinterpret_cast<char*>(&textheader), sizeof(Header));
    std::vector<Pixel> textpixels(textheader.width * textheader.height);
    textfile.read(reinterpret_cast<char*>(textpixels.data()), textpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;

    for(int i = 0; i < layerheader.width * layerheader.height; i++) {
        Pixel temp;
        //multiply layerheader with patternheader
        temp.red = ((layerpixels[i].red * patternpixels[i].red) / 255.0f) + 0.5f;
        temp.green = ((layerpixels[i].green * patternpixels[i].green) / 255.0f) + 0.5f;
        temp.blue = ((layerpixels[i].blue * patternpixels[i].blue) / 255.0f) + 0.5f;

        endresult.push_back(temp);
    }
    std::vector<Pixel> finalresult;
    for(int i = 0; i < layerheader.width * layerheader.height; i++) {
        Pixel temp;

        float firstred = endresult[i].red / 255.0;
        float secondred = textpixels[i].red / 255.0;

        float firstblue = endresult[i].blue / 255.0;
        float secondblue = textpixels[i].blue / 255.0;

        float firstgreen = endresult[i].green / 255.0;
        float secondgreen = textpixels[i].green / 255.0;

        float red = firstred - secondred;
        float green = firstgreen - secondgreen;
        float blue = firstblue - secondblue;

        if(red < 0) {
            red = 0;
        }

        if(green < 0) {
            green = 0;
        }

        if(blue < 0) {
            blue = 0;
        }

        temp.red = static_cast<unsigned char>(red * 255 + 0.5f);
        temp.green = static_cast<unsigned char>(green * 255 + 0.5f);
        temp.blue = static_cast<unsigned char>(blue * 255 + 0.5f);


        finalresult.push_back(temp);

    }




    std::ofstream outFile("output/part4.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&layerheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(finalresult.data()), finalresult.size() * sizeof(Pixel));
    outFile.close();
}

int example5() {
    Header layerheader;
    std::ifstream layerfile("input/layer1.tga", std::ios::binary);
    layerfile.read(reinterpret_cast<char*>(&layerheader), sizeof(Header));
    std::vector<Pixel> layerpixels(layerheader.width * layerheader.height);
    layerfile.read(reinterpret_cast<char*>(layerpixels.data()), layerpixels.size() * sizeof(Pixel));

    Header patternheader;
    std::ifstream patternfile("input/pattern1.tga", std::ios::binary);
    patternfile.read(reinterpret_cast<char*>(&patternheader), sizeof(Header));
    std::vector<Pixel> patternpixels(patternheader.width * patternheader.height);
    patternfile.read(reinterpret_cast<char*>(patternpixels.data()), patternpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;


    for(int i = 0; i < layerheader.width * layerheader.height; i++) {
        Pixel temp;
        float firstred = layerpixels[i].red / 255.0;
        float secondred = patternpixels[i].red / 255.0;

        float firstblue = layerpixels[i].blue / 255.0;
        float secondblue = patternpixels[i].blue / 255.0;

        float firstgreen = layerpixels[i].green / 255.0;
        float secondgreen = patternpixels[i].green / 255.0;

        //second is B aka background
        float red;
        if(secondred > 0.5) {
            red = 1.0 - 2.0 * (1.0 - firstred) * (1.0 - secondred);
        }
        else {
            red = 2.0 * firstred * secondred;
        }
        float green;
        if(secondgreen > 0.5) {
            green = 1.0 - 2.0 * (1.0 - firstgreen) * (1.0 - secondgreen);
        }
        else {
            green = 2.0 * firstgreen * secondgreen;
        }
        float blue;
        if(secondblue > 0.5) {
            blue = 1.0 - 2.0 * (1.0 - firstblue) * (1.0 - secondblue);
        }
        else {
            blue = 2.0 * firstblue * secondblue;
        }


        if(red > 1) {
            red = 1;
        }

        if(green > 1) {
            green = 1;
        }

        if(blue > 1) {
            blue = 1;
        }

        temp.red = 0.5f + red * 255;
        temp.green = 0.5f + green * 255;
        temp.blue = blue * 255 + 0.5f;


        endresult.push_back(temp);
    }


    std::ofstream outFile("output/part5.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&layerheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(endresult.data()), endresult.size() * sizeof(Pixel));
    outFile.close();

}

int example6() {
    Header carheader;
    std::ifstream carfile("input/car.tga", std::ios::binary);
    carfile.read(reinterpret_cast<char*>(&carheader), sizeof(Header));
    std::vector<Pixel> carpixels(carheader.width * carheader.height);
    carfile.read(reinterpret_cast<char*>(carpixels.data()), carpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;

    for(int i = 0; i < carheader.width * carheader.height; i++) {
        Pixel temp;
        double green = static_cast<double>(carpixels[i].green)  + 200.0;
        if(green > 255) {
        green = 255;
        }

        temp.red = carpixels[i].red;
        temp.green = static_cast<unsigned char>(green);
        temp.blue = carpixels[i].blue;

        endresult.push_back(temp);

    }


    std::ofstream outFile("output/part6.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&carheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(endresult.data()), endresult.size() * sizeof(Pixel));
    outFile.close();
}

int example7() {
    Header carheader;
    std::ifstream carfile("input/car.tga", std::ios::binary);
    carfile.read(reinterpret_cast<char*>(&carheader), sizeof(Header));
    std::vector<Pixel> carpixels(carheader.width * carheader.height);
    carfile.read(reinterpret_cast<char*>(carpixels.data()), carpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;

    for(int i = 0; i < carheader.width * carheader.height; i++) {
        Pixel temp;
        double red = static_cast<double>(carpixels[i].red) * 4;
        if(red > 255) {
            red = 255;
        }

        temp.green = carpixels[i].green;
        temp.red = static_cast<unsigned char>(red);
        temp.blue = 0;

        endresult.push_back(temp);

    }


    std::ofstream outFile("output/part7.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&carheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(endresult.data()), endresult.size() * sizeof(Pixel));
    outFile.close();
}

int example8() {
    Header carheader;
    std::ifstream carfile("input/car.tga", std::ios::binary);
    carfile.read(reinterpret_cast<char*>(&carheader), sizeof(Header));
    std::vector<Pixel> carpixels(carheader.width * carheader.height);
    carfile.read(reinterpret_cast<char*>(carpixels.data()), carpixels.size() * sizeof(Pixel));

    std::vector<Pixel> redresult;
    std::vector<Pixel> blueresult;
    std::vector<Pixel> greenresult;


    for(int i = 0; i < carheader.width * carheader.height; i++) {
        Pixel temp1;
        Pixel temp2;
        Pixel temp3;

        temp1.red = carpixels[i].red;
        temp1.green = carpixels[i].red;
        temp1.blue = carpixels[i].red;

        redresult.push_back(temp1);

        temp2.green = carpixels[i].green;
        temp2.red = carpixels[i].green;
        temp2.blue = carpixels[i].green;


        greenresult.push_back(temp2);


        temp3.blue = carpixels[i].blue;
        temp3.red = carpixels[i].blue;
        temp3.green = carpixels[i].blue;
        blueresult.push_back(temp3);


    }


    std::ofstream outFile("output/part8_b.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&carheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(blueresult.data()), blueresult.size() * sizeof(Pixel));
    outFile.close();

    std::ofstream routFile("output/part8_r.tga", std::ios::binary);
    routFile.write(reinterpret_cast<const char*>(&carheader), sizeof(Header));
    routFile.write(reinterpret_cast<const char*>(redresult.data()), redresult.size() * sizeof(Pixel));
    routFile.close();

    std::ofstream goutFile("output/part8_g.tga", std::ios::binary);
    goutFile.write(reinterpret_cast<const char*>(&carheader), sizeof(Header));
    goutFile.write(reinterpret_cast<const char*>(greenresult.data()), greenresult.size() * sizeof(Pixel));
    goutFile.close();
}

int example10() {
    Header carheader;
    std::ifstream carfile("input/text2.tga", std::ios::binary);
    carfile.read(reinterpret_cast<char*>(&carheader), sizeof(Header));
    std::vector<Pixel> carpixels(carheader.width * carheader.height);
    carfile.read(reinterpret_cast<char*>(carpixels.data()), carpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;

    for(int i = 0; i < carheader.width * carheader.height; i++) {
        Pixel temp;

        temp.green = carpixels[carheader.width * carheader.height - i - 1].green;
        temp.red = carpixels[carheader.width * carheader.height - i - 1].red;
        temp.blue = carpixels[carheader.width * carheader.height - i - 1].blue;

        endresult.push_back(temp);

    }


    std::ofstream outFile("output/part10.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&carheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(endresult.data()), endresult.size() * sizeof(Pixel));
    outFile.close();
}

int example9() {
    Header redheader;
    std::ifstream redfile("input/layer_red.tga", std::ios::binary);
    redfile.read(reinterpret_cast<char*>(&redheader), sizeof(Header));
    std::vector<Pixel> redpixels(redheader.width * redheader.height);
    redfile.read(reinterpret_cast<char*>(redpixels.data()), redpixels.size() * sizeof(Pixel));

    Header blueheader;
    std::ifstream bluefile("input/layer_blue.tga", std::ios::binary);
    bluefile.read(reinterpret_cast<char*>(&blueheader), sizeof(Header));
    std::vector<Pixel> bluepixels(blueheader.width * blueheader.height);
    bluefile.read(reinterpret_cast<char*>(bluepixels.data()), bluepixels.size() * sizeof(Pixel));

    Header greenheader;
    std::ifstream greenfile("input/layer_green.tga", std::ios::binary);
    greenfile.read(reinterpret_cast<char*>(&greenheader), sizeof(Header));
    std::vector<Pixel> greenpixels(greenheader.width * greenheader.height);
    greenfile.read(reinterpret_cast<char*>(greenpixels.data()), greenpixels.size() * sizeof(Pixel));

    std::vector<Pixel> endresult;

    for(int i = 0; i < blueheader.width * blueheader.height; i++) {
        Pixel temp;

        temp.green = greenpixels[i].green;
        temp.red = redpixels[i].red;
        temp.blue = bluepixels[i].blue;

        endresult.push_back(temp);

    }


    std::ofstream outFile("output/part9.tga", std::ios::binary);
    outFile.write(reinterpret_cast<const char*>(&redheader), sizeof(Header));
    outFile.write(reinterpret_cast<const char*>(endresult.data()), endresult.size() * sizeof(Pixel));
    outFile.close();
}

int main() {
    example1();
    example2();
    example3();
    example4();
    example5();
    example6();
    example7();
    example8();
    example9();
    example10();
}