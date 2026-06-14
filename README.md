# Parallel Image Processing

## Visão Geral
Este projeto implementa um pipeline de processamento de imagens com suporte a paralelismo usando MPI e OpenMP. Ele lê imagens de um diretório, aplica filtros de imagem e salva os resultados em um diretório de saída.

O código foi escrito em C++17 e usa OpenCV para leitura, processamento e gravação de imagens.

## Arquivos Principais
- `CMakeLists.txt`
  - Configura o build usando CMake.
  - Localiza e linka OpenCV, OpenMP e MPI.
- `src/main.cpp`
  - Ponto de entrada do programa.
  - Escolhe entre os modos `mpi` e `openmp`.
  - Processa as imagens usando um filtro especificado.
- `src/image_processor.h` / `src/image_processor.cpp`
  - Classe `ImageProcessor` para carregar, filtrar e salvar imagens.
  - Filtra com `blur`, `sharpen`, `edge` (preto e branco) e `negative` (cores invertidas).
- `src/mpi_worker.h` / `src/mpi_worker.cpp`
  - Classe `MPIWorker` para inicializar e finalizar MPI.
  - Contém métodos auxiliares para enviar/receber buffers de imagem.
- `src/openmp_filters.cpp`
  - Implementa os filtros imagem para uso em laços OpenMP.

## Funcionamento do Código
### `ImageProcessor`
A classe `ImageProcessor` encapsula operações básicas de I/O de imagem:
- `loadImage(filename, image)`
  - Carrega a imagem colorida de `filename` usando OpenCV.
- `applyFilter(inputImage, outputImage, filterType)`
  - Aplica um filtro com base em `filterType`:
    - `blur`: desfoque Gaussiano 5x5.
    - `sharpen`: filtro de nitidez com kernel 3x3.
    - `edge`: detecção de bordas com Canny.
- `saveImage(filename, image)`
  - Salva a imagem processada em disco.
- `processImages(filterType)`
  - Processa todas as imagens de `inputDirectory` e grava em `outputDirectory`.

### `MPIWorker`
A classe `MPIWorker` controla a inicialização e finalização do ambiente MPI:
- `initMPI(argc, argv)`
  - Chama `MPI_Init`, `MPI_Comm_size` e `MPI_Comm_rank`.
- `finalizeMPI()`
  - Finaliza o ambiente MPI com `MPI_Finalize()`.
- `sendImage` / `receiveImage`
  - Métodos para enviar e receber buffers de imagem (não usados no fluxo principal atual).

### `openmp_filters`
Define funções de filtro que operam diretamente em `cv::Mat`:
- `applyBlur(image, kernelSize)` - desfoque Gaussiano
- `applySharpen(image)` - filtro de nitidez
- `applyEdgeDetection(image)` - conversão para preto e branco
- `applyNegative(image)` - inversão de cores
- `processImagesWithOpenMP(images, filterType)` - aplica o filtro selecionado a todas as imagens em paralelo

## Modos de Execução
O aplicativo suporta dois modos de processamento:

### MPI
Executa o processamento por processo distribuído. Cada processo trata uma parte das imagens conforme seu `rank`.
- Parâmetros:
  - `mode`: `mpi`
  - `input_dir`
  - `output_dir`
  - `filter` (opcional): `blur`, `sharpen`, `edge`, `negative` ou `mixed`
- Comportamento:
  - Cada processo recebe imagens usando a regra `i % size == rank`.
  - No modo `mixed`, cada processo usa um filtro diferente (`blur`, `sharpen`, `edge`, `negative`) de acordo com seu rank.
  - O processo de rank 0 coleta estatísticas de execução de todos os processos.
  - Benchmark de desempenho exibido ao final da execução.

### OpenMP
Executa o processamento em threads na mesma máquina.
- Parâmetros:
  - `mode`: `openmp`
  - `input_dir`
  - `output_dir`
  - `filter` (opcional): `blur`, `sharpen`, `edge`, `negative` ou `mixed`
- Comportamento:
  - Itera sobre as imagens e processa cada uma em paralelo usando `#pragma omp parallel for`.
  - No modo `mixed`, cada thread aplica um filtro diferente com base no ID da thread.
  - Estatísticas de distribuição de carga entre threads ao final da execução.

## Uso
### Compilação
No diretório `parallel-image-processing`:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Executando com OpenMP
```bash
./ParallelImageProcessing openmp <input_dir> <output_dir> [filter]
```
Exemplo:
```bash
./ParallelImageProcessing openmp ../data/input ../data/output blur
```

### Executando com MPI
```bash
mpirun -np <num_processes> ./ParallelImageProcessing mpi <input_dir> <output_dir> [filter]
```
Exemplo:
```bash
mpirun -np 4 ./ParallelImageProcessing mpi ../data/input ../data/output mixed
```

## Filtros Suportados
- `blur` - desfoque Gaussiano 5x5
- `sharpen` - filtro de nitidez com kernel 3x3
- `edge` - conversão para preto e branco (escala de cinza)
- `negative` - inversão de cores (cores negativas)
- `mixed` - modo misto que alterna entre filtros por processo (MPI) ou thread (OpenMP)

