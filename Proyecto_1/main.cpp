#include <iostream>
#include <fstream>
#include <inttypes.h> //para convertir de big endian a little endian



struct Type
{
  char id [8];
}__attribute__((packed));


struct BMPHeader
{
  char BMPType [2];
  uint32_t BmpSize;
	uint16_t Reservado1;
  uint16_t Reservado2;
	uint32_t offset;
	uint32_t Tamano_Header;
	uint32_t width;     
	uint32_t height;    
}__attribute__((packed));


struct PNGHeader
{
  char pngid[16];
  uint32_t width;
  uint32_t heigth;
}__attribute__((packed));

struct JPGHeader
{
  char jpgid [14];
  uint16_t HorizontalRes;
  uint16_t VertiaclRes;
  uint8_t HorizontalPixel;
  uint8_t VerticalPixel;
}__attribute__((packed));

struct WindowsHeader
{
  uint32_t signature;
  uint16_t MachineType;
}__attribute__((packed));

struct LinuxHeader
{
  char Linuxid [4];
  uint8_t type;

}__attribute__((packed));




//argumentos de linea de comando
int main(int argc, char *argv[])
{
    if(argc !=2 ){
      std::cerr << "Usage: "<< argv[0]<< "<input file>\n";
      return 1;
    }

    
    //leo el archivo
    std::ifstream in(argv[1]); 

    //verificar si la operacion de tuvo extio
    if(!in.is_open()){
      std::cerr<<"Cannot open file " << argv[1] <<"\n";
      return 2;
    }
    //std::cout<<"File opened succesfully\n";
    

//============================================================================================================//

    Type t;

    BMPHeader bmp;
    PNGHeader png;
    JPGHeader jpg;
    WindowsHeader win;
    LinuxHeader lx;
   

    //castea el archivo en la variable para indentificar que tipo de archivo es
    in.read(reinterpret_cast<char*>(&t), sizeof (Type));

    /*std::cout << t.id[0] << std::endl;
    std::cout << t.id[1] << std::endl;
    std::cout << t.id[2] << std::endl;
    std::cout << t.id[3] << std::endl;
    std::cout << t.id[4] << std::endl;
    std::cout << t.id[5] << std::endl;
    std::cout << t.id[6] << std::endl;
    std::cout << t.id[7] << std::endl;*/

  

    if(t.id[1] == 0x50 && t.id[2] == 0x4E && t.id[3] == 0x47){ //PNG PNG PNG
      std::cout <<"=============================================" << std::endl;
      std::cout << "Es un archivo PNG" << std::endl;
      
      std::ifstream PNGFile (argv[1]);
      PNGFile.read(reinterpret_cast<char*>(&png), sizeof (PNGHeader));
      
      uint32_t WidthLE = __builtin_bswap32(png.width);
      uint32_t HeigthLE = __builtin_bswap32(png.heigth);

      std::cout <<"=============================================" << std::endl;
      std::cout << "Width: " << WidthLE << std::endl;
      std::cout << "Heigth: " << HeigthLE << std::endl;
      std::cout <<"=============================================" << std::endl;


    }else if (t.id[0] == 0x42 && t.id[1] == 0x4D){ //BMP BMP BMP
      std::cout <<"=============================================" << std::endl;
      std::cout << "Es un archivo Bmp" << std::endl;
      
      std::ifstream BMPFile (argv[1]);
      BMPFile.read(reinterpret_cast<char*>(&bmp), sizeof (BMPHeader));
      std::cout <<"=============================================" << std::endl;
      std::cout << "Width: " << bmp.width << std::endl;
      std::cout << "Heigth: " << bmp.height << std::endl;
      std::cout <<"=============================================" << std::endl;

    }else if( (int) t.id[0] == -1 && (int) t.id[1] == -40 && (int) t.id[2] == -1){ //JPG JPG JPG
      std::cout <<"=============================================" << std::endl;
      std::cout << "Es un archivo JPG" << std::endl;
      std::ifstream JPGFile (argv[1]);
      JPGFile.read(reinterpret_cast<char*>(&jpg), sizeof (JPGHeader));
      std::cout <<"=============================================" << std::endl;
      std::cout << "Horizontal Resolution: " << jpg.HorizontalRes << std::endl;
      std::cout << "Vertical Resolution: " << jpg.VertiaclRes << std::endl;
      std::cout << "Horizontal Pixel Count: " << (int) jpg.HorizontalPixel << std::endl;
      std::cout << "Vertical Pixel Count: " << (int) jpg.VerticalPixel << std::endl;
      std::cout <<"=============================================" << std::endl;

    }else if(t.id[0] == 0x47 && t.id[1] == 0x49 && t.id[2] == 0x46){ //GIF GIF GIF
      std::cout << "Es un archivo GIF" << std::endl;
    }else if(t.id[0] == 0x49 && t.id[1] == 0x49 && t.id[2] == 0x2A && t.id[3] == 0x00){ //TIFF TIFF TIFF  
      std::cout << "Es un archivo TIFF" << std::endl; 
    }else if(t.id[1] == 0x73 && t.id[2] == 0x76 && t.id[3] == 0x67){ //SVG SVG SVG
      std::cout << "Es un archivo SVG"<< std::endl;
    }else if(t.id[0] == 0x4D && t.id[1] == 0x5A){ //WINDOWS WINDOWS WINDOWS
    
      std::ifstream WinFile (argv[1]);
      
      WinFile.seekg(0x80);
      int pos = WinFile.tellg();
      WinFile.read(reinterpret_cast<char*>(&win), sizeof (WindowsHeader));

      uint32_t SigLE = __builtin_bswap32(win.signature);

      if(SigLE == 1346699264){
        std::cout <<"=============================================" << std::endl;
        std::cout << "Es un ejecutable de Windows" << std::endl;
        if(win.MachineType == 0x8664){
          std::cout << "Machine Type: x64" << std::endl;
          std::cout <<"=============================================" << std::endl;
        }else if(win.MachineType == 0x14C){
          std::cout << "Machine Type: x32" << std::endl;
          std::cout <<"=============================================" << std::endl;
        }
      }else{
        std::cout << "DOS executable" << std::endl;
      }

    }else if(t.id[0] == 0x7F && t.id[1] == 0x45 && t.id[2] == 0x4c && t.id[3] == 0x46){// LINUX LINUX LINUX 
      std::cout <<"=============================================" << std::endl;
      std::cout << "Es un ejecutable de Linux" << std::endl;
      std::cout <<"=============================================" << std::endl;
      std::ifstream LinuxFile (argv[1]);
      LinuxFile.read(reinterpret_cast<char*>(&lx), sizeof (LinuxHeader));

      if((int) lx.type == 1 ){
        std::cout << "Class: 32 bit format" << std::endl;
      }else if ((int) lx.type == 2){
        std::cout << "Class: 64 bit format" << std::endl;
      }else{
        std::cout << "No valido" << std::endl;
      }
      std::cout <<"=============================================" << std::endl;

    }else if(t.id[0] == 0x50  && t.id[1] == 0x4B){ //ZIP ZIP ZIP
      std::cout << "Es un archivo ZIP" << std::endl;
    }else if(t.id[0] == 0x52 && t.id[1] == 0x61 && t.id[2] == 0x72 && t.id[3] == 0x21 && t.id[4] == 0x1A && t.id[5] == 0x07){ //RAR RAR RAR 
      std::cout << "Este es un archivo RAR" << std::endl;
    }else if ( (int)t.id[0] == -3 && t.id[1] == 0x37 && t.id[2] == 0x7A && t.id[3] == 0x58 && t.id[4] == 0x5A){ // XZ XZ XZ 
    std::cout << "Es un archivo XZ" << std::endl;
    }else if(t.id[0] == 0x42 && t.id[1] == 0x5A && t.id[2] == 0x68){ //BZIP BZIP BZIP
    std::cout << "Es un arhcivo BZIP" << std::endl;
    }else if( (int) t.id[0] == 31 && (int) t.id[1] == -117){ //GZIP GZIP GZIP 
      std::cout << "Es un archivo GZIP" << std::endl;
    }else{
      std::cerr<<"Archivo no valido " << argv[1] <<"\n";
    }


    in.close(); 
    return 0;
}
