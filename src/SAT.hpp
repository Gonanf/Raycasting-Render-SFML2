#include<SFML/Graphics.hpp>

class SAT{
    public:

    sf::Vector2f SeparatingAxisTheorem(sf::FloatRect Primero,sf::FloatRect Segundo){
        float PX = std::max(Primero.left + Primero.width, Segundo.left + Segundo.width) - std::min(Primero.left,Segundo.left);
        sf::Vector2f Final;
        if (PX < Primero.width + Segundo.width){
            float OVX =  Primero.width + Segundo.width - PX;
            float PY = std::max(Primero.top + Primero.height, Segundo.top + Segundo.height) - std::min(Primero.top,Segundo.top);
            if (PY < Primero.height + Segundo.height){
                float OVY = Primero.height + Segundo.height - PY;
                if (OVX < OVY){
                    return sf::Vector2f(OVX * (Primero.left < Segundo.left?-1:1),0);
                }
                else{
                    return sf::Vector2f(0,OVY * (Primero.top < Segundo.top?-1:1));
                }
            }
        }
        return sf::Vector2f(0,0);
    }

    
};