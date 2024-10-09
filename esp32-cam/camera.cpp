#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // URL do stream da ESP32-CAM
    string url = "http://<IP_DA_ESP32>/stream"; // Substitua <IP_DA_ESP32> pelo IP correto

    VideoCapture cap(url);
    if (!cap.isOpened()) {
        cerr << "Erro ao abrir o stream" << endl;
        return -1;
    }

    Mat frame, hsv, mask;

    while (true) {
        // Captura um novo quadro
        cap >> frame;
        if (frame.empty()) {
            cerr << "Erro ao capturar imagem" << endl;
            break;
        }

        // Converte a imagem de BGR para HSV
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // Define os limites da cor laranja no espaço HSV
        Scalar lowerOrange(10, 100, 100); // Limite inferior
        Scalar upperOrange(25, 255, 255); // Limite superior

        // Cria a máscara
        inRange(hsv, lowerOrange, upperOrange, mask);

        // Encontra contornos na máscara
        vector<vector<Point>> contours;
        findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // Desenha contornos e detecta a bola
        for (size_t i = 0; i < contours.size(); i++) {
            double area = contourArea(contours[i]);
            if (area > 500) { // Tamanho mínimo para detectar a bola
                Rect boundingBox = boundingRect(contours[i]);
                rectangle(frame, boundingBox, Scalar(0, 255, 0), 2); // Desenha o retângulo
            }
        }

        // Mostra a imagem
        imshow("Detecção de Bola Laranja", frame);
        imshow("Máscara", mask);

        // Para sair, pressione a tecla 'q'
        if (waitKey(30) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
