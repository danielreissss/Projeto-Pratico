/*************************************************************************************************
=============================== PROJETO PRÁTICO - TEMA CARROS ====================================
TURMA 10B
PARTICIPANTES:
- Daniel Reis Araújo
- Gabriel Lima Leandro
- João Vitor Rezende Marciano

CAMPOS:
-Identificador (int)
-Modelo (string)
-Marca (string)
-Ano de lançamento (int)
-Descrição (string)
-Preço (float)

**************************************************************************************************/
#include <iostream>
#include <fstream>
#include <cctype>
#include <iomanip>

using namespace std;

// definição de cores = estética
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define PADRAO "\033[0m"

// Declaração da estrutura "carros", que são os dados de um veículo
struct carros
{
	// Declaração das varíaveis dentro da estrutura "carros"
	int identificador, ano_lancamento; 
	char modelo[100], marca[100], descricao[500];
	float valor;
	bool marcacao = true;
};
//cria uma barra de progresso no terminal = estética
void barraProgresso(int total) {
    for (int i = 0; i < total; i++) {
        for (volatile int j = 0; j < 30000000; j++);
        cout << PADRAO << "|" << flush;
    }
    cout << " 100%\n";
}


// Procedimento de escrever em binário
void escrever_bin(string arquivo, carros *vetor, int tamanho)
{
	const char cabecalho[] = "#Identificador,Modelo,Marca,Ano_de_lançamento,Descrição,Preço";
	ofstream escrita(arquivo, ios::binary);
	escrita.write(reinterpret_cast<const char *>(&cabecalho), sizeof(cabecalho));
	escrita.write(reinterpret_cast<const char *>(vetor), tamanho * sizeof(carros));
	escrita.close();
}

// Leitura dos dados em binário
void ler_bin(carros *&vetor, int &tamanho, int &qtd, string arquivo)
{
	bool controle = true;
	char cabecalho[] = "#Identificador,Modelo,Marca,Ano_de_lançamento,Descrição,Preço";
	ifstream leitura(arquivo, ios::binary);
	leitura.read(reinterpret_cast<char *>(&cabecalho), sizeof(cabecalho));
	int i = 0;
	while (controle)
	{
		if (i >= tamanho)
		{
			// redimensionar vetor
			carros *novo = new carros[tamanho + 5];
			for (int j = 0; j < tamanho; j++)
			{
				novo[j] = vetor[j];
			}
			delete[] vetor; // liberar memória antiga
			vetor = novo;
			tamanho += 5;
		}

		// tenta ler um registro
		if (leitura.read(reinterpret_cast<char *>(&vetor[i]), sizeof(vetor[i])))
		{
			i++; // só incrementa se ler
		}
		else
		{
			controle = false; // fim do arquivo ou erro de leitura
		}
	}
	qtd = i;
}

// Procedimento de Leitura do arquivo com redimensionamento
void ler_csv(carros *&carro, int &tamanho, int &qtd, string arquivo)
{
    // Abre o arquivo CSV para leitura
    ifstream leitura(arquivo);

    // Lê e descarta a primeira linha (cabeçalho)
    string cabecalho;
    getline(leitura, cabecalho);

    int i = 0;        // Contador de elementos lidos
    char lixo;        // Variável auxiliar para descartar caracteres como vírgula e aspas
    carros aux;       // Variável auxiliar para armazenar os dados lidos de cada linha

    // Enquanto conseguir ler um identificador do arquivo (início de uma linha de dados)
    while (leitura >> aux.identificador)
    {
        leitura >> lixo; // Lê e descarta a vírgula após o identificador

        // Lê os campos de texto até encontrar a próxima vírgula
        leitura.getline(aux.modelo, 100, ',');
        leitura.getline(aux.marca, 100, ',');

        // Lê o ano de lançamento
        leitura >> aux.ano_lancamento;

        leitura >> lixo; // Descarta a vírgula
        leitura >> lixo; // Descarta a aspas dupla inicial da descrição

        // Lê a descrição até encontrar a aspas final
        leitura.getline(aux.descricao, 500, '"');

        leitura >> lixo; // Descarta a vírgula após a descrição

        // Lê o valor (float)
        leitura >> aux.valor;

        // Se ainda houver espaço no vetor atual
        if (i < tamanho)
        {
            carro[i] = aux;
            i++;
        }
        else
        {
            // Redimensionamento do vetor caso o limite tenha sido atingido

            // Cria um novo vetor com tamanho aumentado em 1
            carros *novo_carro = new carros[tamanho + 1];

            // Copia os dados do vetor antigo para o novo
            for (int j = 0; j < tamanho; j++)
            {
                novo_carro[j] = carro[j];
            }

            // Libera a memória do vetor antigo
            delete[] carro;

            // Atualiza o ponteiro para apontar para o novo vetor
            carro = novo_carro;

            // Atualiza o tamanho do vetor
            tamanho += 1;

            // Adiciona o novo carro no final do vetor
            carro[i] = aux;
            i++;
        }
    }

    // Atualiza a quantidade total de carros lidos
    qtd = i;
}


