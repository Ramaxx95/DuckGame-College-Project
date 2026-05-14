# Usar una imagen base de Ubuntu
FROM ubuntu:22.04

# Establecer variables de entorno
ENV DEBIAN_FRONTEND=noninteractive

# Copiar el script de instalación
COPY installer.sh /installer.sh

# Copiar el código fuente del juego
COPY . /game

# Establecer el directorio de trabajo
WORKDIR /game

# Otorgar permisos de ejecución al script
RUN chmod +x /installer.sh

# Ejecutar el script de instalación
RUN apt update && apt install -y libxcb1 libx11-6 libxcb-xinerama0 libxcb-xinerama0 libxcb-xinerama0-dev libxcb-cursor0 bash && /installer.sh

# Establecer variables de entorno para el juego
ENV QT_QPA_PLATFORM_PLUGIN_PATH=/usr/lib/qt5/plugins/platforms
ENV QT_QPA_PLATFORM=xcb

# Establecer directorio de trabajo final donde estará el juego instalado
WORKDIR /opt/DuckGame

# Comando por defecto
CMD ["/bin/bash"]