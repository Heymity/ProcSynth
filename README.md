# ProcSynth - Sintetizador Digital / "Theremin" Híbrido

## Definição e motivação

ProcSynth é um sintetizador de sons baseado no instrumento Theremin. Diferente de seu predecessor, qu um dispositivo que cria sons baseado na capacitância dos objetos próximos ao seu sensor. Já o ProSynth utiliza um sensor de distância para determinar a frequência do som que irá produzir. O objetivo desse desenvolvimento é gerar uma prova de conceito funcional que exercita os conteúdos aprendidos e praticados durante os experimentos da matéria de Laboratório de Processadores.

## Requisitos do projeto

| Id | Tipo | Nome | Descrição |
| --- | --- | --- | --- |
| RF1 | Funcional | Controle por ultrassom | O sistema deve mapear a distância lida pelo sensor ultrassônico na alteração contínua da frequência (pitch) ou volume do som gerado. |
| RF2 | Funcional  | Entrada Matricial / Sequenciador | O Keypad 4x4 deve permitir o acionamento direto de notas musicais (estilo piano) ou atuar como um sequenciador de batidas programável |
| RF3 | Funcional | Ajuste de Parâmetros Analógicos |   Os potenciômetros devem permitir o ajuste em tempo real de parâmetros do áudio, tais como frequência de corte (Cutoff) de filtro ou distorção.|
| RF4 | Funcional | Interface gráfica no LCD | O Display LCD deve apresentar os menus de configuração, o modo ativo do sintetizador e a visualização dos parâmetros selecionados. |
| RF5 | Funcional | Emissão de Áudio via PWM | O áudio sintetizado deve ser emitido via PWM por hardware acoplado ao Buzzer ou saída física de áudio. |
| RN1 | Não-funcional | Latência baixa e determinismo | O tempo de resposta entre qualquer entrada do usuário e a mudança na saída de áudio não deve ser perceptível (< 20 ms). |
| RN2 | Não-funcional | Otimização por DMA | A alimentação dos buffers do periférico de PWM deve ser realizada via DMA para descarregar a CPU e eliminar atrasos (jitter). |
| RN3 | Não-funcional | Varredura assíncrona e debounce | A leitura das entradas do teclado matricial deve possuir tratamento de debounce e ser realizada de forma não-bloqueante.|
| RN4 | Não-funcional | Exibição visual do volume de saída pela Led Bar | A Led Bar do kit deve exibir uma quantidade crescente de luzes baseada no quão alto é o som sintetizado. |

## 




