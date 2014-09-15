

#ifndef KOS_CustomButton_h
#define KOS_CustomButton_h

#include "../shameConfig.h"


class CustomButton : public ImageButton
{
public:
    CustomButton()
    {
        File offImgFile("/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/GUI_Resources/testButton-off.png");
        File onImgFile("/Users/brianhansen/Documents/Brian/Work/1_KOS/kissofshame/GUI_Resources/testButton-on.png");
//        File offImgFile(String(GUI_PATH) + "testButton-off.png");
//        File onImgFile(String(GUI_PATH) + "testButton-on.png");
        
        //std::cout << "off file = " << offImgFile.getFullPathName() << std::endl;
        
        offImage = ImageCache::getFromFile(offImgFile);
        onImage = ImageCache::getFromFile(onImgFile);
        
        setImages(true, false, true, onImage, 1.0f, Colour(0x0), Image(), 1.0f, Colour(0x0), offImage, 1.0f, Colour(0x0));
        
        setSize(onImage.getWidth(), onImage.getHeight());
    }
    
    ~CustomButton(){}
    
    //void paint (Graphics& g){};

    void resizeButton(float scale)
    {
        offImage = offImage.rescaled(offImage.getWidth()*scale, offImage.getHeight()*scale);
        onImage = onImage.rescaled(onImage.getWidth()*scale, onImage.getHeight()*scale);
//        setImages(true, false, true, onImage, 1.0f, Colour(0x0), Image(), 1.0f, Colour(0x0), offImage, 1.0f, Colour(0x0));
//        repaint();
    }
    
     void setImagePaths(String onImgPath, String offImgPath)
     {
         File offImgFile(String(GUI_PATH) +onImgPath);
         File onImgFile(String(GUI_PATH) + offImgPath);

         offImage = ImageCache::getFromFile(offImgFile);
         onImage = ImageCache::getFromFile(onImgFile);
     
         setImages(true, false, true, onImage, 1.0f, Colour(0x0), Image(), 1.0f, Colour(0x0), offImage, 1.0f, Colour(0x0));
     }
    

private:

	Image offImage;
    Image onImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CustomButton)
};

#endif
