#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <string>
#include <cstdlib>
#include <ctime>

const int WIDTH=1200, HEIGHT=800;
const double G=500.0, SOFT=15.0;

float rF(float a,float b){return a+(b-a)*(rand()/(float)RAND_MAX);}

struct Body{
    double x,y,vx,vy,mass;
    float rad;
    sf::Color col;
    std::vector<sf::Vector2f> trail;
    void tick(){
        trail.push_back({(float)x,(float)y});
        if((int)trail.size()>200) trail.erase(trail.begin());
    }
};

std::vector<Body> bodies;

void makeSolar(){
    bodies.clear();
    bodies.push_back({600,400,0,0,50000,18,sf::Color(255,220,50),{}});
    double rs[]={70,110,155,205,290,370,440,510};
    double vs[]={180,145,120,104,87,77,71,65};
    float rads[]={4,5,6,4,10,8,7,7};
    sf::Color cols[]={
        sf::Color(180,140,100),sf::Color(220,190,120),sf::Color(80,160,255),
        sf::Color(200,80,60),sf::Color(200,160,120),sf::Color(210,185,140),
        sf::Color(140,210,220),sf::Color(80,100,220)
    };
    for(int i=0;i<8;i++){
        double a=rF(0,6.28f);
        bodies.push_back({600+cos(a)*rs[i],400+sin(a)*rs[i],
            -sin(a)*vs[i],cos(a)*vs[i],10,rads[i],cols[i],{}});
    }
}

void makeRandom(){
    bodies.clear();
    bodies.push_back({600,400,0,0,60000,20,sf::Color(255,240,80),{}});
    for(int i=0;i<25;i++){
        double a=rF(0,6.28f),r=rF(60,320);
        double v=sqrt(G*bodies[0].mass/r)*rF(0.85f,1.15f);
        sf::Color c((uint8_t)(rand()%120+135),(uint8_t)(rand()%120+100),(uint8_t)(rand()%120+100));
        bodies.push_back({600+cos(a)*r,400+sin(a)*r,-sin(a)*v,cos(a)*v,
            rF(5,80),rF(3,7),c,{}});
    }
}

void makeBinary(){
    bodies.clear();
    double m=30000,sep=160,v=sqrt(G*m/(2*sep))*0.9;
    bodies.push_back({600-sep/2,400,0, v,m,14,sf::Color(255,160,60),{}});
    bodies.push_back({600+sep/2,400,0,-v,m,14,sf::Color(100,180,255),{}});
    for(int i=0;i<15;i++){
        double a=rF(0,6.28f),r=rF(240,370);
        double ov=sqrt(G*2*m/r);
        sf::Color c((uint8_t)(rand()%80+180),(uint8_t)(rand()%80+180),(uint8_t)(rand()%80+220));
        bodies.push_back({600+cos(a)*r,400+sin(a)*r,-sin(a)*ov,cos(a)*ov,3,3,c,{}});
    }
}

void step(double dt){
    int n=bodies.size();
    std::vector<double> ax(n,0),ay(n,0);
    for(int i=0;i<n;i++) for(int j=i+1;j<n;j++){
        double dx=bodies[j].x-bodies[i].x,dy=bodies[j].y-bodies[i].y;
        double d2=dx*dx+dy*dy+SOFT*SOFT;
        double inv=1.0/sqrt(d2);
        double f=G*bodies[i].mass*bodies[j].mass*inv*inv*inv;
        ax[i]+=f*dx/bodies[i].mass; ay[i]+=f*dy/bodies[i].mass;
        ax[j]-=f*dx/bodies[j].mass; ay[j]-=f*dy/bodies[j].mass;
    }
    for(int i=0;i<n;i++){
        bodies[i].vx+=ax[i]*dt; bodies[i].vy+=ay[i]*dt;
        bodies[i].x+=bodies[i].vx*dt; bodies[i].y+=bodies[i].vy*dt;
        bodies[i].tick();
    }
}

