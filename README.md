#  EmbarcaTech - Atividade Display e Comunicação Serial

##  Objetivos:

  

• Compreender o funcionamento e a aplicação de comunicação serial em microcontroladores.

  

• Aplicar os conhecimentos adquiridos sobre UART e I2C na prática.

  

• Manipular e controlar LEDs comuns e LEDs endereçáveis WS2812.

  

• Fixar o estudo do uso botões de acionamento, interrupções e Debounce.

  

• Desenvolver um projeto funcional que combine hardware e software

  

##  Materiais utilizados:

  

Neste projeto, são utilizados os seguintes componentes conectados à placa BitDogLab:

  

• Matriz 5x5 de LEDs (endereçáveis) WS2812, conectada à GPIO 7.

  

• LED RGB, com os pinos conectados às GPIOs (11, 12 e 13).

  

• Botão A conectado à GPIO 5.

  

• Botão B conectado à GPIO 6.

  

• Display SSD1306 conectado via I2C (GPIO 14 e GPIO15).

  

##  Funcionalidades Implementadas:

  

- Modificação da Biblioteca font.h

  

	- Adicionar caracteres minúsculos à biblioteca font.h. Use sua criatividade para criar os novos
caracteres.

  

- Entrada de caracteres via PC

  

	- Utilize o Serial Monitor do VS Code para digitar os caracteres.

	- Cada caractere digitado no Serial Monitor deve ser exibido no display SSD1306.

  

	- Observação: Apenas um caractere será enviado de cada vez, não é necessário suportar o
envio de strings completas.

  
	- Quando um número entre 0 e 9 for digitado, um símbolo correspondente ao número deve ser exibido, também, na matriz 5x5 WS2812.

  

- Interação com o Botão A

	- Pressionar o botão A deve alternar o estado do LED RGB Verde (ligado/desligado).

	- A operação deve ser registrada de duas formas:

  

		- Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306
		- Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.

  

- Interação com o Botão B

  

	- Pressionar o botão B deve alternar o estado do LED RGB Azul (ligado/desligado).

	- A operação deve ser registrada de duas formas:

		- Uma mensagem informativa sobre o estado do LED deve ser exibida no display SSD1306
		- Um texto descritivo sobre a operação deve ser enviado ao Serial Monitor.

  

##  Tecnologias:

  

1. Git e Github;

2. VScode;

3. Linguagem C;

4. Simulador Wokwi;

  
  

##  Desenvolvedor:
**Luigi Manzoli Almeida Lopez**

  

##  Demonstração: https://youtube.com/shorts/Dfm9ob1SFAM