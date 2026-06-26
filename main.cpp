#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>

// Nome do arquivo de dados CSV
const std::string NOME_ARQUIVO = "Lista_Municipios_com_IBGE_Brasil_Versao_CSV.csv";

// Estrutura para armazenar o índice na memória
struct RegistroBusca {
    int ibge;
    int linha;
};

// Função para converter uma string ISO-8859-1 (Latin1) para UTF-8 (para exibir acentos no terminal)
std::string latin1_para_utf8(const std::string& str) {
    std::string utf8;
    utf8.reserve(str.size() * 1.1);
    for (unsigned char c : str) {
        if (c < 128) {
            utf8.push_back(c);
        } else {
            utf8.push_back(0xC0 | (c >> 6));
            utf8.push_back(0x80 | (c & 0x3F));
        }
    }
    return utf8;
}

// Função auxiliar para dividir uma string por um delimitador
std::vector<std::string> dividir(const std::string& texto, char delimitador) {
    std::vector<std::string> colunas;
    std::string coluna;
    std::istringstream stream(texto);
    while (std::getline(stream, coluna, delimitador)) {
        colunas.push_back(coluna);
    }
    return colunas;
}

// Função para carregar os dados do CSV para o vetor em memória
void carregarDados(std::vector<RegistroBusca>& vetor) {
    // Se o vetor já contiver dados, limpamos primeiro para evitar duplicações
    if (!vetor.empty()) {
        vetor.clear();
        vetor.shrink_to_fit();
    }

    std::ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        std::cout << "\n[ERRO] Nao foi possivel abrir o arquivo: " << NOME_ARQUIVO << std::endl;
        std::cout << "Certifique-se de que o arquivo esta no mesmo diretorio do executavel.\n" << std::endl;
        return;
    }

    std::string linhaTexto;
    int numeroLinha = 0;

    // Ignora a primeira linha (cabeçalho) do CSV, mas incrementa o contador de linha
    if (std::getline(arquivo, linhaTexto)) {
        numeroLinha++;
    }

    // Lê o arquivo linha por linha
    while (std::getline(arquivo, linhaTexto)) {
        numeroLinha++;

        // Remove caractere de quebra de linha do Windows (\r) se presente
        if (!linhaTexto.empty() && linhaTexto.back() == '\r') {
            linhaTexto.pop_back();
        }

        std::vector<std::string> colunas = dividir(linhaTexto, ';');
        // O código IBGE está na segunda coluna (índice 1)
        if (colunas.size() > 1 && !colunas[1].empty()) {
            try {
                int ibge = std::stoi(colunas[1]);
                vetor.push_back({ibge, numeroLinha});
            } catch (...) {
                // Ignora linhas que não possuem um código IBGE numérico válido
            }
        }
    }

    arquivo.close();
    std::cout << "\n[SUCESSO] Dados carregados com sucesso!" << std::endl;
    std::cout << "Total de registros na memoria: " << vetor.size() << " municipios.\n" << std::endl;
}

// Implementação do algoritmo de Busca Sequencial
// Retorna a linha do arquivo se encontrado, ou -1 caso contrário
int buscaSequencial(const std::vector<RegistroBusca>& vetor, int codigoIBGE, int& comparacoes) {
    comparacoes = 0;
    for (size_t i = 0; i < vetor.size(); ++i) {
        comparacoes++;
        if (vetor[i].ibge == codigoIBGE) {
            return vetor[i].linha;
        }
    }
    return -1;
}

// Implementação do algoritmo de Busca Binária
// Retorna a linha do arquivo se encontrado, ou -1 caso contrário
int buscaBinaria(const std::vector<RegistroBusca>& vetor, int codigoIBGE, int& comparacoes) {
    comparacoes = 0;
    int esq = 0;
    int dir = static_cast<int>(vetor.size()) - 1;

    while (esq <= dir) {
        comparacoes++;
        int meio = esq + (dir - esq) / 2;

        if (vetor[meio].ibge == codigoIBGE) {
            return vetor[meio].linha;
        }
        if (vetor[meio].ibge < codigoIBGE) {
            esq = meio + 1;
        } else {
            dir = meio - 1;
        }
    }
    return -1;
}

// Abre o arquivo e lê a linha específica para extrair e exibir os dados do município
void exibirDetalhesMunicipio(int numeroLinha) {
    std::ifstream arquivo(NOME_ARQUIVO);
    if (!arquivo.is_open()) {
        std::cout << "[ERRO] Falha ao abrir o arquivo para exibir detalhes." << std::endl;
        return;
    }

    std::string linhaTexto;
    int contadorLinha = 0;

    while (std::getline(arquivo, linhaTexto)) {
        contadorLinha++;
        if (contadorLinha == numeroLinha) {
            // Remove caractere de quebra de linha do Windows (\r) se presente
            if (!linhaTexto.empty() && linhaTexto.back() == '\r') {
                linhaTexto.pop_back();
            }

            std::vector<std::string> colunas = dividir(linhaTexto, ';');
            // Verifica se possui colunas suficientes para os dados requeridos
            if (colunas.size() > 7) {
                std::cout << "\n==================================================" << std::endl;
                std::cout << "            DADOS DO MUNICIPIO ENCONTRADO         " << std::endl;
                std::cout << "==================================================" << std::endl;
                std::cout << " Municipio:        " << latin1_para_utf8(colunas[4]) << std::endl;
                std::cout << " UF:               " << latin1_para_utf8(colunas[3]) << std::endl;
                std::cout << " Regiao:           " << latin1_para_utf8(colunas[5]) << std::endl;
                std::cout << " Populacao 2010:   " << latin1_para_utf8(colunas[6]) << std::endl;
                std::cout << " Porte:            " << latin1_para_utf8(colunas[7]) << std::endl;
                std::cout << " Linha no Arquivo: " << numeroLinha << std::endl;
                std::cout << "==================================================" << std::endl;
            } else {
                std::cout << "[ERRO] Dados da linha " << numeroLinha << " estao corrompidos ou incompletos." << std::endl;
            }
            break;
        }
    }
    arquivo.close();
}