int main(){
    sf::RenderWindow win({(unsigned)WIDTH,(unsigned)HEIGHT},
        "N-Body Gravity | 1:Solar 2:Random 3:Binary | Space:Pause | T:Trails | LMB:Add");
    win.setFramerateLimit(60);
    srand((unsigned)time(0));
    makeSolar();

    bool paused=false,trails=true;
    double spd=1.0;
    std::string modes[]={"Solar System","Random Orbits","Binary Star"};
    int mode=0;

    sf::Font font; bool hf=font.openFromFile("C:/Windows/Fonts/arial.ttf");
    sf::Text hud(font); hud.setCharacterSize(14);
    hud.setFillColor(sf::Color::White);
    hud.setOutlineColor(sf::Color::Black); hud.setOutlineThickness(1.5f);
    hud.setPosition({8,8});
    sf::Clock clk;

    // Offscreen texture for trails
    sf::Image trailImg({(unsigned)WIDTH,(unsigned)HEIGHT},sf::Color::Black);
    sf::Texture trailTex; trailTex.loadFromImage(trailImg);
    sf::Sprite trailSpr(trailTex);
    sf::Image fadeImg({(unsigned)WIDTH,(unsigned)HEIGHT},sf::Color(0,0,0,12));

    while(win.isOpen()){
        float dt=std::min(clk.restart().asSeconds(),0.02f);
        auto mp=sf::Mouse::getPosition(win);
        while(auto ev=win.pollEvent()){
            if(ev->is<sf::Event::Closed>()) win.close();
            if(auto*k=ev->getIf<sf::Event::KeyPressed>()){
                if(k->code==sf::Keyboard::Key::Escape) win.close();
                if(k->code==sf::Keyboard::Key::Space) paused=!paused;
                if(k->code==sf::Keyboard::Key::T) trails=!trails;
                if(k->code==sf::Keyboard::Key::Num1){makeSolar();mode=0;}
                if(k->code==sf::Keyboard::Key::Num2){makeRandom();mode=1;}
                if(k->code==sf::Keyboard::Key::Num3){makeBinary();mode=2;}
                if(k->code==sf::Keyboard::Key::Equal) spd=std::min(8.0,spd*2);
                if(k->code==sf::Keyboard::Key::Hyphen) spd=std::max(0.25,spd*0.5);
            }
            if(auto*mb=ev->getIf<sf::Event::MouseButtonPressed>())
                if(mb->button==sf::Mouse::Button::Left)
                    bodies.push_back({(double)mp.x,(double)mp.y,
                        rF(-50,50),rF(-50,50),15,4,
                        sf::Color((uint8_t)(rand()%155+100),(uint8_t)(rand()%155+100),(uint8_t)(rand()%155+100)),{}});
        }
        if(!paused) for(int i=0;i<4;i++) step(dt*spd*3.5);

        win.clear(sf::Color(8,10,18));

        // Draw trails as lines
        if(trails){
            for(auto&b:bodies){
                int sz=b.trail.size();
                if(sz<2) continue;
                sf::VertexArray va(sf::PrimitiveType::LineStrip,sz);
                for(int i=0;i<sz;i++){
                    float a=(float)i/sz;
                    va[i].position=b.trail[i];
                    va[i].color=sf::Color(b.col.r,b.col.g,b.col.b,(uint8_t)(a*180));
                }
                win.draw(va);
            }
        }

        sf::CircleShape cs;
        for(auto&b:bodies){
            // Glow
            cs.setRadius(b.rad*2.f);
            cs.setOrigin({b.rad*2.f,b.rad*2.f});
            cs.setPosition({(float)b.x,(float)b.y});
            cs.setFillColor(sf::Color(b.col.r,b.col.g,b.col.b,40));
            win.draw(cs);
            // Body
            cs.setRadius(b.rad);
            cs.setOrigin({b.rad,b.rad});
            cs.setFillColor(b.col);
            win.draw(cs);
        }

        if(hf){
            hud.setString(modes[mode]+(paused?" [PAUSED]":"")+
                "  Bodies:"+std::to_string(bodies.size())+"  Speed:"+std::to_string((int)spd)+"x\n"+
                "1:Solar 2:Random 3:Binary | Space:Pause | T:Trails | LMB:Add | +/-:Speed");
            win.draw(hud);
        }
        win.display();
    }
}
