#define BOOST_DATE_TIME_NO_LIB

#include <iostream>
#include <string>
#include <thread>

#include <boost/asio.hpp>

#include <SFML/Graphics.hpp>
#include <time.h>



using namespace sf;


const int W = 600;
const int H = 480;
int speed = 1;
bool field[W][H] = { 0 };



struct player
{
    int x, y, dir;
    Color color;
    player(Color c)
    {
        x = rand() % W;
        y = rand() % H;
        color = c;
        dir = rand() % 4;
    }
    void tick()
    {
        if (dir == 0) y += 1;
        if (dir == 1) x -= 1;
        if (dir == 2) x += 1;
        if (dir == 3) y -= 1;

        if (x >= W) x = 0;  if (x < 0) x = W - 1;
        if (y >= H) y = 0;  if (y < 0) y = H - 1;
    }

    Vector3f getColor()
    {
        return Vector3f(color.r, color.g, color.b);
    }
};


void read_data_until(boost::asio::ip::tcp::socket& socket, player & p)
{

    boost::asio::streambuf buffer;

    while (boost::asio::read_until(socket, buffer, '!'))
    {
        std::string message;

        std::istream input_stream(&buffer);

        std::getline(input_stream, message, '!');

        if (message == "0")
        {
            if(p.dir != 3)
                p.dir = 0;
        }

        if (message == "3")
        {
            if (p.dir != 0)
                p.dir = 3;
        }

        if (message == "1")
        {
            if (p.dir != 2)
                p.dir = 1;
        }

        if (message == "2")
        {
            if (p.dir != 1)
                p.dir = 2;
        }


        //if (message == "User left the channel")
        //{
        //    boost::asio::write(socket, boost::asio::buffer("User left the channel!"));
        //    break;
        //}
    }
}







