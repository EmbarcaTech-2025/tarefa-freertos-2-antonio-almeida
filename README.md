Tarefa: Roteiro de FreeRTOS #2 - EmbarcaTech 2025

Autor: Antonio Almeida

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, 24 de junho de 2025.

---

## Conceito Técnico – Aplicação FreeRTOS em C, atualizando de forma dinâmica, OLED da BitDogLab (BDL*) em ação de 'ping' feito ao servidor Google https://8.8.8.8, usando suporte a Wi-Fi integrado ao chip CYW43 da Pico W.
**Com total autorização das partes e autores originais** — conforme indicado no cabeçalho de cada arquivo `.c` e `.h`.  
> Nenhum trecho de código foi utilizado sem respeito às licenças. Sigo fielmente as orientações dos autores: manter o código em **formato aberto e sem restrições!!!**.

Neste projeto, a BDL* e seu Pico W roda um sistema **FreeRTOS** com múltiplas tarefas, implementando **conectividade Wi-Fi** e **atualização dinâmica de display OLED.**

- Este é um ótimo exemplo de aproveitamento e integração de código (funcionalidade), pois a uma aplicação totalmente pronta, a qual, já fazia parte do SDK foi possível integrar uma nova funcionalidade sem obstáculos.
- A aplicação do SDK, concebida para rodar multi-usuário/multi-tarefa (RTOS) e que faz 'ping' em um servidor da 'Raspberry Foundation', até então, com suporte stdout para a serial exclusivamente.
- De forma harmoniosa foi integrada a funcionalidade que roda 'oled_ping_task' interage e integra-se para compor uma nova aplicação que possibilita visualizar os tempos de 'GET e resposta ao servidor da Google'.
- Isso é apenas a ponta do iceberg, pois, respeitando as características inerentes ao Pico W (BDL), pode-se criar novas task's que respondam a outras funcionalidades em acordo a suas especificações de projeto...

---

Após inicializar o FreeRTOS, duas tasks principais são criadas:

- **`main_task`**  
  Responsável por **conectar a BDL** à rede Wi-Fi em modo estação.  
  Após a conexão, inicia o módulo de **ping ICMP** usando a **pilha lwIP (RAW API)**, configurada para monitorar a latência até o IP público do Google: `8.8.8.8`.

- **`oled_ping_task`**  
  Uma tarefa dedicada exclusivamente à **atualização do display OLED via I²C**.  
  Verifica constantemente a flag `oled_needs_update`, e quando verdadeiro (true), atualiza as duas linhas de texto usando a função `update_oled_text()`.

---

## Arquitetura

A aplicação segue princípios sólidos de projeto embarcado:

- **Alta coesão**  
  Cada tarefa executa uma responsabilidade bem definida (Wi-Fi/ping ou OLED), facilitando os três tipos principais de manutenção: corretiva, preventiva e preditiva.

- **Baixo acoplamento**  
  As tarefas se comunicam de forma simples e independente sem bloqueios entre si. Tudo que ela precisa encontra-se no seu próprio domínio. (não há chamadas blocantes de ambos lados!!!).

- **Escalabilidade e modularidade**  
  O projeto permite fácil extensão para múltiplas funcionalidades, como: sensores, atuadores ou outros periféricos, em ressonância aos requisítos que qualquer projeto necessite.

---

## ⚙️ Execução

- O FreeRTOS roda sobre o SDK da Pico, utilizando o **core 0** por padrão.
- As tasks são agendadas cooperativamente usando `vTaskDelay()`.
- A **comunicação entre a pilha de rede e o display** é feita por meio de variáveis globais, atualizadas por callbacks do `ping.c`. (Isso demostra baixíssimo acoplamento, uma vez que a variável poderia fornecer informações a quaisquer tarefas que às necessite sem diretamente fazer parte de nenhuma delas).

---

## ✅ Benefícios

- **Interface visual responsiva** graças à thread dedicada do OLED.
- **Separação clara de responsabilidades**, que facilita entendimento e manutenção futuras.
- **Código portátil e reutilizável**, podendo ser adaptado para outras plataformas compatíveis com FreeRTOS.

---

> 💡 *Essa estrutura demonstra como sistemas embarcados modernos podem integrar conectividade, multitarefa e feedback visual de forma profissional e eficiente com FreeRTOS comprovando que: "o céu é o limite!".*

---

### 📽️ Video... 

[![Vídeo de Apresentação do Projeto](https://github.com/EmbarcaTech-2025/tarefa-freertos-2-antonio-almeida/blob/main/assets/ping.png)](https://www.youtube.com/watch?v=GLwqQY0oyi4)

---

## ⚠️ Observação Importante – Variáveis de Ambiente

Para que este exemplo funcione corretamente, é **imprescindível configurar todas variáveis de ambiente** no momento da compilação, aí sim será possível uma execução como a do vídeo:

- Nome da rede Wi-Fi à qual a placa irá se conectar.
- Senha correspondente à rede Wi-Fi.
- *(opcional)* – Endereço IP ou domínio que será utilizado para os testes de ping (por padrão: `8.8.8.8`).

Essas variáveis podem ser definidas de forma:

- **Direta no código** para fins de testes locais,
- Ou preferencialmente via **parâmetros de compilação**.

> 💡 **Dica de segurança:** Evite incluir em arquivos versionados. Prefira passá-la via linha de comando ou arquivos `.env` os quais devem ser ignorados pelo Git.

---

📜 Licença
GNU GPL-3.0.
