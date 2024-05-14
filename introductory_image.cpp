#include "introductory_image.hpp"

IntroductoryImage::IntroductoryImage() {}

IntroductoryImage::~IntroductoryImage() {}

bool IntroductoryImage::loadIntroductoryImage() {
    return mIntroTexture.loadFromFile("background1.png");
}

void IntroductoryImage::render() {
    mIntroTexture.render(0, 0); 
}
