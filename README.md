#  EmbarcaTech - Atividade Conversores Analógicos-Digitais

##  Objetivos:

- Compreender o funcionamento do conversor analógico-digital (ADC) no RP2040.

- Utilizar o PWM para controlar a intensidade de dois LEDs RGB com base nos valores do joystick.

- Representar a posição do joystick no display SSD1306 por meio de um quadrado móvel.

- Aplicar o protocolo de comunicação I2C na integração com o display.


##  Materiais utilizados:

Neste projeto, são utilizados os seguintes componentes conectados à placa BitDogLab:


- LED RGB, com os pinos conectados às GPIOs (11, 12 e 13).

  
- Botão A conectado à GPIO 5.


- Botão B conectado à GPIO 6.


- Joystick possuindo um potenciometro para cada eixo (X e Y) e um botão conectado à GPIO 22.


- Conversores Analógicos-Digitais conectados às GPIOs 26 e 27.


- Display SSD1306 conectado via I2C (GPIO 14 e GPIO 15).


##  Funcionalidades Implementadas:

O joystick fornecerá valores analógicos correspondentes aos eixos X e Y, que serão utilizados para:

- Controlar a intensidade luminosa dos LEDs RGB, onde:
	- O LED Azul terá seu brilho ajustado conforme o valor do eixo Y. Quando o joystick estiver solto  (posição central - valor 2048), o LED permanecerá apagado. À medida que o joystick for movido para cima (valores menores) ou para baixo (valores maiores), o LED aumentará seu brilho gradualmente, atingindo a intensidade máxima nos extremos (0 e 4095).
	- O LED Vermelho seguirá o mesmo princípio, mas de acordo com o eixo X. Quando o joystick estiver solto (posição central - valor 2048), o LED estará apagado. Movendo o joystick para a esquerda (valores menores) ou para a direita (valores maiores), o LED aumentará de brilho, sendo mais intenso nos extremos (0 e 4095).
	- Os LEDs serão controlados via PWM para permitir variação suave da intensidade luminosa.

- Exibir no display SSD1306 um quadrado de 8x8 pixels, inicialmente centralizado, que se moverá proporcionalmente aos valores capturados pelo joystick.

- Adicionalmente, o botão do joystick terá as seguintes funcionalidades:
	- Alternar o estado do LED Verde a cada acionamento.
	- Modificar a borda do display para indicar quando foi pressionado, alternando entre diferentes estilos de borda a cada novo acionamento
	
- O botão A terá a seguinte funcionalidade:
	- Ativar ou desativar os LED PWM a cada acionamento.


##  Tecnologias:

1. Linguagem C

2. VSCode

3. Git e Github


##  Desenvolvedor:

**Luigi Manzoli Almeida Lopez**


##  Demonstração: https://youtube.com/shorts/UcgVbEEJRjY