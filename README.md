

# ESP32-CAM: Detecção de Bola Laranja com OpenCV

Este projeto utiliza uma **ESP32-CAM** para capturar vídeo em tempo real e processa as imagens usando **OpenCV** para detectar uma bola laranja. O sistema pode ser utilizado em projetos de visão computacional, como robôs de futebol, rastreamento de objetos ou automação.

---

## Índice

- [Visão Geral](#visão-geral)
- [Funcionalidades](#funcionalidades)
- [Requisitos](#requisitos)
- [Instalação](#instalação)
- [Execução](#execução)
- [Contribuições](#contribuições)
- [Licença](#licença)

---

## Visão Geral

Este projeto combina a ESP32-CAM, uma microcontroladora com capacidade de captura de vídeo, com o OpenCV, uma biblioteca poderosa de visão computacional, para realizar a detecção de objetos com base em cores. Neste caso, o foco é a detecção de uma bola laranja no stream de vídeo.

### Tecnologias Utilizadas:

- **ESP32-CAM**: Para captura de vídeo.
- **OpenCV (C++)**: Para processamento de imagens.
- **C++**: Linguagem utilizada no código.

---

## Funcionalidades

- 📷 **Captura de vídeo**: Utilizando a ESP32-CAM.
- 🎨 **Detecção de cor**: Identifica objetos com cor laranja no espaço de cor HSV.
- 🖥️ **Desenho de contornos**: Desenha retângulos ao redor de objetos detectados.
- 💾 **Processamento em tempo real**: Captura e processa frames em tempo real.

---

## Requisitos

- **Placa ESP32-CAM**: Usada para capturar o vídeo.
- **OpenCV 4.0+**: Biblioteca para visão computacional e processamento de imagem.
- **Arduino IDE**: Para programar e carregar o código na ESP32-CAM.
- **Conexão Wi-Fi**: Para enviar o stream de vídeo da ESP32-CAM.

### Instalação do OpenCV (C++)

Para instalar o OpenCV em seu ambiente C++ no Linux:

```bash
sudo apt update
sudo apt install libopencv-dev
```

---

## Instalação

### 1. Código da ESP32-CAM

1. **Configuração da ESP32-CAM**:
   - Abra o **Arduino IDE**.
   - Instale a biblioteca **ESP32** para o Arduino.
   - Carregue o código da ESP32-CAM (disponível em `esp32_cam_code.ino`).
   - Conecte a ESP32-CAM ao computador via USB e faça o upload do código.

2. **Conexão com o Wi-Fi**:
   - No código, configure o SSID e a senha do Wi-Fi:

   ```cpp
   const char* ssid = "NOME_DO_SEU_WIFI";
   const char* password = "SENHA_DO_SEU_WIFI";
   ```

### 2. Código OpenCV

1. Clone o repositório:

   ```bash
   git clone https://github.com/seu-usuario/esp32-cam-detection.git
   cd esp32-cam-detection
   ```

2. Compile o código C++ com OpenCV.

### 3. Configuração do IP

No código C++ (`main.cpp`), substitua `<IP_DA_ESP32>` pelo endereço IP gerado pela ESP32-CAM ao se conectar ao Wi-Fi:

```cpp
string url = "http://<IP_DA_ESP32>/stream";
```

---

## Execução

### 1. Executando a ESP32-CAM

1. Acesse o endereço IP da ESP32-CAM no navegador (geralmente será mostrado no monitor serial da Arduino IDE).

### 2. Rodando o Código de Detecção

1. Compile e execute o programa C++ para iniciar o processamento de imagem.

```bash
g++ main.cpp -o detect_ball `pkg-config --cflags --libs opencv4`
./detect_ball
```

2. O vídeo será processado e a detecção de uma bola laranja será feita em tempo real.

---

## Contribuições

Contribuições são bem-vindas! Siga os passos abaixo para contribuir:

1. Faça um fork do repositório.
2. Crie uma branch: `git checkout -b minha-nova-feature`.
3. Faça suas modificações e commit: `git commit -m 'Adicionei uma nova feature'`.
4. Envie para o repositório: `git push origin minha-nova-feature`.
5. Abra um pull request.

---

## Licença

Este projeto está licenciado sob a licença MIT - veja o arquivo [LICENSE](LICENSE) para mais detalhes.

---

Esse README deve fornecer uma boa descrição do projeto, facilitando a instalação, execução e contribuições futuras.