int main()
{


    system("chcp 1251");

    std::cout << "Client" << std::endl;

    system("pause");

    std::string raw_ip_address = "127.0.0.1";

    auto port = 3333;

    try
    {
        boost::asio::ip::tcp::endpoint endpoint(
            boost::asio::ip::address::from_string(raw_ip_address), port);

        boost::asio::io_service io_service;

        boost::asio::ip::tcp::socket socket(io_service, endpoint.protocol());

        socket.connect(endpoint);

        //std::thread Thread(read_data_until, std::ref(socket));

        //прием от другого игрока










        srand(time(0));

        RenderWindow window(VideoMode(W, H), "The Tron Game! Client");
        window.setFramerateLimit(60);

        Texture texture;
        texture.loadFromFile("background.jpg");
        Sprite sBackground(texture);

        player p1(Color::Red), p2(Color::Green);

        // 
        p2.x = 100;
        p2.y = 100;
        p2.dir = 1;
        p1.x = 200;
        p1.y = 200;
        p1.dir = 2;
        //



        Sprite sprite;
        RenderTexture t;
        t.create(W, H);
        t.setSmooth(true);
        sprite.setTexture(t.getTexture());
        t.clear();  t.draw(sBackground);

        bool Game = 1;


        

        boost::asio::streambuf buffer1;

        system("pause");

        boost::asio::read_until(socket, buffer1, '!');

        std::thread Thread(read_data_until, std::ref(socket), std::ref(p2));

        while (window.isOpen())
        {
            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                    window.close();
            }



            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                if (p1.dir != 2)
                {
                    p1.dir = 1;
                    boost::asio::write(socket, boost::asio::buffer("1!"));
                }

            }


            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                if (p1.dir != 1)
                {
                    p1.dir = 2;
                    boost::asio::write(socket, boost::asio::buffer("2!"));
                }
            }


            if (Keyboard::isKeyPressed(Keyboard::Up))
            {
                if (p1.dir != 0)
                {
                    p1.dir = 3;
                    boost::asio::write(socket, boost::asio::buffer("3!"));
                }
            }


            if (Keyboard::isKeyPressed(Keyboard::Down))
            {
                if (p1.dir != 3)
                {
                    p1.dir = 0;
                    boost::asio::write(socket, boost::asio::buffer("0!"));
                }
            }



            //if (Keyboard::isKeyPressed(Keyboard::A)) if (p2.dir != 2) p2.dir = 1;
            //if (Keyboard::isKeyPressed(Keyboard::D)) if (p2.dir != 1)  p2.dir = 2;
            //if (Keyboard::isKeyPressed(Keyboard::W)) if (p2.dir != 0) p2.dir = 3;
            //if (Keyboard::isKeyPressed(Keyboard::S)) if (p2.dir != 3) p2.dir = 0;



            if (!Game)    continue;

            for (int i = 0; i < speed; i++)
            {
                p1.tick(); p2.tick();
                if (field[p1.x][p1.y] == 1) Game = 0;
                if (field[p2.x][p2.y] == 1) Game = 0;
                field[p1.x][p1.y] = 1;
                field[p2.x][p2.y] = 1;

                CircleShape c(3);
                c.setPosition(p1.x, p1.y); c.setFillColor(p1.color);    t.draw(c);
                c.setPosition(p2.x, p2.y); c.setFillColor(p2.color);    t.draw(c);
                t.display();
            }

            ////// draw  ///////
            window.clear();
            window.draw(sprite);
            window.display();
        }




    }



    catch (boost::system::system_error& e)
    {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

        system("pause");

        return e.code().value();
    }















    //srand(time(0));

    //RenderWindow window(VideoMode(W, H), "The Tron Game!");
    //window.setFramerateLimit(60);

    //Texture texture;
    //texture.loadFromFile("background.jpg");
    //Sprite sBackground(texture);

    //player p1(Color::Red), p2(Color::Green); 

    //Sprite sprite;
    //RenderTexture t;
    //t.create(W, H);
    //t.setSmooth(true);
    //sprite.setTexture(t.getTexture());
    //t.clear();  t.draw(sBackground);

    //bool Game=1;

    //while (window.isOpen())
    //{
    //    Event e;
    //    while (window.pollEvent(e))
    //    {
    //        if (e.type == Event::Closed)
    //            window.close();
    //    }

    //    if (Keyboard::isKeyPressed(Keyboard::Left)) if (p1.dir!=2) p1.dir=1;
    //    if (Keyboard::isKeyPressed(Keyboard::Right)) if (p1.dir!=1)  p1.dir=2;
    //    if (Keyboard::isKeyPressed(Keyboard::Up)) if (p1.dir!=0) p1.dir=3;
    //    if (Keyboard::isKeyPressed(Keyboard::Down)) if (p1.dir!=3) p1.dir=0;

    //    if (Keyboard::isKeyPressed(Keyboard::A)) if (p2.dir!=2) p2.dir=1;
    //    if (Keyboard::isKeyPressed(Keyboard::D)) if (p2.dir!=1)  p2.dir=2;
    //    if (Keyboard::isKeyPressed(Keyboard::W)) if (p2.dir!=0) p2.dir=3;
    //    if (Keyboard::isKeyPressed(Keyboard::S)) if (p2.dir!=3) p2.dir=0;

    //    if (!Game)    continue;

    //    for(int i=0;i<speed;i++)
    //    {
    //        p1.tick(); p2.tick();
    //        if (field[p1.x][p1.y]==1) Game=0; 
    //        if (field[p2.x][p2.y]==1) Game=0;
    //        field[p1.x][p1.y]=1; 
    //        field[p2.x][p2.y]=1;
    //
    //        CircleShape c(3);
    //        c.setPosition(p1.x,p1.y); c.setFillColor(p1.color);    t.draw(c);
    //        c.setPosition(p2.x,p2.y); c.setFillColor(p2.color);    t.draw(c);
    //        t.display();    
    //    }

    //   ////// draw  ///////
    //    window.clear();
    //    window.draw(sprite);
    //    window.display();
    //}

    return 0;
}
