# Captura de imagenes usando gstreamer

## Requerimientos  

### Instalar gstreamer

```bash
apt-get install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudi
```

### software de compilación 

```bash
sudo apt-get install build-essential cmake 
```

## Ejecutar (modo visualizador)

```bash
./demo
```

* En la consola aplastar <Enter> para finalizar el programa

## Ejecutar (modo captura)

```bash
./demo [[numero de imagenes]]
```

* Las imágenes capturadas estarán guardadas en la carpeta images 
 

