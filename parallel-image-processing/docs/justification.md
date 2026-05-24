# Justificativa para o Projeto de Processamento Paralelo de Imagens

## Definição do Problema
O crescimento exponencial de imagens digitais em áreas como saúde, redes sociais e pesquisa científica criou uma necessidade urgente por técnicas de processamento de imagens mais eficientes. Métodos sequenciais tradicionais podem ser insuficientes para conjuntos de dados grandes, resultando em tempos de processamento elevados e uso ineficiente de recursos. Este projeto busca resolver o desafio de processar um grande volume de imagens aplicando técnicas de processamento paralelo, usando MPI (Message Passing Interface) e OpenMP (Open Multi-Processing).

## Justificativa da Escolha das Tecnologias
1. **Processamento Paralelo**: O uso de processamento paralelo é essencial para lidar com grandes volumes de dados de imagem de forma eficiente. Distribuir a carga de trabalho entre múltiplos processadores reduz significativamente o tempo de processamento.

2. **MPI**: MPI é escolhido por sua robustez em ambientes de computação distribuída. Ele permite comunicação eficaz entre processos em diferentes nós, tornando-o adequado para tarefas de processamento de imagens em larga escala distribuídas em um cluster.

3. **OpenMP**: OpenMP é selecionado por sua simplicidade e eficácia em programação paralela de memória compartilhada. Ele fornece uma maneira direta de implementar paralelismo em aplicações que rodam em processadores multicore, permitindo desenvolvimento rápido e integração de filtros paralelos.

4. **Bibliotecas de Processamento de Imagem**: A utilização de bibliotecas existentes de processamento de imagem aumenta a funcionalidade e o desempenho da aplicação. Bibliotecas como OpenCV podem ser integradas para oferecer capacidades avançadas de manipulação de imagens.

## Resultados Esperados
O projeto pretende entregar uma aplicação funcional capaz de:
- Carregar um grande conjunto de imagens de um diretório especificado.
- Aplicar vários filtros (por exemplo, borrão, nitidez, detecção de bordas) em paralelo usando MPI ou OpenMP.
- Salvar as imagens processadas em um diretório de saída de maneira eficiente.

Ao utilizar técnicas de processamento paralelo, o projeto busca demonstrar melhorias significativas na velocidade de processamento e na utilização de recursos em comparação com abordagens tradicionais sequenciais. Isso vai melhorar a experiência do usuário e prover uma solução escalável para desafios reais de processamento de imagens.