void escrever_csv(string arquivo, carros *carro, int tamanho)
{
	string cabecalho;
	ifstream ler("Carros.csv");
	ler >> cabecalho;
	ofstream escrita(arquivo);
	escrita << cabecalho << endl;
	for (int i = 0; i < tamanho; i++)
	{
		escrita << carro[i].identificador << ","
				<< carro[i].modelo << ","
				<< carro[i].marca << ","
				<< carro[i].ano_lancamento << "," << "\""
				<< carro[i].descricao << "\"" << ","
				<< fixed << setprecision(2) << carro[i].valor << endl;
	}
	escrita.close();
}

// Procedimento que imprime parte do arquivo ou o arquivo todo
void show_vetor(carros *carro, int tamanho)
{
	int option;
	cout << YELLOW << "======================== MOSTRAR =======================" << endl;
	cout << PADRAO << "Como deseja ver os dados: \n[1] Todos os dados \n[2] Parte dos dados \nDigite aqui: ";
	cin >> option;
	switch (option)
	{
	case 1:
		for (int i = 0; i < tamanho; i++)
		{
			if (carro[i].marcacao)
			{
				cout << YELLOW << "| " << PADRAO << left << setw(20) << "Identificador: " << carro[i].identificador << endl
					 << YELLOW << "| " << PADRAO << left << setw(20) << "Modelo: " << carro[i].modelo << endl
					 << YELLOW << "| " << PADRAO << left << setw(20) << "Marca: " << carro[i].marca << endl
					 << YELLOW << "| " << PADRAO << left << setw(20) << "Ano de lançamento: " << carro[i].ano_lancamento << endl
					 << YELLOW << "| " << PADRAO << left << setw(20) << "Descricação: " << endl
					 << YELLOW << "| " << PADRAO << carro[i].descricao << endl
					 << fixed << setprecision(2) << YELLOW << "| " << PADRAO << left << setw(20) << "Preço(R$): " << carro[i].valor << endl;
				cout << YELLOW << "+-----------------------------------------------------+\n";
			}
		}
		break;
	case 2:

		int inicio, fim;
		cout  << PADRAO << "Selecione a posicao inicial e a posicao final desejada:" << endl;
		cin >> inicio >> fim;
		// Caso o inicio ou o fim do intervalo da entrada sejam inválidos.
		if (inicio < 0)
		{
			inicio = 0;
		}
		if (fim > tamanho)
		{
			fim = tamanho;
		}
		if (inicio >= fim)
		{
			cout << RED << "Intervalo não encontrado." << endl;
			break;
		}
		for (int i = inicio; i < fim; i++)
		{

			cout << YELLOW << "| " << PADRAO << left << setw(20) << "Identificador: " << carro[i].identificador << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Modelo: " << carro[i].modelo << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Marca: " << carro[i].marca << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Ano de lançamento: " << carro[i].ano_lancamento << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Descricação: " << endl
				 << YELLOW << "| " << PADRAO << carro[i].descricao << endl
				 << fixed << setprecision(2) << YELLOW << "| " << PADRAO << left << setw(20) << "Preço(R$): " << carro[i].valor << endl;
			cout << YELLOW << "+-----------------------------------------------------+\n";
		}
		break;
	}
}

void marcar_vetor_id(carros *vet, int &tamanho, int id)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (id == vet[i].identificador)
		{
			//Falso significa excluido
			vet[i].marcacao = false;
		}
	}
}

void lista_marcados(carros *carro, int &tamanho)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (!carro[i].marcacao)
		{
			cout << YELLOW << "| " << PADRAO << left << setw(20) << "Identificador: " << carro[i].identificador << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Modelo: " << carro[i].modelo << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Marca: " << carro[i].marca << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Ano de lançamento: " << carro[i].ano_lancamento << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Descricação: " << endl
				 << YELLOW << "| " << PADRAO << carro[i].descricao << endl
				 << fixed << setprecision(2) << YELLOW << "| " << PADRAO << left << setw(20) << "Preço(R$): " << carro[i].valor << endl;
			cout << YELLOW << "+-----------------------------------------------------+\n";
		}
	}
}

void desmarcar_vetor_id(carros *vet, int &tamanho, int id)
{
	for (int i = 0; i < tamanho; i++)
	{
		if (id == vet[i].identificador)
		{
			vet[i].marcacao = true;
		}
	}
}

