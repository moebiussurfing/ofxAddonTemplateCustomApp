#!/bin/bash

# Buscar archivos 'addons.make' en todas las subcarpetas
files=$(find . -name "addons.make")

# Iterar por cada archivo encontrado
for file in $files
do
  # Buscar si la línea 'ofxSurfingHelpers' está presente en el archivo
  grep -q "ofxSurfingHelpers" $file

  # Si la línea está presente, eliminarla
  if [ $? -eq 0 ]
  then
    sed -i '/ofxSurfingHelpers/d' $file
    echo "La línea 'ofxSurfingHelpers' se eliminó del archivo $file"
  fi
done
