# Captura de Fotogramas de Webcam en Múltiples Lenguajes

Este proyecto demuestra cómo abrir webcams y capturar fotogramas usando varios lenguajes de programación y bibliotecas. Proporciona ejemplos en C con GStreamer, C++ con OpenCV, Python con OpenCV, C con libuvc y C con Video4Linux2. El objetivo es ofrecer una comparación integral y ejemplos prácticos para desarrolladores que trabajan con diferentes tecnologías.

## Descripción

El proyecto incluye implementaciones para capturar fotogramas de webcam utilizando tanto bibliotecas de alto nivel como de bajo nivel:

1. **GStreamer (C)**: GStreamer es un potente framework multimedia que permite el manejo complejo de medios, incluida la captura, el procesamiento y la transmisión de video. Se considera una biblioteca de alto nivel debido a su extensa abstracción y versatilidad.

2. **OpenCV (C++)**: OpenCV es una biblioteca de visión por computadora y aprendizaje automático de código abierto. Proporciona una interfaz simple y fácil de usar para la captura y el procesamiento de video, lo que la convierte en una biblioteca de alto nivel adecuada para el desarrollo rápido y la creación de prototipos.

3. **OpenCV (Python)**: Las vinculaciones de Python para OpenCV ofrecen la misma funcionalidad que la API de C++, pero con la facilidad y flexibilidad de Python. Esto la hace altamente accesible para el desarrollo rápido y la experimentación.

4. **libuvc (C)**: libuvc es una biblioteca para dispositivos de Clase de Video USB (UVC), que proporciona acceso de bajo nivel al hardware de la webcam. Se considera una biblioteca de bajo nivel debido a su interacción directa con el protocolo USB y el control del hardware.

5. **Video4Linux2 (C)**: Video4Linux2 (V4L2) es una API de bajo nivel para la captura de video en Linux. Proporciona acceso directo a los dispositivos de captura de video, brindando a los desarrolladores un control detallado sobre el hardware.

6. **Opencv tcam (C++):** Tcam es el SDK más reciente desarrollado por la empresa *TheImagingSource*. Proporciona una API de alto nivel para interactuar con sus cámaras.

## Contenido

El proyecto está organizado en directorios, cada uno de los cuales contiene la implementación para una biblioteca y un lenguaje específicos:

- `gstreamer_c/`: Ejemplo de captura de fotogramas usando GStreamer en C.
- `opencv_cpp/`: Ejemplo de captura de fotogramas usando OpenCV en C++.
- `opencv_python/`: Ejemplo de captura de fotogramas usando OpenCV en Python.
- `uvc_c/`: Ejemplo de captura de fotogramas usando libuvc en C.
- `video4linux_c/`: Ejemplo de captura de fotogramas usando Video4Linux2 en C.
- `opencv_tcam_cpp/`: Ejemplo de captura fotogramas usando tcam library y opencv en C++.

## Prerrequisitos

Cada implementación requiere que se instalen dependencias específicas. A continuación, se muestra una lista de las dependencias necesarias para cada implementación:

### GStreamer (C)

- Bibliotecas de desarrollo de GStreamer
- `sudo apt install libgstreamer1.0-dev`
- `sudo apt install libgstreamer-plugins-base1.0-dev`

### OpenCV (C++)

- Bibliotecas de desarrollo de OpenCV
- `sudo apt install libopencv-dev`

### OpenCV (Python)

- Paquete de OpenCV para Python
- `pip install opencv-python`

### libuvc (C)

- Bibliotecas de desarrollo de libusb
- `sudo apt install libusb-1.0-0-dev`
- `sudo apt install libuvc`

### Video4Linux2 (C)

- Bibliotecas de desarrollo de V4L2
- Generalmente incluidas con los encabezados del kernel de Linux

## Configuración y Uso

Instrucciones adicionales sobre cómo compilar y ejecutar cada proyecto se pueden encontrar dentro de cada proyecto.