//função de prticionamento do vetor para aplicação do quicksort
int particiona_id(carros *vetor, int indiceInicial, int indiceFinal)
{
	int i = indiceInicial + 1, j = indiceFinal;
	int pivo = vetor[indiceInicial].identificador;
	while (i <= j)
	{
		if (vetor[i].identificador <= pivo)
		{
			i++;
		}
		else if (pivo <= vetor[j].identificador)
		{
			j--;
		}
		else
		{
			swap(vetor[i], vetor[j]);
			i++;
			j--;
		}
	}
	swap(vetor[indiceInicial], vetor[j]);
	return j;
}

// Procedimento que implementa o QuickSort com base no ID dos carros
void quicksort_id(carros *&vetor, int indicePivo, int indiceFinal)
{
	int novoIndicepivo;
	if (indicePivo < indiceFinal)  // verifica se existe mais de um elemento no intervalo atual para continuar a ordenação
	{
		novoIndicepivo = particiona_id(vetor, indicePivo, indiceFinal); // chama a função de particionamento
		quicksort_id(vetor, indicePivo, novoIndicepivo - 1); // chama recursivamente o quicksort para os elementos à esquerda
		quicksort_id(vetor, novoIndicepivo + 1, indiceFinal); // chama recursivamente o quicksort para os elementos à direita
	}
}

// Procedimento que faz a busca dos carros pelo seu id por meio da busca binaria
int busca_id(carros *carro, int id, int tamanho)
{
	int indiceInicial = 0, indiceFinal = tamanho - 1, posicaoElemento = -1;
	// Busca Binaria
	while (indiceInicial <= indiceFinal)
	{
		// dividir o vetor ao meio
		int indiceMeioVetor = (indiceInicial + indiceFinal) / 2;
		if (carro[indiceMeioVetor].identificador == id)
		{
			posicaoElemento = indiceMeioVetor;
			// para o loop
			indiceInicial = indiceFinal + 1;
		}
		else if (id > carro[indiceMeioVetor].identificador)
		{
			// torna o indice inicial igual ao do meio do vetor, para dividir o vetor novamente, uma posição após a do meio
			// quer dizer que o elemento está depois do meio do vetor
			indiceInicial = indiceMeioVetor + 1;
		}
		else
		{
			// torna o indice inicial igual ao do meio do vetor, para dividir o vetor novamente, uma posição anterior à do meio
			// quer dizer que o elemento está antes do meio do vetor
			indiceFinal = indiceMeioVetor - 1;
		}
	}

	return posicaoElemento;
}

//função que deixa todos os caracteres de uma string minusculos
string tolowerString(string str)
{
	// cria uma string com o mesmo tamanho da string que deseja transformar
	string lowerString(str.size(), ' ');
	for (size_t i = 0; i < static_cast<size_t>(str.size()); i++) //static_cast<size_t> para evitar erro de comparação de signed e unsigned
	{
		lowerString[i] = tolower(str[i]);
	}
	return lowerString;
}

//função de prticionamento do vetor para aplicação do quicksort
int particiona_marca(carros *vetor, int indiceInicial, int indiceFinal)
{
	int i = indiceInicial + 1, j = indiceFinal;
	string pivo = tolowerString(vetor[indiceInicial].marca);
	while (i <= j)
	{
		if (tolowerString(vetor[i].marca) <= pivo)
		{
			i++;
		}
		else if (pivo <= tolowerString(vetor[j].marca))
		{
			j--;
		}
		else
		{
			swap(vetor[i], vetor[j]);
			i++;
			j--;
		}
	}
	swap(vetor[indiceInicial], vetor[j]);
	return j;
}

// Procedimento que implementa o QuickSort com base na marca dos carros
void quicksort_marca(carros *&vetor, int indicePivo, int indiceFinal)
{
	int novoIndicepivo;
	if (indicePivo < indiceFinal) // verifica se existe mais de um elemento no intervalo atual para continuar a ordenação
	{
		novoIndicepivo = particiona_marca(vetor, indicePivo, indiceFinal);
		quicksort_marca(vetor, indicePivo, novoIndicepivo - 1); // chama recursivamente o quicksort para os elementos à esquerda
		quicksort_marca(vetor, novoIndicepivo + 1, indiceFinal); // chama recursivamente o quicksort para os elementos à direita
	}
}

