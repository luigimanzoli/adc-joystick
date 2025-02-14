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

// Definição dos LEDs RGB
#define RLED_PIN 13
#define GLED_PIN 11
#define BLED_PIN 12

// Definição dos botões
#define BTNA_PIN 5
#define BTNB_PIN 6
#define BTNJ_PIN 22 // Botão do Joystick

// Variável ligada ao debounce dos botões
static volatile uint32_t last_time = 0; 

// Inicializa a estrutura do display
ssd1306_t ssd; 

// Inicialização dos lEDs e Botões
void init_all() {
    gpio_init(RLED_PIN);
    gpio_set_dir(RLED_PIN, GPIO_OUT);
    gpio_put(RLED_PIN, 0);

    gpio_init(GLED_PIN);
    gpio_set_dir(GLED_PIN, GPIO_OUT);
    gpio_put(GLED_PIN, 0);

    gpio_init(BLED_PIN);
    gpio_set_dir(BLED_PIN, GPIO_OUT);
    gpio_put(BLED_PIN, 0);

    gpio_init(BTNA_PIN);
    gpio_set_dir(BTNA_PIN, GPIO_IN);
    gpio_pull_up(BTNA_PIN);

    gpio_init(BTNB_PIN);
    gpio_set_dir(BTNB_PIN, GPIO_IN);
    gpio_pull_up(BTNB_PIN);

    gpio_init(BTNJ_PIN);
    gpio_set_dir(BTNJ_PIN, GPIO_IN);
    gpio_pull_up(BTNJ_PIN);
}

// Função que é chamada quando ocorre a interrupção
void gpio_irq_handler(uint gpio, uint32_t events){

    // Definição da variável do tempo atual do sistema dês do começo
    uint32_t current_time = to_us_since_boot(get_absolute_time());
        if (current_time - last_time > 200000){
            last_time = current_time;
            
            if (gpio == BTNA_PIN){

                gpio_put(GLED_PIN, !gpio_get(GLED_PIN)); // Alterna o estado do LED verde
                printf("Estado do LED Verde Alternado.\n");

                if (gpio_get(GLED_PIN)){
                    ssd1306_draw_string(&ssd, "LED Verde", 2, 8); // Desenha uma string 
                    ssd1306_draw_string(&ssd, "         ", 2, 21);
                    ssd1306_draw_string(&ssd, "LIGADO", 2, 21);
                }else{
                    ssd1306_draw_string(&ssd, "LED Verde", 2, 8); // Desenha uma string 
                    ssd1306_draw_string(&ssd, "DESLIGADO", 2, 21);
                }
                ssd1306_send_data(&ssd); // Atualiza o display  

            }
            else if (gpio == BTNB_PIN){

                gpio_put(BLED_PIN, !gpio_get(BLED_PIN)); // Alterna o estado do LED azul
                printf("Estado do LED Azul Alternado.\n");

                if (gpio_get(BLED_PIN)){
                    ssd1306_draw_string(&ssd, "LED Azul", 2, 38); // Desenha uma string
                    ssd1306_draw_string(&ssd, "         ", 2, 51); 
                    ssd1306_draw_string(&ssd, "LIGADO", 2, 51);
                }else{
                    ssd1306_draw_string(&ssd, "LED Azul", 2, 38); // Desenha uma string 
                    ssd1306_draw_string(&ssd, "DESLIGADO", 2, 51);
                }
                ssd1306_send_data(&ssd); // Atualiza o display

            }
            else if (gpio == BTNJ_PIN){

            }
    }

}

// Função principal
int main() {
    // Inicializa clock, stdio e configurações
    stdio_init_all();
    init_all();

    printf("Sistema inicializado.\n");

    // Configuração dos botões como interrupções
    gpio_set_irq_enabled_with_callback(BTNA_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTNB_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BTNJ_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA); // Pull up the data line
    gpio_pull_up(I2C_SCL); // Pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    while (true) {
        
        sleep_ms(1000);
    }
    return 0;

}
