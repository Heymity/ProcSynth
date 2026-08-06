# ProcSynth - Lightweight Synthesizer 

[Installation and Use](#installation-and-use)

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

## Installation and Use

### Linux System

If running in a linux system, simply clone the repository and in its root folder execute
```sh
make configure
make run
```
And the synthesizer should be running. It will always use the default audio output as its output, so if no audio is played right after its execution, check which is the system default audio outputs with
```sh
aplay -l
```

If using a MIDI keyboard, check the program output for instructions on routing the device.

### Running on Windows via WSL

The program can be executed in Windows via WSL2 with WSLg enabled, though for MIDI keyboard support you will need to recompile the linux kernel with some additional features and install usbipd on your windows system.
To just run the program without MIDI support, simply follow the [Linux System](#linux-system) guide.

To enable MIDI in WSL2 you will need first to download and recompile the linux kernel with the provided `wslKernelConfig` file at the root of the repository. Inside your WSL execute:
```shell
cd ~
git clone https://github.com/microsoft/WSL2-Linux-Kernel.git
cd WSL2-Linux-Kernel
sudo apt install build-essential flex bison dwarves libssl-dev libelf-dev cpio qemu-utils rsync libncurses-dev bc pkg-config
cp </path/to/procSynth>/wslKernelConfig ./procSynthWslKernelConfig
```

If you wish to add any other features to your WSL, execute `make menuconfig KCONFIG_CONFIG=procSynthWslKernelConfig` and change the desired fields
In case the provided config file creates a problem, the options needed for MIDI support are the following:
1. Navigate to: Device Drivers -> Sound card support -> Advanced Linux Sound Architecture (Use `/` and search for `SOUND`, then press the number shown for the correct option)
2. Make sure `Sequencer Support` is already checked as `<*>` or `<M>`
3. Enter `USB Sound Devices` and enable USB Audio/MIDI drivers (as `<*>` or `<M>`) and under it enable `MIDI 2.0 support by USB Audio driver`

Now just compile the kernel and install its modules:
```shell
make KCONFIG_CONFIG=procSynthWslKernelConfig -j$(nproc) && make modules_install
```

Now copy the file generated under `arch/x86/boot/bzImage` to your Windows user folder (Win + R, `%userprofile%`). You may get permission denied trying to copy it directly from WSL shell (`cp arch/x86/boot/bzImage /mnt/c/Users/<USER_NAME>/bzImage`). Copy it via Windows explorer to avoid it.
In this same folder (`%userprofile%`) create (or edit) the file `.wslconfig` with the following content:
```toml
[wsl2]
kernel=C:\\Users\\<YOU_USER_NAME>\\bzImage
```

Now on your Windows powershell, execute 

`````shell
wsl --shutdown
`````

Wait a few seconds, enter your WSL again and navigate to the ProcSynth project root folder. In it execute the following:
```shell
make wslConfig
make configure
make run
```

And the program should be running with now errors on the MIDI thread.