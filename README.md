# Compilando a biblioteca

git clone https://github.com/Bigsho0t/LibCollections.git

cd LibCollections

make

gcc meu_programa.c \
    -ILibCollections/include \
    -LLibCollections \
    -lcollections \
    -o meu_programa
