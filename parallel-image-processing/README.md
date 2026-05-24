# Projeto de Processamento Paralelo de Imagens

## Visão Geral
O projeto de Processamento Paralelo de Imagens busca aproveitar técnicas de computação distribuída para processar de forma eficiente um grande conjunto de imagens, aplicando diversos filtros. Este projeto utiliza MPI (Message Passing Interface) e OpenMP (Open Multi-Processing) para demonstrar a eficácia do processamento paralelo em tarefas computacionalmente intensivas.

## Definição do Problema
No cenário atual, o volume de imagens gerado é imenso, e o processamento dessas imagens pode ser demorado. Métodos sequenciais tradicionais podem não ser eficientes para lidar com grandes conjuntos de dados. Este projeto resolve o desafio de processar imagens em paralelo, reduzindo significativamente o tempo necessário para aplicar filtros e melhorar imagens.

## Justificativa da Escolha das Tecnologias
- **MPI**: escolhido pela capacidade de suportar computação distribuída entre múltiplos nós, adequado para tarefas de processamento de imagens em larga escala.
- **OpenMP**: selecionado pela facilidade de paralelizar código em arquiteturas de memória compartilhada, permitindo multi-threading eficiente em uma única máquina.
- **C++**: linguagem utilizada por sua eficiência de desempenho e controle sobre recursos do sistema, importante para tarefas de processamento de imagem. Também foi a escolhida pela implementação de POO (Programação Orientada a Objetos)

## Arquitetura do Sistema
A arquitetura do sistema consiste em vários componentes:
- **ImageProcessor**: responsável por carregar imagens, aplicar filtros e salvar as imagens processadas.
- **MPIWorker**: gerencia a distribuição de tarefas entre múltiplos processos usando MPI.
- **OpenMP Filters**: implementa diversos filtros de imagem que podem ser aplicados em paralelo com OpenMP.

Consulte `docs/architecture.md` para o diagrama de blocos e a representação UML detalhada do sistema.

## Instruções de Configuração
1. Clone o repositório:
   ```
   git clone https://github.com/yourusername/parallel-image-processing.git
   ```
2. Navegue até o diretório do projeto:
   ```
   cd parallel-image-processing
   ```
3. Crie um diretório de build e navegue até ele:
   ```
   mkdir build && cd build
   ```
4. Execute CMake para configurar o projeto:
   ```
   cmake ..
   ```
5. Construa o projeto:
   ```
   make
   ```

## Uso
Para executar o aplicativo, certifique-se de que você tenha imagens de entrada no diretório `data/input`. Você pode executar o programa com o seguinte comando:
```
mpirun -np <number_of_processes> ./your_executable
```
ou para OpenMP:
```
export OMP_NUM_THREADS=<number_of_threads>
./your_executable
```

## Contribuição
Contribuições são bem-vindas! Por favor, não hesite em enviar um pull request ou abrir um issue para qualquer sugestão ou melhoria.

## Licença
Este projeto é licenciado sob a Licença MIT - veja o arquivo LICENSE para detalhes.