// introductory_image.hpp
#ifndef INTRODUCTORY_IMAGE_HPP
#define INTRODUCTORY_IMAGE_HPP

#include "Texture.hpp" 

class IntroductoryImage {
public:
    IntroductoryImage();
    ~IntroductoryImage();

    // Load the introductory image
    bool loadIntroductoryImage();

    // Render the introductory image
    void render();

private:
    LTexture mIntroTexture; 
};

#endif // INTRODUCTORY_IMAGE_HPP
