// introductory_image.cpp
#include "introductory_image.hpp"

IntroductoryImage::IntroductoryImage() {}

IntroductoryImage::~IntroductoryImage() {}

bool IntroductoryImage::loadIntroductoryImage() {
    // Load the introductory image
    return mIntroTexture.loadFromFile("background1.png");
}

void IntroductoryImage::render() {
    // Render the introductory image
    mIntroTexture.render(0, 0); // Adjust position as needed
}
