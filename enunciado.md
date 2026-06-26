# Trabalho 4 - Algoritmos de Busca

## Descrição da Entrega

A atividade deverá ser enviada na plataforma **SIGAA** em um único arquivo no formato **.pdf**.

O documento deve conter:
- Nome completo do aluno
- Matrícula
- Turma
- Resolução completa do exercício com código-fonte e explicações

> [!WARNING]
> Não serão aceitos arquivos em formatos diferentes de `.pdf`.

---

## Especificação do Problema

O arquivo de dados do Censo 2010 do IBGE está ordenado pelo código do município, armazenado no campo `IBGE`. 

Desenvolva um programa que apresente um menu com as seguintes opções:

### 1. Carregar dados
Carrega em um vetor em memória os registros contendo os campos:
- `IBGE`: Código do município
- `Linha`: Número da linha correspondente no arquivo

### 2. Consultar município
Lê um código de município e exibe na tela as seguintes informações associadas:
- **Município**
- **UF**
- **Região**
- **População em 2010**
- **Porte**

O programa também deve medir e exibir o tempo de execução da busca utilizando os seguintes algoritmos:
- **Busca Binária**
- **Busca Sequencial**

### 3. Sair

---

> [!IMPORTANT]
> **Observação:** Não deixe lixo na memória (faça a liberação correta dos recursos alocados).

### Base de Dados
Os dados para o trabalho podem ser obtidos em:
[Lista de Municípios Brasileiros (Censo 2010)](http://blog.mds.gov.br/redesuas/lista-de-municipios-brasileiros/)