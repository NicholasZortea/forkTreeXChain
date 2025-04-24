#!/bin/bash

# Verifica se o argumento foi passado
if [ $# -ne 1 ]; then
    echo "Uso: $0 <valor_para_argumento_da_main>"
    exit 1
fi

ARG=$1

for i in $(seq 1 20); do
    echo -e "\n--- Execução $i ---"
    ./teste "$ARG"
done
