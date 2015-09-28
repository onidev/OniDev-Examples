#include <onidev.h>

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        printf("Usage:\nalpharemove image_path\n");
        return 1;
    }
    
    std::string file;
    for(int i=1; i<argc; i+=1) {
        file += std::string(argv[i]);
        if(i != argc - 1) file += " ";
    }
    
    od::Image im(file);
    unsigned int alpha = im.getpixel(0, im.height() - 1);
    unsigned int * c = const_cast<unsigned int*>( im.getpixels() );
    unsigned int alphaColor = 0xFF00FF; // fushia
    
    if(alpha >> 24 == 0) // alpha == 0
    {
        if(alpha != 0)
            alphaColor = alpha;
    }
    
    const int size = im.width() * im.height();
    for(int i=0; i<size; ++i)
    {
        if(*c == alpha || *c >> 24 == 0)
        {
            *c = alphaColor;
        }
        c++;
    }
    
    im.save(file, "png");
    
    return 0;
}