// Procedimento que faz a busca dos carros pela sua marca
void busca_marca(carros *carro, string marca, int tamanho)
{
	int indiceInicial = 0, indiceFinal = tamanho - 1, posicaoInicialElemento = -1;
	// Busca Binaria para a posição inicial (encontra a primeira incidencia do elemento)
	while (indiceInicial <= indiceFinal)
	{
		// dividir o vetor ao meio
		int indiceMeioVetor = (indiceInicial + indiceFinal) / 2;
		if (tolowerString(carro[indiceMeioVetor].marca) == tolowerString(marca))
		{
			posicaoInicialElemento = indiceMeioVetor;
			// para o loop
			// indiceInicial = indiceFinal + 1;
			indiceFinal = indiceMeioVetor - 1;
		}
		else if (tolowerString(marca) > tolowerString(carro[indiceMeioVetor].marca))
		{
			// torna o indice inicial igual ao do meio do vetor, para dividir o vetor novamente, uma posição após a do meio
			// quer dizer que o elemento está depois do meio do vetor
			indiceInicial = indiceMeioVetor + 1;
		}
		else
		{
			// torna o indice inicial igual ao do meio do vetor, para dividir o vetor novamente, uma posição anterior à do meio
			// quer dizer que o elemento está antes do meio do vetor
			indiceFinal = indiceMeioVetor - 1;
		}
	}

	int posicaoFinalElemento = -1;
	indiceInicial = 0, indiceFinal = tamanho - 1;

	// Busca binaria para a posição final (encontra a ultima incidencia do elemento)
	while (indiceInicial <= indiceFinal)
	{
		int indiceMeioVetor = (indiceInicial + indiceFinal) / 2;
		if (tolowerString(carro[indiceMeioVetor].marca) == tolowerString(marca))
		{
			posicaoFinalElemento = indiceMeioVetor;
			// permite que o algoritmo procure mais incidencias do elemento na posição posterior ao encontro
			indiceInicial = indiceMeioVetor + 1;
		}
		else
		{
			if (tolowerString(marca) > tolowerString(carro[indiceMeioVetor].marca))
			{
				indiceInicial = indiceMeioVetor + 1;
			}
			else
			{
				indiceFinal = indiceMeioVetor - 1;
			}
		}
	}
	if (posicaoFinalElemento == -1 and posicaoInicialElemento == -1)
	{
		cout << RED << "Marca não encontrada!" << endl;
	}
	else
	{
		for (int i = posicaoInicialElemento; i <= posicaoFinalElemento; i++)
		{
			cout << YELLOW << "| " << PADRAO << left << setw(20) << "Identificador: " << carro[i].identificador << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Modelo: " << carro[i].modelo << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Marca: " << carro[i].marca << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Ano de lançamento: " << carro[i].ano_lancamento << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Descricação: " << endl
				 << YELLOW << "| " << PADRAO << carro[i].descricao << endl
				 << fixed << setprecision(2) << YELLOW << "| " << PADRAO << left << setw(20) << "Preço(R$): " << carro[i].valor << endl;
			cout << YELLOW << "+-----------------------------------------------------+\n";
		}
	}
}

// Aplicação do quicksort para ordenação (em ordem alfabética) com bse no modelo
int particiona_modelo(carros *vetor, int indiceInicial, int indiceFinal)
{
	int i = indiceInicial + 1, j = indiceFinal;
	string pivo = tolowerString(vetor[indiceInicial].modelo);
	while (i <= j)
	{
		if (tolowerString(vetor[i].modelo) <= pivo)
		{
			i++;
		}
		else if (pivo <= tolowerString(vetor[j].modelo))
		{
			j--;
		}
		else
		{
			swap(vetor[i], vetor[j]);
			i++;
			j--;
		}
	}
	swap(vetor[indiceInicial], vetor[j]);
	return j;
}

// Procedimento que implementa o QuickSort com base no modelo dos carros
void quicksort_modelo(carros *&vetor, int indicePivo, int indiceFinal)
{
	int novoIndicepivo;
	if (indicePivo < indiceFinal) // verifica se existe mais de um elemento no intervalo atual para continuar a ordenação
	{
		novoIndicepivo = particiona_modelo(vetor, indicePivo, indiceFinal);
		quicksort_modelo(vetor, indicePivo, novoIndicepivo - 1); // chama recursivamente o quicksort para os elementos à esquerda
		quicksort_modelo(vetor, novoIndicepivo + 1, indiceFinal); // chama recursivamente o quicksort para os elementos à direita
	}
}

int busca_modelo(carros *carro, string modelo, int tamanho)
{
	int indiceInicial = 0, indiceFinal = tamanho - 1, posicaoElemento = -1;
	string modelo_lower = tolowerString(modelo);
	// Busca Binaria
	while (indiceInicial <= indiceFinal)
	{
		// dividir o vetor ao meio
		int indiceMeioVetor = (indiceInicial + indiceFinal) / 2;
		if (tolowerString(carro[indiceMeioVetor].modelo) == modelo_lower)
		{
			posicaoElemento = indiceMeioVetor;
			// para o loop
			indiceInicial = indiceFinal + 1;
		}
		else if (modelo_lower > tolowerString(carro[indiceMeioVetor].modelo))
		{
			// torna o indice inicial igual ao do meio do vetor, para dividir o vetor novamente, uma posição após a do meio
			// quer dizer que o elemento está depois do meio do vetor
			indiceInicial = indiceMeioVetor + 1;
		}
		else
		{
			// torna o indice inicial igual ao do meio do vetor, para dividir o vetor novamente, uma posição anterior à do meio
			// quer dizer que o elemento está antes do meio do vetor
			indiceFinal = indiceMeioVetor - 1;
		}
	}

	return posicaoElemento;
}

