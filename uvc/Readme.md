# Captura de imagenes usand UVC  

## Requerimientos  

### software de compilación 

```bash
sudo apt-get install build-essential cmake 
```

### Instalar libusb 

```bash
sudo apt-get install libusb-1.0-0-dev 
```

### Instalar libuvc 

```bash
git clone https://github.com/libuvc/libuvc.git 

cd libuvc 

mkdir build 

cd build 

cmake -DCMAKE_BUILD_TYPE=Release .. 

make && make install 
```

 

## Compilar el codigo 

```bash
make 
```

 

## Ejecutar 

```bash
./capture_frames 
```

Las imágenes capturadas estarán guardadas en la carpeta images 

 

## Observaciones 

Si el programa falla en ejecutar se recomienda ejecutarlo de la siguiente manera 

```bash
sudo su 
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH 
./capture_frames 
```

 
