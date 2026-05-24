# Arquitetura do Sistema de Processamento Paralelo de Imagens

## Visão Geral

O sistema de Processamento Paralelo de Imagens foi projetado para processar de forma eficiente um grande conjunto de imagens aplicando diversos filtros usando técnicas de computação paralela. O sistema aproveita MPI (Message Passing Interface) e OpenMP (Open Multi-Processing) para alcançar desempenho e escalabilidade.

## Arquitetura do Sistema

A arquitetura pode ser representada pelo diagrama de blocos abaixo:

```
+---------------------+
|   Input Directory   |
|  (data/input)      |
+---------------------+
          |
          v
+---------------------+
|   Image Processor   |
|  (src/image_processor)|
+---------------------+
          |
          |  Aplica filtros
          v
+---------------------+
|   Filter Module     |
|  (OpenMP / MPI)     |
+---------------------+
          |
          v
+---------------------+
|   Output Directory  |
|  (data/output)     |
+---------------------+
```

### Componentes

1. **Input Directory**: Contém as imagens a serem processadas. As imagens são carregadas pelo `ImageProcessor` classe.

2. **Image Processor**: Este componente é responsável por carregar imagens, aplicar filtros e salvar as imagens processadas. Ele atua como a interface principal para tarefas de processamento de imagens.

3. **Filter Module**: Este módulo implementa a lógica de filtragem. Ele pode utilizar OpenMP para paralelismo em memória compartilhada ou MPI para paralelismo em memória distribuída, dependendo da configuração e do ambiente.

4. **Output Directory**: Armazena as imagens processadas após os filtros terem sido aplicados.

## Fluxo de Interacção

1. O aplicativo começa por inicializar o ambiente MPI ou OpenMP com base na escolha do usuário.
2. As imagens são carregadas do diretório de entrada usando o `ImageProcessor`.
3. O `ImageProcessor` delega as tarefas de filtragem ao módulo de filtro apropriado (OpenMP ou MPI).
4. Os filtros são aplicados em paralelo, e as imagens processadas são salvas no diretório de saída.

## Tecnologias Utilizadas

- **C++**: A linguagem principal para implementar o aplicativo.
- **MPI**: Usado para processamento distribuído em múltiplos nós.
- **OpenMP**: Usado para processamento paralelo em sistemas com memória compartilhada.
- **CMake**: Para gerenciar o processo de build e dependências.

Esta arquitetura garante que o sistema possa lidar eficientemente com grandes conjuntos de dados, aproveitando as forças de MPI e OpenMP, fornecendo flexibilidade e escalabilidade para tarefas de processamento de imagens.