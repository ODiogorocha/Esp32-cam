
```markdown
# ESP32-CAM: Visão Computacional para Reconhecimento de Bola Laranja

Este projeto utiliza a **ESP32-CAM** para realizar o reconhecimento de uma bola laranja utilizando **OpenCV**. O objetivo é demonstrar a capacidade de captura de imagens e processamento de imagem em tempo real na placa ESP32.

---

## Índice

- [Recursos](#recursos)
- [Tecnologias Utilizadas](#tecnologias-utilizadas)
- [Pré-requisitos](#pré-requisitos)
- [Instalação](#instalação)
- [Uso](#uso)
- [Contribuição](#contribuição)
- [Licença](#licença)

---

## Recursos

- 📷 Captura de vídeo em tempo real.
- 🎨 Processamento de imagem para detecção de cor laranja.
- 💻 Integração com a IDE do Arduino.
- 📚 Exemplos de uso e código de implementação.

---

## Tecnologias Utilizadas

- **ESP32-CAM**
- **Arduino IDE**
- **OpenCV**
- **Python** (para processamento de imagem)

---

## Pré-requisitos

Antes de começar, você precisará ter os seguintes programas instalados:

- [Arduino IDE](https://www.arduino.cc/en/software)
- [Biblioteca ESP32 para Arduino](https://github.com/espressif/arduino-esp32)
- [OpenCV](https://opencv.org/releases/)

### Instalação do OpenCV (Python)

Para instalar o OpenCV, execute o seguinte comando:

```bash
pip install opencv-python
```

---

## Instalação

1. Clone este repositório:

   ```bash
   git clone https://github.com/seu-usuario/esp32-cam-bola-laranja.git
   cd esp32-cam-bola-laranja
   ```

2. Abra a Arduino IDE.

3. Carregue o código da ESP32-CAM (localizado na pasta `codigo/ESP32_Camera.ino`).

4. Configure a IDE para usar a ESP32-CAM (selecione a placa correta em **Ferramentas > Placa**).

5. Instale as bibliotecas necessárias se não estiverem incluídas por padrão.

---

## Uso

1. Conecte a ESP32-CAM ao seu computador e faça o upload do código.
2. Abra a porta serial na IDE do Arduino para ver os logs.
3. Acesse o endereço IP da ESP32-CAM em um navegador para visualizar o stream de vídeo.
4. A detecção da bola laranja ocorrerá em tempo real no stream de vídeo.

---

## Contribuição

Contribuições são bem-vindas! Sinta-se à vontade para enviar um pull request ou abrir uma issue.

1. Faça um fork do projeto.
2. Crie uma nova branch: 

   ```bash
   git checkout -b minha-contribuicao
   ```

3. Faça suas alterações e commit: 

   ```bash
   git commit -m 'Adicionando uma nova funcionalidade'
   ```

4. Envie para o repositório remoto: 

   ```bash
   git push origin minha-contribuicao
   ```

5. Abra um pull request.

---

## Licença

Este projeto está licenciado sob a Licença MIT - veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---

## Instruções de Personalização

1. **Título e Descrição:** Altere o título e a descrição do projeto para refletir o que você deseja.
2. **URL do Repositório:** Certifique-se de substituir `seu-usuario` pela sua conta do GitHub.
3. **Código da ESP32-CAM:** Certifique-se de que o caminho para o código e qualquer outra informação esteja correta.
4. **Seções Opcionais:** Você pode adicionar seções adicionais como "Demonstração", "Screenshots" ou "Referências" conforme necessário.

---

## Código da ESP32-CAM

```cpp
#include "esp_camera.h"
#include <WiFi.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "fb_gfx.h"
#include "soc/soc.h" 
#include "soc/rtc_cntl_reg.h"  
#include "esp_http_server.h"

// Definindo as credenciais do Wi-Fi
const char* ssid = "TauraBots";
const char* password = "robotica2024";

// Definindo os pinos da câmera
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Definindo o formato de vídeo do servidor 
#define PART_BOUNDARY "123456789000000000000987654321"
static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

httpd_handle_t stream_httpd = NULL;

// Manipulador do servidor para streaming
static esp_err_t stream_handler(httpd_req_t *req) {
  camera_fb_t * fb = NULL;
  esp_err_t res = ESP_OK;
  size_t _jpg_buf_len = 0;
  uint8_t * _jpg_buf = NULL;
  char part_buf[64];

  res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
  if (res != ESP_OK) {
    return res;
  }

  while (true) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      res = ESP_FAIL;
    } else {
      if (fb->format != PIXFORMAT_JPEG) {
        bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
        esp_camera_fb_return(fb);
        fb = NULL;
        if (!jpeg_converted) {
          Serial.println("JPEG compression failed");
          res = ESP_FAIL;
        }
      } else {
        _jpg_buf_len = fb->len;
        _jpg_buf = fb->buf;
      }
    }

    if (res == ESP_OK) {
      size_t hlen = snprintf(part_buf, 64, _STREAM_PART, _jpg_buf_len);
      res = httpd_resp_send_chunk(req, part_buf, hlen);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
    }
    if (res == ESP_OK) {
      res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
    }
    
    if (fb) {
      esp_camera_fb_return(fb);
      fb = NULL;
      _jpg_buf = NULL;
    } else if (_jpg_buf) {
      free(_jpg_buf);
      _jpg_buf = NULL;
    }
    
    if (res != ESP_OK) {
      break;
    }
  }
  return res;
}

// Inicia o servidor HTTP
void startCameraServer() {
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.server_port = 80;

  httpd_uri_t index_uri = {
    .uri       = "/",
    .method    = HTTP_GET,
    .handler   = stream_handler,
    .user_ctx  = NULL
  };

  if (httpd_start(&stream_httpd, &config) == ESP_OK) {
    httpd_register_uri_handler(stream_httpd, &index_uri);
  }
}

// Configura a câmera
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Desativa o detector de brownout

  Serial.begin(115200);
  Serial.setDebugOutput(false);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk
