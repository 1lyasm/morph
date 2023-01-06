#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>

#include "image_processing.h"
#include "image.h"

int **resimOku(char *resimadi){
	
    int M, N, Q; // rows, cols, grayscale
    bool type;
    
	// read image header
    readImageHeader(resimadi, N, M, Q, type);

    // allocate memory for the image array
    Image image(N, M, Q);
    
    //Image outImage();
    Image tempImage(image);
    Image secImage(image);

    // read image
    readImage(resimadi, image);
    
    int i,j;
    int **resim = (int **) malloc(sizeof(int*)*M);		
    
    for(i=0;i<N;i++){
    	resim[i] = (int*) malloc(sizeof(int)*M);
    	for(j=0;j<M;j++){
    		resim[i][j] = image.getPixelVal(i,j);
    	}
    }

    return resim;
}

void resimYaz(char *resimadi, short *resim, int N, int M, int Q){
	
	Image image(N, M, Q);
    Image tempImage(image);
    Image secImage(image);
    
    int i,j;
    for(i = 0; i < N; i++){
    	for(j = 0; j < M; j++){
    		image.setPixelVal(i, j, resim[i*N+j]);
    	}
    }
    
	writeImage(resimadi, image);
}

int readImage(char fname[], Image& image)
{
    int i, j;
    int N, M, Q;
    unsigned char *charImage;
    char header [100], *ptr;
    std::ifstream ifp;

    ifp.open(fname, std::ios::in | std::ios::binary);

    if (!ifp) 
    {
        std::cout << "Can't read image: " << fname << std::endl;
        exit(1);
    }

 // read header

    ifp.getline(header,100,'\n');
    if ( (header[0]!=80) || (header[1]!=53) ) 
    {   
        std::cout << "Image " << fname << " is not PGM" << std::endl;
        exit(1);
    }

    ifp.getline(header,100,'\n');
    while(header[0]=='#')
        ifp.getline(header,100,'\n');

    M=strtol(header,&ptr,0);
    N=atoi(ptr);

    ifp.getline(header,100,'\n');
    Q=strtol(header,&ptr,0);

    charImage = (unsigned char *) new unsigned char [M*N];

    ifp.read( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

    if (ifp.fail()) 
    {
        std::cout << "Image " << fname << " has wrong size" << std::endl;
        exit(1);
    }

    ifp.close();

 //
 // Convert the unsigned characters to integers
 //

    int val;

    for(i=0; i<N; i++)
        for(j=0; j<M; j++) 
        {
            val = (int)charImage[i*M+j];
            image.setPixelVal(i, j, val);     
        }

    delete [] charImage;


    return (1);

}

int readImageHeader(char fname[], int& N, int& M, int& Q, bool& type)
{
    char header [100], *ptr;
    std::ifstream ifp;
    ifp.open(fname, std::ios::in | std::ios::binary);

    if (!ifp){
        std::cout << "Boyle bir resim bulunamadi: " << fname << std::endl;
        return(2);
    }

 // read header

    type = false; // PGM

    ifp.getline(header,100,'\n');
    if ( (header[0] == 80) && (header[1]== 53) ) 
    {  
      type = false;
    }
    else if ( (header[0] == 80) && (header[1] == 54) ) 
    {       
      type = true;
    } 
    else 
    {
        std::cout << "Resim PGM formatinda olmali:  " << fname << std::endl;
        return(3);
    }

    ifp.getline(header,100,'\n');
    while(header[0]=='#')
        ifp.getline(header,100,'\n');

    M=strtol(header,&ptr,0);
    N=atoi(ptr);

    ifp.getline(header,100,'\n');

    Q=strtol(header,&ptr,0);

    ifp.close();

    return(1);
}

int writeImage(char fname[], Image& image)
{
    int i, j;
    int N, M, Q;
    unsigned char *charImage;
    std::ofstream ofp;

    image.getImageInfo(N, M, Q);

    charImage = (unsigned char *) new unsigned char [M*N];

 // convert the integer values to unsigned char

    int val;

    for(i=0; i<N; i++)
    {
        for(j=0; j<M; j++) 
        {
        val = image.getPixelVal(i, j);
        charImage[i*M+j]=(unsigned char)val;
        }
    }

    ofp.open(fname, std::ios::out | std::ios::binary);

    if (!ofp) 
    {
        std::cout << "Can't open file: " << fname << std::endl;
        exit(1);
    }

    ofp << "P5" << std::endl;
    ofp << M << " " << N << std::endl;
    ofp << Q << std::endl;

    ofp.write( reinterpret_cast<char *>(charImage), (M*N)*sizeof(unsigned char));

    if (ofp.fail()) 
    {
        std::cout << "Can't write image " << fname << std::endl;
        exit(0);
    }

    ofp.close();

    delete [] charImage;

    return(1);

}
