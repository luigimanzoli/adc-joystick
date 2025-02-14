#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// Bibliotecas referentes à configuração do display
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

// Definição dos pinos ADC

const int XAXIS = 26; const int ADCC_0 = 0; // Puno do eixo X do Joystick e seu canal correspondente
const int YAXIS = 27; const int ADCC_1 = 1; // Puno do eixo Y do Joystick e seu canal correspondente

// Definição dos pinos PWM

const float PWM_DIVISER = 16.0; // Definindo o Divisor do PWM
const uint16_t PERIOD = 4096; // Definindo o WRAP ou máxima contagem do PWM
uint16_t R_LED_level, B_LED_level = 100;
uint R_LED_slice, B_LED_slice;

// Definição dos LEDs RGB
#define R_LED 13
#define G_LED 11
#define B_LED 12

// Definição dos botões
#define A_BUTTON 5
#define B_BUTTON 6
#define J_BUTTON 22 // Botão do Joystick

// Variável ligada ao debounce dos botões
static volatile uint32_t last_time = 0; 

// Inicializa a estrutura do display
ssd1306_t ssd; 

// Inicialização dos lEDs e Botões
void init_all() {
    gpio_init(R_LED);
    gpio_set_dir(R_LED, GPIO_OUT);
    gpio_put(R_LED, 0);

    gpio_init(G_LED);
    gpio_set_dir(G_LED, GPIO_OUT);
    gpio_put(G_LED, 0);

    gpio_init(B_LED);
    gpio_set_dir(B_LED, GPIO_OUT);
    gpio_put(B_LED, 0);

    gpio_init(A_BUTTON);
    gpio_set_dir(A_BUTTON, GPIO_IN);
    gpio_pull_up(A_BUTTON);

    gpio_init(B_BUTTON);
    gpio_set_dir(B_BUTTON, GPIO_IN);
    gpio_pull_up(B_BUTTON);

    gpio_init(J_BUTTON);
    gpio_set_dir(J_BUTTON, GPIO_IN);
    gpio_pull_up(J_BUTTON);
}

// Função que é chamada quando ocorre a interrupção
void gpio_irq_handler(uint gpio, uint32_t events){

    // Definição da variável do tempo atual do sistema dês do começo
    uint32_t current_time = to_us_since_boot(get_absolute_time());
        if (current_time - last_time > 200000){
            last_time = current_time;
            
            if (gpio == A_BUTTON){

                gpio_put(G_LED, !gpio_get(G_LED)); // Alterna o estado do LED verde
                printf("Estado do LED Verde Alternado.\n");

                if (gpio_get(G_LED)){
                    ssd1306_draw_string(&ssd, "LED Verde", 2, 8); // Desenha uma string 
                    ssd1306_draw_string(&ssd, "         ", 2, 21);
                    ssd1306_draw_string(&ssd, "LIGADO", 2, 21);
                }else{
                    ssd1306_draw_string(&ssd, "LED Verde", 2, 8); // Desenha uma string 
                    ssd1306_draw_string(&ssd, "DESLIGADO", 2, 21);
                }
                ssd1306_send_data(&ssd); // Atualiza o display  

            }
            else if (gpio == B_BUTTON){

                gpio_put(B_LED, !gpio_get(B_LED)); // Alterna o estado do LED azul
                printf("Estado do LED Azul Alternado.\n");

                if (gpio_get(B_LED)){
                    ssd1306_draw_string(&ssd, "LED Azul", 2, 38); // Desenha uma string
                    ssd1306_draw_string(&ssd, "         ", 2, 51); 
                    ssd1306_draw_string(&ssd, "LIGADO", 2, 51);
                }else{
                    ssd1306_draw_string(&ssd, "LED Azul", 2, 38); // Desenha uma string 
                    ssd1306_draw_string(&ssd, "DESLIGADO", 2, 51);
                }
                ssd1306_send_data(&ssd); // Atualiza o display

            }
            else if (gpio == J_BUTTON){

            }
    }

}

void display_init(){

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Configura o pino do GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Configura o pino do GPIO para I2C
    gpio_pull_up(I2C_SDA); // Ativa um resistor Pull Up para linha de data
    gpio_pull_up(I2C_SCL); // Ativa um resistor Pull Up para linha de clock
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

}

// Função principal
int main() {
    // Inicializa clock, stdio e configurações
    stdio_init_all();
    init_all();
    display_init();

    printf("Sistema inicializado.\n");

    // Configuração dos botões como interrupções
    gpio_set_irq_enabled_with_callback(A_BUTTON, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(B_BUTTON, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(J_BUTTON, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    while (true) {
        
        sleep_ms(1000);
    }
    return 0;

}
