# Trabalho 4 - Algoritmos de Busca

Este documento apresenta o resumo do desenvolvimento, as decisões de projeto adotadas e os resultados de validação e desempenho para o Trabalho 4 de Estruturas de Dados.

---

## O que foi Feito

1. **Estrutura de Índices em Memória (`RegistroBusca`)**:
   - Desenvolveu-se uma struct que armazena apenas o código `ibge` (como um inteiro) e o número da `linha` no arquivo CSV.
   - Ao carregar os dados, apenas essa struct compacta é mantida no vetor de índices, economizando memória. O vetor ocupa menos de 50 KB na memória para todos os 5.570 municípios.

2. **Leitura e Conversão de Codificação (Latin1 para UTF-8)**:
   - O arquivo CSV de entrada está codificado em **ISO-8859-1 (Latin1)**.
   - Criou-se a função auxiliar `latin1_para_utf8` que converte os caracteres estendidos de 8 bits para a codificação UTF-8 multibyte do Linux. Isso garante a exibição correta dos acentos no terminal (ex: "Brasília", "Região", "Metrópole").

3. **Algoritmos de Busca e Medição**:
   - **Busca Sequencial**: Percorre o vetor elemento por elemento. Conta as comparações realizadas.
   - **Busca Binária**: Utiliza a ordenação natural do código IBGE do arquivo para buscar em tempo logarítmico $O(\log n)$. Conta as comparações.
   - A medição do tempo de execução foi realizada com precisão de nanosegundos utilizando a biblioteca `<chrono>`.

4. **Exibição dos Detalhes Sob Demanda**:
   - Em vez de manter todos os nomes de municípios e dados populacionais em memória, o programa abre o arquivo CSV em modo leitura apenas quando um município é encontrado, salta até a linha correspondente de forma direta e lê as colunas requeridas (Município, UF, Região, População de 2010 e Porte).

5. **Gerenciamento de Recursos e Memória**:
   - Implementou-se a liberação explícita de memória com `vetor.clear()` e `vetor.shrink_to_fit()` no encerramento (Opção 3), garantindo que nenhum resíduo de memória permaneça ativo.

---

## Arquivo Criado

- **[main.cpp](main.cpp)**: Código-fonte principal contendo todas as estruturas e funções do programa.

---

## Resultados e Validação

O programa foi compilado utilizando `g++ -std=c++17 -O2 main.cpp -o programa` e testado com diferentes entradas.

### Cenário 1: Busca pelo primeiro elemento (Alta Floresta D'oeste - IBGE `110001`)
- **Busca Sequencial**: 1 comparação (30.00 ns) — extremamente rápida por estar na primeira posição.
- **Busca Binária**: 12 comparações (301.00 ns) — realiza a busca logarítmica padrão dividindo o vetor ao meio.

### Cenário 2: Busca pelo último elemento (Brasília - IBGE `530010`)
- **Busca Sequencial**: 5.570 comparações (6.028,00 ns / 6,03 us).
- **Busca Binária**: 13 comparações (277,00 ns / 0,28 us).
- *Observação:* A busca binária demonstrou ser aproximadamente **21 vezes mais rápida** que a busca sequencial neste caso de pior cenário para a busca sequencial.

### Cenário 3: Busca por elemento inexistente (IBGE `999999`)
- **Busca Sequencial**: 5.570 comparações (5.463,00 ns / 5,46 us).
- **Busca Binária**: 13 comparações (213,00 ns / 0,21 us).

---

## Log de Execução no Terminal

```text
=== MENU - TRABALHO 4 (ALGORITMOS DE BUSCA) ===
1. Carregar dados
2. Consultar municipio
3. Sair
Escolha uma opcao: 1

[SUCESSO] Dados carregados com sucesso!
Total de registros na memoria: 5570 municipios.

=== MENU - TRABALHO 4 (ALGORITMOS DE BUSCA) ===
1. Carregar dados
2. Consultar municipio
3. Sair
Escolha uma opcao: 2
Digite o codigo do municipio (IBGE): 530010

==================================================
            DADOS DO MUNICIPIO ENCONTRADO         
==================================================
 Municipio:        Brasília
 UF:               DF
 Regiao:           Região Centro-Oeste
 Populacao 2010:   2570160
 Porte:            Metrópole
 Linha no Arquivo: 5571
==================================================

--- DESEMPENHO DOS ALGORITMOS DE BUSCA ---
Busca Binaria:
  - Tempo de execucao: 277.00 ns (0.28 us)
  - Comparacoes feitas: 13
Busca Sequencial:
  - Tempo de execucao: 6028.00 ns (6.03 us)
  - Comparacoes feitas: 5570
------------------------------------------
```