int main() {
    std::vector<RegistroBusca> vetorIndices;
    int opcao = 0;

    while (true) {
        std::cout << "=== MENU - TRABALHO 4 (ALGORITMOS DE BUSCA) ===" << std::endl;
        std::cout << "1. Carregar dados" << std::endl;
        std::cout << "2. Consultar municipio" << std::endl;
        std::cout << "3. Sair" << std::endl;
        std::cout << "Escolha uma opcao: ";
        
        if (!(std::cin >> opcao)) {
            std::cin.clear();
            std::string lixo;
            std::cin >> lixo;
            std::cout << "\n[ERRO] Opcao invalida. Digite um numero entre 1 e 3.\n" << std::endl;
            continue;
        }

        if (opcao == 1) {
            carregarDados(vetorIndices);
        } 
        else if (opcao == 2) {
            if (vetorIndices.empty()) {
                std::cout << "\n[AVISO] Voce precisa carregar os dados (Opcao 1) primeiro!\n" << std::endl;
                continue;
            }

            int codigoIBGE;
            std::cout << "Digite o codigo do municipio (IBGE): ";
            if (!(std::cin >> codigoIBGE)) {
                std::cin.clear();
                std::string lixo;
                std::cin >> lixo;
                std::cout << "\n[ERRO] Codigo IBGE invalido. Deve ser um numero inteiro.\n" << std::endl;
                continue;
            }

            // Realiza a Busca Binária medindo o tempo
            int comparacoesBin = 0;
            auto inicioBin = std::chrono::high_resolution_clock::now();
            int linhaBin = buscaBinaria(vetorIndices, codigoIBGE, comparacoesBin);
            auto fimBin = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::nano> tempoBin = fimBin - inicioBin;

            // Realiza a Busca Sequencial medindo o tempo
            int comparacoesSeq = 0;
            auto inicioSeq = std::chrono::high_resolution_clock::now();
            int linhaSeq = buscaSequencial(vetorIndices, codigoIBGE, comparacoesSeq);
            auto fimSeq = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::nano> tempoSeq = fimSeq - inicioSeq;

            // Exibe os resultados
            if (linhaBin != -1) {
                exibirDetalhesMunicipio(linhaBin);
                
                std::cout << "\n--- DESEMPENHO DOS ALGORITMOS DE BUSCA ---" << std::endl;
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Busca Binaria:" << std::endl;
                std::cout << "  - Tempo de execucao: " << tempoBin.count() << " ns (" << tempoBin.count() / 1000.0 << " us)" << std::endl;
                std::cout << "  - Comparacoes feitas: " << comparacoesBin << std::endl;
                
                std::cout << "Busca Sequencial:" << std::endl;
                std::cout << "  - Tempo de execucao: " << tempoSeq.count() << " ns (" << tempoSeq.count() / 1000.0 << " us)" << std::endl;
                std::cout << "  - Comparacoes feitas: " << comparacoesSeq << std::endl;
                std::cout << "------------------------------------------\n" << std::endl;
            } else {
                std::cout << "\n[AVISO] Municipio com codigo IBGE " << codigoIBGE << " nao foi encontrado." << std::endl;
                std::cout << "--- DESEMPENHO DOS ALGORITMOS DE BUSCA (FALHA) ---" << std::endl;
                std::cout << std::fixed << std::setprecision(2);
                std::cout << "Busca Binaria: " << tempoBin.count() << " ns (" << comparacoesBin << " comparacoes)" << std::endl;
                std::cout << "Busca Sequencial: " << tempoSeq.count() << " ns (" << comparacoesSeq << " comparacoes)" << std::endl;
                std::cout << "--------------------------------------------------\n" << std::endl;
            }
        } 
        else if (opcao == 3) {
            std::cout << "\nLiberando recursos de memoria..." << std::endl;
            vetorIndices.clear();
            vetorIndices.shrink_to_fit();
            std::cout << "Memoria liberada com sucesso." << std::endl;
            std::cout << "Saindo do programa. Ate logo!" << std::endl;
            break;
        } 
        else {
            std::cout << "\n[ERRO] Opcao invalida. Digite 1, 2 ou 3.\n" << std::endl;
        }
    }

    return 0;
}
