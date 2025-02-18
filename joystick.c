#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include <math.h>

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
const uint16_t PERIOD = 2048; // Definindo o WRAP ou máxima contagem do PWM
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

static volatile bool led_state = true; 

// Inicializa a estrutura do display
ssd1306_t ssd; 

// Inicialização dos lEDs e Botões
void init_all() {
    gpio_init(G_LED);
    gpio_set_dir(G_LED, GPIO_OUT);
    gpio_put(G_LED, 0);

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

void adc_setup(){

    adc_init();
    adc_gpio_init(XAXIS);
    adc_gpio_init(YAXIS);

}

void pwm_setup(){
    gpio_set_function(R_LED, GPIO_FUNC_PWM); 
    uint R_LED_slice = pwm_gpio_to_slice_num(R_LED);   
    pwm_set_clkdiv(R_LED_slice, PWM_DIVISER);            
    pwm_set_wrap(R_LED_slice, PERIOD);  
    pwm_set_gpio_level(R_LED, R_LED_level);            
    pwm_set_enabled(R_LED_slice, true); 
    
    gpio_set_function(B_LED, GPIO_FUNC_PWM); 
    uint B_LED_slice = pwm_gpio_to_slice_num(B_LED);   
    pwm_set_clkdiv(B_LED_slice, PWM_DIVISER);            
    pwm_set_wrap(B_LED_slice, PERIOD);
    pwm_set_gpio_level(B_LED, B_LED_level);              
    pwm_set_enabled(B_LED_slice, true); 
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

            }
            else if (gpio == B_BUTTON){

            }
            else if (gpio == J_BUTTON){

                led_state = !led_state;
                if (led_state == false){
                    pwm_set_gpio_level(R_LED, 0); 
                    pwm_set_gpio_level(B_LED, 0);
                }


            }
    }

}

// Função principal
int main() {
    // Inicializa clock, stdio e configurações
    stdio_init_all();
    init_all();
    display_init();
    adc_setup();
    pwm_setup();

    printf("Sistema inicializado.\n");

    // Configuração dos botões como interrupções
    gpio_set_irq_enabled_with_callback(A_BUTTON, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(B_BUTTON, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(J_BUTTON, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C); // Configura o pino do GPIO para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C); // Configura o pino do GPIO para I2C
    gpio_pull_up(I2C_SDA); // Ativa um resistor Pull Up para linha de data
    gpio_pull_up(I2C_SCL); // Ativa um resistor Pull Up para linha de clock
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    int x_value, y_value = 0;

    int dx, dy = 0;

    int TAM = 8;

    while (true) {
        
        adc_select_input(ADCC_0);
        sleep_us(10);
        x_value = adc_read();

        adc_select_input(ADCC_1);
        sleep_us(10);
        y_value = adc_read();

        printf("x_value = %i y_value = %i \n", x_value, y_value);

        if (led_state == true){
            if (x_value > 2048){
                pwm_set_gpio_level(B_LED, x_value-2048);
            }
            else if (x_value < 2048){
                pwm_set_gpio_level(B_LED, 2048-x_value);
            }

            if (y_value > 2048){
                pwm_set_gpio_level(R_LED, y_value-2048); 
            }
            else if (y_value < 2048){
                pwm_set_gpio_level(R_LED, 2048-y_value);
            }
        }
        
        dx = 60-(x_value*60/4096)-(TAM/2);
        dy = y_value*120/4096;

        printf("dx = %i, dy = %i \n", dx, dy);

        ssd1306_fill(&ssd, false);

        if (led_state == false){
            ssd1306_rect(&ssd, 2, 2, 124, 60, 1, 0);
        }
        ssd1306_rect(&ssd, 3, 3, 122, 58, 1, 0);

        if (dx < 0){
            ssd1306_rect(&ssd, -dx, dy, TAM, TAM, 1, 1);
        }
        else{
            ssd1306_rect(&ssd, dx, dy, TAM, TAM, 1, 1);
        }

        ssd1306_send_data(&ssd); // Manda a informação para o display

        sleep_ms(50);
    }
    return 0;

}
