#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <memory>

sf::Time Delta;
sf::RenderWindow window(sf::VideoMode(720, 480), "Doom", sf::Style::Titlebar | sf::Style::Close);
sf::Event event;

enum Movimiento
{
    Arriba,
    Abajo,
    Izquierda,
    Derecha
};

class Sprite
{
public:
    double X;
    double Y;
    int Texture;
    int ID = 0;

    int uDiv = 1;
    int vDiv = 1;
    float vMove = 0;

    Sprite(double PosX, double PosY, int Textura, int HorizontalSize = 1, int VerticalSize = 1, float VerticalPosition = 0)
    {
        this->X = PosX;
        this->Y = PosY;
        this->Texture = Textura;
        this->uDiv = HorizontalSize;
        this->vDiv = VerticalSize;
        this->vMove = VerticalPosition;
    }
};

class SpriteManager
{
public:
    std::vector<Sprite *> Sprites;
    std::vector<int> SpriteOrder;
    std::vector<double> SpriteDistance;
    std::vector<float> ZBuffer;
    int ID;
    void AnadirSprite(Sprite *SP)
    {
        this->ID++;
        SP->ID = this->ID;
        this->Sprites.push_back(SP);
    }

    void EliminarSprite(Sprite *SP)
    {
        for (int i = 0; i < Sprites.size(); i++){
            if (Sprites[i] ->ID == SP ->ID){
                Sprites.erase(Sprites.begin() + i);
            }
        }
    }

    void OrdenarSprites()
    {
        std::vector<std::pair<double, int>> Spr(Sprites.size());
        for (int i = 0; i < Sprites.size(); i++)
        {
            Spr[i].first = SpriteDistance[i];
            Spr[i].second = SpriteOrder[i];
        }
        std::sort(Spr.begin(), Spr.end());

        for (int i = 0; i < Sprites.size(); i++)
        {
            SpriteDistance[i] = Spr[Sprites.size() - i - 1].first;
            SpriteOrder[i] = Spr[Sprites.size() - i - 1].second;
        }
    }
};

