# ProcSynth - Lightweight Synthesizer 

1. [Definition and Motivation](#definição-e-motivação)
2. [Project Functional and Non-functional Requirements](#requisitos-do-projeto)
3. [Working Principle](#working-principle)
4. [Installation and Use](#installation-and-use)

## Definição e motivação (PT-BR)

ProcSynth é um sintetizador de sons baseado no instrumento Theremin. Diferente de seu predecessor, qu um dispositivo que cria sons baseado na capacitância dos objetos próximos ao seu sensor. Já o ProSynth utiliza um sensor de distância para determinar a frequência do som que irá produzir. O objetivo desse desenvolvimento é gerar uma prova de conceito funcional que exercita os conteúdos aprendidos e praticados durante os experimentos da matéria de Laboratório de Processadores.

[Vídeo demonstrativo](https://www.youtube.com/watch?v=lAhP2kJ4F-w)

## Definition and Motivation (EN)

ProcSynth is a sound synthesizer based on the Theremin instrument. Unlike its predecessor, which is a device that generates sounds based on the capacitance of objects near its sensor, ProcSynth uses a distance sensor to determine the frequency of the sound it will produce. The goal of this development is to create a functional proof of concept that applies the concepts learned and practiced throughout the experiments in the Processor Laboratory course.

[Demo Video](https://www.youtube.com/watch?v=lAhP2kJ4F-w)

## Requisitos do projeto (PT-BR only)

| Id | Tipo | Nome                                             | Descrição |
| --- | --- |--------------------------------------------------| --- |
| RF1 | Funcional | Controle por ultrassom                           | O sistema deve mapear a distância lida pelo sensor ultrassônico na alteração contínua da frequência (pitch) ou volume do som gerado. |
| RF2 | Funcional  | Entrada Matricial / Sequenciador                 | O Keypad 4x4 deve permitir o acionamento direto de notas musicais (estilo piano) ou atuar como um sequenciador de batidas programável |
| RF3 | Funcional | Ajuste de Parâmetros Analógicos                  |   Os potenciômetros devem permitir o ajuste em tempo real de parâmetros do áudio, tais como frequência de corte (Cutoff) de filtro ou distorção.|
| RF4 | Funcional | Interface gráfica no LCD                         | O Display LCD deve apresentar os menus de configuração, o modo ativo do sintetizador e a visualização dos parâmetros selecionados. |
| RF5 | Funcional | Emissão de Áudio via PWM                         | O áudio sintetizado deve ser emitido via PWM por hardware acoplado ao Buzzer ou saída física de áudio. |
| RN1 | Não-funcional | Latência baixa e determinismo                    | O tempo de resposta entre qualquer entrada do usuário e a mudança na saída de áudio não deve ser perceptível (< 20 ms). |
| RN2 | Não-funcional | Audio com mínima fidelidade ao instrumento real  | A alimentação dos buffers do periférico de PWM deve ser realizada via DMA para descarregar a CPU e eliminar atrasos (jitter). |
| RN3 | Não-funcional | Varredura assíncrona e debounce                  | A leitura das entradas do teclado matricial deve possuir tratamento de debounce e ser realizada de forma não-bloqueante.|
| RN4 | Não-funcional | Exibição visual do volume de saída pela Led Bar  | A Led Bar do kit deve exibir uma quantidade crescente de luzes baseada no quão alto é o som sintetizado. |

## Principio de Funcionamento (PT-BR)

Um instrumento é caracterizado por seu timbre e sua modulação de amplitude. O timbre pode ser obtido por meio de uma Transformada Inversa de Fourier de seus componentes harmônicos. Utilizamos a biblioteca [PFFFT](https://github.com/marton78/pffft/tree/master) para executar a transformada de Fourier e calcular uma LUT para cada instrumento definido.

De acordo com a frequência fundamental da nota, o sintetizador calcula a fase correta para cada instante e utiliza o índice correspondente na LUT.

O outro componente principal do instrumento é o seu envelope de amplitude. Esse envelope é geralmente chamado de envelope ADSR (Attack, Decay, Sustain, Release). O Attack de uma nota é o tempo necessário para que ela passe de 0 até sua amplitude máxima. O Decay é o tempo necessário para que a amplitude passe do valor máximo até seu nível de sustain. Durante a etapa de Sustain, a amplitude permanece constante até que a tecla seja solta. Já o Release é o tempo necessário para que a amplitude passe do nível de sustain até 0 após a tecla ser solta.

Um piano, por exemplo, possui um tempo de attack próximo de zero, um decay longo, um nível de sustain próximo de zero (ou até mesmo zero) e um release relativamente rápido (mais longo que o attack, mas muito mais rápido que o decay).

Um violino, por outro lado, possui um tempo de attack relativamente lento (e inversamente proporcional à velocidade com que a tecla é pressionada), decay zero, sustain de 100% e release rápido.

Além dos princípios de síntese, utilizamos a biblioteca ALSA do Linux para fazer a interface com a saída de áudio e a entrada MIDI do sistema. No entanto, toda a geração do som é realizada neste projeto.

## Working Principle (EN)

An instrument is characterized by its timbre and its amplitude modulation. The timbre can be achieved via a backwards Fourier Transform of its harmonic components. We have used the [PFFFT](https://github.com/marton78/pffft/tree/master) library to run the fourier transform and compute a LUT for each instrument defined.
According to the base frequency of the note, the synthesizer computes the correct phase for each moment and indexes the LUT accordingly.

The other main component of the instrument is it's amplitude envelope. This envelope is usually called the ADSR envelope (Attack, Decay, Sustain, Release). The Attack of a note is the time it takes to go from 0 to its full amplitude. Decay is the time for the amplitude to go from the max to its sustain amplitude. In the sustain stage it remains with constant amplitude until the key is released. And the Release time is how long it takes to go from the sustain level to 0 when a key is released. A piano, for example, has near zero attack time, long decay, near zero sustain level (or even zero) and somewhat quick release (longer than attack, way quicker than decay)
A violin on the other hand has a somewhat slow attack time (and inversely proportional to the key speed), zero decay, 100% sustain, and quick release.

Aside from the synthesis principles, we use the ALSA Linux library to interface with the systems sound output and MIDI input. But all the actual sound is produced in this project.

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

Be aware that the synthesizer will most likely lag after running for a bit due to it being virtualized inside the WSL. This is not really recommended, but it's a good way to test it and develop it in a Windows system.

The program can be executed in Windows via WSL2 with WSLg enabled, though for MIDI keyboard support you will need to recompile the linux kernel with some additional features and install usbipd on your Windows system.
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

And the program should be running with now errors on the MIDI thread. If the `make wslConfig` command generate an error about a module not being present, go back to the kernel folder and execute again `make modules_install`. If that doesn't work, check if the version given by `uname -r` matches the folder where it is looking for the module, you might be using the wrong kernel build.


Now on a Windows *admin* terminal execute the following to add and share your MIDI device to the wsl
````shell
winget install usbipd
# The above command must be executed only once; restart your terminal to take effect
# The command below will list the usb devices
usbipd list
usbipd bind --usbid XX-YY # Replace XX-YY with the ID of your device shown in the list command
usbipd attach --wsl --busid XX-YY # Replace XX-YY with the ID of your device shown in the list command
````

Now execute the procSynth (`make run`). In its therminal it will show instructions to route your MIDI device (`aconnecet -l` and `aconnect XX YY` where XX and YY are the ids seen on the first command of your device and procSynth) 