// aplicação do quicksort para ordenação com base no ano
int particiona_ano(carros *vetor, int indiceInicial, int indiceFinal)
{
	int i = indiceInicial + 1, j = indiceFinal;
	int pivo = vetor[indiceInicial].ano_lancamento;
	while (i <= j)
	{
		if (vetor[i].ano_lancamento <= pivo)
		{
			i++;
		}
		else if (pivo <= vetor[j].ano_lancamento)
		{
			j--;
		}
		else
		{
			swap(vetor[i], vetor[j]);
			i++;
			j--;
		}
	}
	swap(vetor[indiceInicial], vetor[j]);
	return j;
}
void quicksort_ano(carros *&vetor, int indicePivo, int indiceFinal)
{
	int novoindicePivo;
	if (indicePivo < indiceFinal)
	{
		novoindicePivo = particiona_ano(vetor, indicePivo, indiceFinal);
		quicksort_ano(vetor, indicePivo, novoindicePivo - 1);
		quicksort_ano(vetor, novoindicePivo + 1, indiceFinal);
	}
}

// Procedimento que faz a busca dos carros pelo seu ano
void busca_ano(carros *carro, int ano, int tamanho)
{
	int indiceInicial = 0, indiceFinal = tamanho - 1, posicaoInicialElemento = -1;
	// Busca Binaria para a posição inicial (encontra a primeira incidencia do elemento)
	while (indiceInicial <= indiceFinal)
	{
		// dividir o vetor ao meio
		int indiceMeioVetor = (indiceInicial + indiceFinal) / 2;
		if (carro[indiceMeioVetor].ano_lancamento == ano)
		{
			posicaoInicialElemento = indiceMeioVetor;
			// para o loop
			// indiceInicial = indiceFinal + 1;
			indiceFinal = indiceMeioVetor - 1;
		}
		else if (ano > carro[indiceMeioVetor].ano_lancamento)
		{
			// torna o indice inicial igual ao do meio do vetor, para dividir o vetor novamente, uma posição após a do meio
			// quer dizer que o elemento está depois do meio do vetor
			indiceInicial = indiceMeioVetor + 1;
		}
		else
		{
			// torna o indice inicial igual ao do meio do vetor, para dividir o vetor novamente, uma posição anterior à do meio
			// quer dizer que o elemento está antes do meio do vetor
			indiceFinal = indiceMeioVetor - 1;
		}
	}

	int posicaoFinalElemento = -1;
	indiceInicial = 0, indiceFinal = tamanho - 1;

	// Busca binaria para a posição final (encontra a ultima incidencia do elemento)
	while (indiceInicial <= indiceFinal)
	{
		int indiceMeioVetor = (indiceInicial + indiceFinal) / 2;
		if (carro[indiceMeioVetor].ano_lancamento == ano)
		{
			posicaoFinalElemento = indiceMeioVetor;
			// permite que o algoritmo procure mais incidencias do elemento na posição posterior ao encontro
			indiceInicial = indiceMeioVetor + 1;
		}
		else
		{
			if (ano > carro[indiceMeioVetor].ano_lancamento)
			{
				indiceInicial = indiceMeioVetor + 1;
			}
			else
			{
				indiceFinal = indiceMeioVetor - 1;
			}
		}
	}
	if (posicaoFinalElemento == -1 and posicaoInicialElemento == -1)
	{
		cout << "Ano não encontrado!" << endl;
	}
	else
	{
		for (int i = posicaoInicialElemento; i <= posicaoFinalElemento; i++)
		{
			cout << YELLOW << "| " << PADRAO << left << setw(20) << "Identificador: " << carro[i].identificador << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Modelo: " << carro[i].modelo << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Marca: " << carro[i].marca << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Ano de lançamento: " << carro[i].ano_lancamento << endl
				 << YELLOW << "| " << PADRAO << left << setw(20) << "Descricação: " << endl
				 << YELLOW << "| " << PADRAO << carro[i].descricao << endl
				 << fixed << setprecision(2) << YELLOW << "| " << PADRAO << left << setw(20) << "Preço(R$): " << carro[i].valor << endl;
			cout << YELLOW << "+-----------------------------------------------------+\n";
		}
	}
}

