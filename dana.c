#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void managment(){

    char og[16];
    snprintf(og, 16, "input_%i.bmp", omp_get_thread_num()+1);
    char ex[16];
    snprintf(ex, 16, "output_%i.bmp", omp_get_thread_num()+1);
    FILE *image, *outputImage;
    image = fopen(og,"rb");          //Imagen original a transformar
    outputImage = fopen(ex,"wb");    //Imagen transformada
    long ancho;
    long alto;
    unsigned char r, g, b;               //Pixel
    unsigned char* ptr;
    unsigned char xx[54];
    int cuenta = 0;
    for(int i=0; i<54; i++){
      xx[i] = fgetc(image);
      fputc(xx[i], outputImage);   //Copia cabecera a nueva imagen
    }
    ancho = (long)xx[20]*65536+(long)xx[19]*256+(long)xx[18];
    alto = (long)xx[24]*65536+(long)xx[23]*256+(long)xx[22];

    printf("%li\n", alto);
    printf("%li\n", ancho);
    ptr = (unsigned char*)malloc(alto*ancho*3*3*sizeof(unsigned char));
    while(!feof(image)){
      b = fgetc(image);
      g = fgetc(image);
      r = fgetc(image);
      unsigned char pixel = 0.21*r+0.72*g+0.07*b;
      ptr[cuenta] = pixel; //b
      ptr[cuenta+1] = pixel; //g
      ptr[cuenta+2] = pixel; //r
      cuenta+=3;
    }                                        //Grises
    for(int j=0; j<alto-1; j++){
      for(int i=(ancho*3); i>2; i-=3){
        fputc(ptr[(ancho*j*3)+i+162], outputImage);
        fputc(ptr[(ancho*j*3)+1+i+162], outputImage);
        fputc(ptr[(ancho*j*3)+2+i+162], outputImage);
    }
  }
    free(ptr);
    fclose(image);
    fclose(outputImage);
}

int main(){
  int thr=20;
  omp_set_num_threads(thr);
  #pragma omp parallel
    managment();
}
