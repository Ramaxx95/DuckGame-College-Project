#!/bin/bash

set -e  # Terminar el script si ocurre un error

# Variables de instalación
PREFIX="/opt/DuckGame"
BINDIR="$PREFIX/bin"
SHAREDIR="$PREFIX/share/DuckGame"
CLIENT_EXECUTABLE="taller_client"
SERVER_EXECUTABLE="taller_server"
BUILD_DIR="build"

# Colores para mensajes
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'

# Mensaje de bienvenida
echo -e "${GREEN}Instalador de DuckGame para Ubuntu.${NC}"

# Instalar dependencias necesarias si no están instaladas
echo -e "${GREEN}Instalando dependencias necesarias...${NC}"

apt update
apt install -y build-essential \
 cmake \
 libsdl2-dev \
 libsdl2-image-dev \
 libsdl2-ttf-dev \
 libyaml-cpp-dev \
 libsdl2-mixer-dev \
 qtbase5-dev \
 qttools5-dev-tools \
 qtdeclarative5-dev

# Compilar el cliente y el servidor
echo -e "${GREEN}Compilando el cliente y el servidor...${NC}"
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
cmake ..
make

# Verificar que los ejecutables se hayan creado
if [ ! -f "$CLIENT_EXECUTABLE" ] || [ ! -f "$SERVER_EXECUTABLE" ]; then
    echo -e "${RED}Error: No se encontraron los ejecutables tras la compilación.${NC}"
    exit 1
fi

# Confirmación
echo -e "${GREEN}Instalación completa.${NC}"
echo -e "Ejecuta '${CLIENT_EXECUTABLE}' para iniciar el cliente."
echo -e "Ejecuta '${SERVER_EXECUTABLE}' para iniciar el servidor."
