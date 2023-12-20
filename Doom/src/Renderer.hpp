#include <SFML/Graphics.hpp>
#include<math.h>

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
    sf::RenderWindow* window;
    sf::Event event;
    int Width = 1024, Height = 720;
    #define MapHeight 10
    #define MapWidth 10
    int Map[MapHeight][MapWidth]{
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
        {0, 0, 0, 1, 1, 1, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };

    Renderer(sf::RenderWindow* Ventana,int X, int Y){
        window = Ventana;
        Width = X;
        Height = Y;
    }

    // Posicion de la camara (asignarle un valor determina la posicion inicial)
    double PosicionX;
    double PosicionY;
    // Plano de camara (Esta a la altura 0.66)
    double PlanoX = 0;
    double PlanoY = 0.66;
    // Direccion actual de la camara (Adonde esta mirando)
    double DireccionX = -1;
    double DireccionY = 0;
    // Velocidad de movimiento y de rotacion
    double VelocidadCamara = 5;
    double VelocidadRotacion = 6;

    void Moverse(Movimiento Direccion, sf::Time Delta)
    {
        double VelCam = VelocidadCamara * Delta.asSeconds();
        double DireccionAntigua[2] = {DireccionX, DireccionY};
        double PlanoAntiguo[2] = {PlanoX, PlanoY};
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
            DireccionY = DireccionAntigua[0] * sin(VRot) + DireccionY * cos(VRot);
            PlanoX = PlanoX * cos(VRot) - PlanoY * sin(VRot);
            PlanoY = PlanoAntiguo[0] * sin(VRot) + PlanoY * cos(VRot);
            break;
        case Derecha:
            VRot = -VRot;
            DireccionX = DireccionX * cos(VRot) - DireccionY * sin(VRot);
            DireccionY = DireccionAntigua[0] * sin(VRot) + DireccionY * cos(VRot);
            PlanoX = PlanoX * cos(VRot) - PlanoY * sin(VRot);
            PlanoY = PlanoAntiguo[0] * sin(VRot) + PlanoY * cos(VRot);
            break;
        default:
            break;
        }
    }

    void Renderizar(sf::Time Delta)
    {
        for (int x = 0; x < Width; x++)
        {
            double CamX = 2 * x / double(Width) - 1;
            double PosicionRayoX = PosicionX;
            double PosicionRayoY = PosicionY;
            double DireccionRayoX = DireccionX + PlanoX * CamX;
            double DireccionRayoY = DireccionY + PlanoX * CamX;
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

            //Respresentacion grafica
            int LineSize = abs(int(Height / DistanciaPared));

            int LineaInicio = -LineSize / 2 + Height / 2;
            if (LineaInicio < 0) LineaInicio = 0;
            int LineaFinal = LineSize / 2 + Height / 2;
            if (LineaFinal >= Height) LineaFinal = Height - 1;

            //Coloreando
            sf::Color Color;
            switch (Map[MapPosX][MapPosY])
            {
            case 1:
                Color = sf::Color::Magenta;
                break;
            case 2:
                Color = sf::Color::Green;
                break;
            case 3:
                Color = sf::Color::Red;
                break;
            case 4:
                Color = sf::Color::Cyan;
                break;
            default:
                Color = sf::Color::White;
                break;
            }

            if (Lado == 1){
                Color = sf::Color(Color.r / 2,Color.g / 2,Color.b / 2);
            }

            sf::Vertex Linea[2]{
                sf::Vertex(sf::Vector2f(x,LineaInicio),Color),
                sf::Vertex(sf::Vector2f(x,LineaFinal),Color)
            };
            window -> draw(Linea,2,sf::Lines);
        }
    }

void PollEvents(sf::Time Delta)
{
    while (window ->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window -> close();
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        Moverse(Arriba, Delta);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        Moverse(Abajo, Delta);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){
        Moverse(Izquierda, Delta);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        Moverse(Derecha, Delta);
    }
}

};
