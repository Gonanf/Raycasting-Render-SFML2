#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

sf::Time Delta;
sf::RenderWindow window(sf::VideoMode(1024,720),"Doom");
sf::Event event;

enum Movimiento
{
    Arriba,
    Abajo,
    Izquierda,
    Derecha
};

class Renderer
{
public:
    sf::Event event;
    int Width = 1024, Height = 720;
    #define TexWidth 64
    #define TexHeight 64
    #define MapHeight 10
    #define MapWidth 10
    const int Map[MapHeight][MapWidth]{
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 3, 3, 3, 3, 3, 0, 0, 1},
        {1, 0, 2, 1, 1, 1, 3, 0, 0, 1},
        {1, 0, 2, 1, 0, 1, 3, 0, 0, 1},
        {1, 0, 2, 1, 0, 1, 3, 0, 0, 1},
        {1, 0, 2, 2, 0, 2, 3, 0, 0, 1},
        {1, 0, 2, 2, 0, 2, 3, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
    #define TextureCant 3
    sf::Image Texturas[TextureCant];
    Renderer(int X, int Y){
        Width = X;
        Height = Y;
        if (!Texturas[0].loadFromFile("textures/Pared64.png")){
            std::cout << "Error al cargar textura de pared\n";
            system("PAUSE");
        };
        if (!Texturas[1].loadFromFile("textures/Piso64.png")){
            std::cout << "Error al cargar textura de pared\n";
            system("PAUSE");
        };
        if (!Texturas[2].loadFromFile("textures/Rug64.png")){
            std::cout << "Error al cargar textura de pared\n";
            system("PAUSE");
        };
    }
    bool Activo = false;
    // Posicion de la camara (asignarle un valor determina la posicion inicial)
    double PosicionX = 4.5;
    double PosicionY = 4.5;
    // Plano de camara
    double PlanoX = 0;
    double PlanoY = 0.66;
    // Direccion actual de la camara
    double DireccionX = -1;
    double DireccionY = 0;
    // Velocidad de movimiento y de rotacion
    double VelocidadCamara = 3;
    double VelocidadRotacion = 6;

    void Moverse(Movimiento Direccion)
    {
        double VelCam = VelocidadCamara * Delta.asSeconds();
        double DireccionAntiguaX = DireccionX;
        double PlanoAntiguoX = PlanoX;
        double VRot = VelocidadRotacion * Delta.asSeconds();
        switch (Direccion)
        {
        case Arriba:
            if (Map[int(PosicionX + DireccionX * VelCam)][int(PosicionY)] == false)
                PosicionX += DireccionX * VelCam;
            if (Map[int(PosicionX)][int(PosicionY + DireccionY * VelCam)] == false)
                PosicionY += DireccionY * VelCam;
            break;
        case Abajo:
            if (Map[int(PosicionX - DireccionX * VelCam)][int(PosicionY)] == false)
                PosicionX -= DireccionX * VelCam;
            if (Map[int(PosicionX)][int(PosicionY - DireccionY * VelCam)] == false)
                PosicionY -= DireccionY * VelCam;
            break;
        case Izquierda:
            DireccionX = DireccionX * cos(VRot) - DireccionY * sin(VRot);
            DireccionY = DireccionAntiguaX * sin(VRot) + DireccionY * cos(VRot);
            PlanoX = PlanoX * cos(VRot) - PlanoY * sin(VRot);
            PlanoY = PlanoAntiguoX * sin(VRot) + PlanoY * cos(VRot);
            break;
        case Derecha:
            VRot = -VRot;
            DireccionX = DireccionX * cos(VRot) - DireccionY * sin(VRot);
            DireccionY = DireccionAntiguaX * sin(VRot) + DireccionY * cos(VRot);
            PlanoX = PlanoX * cos(VRot) - PlanoY * sin(VRot);
            PlanoY = PlanoAntiguoX * sin(VRot) + PlanoY * cos(VRot);
            break;
        default:
            break;
        }
    }

    int FramesPrueba = 0;
    void Renderizar()
    {       
            sf::Image Buffer;
            Buffer.create(Width,Height);
            sf::Texture Frame;
            Frame.create(Width,Height);
        for (int x = 0; x < Width; x++)
        {
            double CamX = 2 * x / double(Width) - 1;
            double PosicionRayoX = PosicionX;
            double PosicionRayoY = PosicionY;
            double DireccionRayoX = DireccionX + PlanoX * CamX;
            double DireccionRayoY = DireccionY + PlanoY * CamX;
            int MapPosX = int(PosicionX);
            int MapPosY = int(PosicionY);
            double DistanciaLadoX;
            double DistanciaLadoY;
            double DistanciaDeltaX = sqrt(1 + (DireccionRayoY * DireccionRayoY) / (DireccionRayoX * DireccionRayoX));
            double DistanciaDeltaY = sqrt(1 + (DireccionRayoX * DireccionRayoX) / (DireccionRayoY * DireccionRayoY));
            float DistanciaPared;
            int PasoX;
            int PasoY;
            int Hit = 0;
            int Lado;

            if (DireccionRayoX < 0)
            {
                PasoX = -1;
                DistanciaLadoX = (PosicionRayoX - MapPosX) * DistanciaDeltaX;
            }
            else
            {
                PasoX = 1;
                DistanciaLadoX = (MapPosX + 1 - PosicionRayoX) * DistanciaDeltaX;
            }

            if (DireccionRayoY < 0)
            {
                PasoY = -1;
                DistanciaLadoY = (PosicionRayoY - MapPosY) * DistanciaDeltaY;
            }
            else
            {
                PasoY = 1;
                DistanciaLadoY = (MapPosY + 1 - PosicionRayoY) * DistanciaDeltaY;
            }

            while (Hit == 0)
            {
                if (DistanciaLadoX < DistanciaLadoY)
                {
                    DistanciaLadoX += DistanciaDeltaX;
                    MapPosX += PasoX;
                    Lado = 0;
                }
                else
                {
                    DistanciaLadoY += DistanciaDeltaY;
                    MapPosY += PasoY;
                    Lado = 1;
                }

                if (Map[MapPosX][MapPosY] > 0)
                    Hit = 1;
            }

            if (Lado == 0){
                DistanciaPared = fabs((MapPosX - PosicionX  + (1 - PasoX) / 2) / DireccionRayoX);
            }
            else{
                 DistanciaPared = fabs((MapPosY - PosicionY  + (1 - PasoY) / 2) / DireccionRayoY);
            }
            int Pitch = 100;

            //Respresentacion grafica
            int LineSize = int(Height / DistanciaPared);

            int LineaInicio = -LineSize / 2 + Height / 2 + Pitch;
            if (LineaInicio < 0) LineaInicio = 0;
            int LineaFinal = LineSize / 2 + Height / 2 + Pitch;
            if (LineaFinal >= Height) LineaFinal = Height - 1;

            int NumTextura = Map[MapPosX][MapPosY] - 1;

            double HitParedX;
            if (Lado == 0) HitParedX = PosicionY + DistanciaPared * DireccionRayoY;
            else HitParedX = PosicionX + DistanciaPared * DireccionRayoX;
            HitParedX -= floor((HitParedX));

            int TexCordenadasX = int(HitParedX * double(TexWidth));
            if (Lado == 0 && DireccionRayoX > 0) TexCordenadasX =  TexWidth - TexCordenadasX - 1;
            if (Lado == 1 && DireccionRayoY < 0) TexCordenadasX =  TexWidth - TexCordenadasX - 1;
            double TexPaso = 1.0 * (double)TexHeight / LineSize;
            sf::Color Color;
            double TexPos = (LineaInicio - Pitch - Height / 2 + LineSize / 2) * TexPaso;
            for (int y = LineaInicio; y < LineaFinal; y++){
                int TexCordenadasY = (int)TexPos & (TexHeight - 1);
                TexPos += TexPaso;
                Color = Texturas[NumTextura].getPixel(TexCordenadasX,TexCordenadasY);
                if (Lado == 1){
                Color = sf::Color(Color.r / 2,Color.g / 2,Color.b / 2);
                }
                Buffer.setPixel(x,y,Color);
            }
        }
        Frame.update(Buffer);
        window.draw(sf::Sprite(Frame));
    }

    void PollEvents()
{
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        Moverse(Arriba);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        Moverse(Abajo);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        Moverse(Izquierda);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        Moverse(Derecha);
    }
}

};



int main()
{   
    int FPST = 0;
    sf::Clock clock;
    Renderer Doom(1024,720);
    while (window.isOpen())
    {
        FPST++;
        if (FPST >= 60){
            FPST = 0;
        }
        window.clear();
        Delta = clock.restart();
        Doom.PollEvents();
        Doom.Renderizar();
        window.display();
    }
}