// aplicação do insertionsort para ordenação com base no valor
void insertionsort_valor(carros *&vetor, int tamanho)
{
	int j;
	carros pivo;
	for (int i = 1; i < tamanho; i++)
	{
		pivo = vetor[i];
		j = i - 1;
		while (j >= 0 and pivo.valor < vetor[j].valor)
		{
			vetor[j + 1] = vetor[j];
			j--;
		}
		vetor[j + 1] = pivo;
	}
}

//filtro que mostra os elementos com os preços entre 2 extremos
void filtro_valor(carros *carro, int tamanho)
{
	float minimo, maximo;
	cout << YELLOW << "=================== FILTRO DE PREÇO ====================" << endl;
	cout << PADRAO << "Insira a faixa de preço (menor valor, maior valor):" << endl;
	cin >> minimo >> maximo;
	if (minimo < 0 or minimo > maximo)
	{
		cout << RED << "Intervalo inválido!";
	}
	else
	{
		for (int i = 0; i < tamanho; i++)
		{
			if (carro[i].valor >= minimo and carro[i].valor <= maximo)
			{
				cout << YELLOW << "| " << PADRAO << left << setw(20) << "Identificador: " << carro[i].identificador << endl
					 << YELLOW << "| " << PADRAO << left << setw(20) << "Modelo: " << carro[i].modelo << endl
					 << YELLOW << "| " << PADRAO << left << setw(20) << "Marca: " << carro[i].marca << endl
					 << YELLOW << "| " << PADRAO << left << setw(20) << "Ano de lançamento: " << carro[i].ano_lancamento << endl
					 << YELLOW << "| " << PADRAO << left << setw(20) << "Descricação: " << endl
					 << YELLOW << "| " << PADRAO << carro[i].descricao << endl
					 << fixed << setprecision(2) << YELLOW << "| " << PADRAO << left << setw(20) << "Preço(R$): " << carro[i].valor << endl;
				cout << YELLOW << "+-----------------------------------------------------+\n";
			}
		}
	}
}

void add_vetor(carros *&vetor, int &qtd, int &tamanho)
{
	int maiorId = 100;
	if (qtd >= tamanho)
	{
		// Redimensionamento
		carros *novo = new carros[tamanho + 1];
		for (int j = 0; j < tamanho; j++)
		{
			novo[j] = vetor[j];
		}
		delete[] vetor;
		vetor = novo;
		tamanho += 1;
	}
	cin.ignore(); // limpar o buffer
	cout << "Nome: ";
	cin.getline(vetor[qtd].modelo, 100);
	cout << "\nMarca: ";
	cin.getline(vetor[qtd].marca, 100);
	cout << "\nAno de Lancamento: ";
	cin >> vetor[qtd].ano_lancamento;
	cin.ignore(); // limpar o buffer
	cout << "\nDescricao: ";
	cin.getline(vetor[qtd].descricao, 500);
	cout << "\nValor: ";
	cin >> vetor[qtd].valor;

	for (int j = 0; j < qtd; j++)
	{
		if (vetor[j].identificador > maiorId)
		{
			maiorId = vetor[j].identificador;
		}
	}
	vetor[qtd].identificador = maiorId + 1;

	qtd++;
}

