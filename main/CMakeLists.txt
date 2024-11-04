# Define o executável usando 'main.c'
add_executable(main main.c)

# Linka o executável com as bibliotecas padrão do Pico
target_link_libraries(main pico_stdlib hardware_adc hardware_pwm)

# Cria os arquivos binários para upload na Pico
pico_add_extra_outputs(main)