## Tutorial de Execução

### Pré-requisitos
1. **Compiladores e Ferramentas:**
   - Visual Studio 2022 Build Tools (ou similar) com suporte a C++17
   - CMake 3.10+
   - MS-MPI (Microsoft MPI)

2. **Bibliotecas:**
   - OpenCV (build com suporte MSVC)
   - OpenMP (incluído no MSVC)
   - MPI (MS-MPI)

### Passo a Passo de Instalação e Compilação

#### 1. Clonar ou Preparar o Projeto
```bash
cd "c:\Users\Hugog\OneDrive\Área de Trabalho\ProjetosCPD7Semestre\parallel-image-processing"
```

#### 2. Configurar OpenCV_DIR (se necessário)
Ajuste o `CMakeLists.txt` com o caminho correto da sua instalação do OpenCV:
```cmake
set(OpenCV_DIR "C:/caminho/para/opencv/build")
```

#### 3. Criar e Configurar a Build
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022" -A x64
```

#### 4. Compilar o Projeto
```bash
cmake --build . --config Release
```

#### 5. Preparar Dados de Entrada
- Coloque suas imagens em `data/input/` (formatos: `.jpg`, `.png`, `.jpeg`, `.bmp`)
- Certifique-se de que `data/output/` existe

#### 6. Configurar PATH do OpenCV (no Terminal)
Antes de executar, adicione ao PATH:
```bash
set PATH=%PATH%;C:\caminho\para\opencv\build\x64\vc15\bin
```

### Exemplos de Execução

#### OpenMP - Filtro Blur
```bash
Release\ParallelImageProcessing.exe openmp ..\data\input ..\data\output blur
```

#### OpenMP - Filtro Sharpen
```bash
Release\ParallelImageProcessing.exe openmp ..\data\input ..\data\output sharpen
```

#### OpenMP - Filtro Edge (Preto e Branco)
```bash
Release\ParallelImageProcessing.exe openmp ..\data\input ..\data\output edge
```

#### OpenMP - Filtro Negative (Cores Invertidas)
```bash
Release\ParallelImageProcessing.exe openmp ..\data\input ..\data\output negative
```

#### OpenMP - Modo Mixed (Threads com Filtros Diferentes)
```bash
Release\ParallelImageProcessing.exe openmp ..\data\input ..\data\output mixed
```

#### MPI com 4 Processos - Filtro Blur
```bash
mpiexec -n 4 Release\ParallelImageProcessing.exe mpi ..\data\input ..\data\output blur
```

#### MPI com 4 Processos - Modo Mixed (Processos com Filtros Diferentes)
```bash
mpiexec -n 4 Release\ParallelImageProcessing.exe mpi ..\data\input ..\data\output mixed
```

### Saída Esperada

**Exemplo OpenMP:**
```
OpenMP: threads = 4, imagens totais = 100
Mixed mode: thread-to-filter mapping:
  thread 0 -> blur
  thread 1 -> sharpen
  thread 2 -> edge
  thread 3 -> negative
OpenMP finalizado: 100 imagens em 2.5 s -> 40.0 img/s
  thread 0 processou 25 imagens
  thread 1 processou 25 imagens
  thread 2 processou 25 imagens
  thread 3 processou 25 imagens
```

**Exemplo MPI:**
```
MPI: processos = 4, imagens totais = 100
[rank 0] usando filtro (mixed) -> blur
[rank 1] usando filtro (mixed) -> sharpen
[rank 2] usando filtro (mixed) -> edge
[rank 3] usando filtro (mixed) -> negative
[rank 0] finalizado: 25 imagens em 0.65 s -> 38.46 img/s
[rank 1] finalizado: 25 imagens em 0.68 s -> 36.76 img/s
[rank 2] finalizado: 25 imagens em 0.70 s -> 35.71 img/s
[rank 3] finalizado: 25 imagens em 0.66 s -> 37.88 img/s
```

## Observações Importantes
- O código filtra arquivos em `main.cpp` por extensões suportadas (`.jpg`, `.png`, `.jpeg`, `.bmp`).
- As imagens processadas no modo `mixed` recebem sufixo indicando filtro e núcleo/thread.
- O desempenho varia conforme tamanho das imagens e número de núcleos/processos disponíveis.
- Use `set OMP_NUM_THREADS=<N>` para limitar threads do OpenMP antes de executar.
- Para obter melhor desempenho em MPI, use número de processos próximo ao número de cores do processador.

## Estrutura do Projeto
- `CMakeLists.txt`: configuração do build.
- `src/main.cpp`: controle de execução e modos de processamento.
- `src/image_processor.h/cpp`: abstração de carregamento, filtro e salvamento de imagens.
- `src/mpi_worker.h/cpp`: wrapper e inicialização de MPI.
- `src/openmp_filters.cpp`: filtros usados em processamento OpenMP.