class Renderer
{
public:
    int Width = 720, Height = 480;
#define TexWidth 64
#define TexHeight 64
#define MapHeight 30
#define MapWidth 30
    const int Map[MapHeight][MapWidth]{
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,1},
{1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1},
{1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1},
{1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1},
{1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1},
{1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1},
{1,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,1},
{1,0,0,3,3,3,3,3,3,3,3,3,3,3,0,3,3,3,3,3,3,3,3,3,3,3,3,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,3,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,0,3,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,1},
{1,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,0,3,0,0,0,1},
{1,0,0,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
#define TextureCant 9
    sf::Image Texturas[TextureCant];

    SpriteManager SPM;
    sf::Texture UI;
    sf::Image AntesDisparo;
    sf::Image DespuesDisparo;
    sf::Texture Puntero;
    Renderer(int X, int Y)
    {   
        Puntero.create(Width,Height);
        AntesDisparo.create(Width, Height);
        DespuesDisparo.create(Width, Height);
        UI.create(Width, Height);
        if (!Puntero.loadFromFile("textures/Puntero.png")){
            std::cout << "Error al cargar la textura de la interfaz\n";
            system("PAUSE");
        }
        if (!AntesDisparo.loadFromFile("textures/UI.png"))
        {
            std::cout << "Error al cargar la textura de la interfaz\n";
            system("PAUSE");
        }
        if (!DespuesDisparo.loadFromFile("textures/UI2.png"))
        {
            std::cout << "Error al cargar la textura de la interfaz\n";
            system("PAUSE");
        }
        UI.update(AntesDisparo);
        Width = X;
        Height = Y;
        if (!Texturas[0].loadFromFile("textures/Pared64.png"))
        {
            std::cout << "Error al cargar textura de pared\n";
            system("PAUSE");
        };
        if (!Texturas[1].loadFromFile("textures/Piso64.png"))
        {
            std::cout << "Error al cargar textura de piso\n";
            system("PAUSE");
        };
        if (!Texturas[2].loadFromFile("textures/Rug64.png"))
        {
            std::cout << "Error al cargar textura de pared\n";
            system("PAUSE");
        };
        if (!Texturas[3].loadFromFile("textures/Nubes1.png"))
        {
            std::cout << "Error al cargar textura de techo\n";
            system("PAUSE");
        };
        if (!Texturas[4].loadFromFile("textures/Nubes2.png"))
        {
            std::cout << "Error al cargar textura de techo\n";
            system("PAUSE");
        };
        if (!Texturas[5].loadFromFile("textures/stockman.png"))
        {
            std::cout << "Error al cargar textura de STOCKMAN\n";
            system("PAUSE");
        };
        if (!Texturas[6].loadFromFile("textures/Radiacion.png"))
        {
            std::cout << "Error al cargar textura de textura\n";
            system("PAUSE");
        };
        if (!Texturas[7].loadFromFile("textures/Fuego.png"))
        {
            std::cout << "Error al cargar textura de textura\n";
            system("PAUSE");
        };
        if (!Texturas[8].loadFromFile("textures/Wather.png"))
        {
            std::cout << "Error al cargar textura de textura\n";
            system("PAUSE");
        };
        SPM.AnadirSprite(new Sprite(8, 8, 5));
        SPM.AnadirSprite(new Sprite(6, 10, 5));
        SPM.AnadirSprite(new Sprite(7, 6, 5));
        SPM.AnadirSprite(new Sprite(8, 20, 5));
        SPM.AnadirSprite(new Sprite(9, 20, 5));
        SPM.AnadirSprite(new Sprite(21, 4.5, 5));
        SPM.AnadirSprite(new Sprite(21, 4.5, 5));
        SPM.AnadirSprite(new Sprite(28, 10, 6));
        SPM.AnadirSprite(new Sprite(27, 11, 6));
        SPM.AnadirSprite(new Sprite(26, 9, 6));
        SPM.AnadirSprite(new Sprite(28, 12, 7));
        SPM.AnadirSprite(new Sprite(7, 8, 7));
    }
    // Posicion de la camara (asignarle un valor determina la posicion inicial)
    double PosicionX = 28;
    double PosicionY = 16;
    // Plano de camara
    double PlanoX = 0;
    double PlanoY = 0.66;
    // Direccion actual de la camara
    double DireccionX = -1;
    double DireccionY = 0;
    // Velocidad de movimiento y de rotacion
    double VelocidadCamara = 3;
    double VelocidadRotacion = 2;

    bool Disparo = false;
    bool DisparoEstado = false;
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

    void Renderizar()
    {
        SPM.ZBuffer.clear();
        SPM.SpriteOrder.clear();
        SPM.SpriteDistance.clear();
        sf::Image Buffer;
        Buffer.create(Width, Height);
        sf::Texture Frame;
        Frame.create(Width, Height);
        // Floor and cealing casting
        for (int y = Height / 2 + 1; y < Height; ++y)
        {
            float DireccionRayoX0 = DireccionX - PlanoX;
            float DireccionRayoY0 = DireccionY - PlanoY;
            float DireccionRayoX1 = DireccionX + PlanoX;
            float DireccionRayoY1 = DireccionY + PlanoY;

            int Pos = y - Height / 2;
            int PosZ = 0.5 * Height;

            float DistanciaFila = (float)PosZ / Pos;

            float PisoPasoX = DistanciaFila * (DireccionRayoX1 - DireccionRayoX0) / Width;
            float PisoPasoY = DistanciaFila * (DireccionRayoY1 - DireccionRayoY0) / Width;

            float PisoX = PosicionX + DistanciaFila * DireccionRayoX0;
            float PisoY = PosicionY + DistanciaFila * DireccionRayoY0;

            for (int x = 0; x < Width; ++x)
            {
                int CellX = int(PisoX);
                int CellY = int(PisoY);

                int TextureX = (int)(TexWidth * (PisoX - CellX)) & (TexWidth - 1);
                int TextureY = (int)(TexHeight * (PisoY - CellY)) & (TexHeight - 1);

                PisoX += PisoPasoX;
                PisoY += PisoPasoY;

                int FloorTexture = 1;
                int CealingTexture = 0;
                int Patron = (int(CellX + CellY)) & 1;
                if (Patron == 0)
                    CealingTexture = 4;
                else
                    CealingTexture = 3;

                sf::Color Color;
                float Fog = DistanciaFila;
                if (Fog < 1) Fog = 1;
                if (Fog > 10) Fog = 10;
                Color = Texturas[FloorTexture].getPixel(TextureX, TextureY);
                Color = sf::Color(Color.r / Fog, Color.g / Fog, Color.b / Fog);
                Buffer.setPixel(x, y, Color);

                Color = Texturas[CealingTexture].getPixel(TextureX, TextureY);
                Color = sf::Color(Color.r / Fog, Color.g / Fog, Color.b / Fog);
                Buffer.setPixel(x, Height - y - 1, Color);
            }
        }
        // Wall Casting
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

            if (Lado == 0)
            {
                DistanciaPared = fabs((MapPosX - PosicionX + (1 - PasoX) / 2) / DireccionRayoX);
            }
            else
            {
                DistanciaPared = fabs((MapPosY - PosicionY + (1 - PasoY) / 2) / DireccionRayoY);
            }

            // Respresentacion grafica
            int LineSize = int(Height / DistanciaPared);

            int LineaInicio = -LineSize / 2 + Height / 2;
            if (LineaInicio < 0)
                LineaInicio = 0;
            int LineaFinal = LineSize / 2 + Height / 2;
            if (LineaFinal >= Height)
                LineaFinal = Height - 1;

            int NumTextura = Map[MapPosX][MapPosY] - 1;

            double HitParedX;
            if (Lado == 0)
                HitParedX = PosicionY + DistanciaPared * DireccionRayoY;
            else
                HitParedX = PosicionX + DistanciaPared * DireccionRayoX;
            HitParedX -= floor((HitParedX));

            int TexCordenadasX = int(HitParedX * double(TexWidth));
            if (Lado == 0 && DireccionRayoX > 0)
                TexCordenadasX = TexWidth - TexCordenadasX - 1;
            if (Lado == 1 && DireccionRayoY < 0)
                TexCordenadasX = TexWidth - TexCordenadasX - 1;
            double TexPaso = 1.0 * (double)TexHeight / LineSize;
            sf::Color Color;
            double TexPos = (LineaInicio - Height / 2 + LineSize / 2) * TexPaso;
            for (int y = LineaInicio; y < LineaFinal; y++)
            {
                int TexCordenadasY = (int)TexPos & (TexHeight - 1);
                TexPos += TexPaso;
                Color = Texturas[NumTextura].getPixel(TexCordenadasX, TexCordenadasY);
                if (Lado == 1)
                {
                    Color = sf::Color(Color.r / 2, Color.g / 2, Color.b / 2);
                }
                double Fog = DistanciaPared;
                if (Fog < 1) Fog = 1;
                if (Fog > 10) Fog = 10;
                Color = sf::Color(Color.r / Fog, Color.g / Fog, Color.b / Fog);
                Buffer.setPixel(x, y, Color);
            }
            SPM.ZBuffer.push_back(DistanciaPared);
        }
        for (int i = 0; i < SPM.Sprites.size(); i++)
        {
            SPM.SpriteOrder.push_back(i);
            SPM.SpriteDistance.push_back(((PosicionX - SPM.Sprites[i]->X) * (PosicionX - SPM.Sprites[i]->X) + (PosicionY - SPM.Sprites[i]->Y) * (PosicionY - SPM.Sprites[i]->Y)));
        }
        SPM.OrdenarSprites();
        for (int i = 0; i < SPM.Sprites.size(); i++)
        {
            double SpriteX = SPM.Sprites[SPM.SpriteOrder[i]]->X - PosicionX;
            double SpriteY = SPM.Sprites[SPM.SpriteOrder[i]]->Y - PosicionY;

            double InversDet = 1.0 / (PlanoX * DireccionY - DireccionX * PlanoY);
            double TransformX = InversDet * (DireccionY * SpriteX - DireccionX * SpriteY);
            double TransformY = InversDet * (-PlanoY * SpriteX + PlanoX * SpriteY);

            int SpriteSCX = int((Width / 2) * (1 + TransformX / TransformY));

            int SpritePosicionY = int(SPM.Sprites[SPM.SpriteOrder[i]]->vMove / TransformY);
            int SpriteAltura = abs(int(Height / (TransformY))) / SPM.Sprites[SPM.SpriteOrder[i]]->vDiv;

            int SpriteInicioY = -SpriteAltura / 2 + Height / 2 + SpritePosicionY;
            if (SpriteInicioY < 0)
                SpriteInicioY = 0;
            int SpriteFinalY = SpriteAltura / 2 + Height / 2 + SpritePosicionY;
            if (SpriteFinalY >= Height)
                SpriteFinalY = Height - 1;

            int SpriteAnchura = abs(int(Height / (TransformY))) / SPM.Sprites[SPM.SpriteOrder[i]]->uDiv;

            int SpriteInicioX = -SpriteAnchura / 2 + SpriteSCX;
            if (SpriteInicioX < 0)
                SpriteInicioX = 0;
            int SpriteFinalX = SpriteAnchura / 2 + SpriteSCX;
            if (SpriteFinalX > Width)
                SpriteFinalX = Width;

            for (int Linea = SpriteInicioX; Linea < SpriteFinalX; Linea++)
            {
                int TexSpriteX = int(256 * (Linea - (-SpriteAnchura / 2 + SpriteSCX)) * TexWidth / SpriteAnchura) / 256;

                if (TransformY > 0 && TransformY < SPM.ZBuffer[Linea])
                {
                    for (int y = SpriteInicioY; y < SpriteFinalY; y++)
                    {
                        int d = (y - SpritePosicionY) * 256 - Height * 128 + SpriteAltura * 128;
                        int TexSpriteY = ((d * TexHeight) / SpriteAltura) / 256;
                        sf::Color Color = Texturas[SPM.Sprites[SPM.SpriteOrder[i]]->Texture].getPixel(TexSpriteX, TexSpriteY);
                        if (Color.a != 0)
                            Buffer.setPixel(Linea, y, Color);
                    }
                }
            }
        }

        Frame.update(Buffer);
        window.draw(sf::Sprite(Frame));
        window.draw(sf::Sprite(UI));
    }

