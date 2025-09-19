# Projeto-Pratico
Projeto realizado no primeiro período do curso de Ciência da Computação da UFLA na matéria de Introdução aos Algoritmos.

##📂 Sistema de Cadastro de Carros em C++
📌 Descrição do Projeto
Este projeto é um sistema de cadastro, gerenciamento e consulta de veículos desenvolvido em C++, como parte da disciplina GAC124 - Introdução aos Algoritmos. Seu principal objetivo é aplicar os conceitos fundamentais de algoritmos e estruturas de dados, além de manipular arquivos nos formatos CSV e BIN para garantir persistência de dados.

Lavras, Junho de 2025.

###🛠️ Funcionalidades
-📥 Importação de dados de arquivos .csv e .bin
-🔍 Busca por identificador, modelo, marca ou ano de lançamento
-📊 Ordenação dos dados por identificador, valor, marca ou ano
-💰 Filtragem por faixa de valor
-📄 Visualização completa ou parcial dos veículos
-➕ Adição de novos veículos, com atribuição automática de ID
-💾 Exportação dos dados em .csv e .bin ao encerrar o sistema
-🧱 Estrutura dos Dados
Os veículos são representados por um struct chamado carros com os seguintes campos:

```struct carros {
    int identificador;
    int ano_lancamento;
    char modelo[100];
    char marca[100];
    char descricao[500];
    float valor;
};```