int main()
{
	int tamanhoInicial = 40, i = 0; //O i é o contador de elementos lidos no vetor; é atualizado dentro das funções de leitura
	carros *carro = new carros[tamanhoInicial]; // vetor principal de trabalho
	int ano, id, opcaoLeitura, opcaoTrabalho;
	char marca[100], modelo[100];
	bool controleOpcoes = true;
	cout << YELLOW << "================== LEITURA DE ARQUIVO ==================" << endl;
	cout << PADRAO << "Escolha como quer acessar os dados:" << endl
		 << "[1] 📄 Binário " << endl
		 << "[2] 📄 Arquivo Csv" << endl
		 << RED << "[0] ❌ Encerrar" << endl
		 << PADRAO << "Digite aqui: ";
	cin >> opcaoLeitura;
	cin.ignore();
	switch (opcaoLeitura)
	{
	case 0:
		controleOpcoes = false;
		break;
	case 1:
		ler_bin(carro, tamanhoInicial, i, "Carros.bin");
		break;
	case 2:
		ler_csv(carro, tamanhoInicial, i, "Carros.csv");
		break;
	default:
		cout << RED << "Opção não encontrada!" << endl;
		break;
	}
	controleOpcoes = true;
	while (controleOpcoes)
	{
		cout << YELLOW << "========================= MENU =========================" << endl;
		cout << PADRAO << "Escolha o que deseja:" << endl
			 << "[1] 🔍 Buscar" << endl
			 << "[2] 💵 Filtro de preço" << endl
			 << "[3] 📲 Ordenar" << endl
			 << "[4] 📖 Mostrar Dados" << endl
			 << "[5] 📝 Adicionar" << endl
			 << "[6] 🗑️  Apagar" << endl
			 << RED << "[0] ❌ Sair" << endl
			 << PADRAO << "Digite aqui: ";
		cin >> opcaoTrabalho;
		switch (opcaoTrabalho)
		{
		case 0:
			cout << YELLOW << "======================== SALVAR ========================" << endl;
			cout << PADRAO << "Deseja Salvar alterações:" << endl
				 << GREEN << "[1] Sim" << endl
				 << RED << "[2] Não " << endl
				 << PADRAO << "Digite aqui: " << endl;
			int opcaoSalvamento;
			cin >> opcaoSalvamento;
			if (opcaoSalvamento == 1)
			{
				escrever_bin("Carros.bin", carro, i);
				escrever_csv("Carros.csv", carro, i);

				cout << GREEN << "Arquivo atualizado com sucesso!" << endl;
				controleOpcoes = false;
				break;
			}
			if (opcaoSalvamento == 2)
			{
				cout << BLUE << "Alterações descartadas!" << endl;
				controleOpcoes = false;
				break;
			}
			else
			{
				cout << RED << "Opção não encontrada!" << endl;
				controleOpcoes = false;
				break;
			}
			cin.ignore();
		case 1:
			cout << YELLOW << "========================= BUSCA ========================" << endl;
			cout << PADRAO << "Insira o campo que deseja buscar:" << endl
				 << "[1] 🔢 Identificador" << endl
				 << "[2] 🏷️  Marca" << endl
				 << "[3] 🚗 Modelo" << endl
				 << "[4] 🗓️  Ano" << endl
				 << "Digite aqui: ";
			int opcaoBusca;
			cin >> opcaoBusca;
			cin.ignore();
			switch (opcaoBusca)
			{
			case 1:
			{
				cout << PADRAO << "Insira o ID desejado: " << endl;
				cin >> id;
				cout << YELLOW << "+-----------------------------------------------------+\n";
				quicksort_id(carro, 0, i - 1);
				int posicaoId = busca_id(carro, id, i);
				if (posicaoId == -1)
				{
					cout << RED << "Identificador não encontrado!" << endl;
				}
				else
				{
					cout << YELLOW << "| " << PADRAO << left << setw(20) << "Identificador: " << carro[posicaoId].identificador << endl
						 << YELLOW << "| " << PADRAO << left << setw(20) << "Modelo: " << carro[posicaoId].modelo << endl
						 << YELLOW << "| " << PADRAO << left << setw(20) << "Marca: " << carro[posicaoId].marca << endl
						 << YELLOW << "| " << PADRAO << left << setw(20) << "Ano de lançamento: " << carro[posicaoId].ano_lancamento << endl
						 << YELLOW << "| " << PADRAO << left << setw(20) << "Descricação: " << endl
						 << YELLOW << "| " << PADRAO << carro[posicaoId].descricao << endl
						 << fixed << setprecision(2) << YELLOW << "| " << PADRAO << left << setw(20) << "Preço(R$): " << carro[posicaoId].valor << endl;
					cout << YELLOW << "+-----------------------------------------------------+\n";
				}
				cin.ignore();
			}
			break;
			case 2:
				cout << PADRAO << "Insira a marca desejada: " << endl;
				cin.getline(marca, 100);
				cout << YELLOW << "+-----------------------------------------------------+\n";
				quicksort_marca(carro, 0, i - 1);
				busca_marca(carro, marca, i);
				break;
			case 3:
			{
				cout << PADRAO << "Insira o modelo desejado: " << endl;
				cin.getline(modelo, 100);
				cout << YELLOW << "+-----------------------------------------------------+\n";
				quicksort_modelo(carro, 0, i - 1);
				int posicaoModelo = busca_modelo(carro, modelo, i);
				if (posicaoModelo == -1)
				{
					cout << RED << "Modelo não encontrado!" << endl;
				}
				else
				{
					cout << YELLOW << "| " << PADRAO << left << setw(20) << "Identificador: " << carro[posicaoModelo].identificador << endl
						 << YELLOW << "| " << PADRAO << left << setw(20) << "Modelo: " << carro[posicaoModelo].modelo << endl
						 << YELLOW << "| " << PADRAO << left << setw(20) << "Marca: " << carro[posicaoModelo].marca << endl
						 << YELLOW << "| " << PADRAO << left << setw(20) << "Ano de lançamento: " << carro[posicaoModelo].ano_lancamento << endl
						 << YELLOW << "| " << PADRAO << left << setw(20) << "Descricação: " << endl
						 << YELLOW << "| " << PADRAO << carro[posicaoModelo].descricao << endl
						 << fixed << setprecision(2) << YELLOW << "| " << PADRAO << left << setw(20) << "Preço(R$): " << carro[posicaoModelo].valor << endl;
					cout << YELLOW << "+-----------------------------------------------------+\n";
				}
			}
			break;
			case 4:
				cout << PADRAO << "Insira a ano desejado: " << endl;
				cin >> ano;
				cout << YELLOW << "+-----------------------------------------------------+\n";
				quicksort_ano(carro, 0, i - 1);
				busca_ano(carro, ano, i);
				cin.ignore();
				break;
			default:
				cout << "Voltando..." << endl;
				break;
			}
			break;
		case 2:
			insertionsort_valor(carro, i);
			filtro_valor(carro, i);
			quicksort_id(carro, 0, i - 1);
			break;
		case 3:
			cout << YELLOW << "======================== ORDENAR =======================" << endl;
			cout << PADRAO << "Escolha o campo de ordenação: " << endl
				 << "[1] 🔢 Identificador" << endl
				 << "[2] 🗓️  Ano" << endl
				 << "[3] 💵 Valor" << endl
				 << "[4] 🏷️  Marca(Ordem alfabética)" << endl
				 << "Digite aqui:";
			int opcaoOrdenacao;
			cin >> opcaoOrdenacao;
			switch (opcaoOrdenacao)
			{
			case 1:
				quicksort_id(carro, 0, i - 1);
				cout << BLUE << "Ordenando: ";
				barraProgresso(40);
				cout << GREEN << "Dados ordenados com sucesso! Vá até a opção de ver os dados!" << endl;
				break;
			case 2:
				cout << BLUE << "Ordenando: ";
				barraProgresso(40);
				quicksort_ano(carro, 0, i - 1);
				cout << GREEN << "Dados ordenados com sucesso! Vá até a opção de ver os dados!" << endl;
				break;
			case 3:
				cout << BLUE << "Ordenando: ";
				barraProgresso(40);
				insertionsort_valor(carro, i);
				cout << GREEN << "Dados ordenados com sucesso! Vá até a opção de ver os dados!" << endl;
				break;
			case 4:
				cout << BLUE << "Ordenando: ";
				barraProgresso(40);
				quicksort_marca(carro, 0, i - 1);
				cout << GREEN << "Dados ordenados com sucesso! Vá até a opção de ver os dados!" << endl;
				break;
			default:
				cout << RED << "Campo não encontrado." << endl;
				break;
			}
			cin.ignore();
			break;
		case 4:
			show_vetor(carro, i);
			break;
		case 5:
			add_vetor(carro, i, tamanhoInicial);
			break;
		case 6:
			cout << YELLOW << "======================== APAGAR ========================" << endl;
			cout << PADRAO << "O que deseja:" << endl
				 << "[1] 🗑️  Apagar item" << endl
				 << "[2] 📋 Ver lista de apagados" << endl
				 << "[3] 🔄 Recuperar Itens" << endl
				 << RED << "[0] ❌ Sair" << endl
				 << PADRAO << "Digite aqui: ";
			int opcaoApagar;
			cin >> opcaoApagar;
			switch (opcaoApagar)
			{
			case 0:
				break;
			case 1:
			{
				cout << PADRAO << "Insira o identificador do item a ser marcado: " << endl;
				int idMarcar;
				cin >> idMarcar;
				quicksort_id(carro, 0, i - 1);
				int posicaoId = busca_id(carro, idMarcar, i);
				if (posicaoId != -1)
				{
					marcar_vetor_id(carro, i, idMarcar);
					cout << GREEN << "Elemento marcado com sucesso!" << endl;
				}
				else
				{
					cout << RED << "Identificador Inexistente" << endl;
				}
			}
			break;
			case 2:
				cout << YELLOW << "================== LISTA DE APAGADOS ===================" << endl;
				lista_marcados(carro, i);
				break;
			case 3:
			{
				int idDesmarcar;
				cout << YELLOW << "================== LISTA DE APAGADOS ===================" << endl;
				lista_marcados(carro, i);
				cout << PADRAO << "Insira o id que quer desmarcar \nDigite aqui: ";
				cin >> idDesmarcar;
				int posicaoIDApagar;
				do
				{
					posicaoIDApagar = busca_id(carro, idDesmarcar, i);
					if (posicaoIDApagar == -1)
					{
						cout << RED << "Identificador Inexistente. Tente outro!" << endl;
					}
				} while (posicaoIDApagar == -1);
				if (!carro[posicaoIDApagar].marcacao)
				{
					desmarcar_vetor_id(carro, i, idDesmarcar);
					cout << GREEN << "Elemento desmarcado com sucesso!" << endl;
				}else{
					cout << RED << "Elemento não se encontra na lista de marcados!" << endl;
				}
			}
			break;
			default:
				cout  << RED << "Opção não encontrada!" << endl;
				break;
			}
		}
	}
	delete[] carro;
	return 0;
}