    bool Range(float Value, float Min, float Max){
        return Value <= Max && Value >= Min;
    }

    float AimBot = 0.5;

    void Disparar()
    {//A reparar (No lo reparare nunca)
        /*double PosicionRayoX = PosicionX;
        double PosicionRayoY = PosicionY;
        double DireccionRayoX = DireccionX + PlanoX * 0;
        double DireccionRayoY = DireccionY + PlanoY * 0;
        int MapPosX = int(PosicionX);
        int MapPosY = int(PosicionY);
        double DistanciaLadoX;
        double DistanciaLadoY;
        double DistanciaDeltaX = sqrt(1 + (DireccionRayoY * DireccionRayoY) / (DireccionRayoX * DireccionRayoX));
        double DistanciaDeltaY = sqrt(1 + (DireccionRayoX * DireccionRayoX) / (DireccionRayoY * DireccionRayoY));
        int PasoX;
        int PasoY;
        int Hit = 0;

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
                MapPosX += PasoX;
            }
            else
            {
                MapPosY += PasoY;
            }
            for (auto i : SPM.Sprites)
            {
                if (Range(MapPosX, i -> X - AimBot, i ->X) && Range(MapPosY, i -> Y - AimBot, i ->Y))
                {
                    SPM.EliminarSprite(i);
                    Hit = 1;
                    std::cout << "hot\n";
                    break;
                }
            }
            if (Map[MapPosX][MapPosY] > 0){
                Hit = 1;
                std::cout << "PARED\n";
            }
            }*/
    }

void PollEvents()
{
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed)
        {
            Disparo = true;
            DisparoEstado = true;
            Disparar();
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
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        Moverse(Izquierda);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        Moverse(Derecha);
    }
}
}
;

int main()
{
    int FPST = 0;
    sf::Clock clock;
    Renderer Doom(window.getSize().x, window.getSize().y);
    int FPSDisp = 0;
    while (window.isOpen())
    {
        window.clear();
        FPST++;
        if (FPST >= 60)
        {
            FPST = 0;
        }
        Delta = clock.restart();
        Doom.PollEvents();
        Doom.Renderizar();
        if (Doom.Disparo)
        {
            Doom.UI.update(Doom.DespuesDisparo);
            FPSDisp = 0;
            Doom.Disparo = false;
        }
        else if (Doom.DisparoEstado)
        {
            FPSDisp++;
            if (FPSDisp > 5)
            {
                Doom.DisparoEstado = false;
                FPSDisp = 0;
                Doom.UI.update(Doom.AntesDisparo);
            }
        }
        window.display();
    }
}
