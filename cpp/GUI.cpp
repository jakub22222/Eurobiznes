#include "GUI.h"

GUI::GUI(float resx, float resy)
{
    resolution.x = resx;
    resolution.y = resy;
}

void GUI::start()
{
    sf::RenderWindow  window(sf::VideoMode(resolution.x, resolution.y), "Eurobiznes");
    window.setFramerateLimit(60);
    //window.setMouseCursorVisible(0);
    std::unique_ptr<button> mainMenuButtons[5]
    {
        move(std::unique_ptr<button>(new button(resolution.x / 2.f,650.f,500.f,50.f, "img/menu1.png", "img/menu1_2.png"))),
        move(std::unique_ptr<button>(new button(resolution.x / 2.f,725.f,500.f,50.f, "img/menu2.png", "img/menu2_2.png"))),
        move(std::unique_ptr<button>(new button(resolution.x / 2.f,800.f,500.f,50.f, "img/menu3.png", "img/menu3_2.png"))),
        move(std::unique_ptr<button>(new button((resolution.x - 250.f) / 2.f,875.f,245.f,50.f, "img/menu4.png", "img/menu4_2.png"))),
        move(std::unique_ptr<button>(new button((resolution.x + 250.f) / 2.f,875.f,245.f,50.f, "img/menu5.png", "img/menu5_2.png")))
    };

    int currentButton = 0;
    mainMenuButtons[currentButton]->select();
    sf::Sprite bg;
    sf::Texture bgTexture;
    bgTexture.loadFromFile("img/bg.jpg");
    bg.setTexture(bgTexture);
    bg.setPosition(1, 1);
    sf::Texture logoTexture;
    logoTexture.loadFromFile("img/logo.png");
    sf::Sprite logo;
    logo.setTexture(logoTexture);
    logo.setOrigin(308.f, 55.f);
    logo.setScale(1, 1);
    logo.setTextureRect(sf::IntRect(1, 1, 615, 110));
    logo.setPosition(resolution.x/2, 130);
    sf::SoundBuffer buffer;
    buffer.loadFromFile("sound/mainTheme.wav");
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setLoop(true);
    sound.play();
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                sound.stop();
                window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                switch (currentButton)
                {
                case 0:
                {
                    subMenu0(window, bg, sound);
                    break;
                }
                case 1:
                {
                    subMenu1(window, bg, sound);
                    break;
                }
                case 2:
                {
                    subMenu2(window, bg, sound);
                    break;
                }
                case 3:
                {
                    subMenu3(window, bg, sound);
                    break;
                }
                case 4:
                {
                    subMenu4(window, bg, sound);
                    break;
                }
                default: break;
                }
                bg.setColor(sf::Color(255, 255, 255, 255));
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && currentButton < 4)
            {
                mainMenuButtons[currentButton]->unselect();
                currentButton++;
                mainMenuButtons[currentButton]->select();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && currentButton > 0)
            {
                mainMenuButtons[currentButton]->unselect();
                currentButton--;
                mainMenuButtons[currentButton]->select();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && currentButton == 3)
            {
                mainMenuButtons[currentButton]->unselect();
                currentButton++;
                mainMenuButtons[currentButton]->select();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && currentButton == 4)
            {
                mainMenuButtons[currentButton]->unselect();
                currentButton--;
                mainMenuButtons[currentButton]->select();
            }
        }
        window.clear();
        window.draw(bg);
        for (int i = 0; i < 5; ++i)
        {
            window.draw(*mainMenuButtons[i]);
        }
        window.draw(logo);
        window.display();
    }
    sound.stop();
    return;
};

bool GUI::validIp(std::string ip)
{
    std::regex ipv4("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
    if (std::regex_match(ip, ipv4))return true;
    else return false;
}

void GUI::subMenu0(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s)
{
    bool goBack = false;
    bool startGame = false;
    sf::Font font;
    font.loadFromFile("fonts/arial.ttf");
    sf::Text text1(L"Podaj nick: ", font);
    text1.setCharacterSize(30);
    text1.setPosition(100, 100);
    text1.setFillColor(sf::Color::Red);
    sf::Text text2(L"Iloœæ graczy: ", font);
    text2.setCharacterSize(30);
    text2.setPosition(100, 200);
    text2.setFillColor(sf::Color::Red);
    sf::Text text3(L"Twój kolorek: ", font);
    text3.setCharacterSize(30);
    text3.setPosition(100, 300);
    text3.setFillColor(sf::Color::Red);
    int actual = 0;
    int players = 2;
    int colorIndex = 10;
    std::unique_ptr<button> buttons[11]
    {
        move(std::unique_ptr<button>(new button(resolution.x / 2.f,115.f,500.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) - 175.f,215.f,150.f,50.f, "img/input2.png", "img/input2_2.png"))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f),215.f,150.f,50.f, "img/input3.png", "img/input3_2.png"))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) + 175.f,215.f,150.f,50.f, "img/input4.png", "img/input4_2.png"))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) - 175.f,315.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f),315.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) + 175.f,315.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) - 175.f,415.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f),415.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) + 175.f,415.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f),515.f,500.f,50.f, "img/menu1.png", "img/menu1_2.png")))
    };
    buttons[0]->addLetter("Tu wpisz swój nick...");
    for (int i = 0; i < 6; ++i)buttons[i + 4]->setColor(game::getColor(i));
    buttons[0]->select();
    while (!goBack)
    {
        bool backspace = true;
        sf::Event event2;
        while (window.pollEvent(event2))
        {
            if (event2.type == sf::Event::Closed)
            {
                window.close();
                s.stop();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                goBack = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                if (actual >= 1 && actual <= 3)
                {
                    buttons[players - 1]->setColor(sf::Color::Transparent);
                    players = actual + 1;
                    buttons[actual]->setColor(sf::Color::Green);
                }
                else if (actual > 3 && actual < 10)
                {
                    buttons[colorIndex]->clearInnerText();
                    buttons[actual]->clearInnerText();
                    buttons[actual]->addLetter("Wybrano");
                    colorIndex = actual;
                }
                else if (actual == 10)
                {
                    startGame = true;
                    goBack = true;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                buttons[actual]->unselect();
                if (actual > 0 && actual != colorIndex)buttons[actual]->clearInnerText();
                actual = (actual + 1) % 11;
                buttons[actual]->select();
                if (actual >= 4 && actual <= 9)
                {
                    if (colorIndex != actual)
                    {
                        buttons[actual]->clearInnerText();
                        buttons[actual]->addLetter("X");
                    }
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                buttons[actual]->unselect();
                if (actual > 0 && actual != colorIndex)buttons[actual]->clearInnerText();
                if (actual == 0)
                    actual = actual + 1;
                else if (actual == 10)
                    actual = 0;
                else if (actual <= 7)
                    actual = actual + 3;
                else if (actual == 8 || actual == 9)
                    actual++;
                buttons[actual]->select();
                if (actual >= 4 && actual <= 9)
                {
                    if (colorIndex != actual)
                    {
                        buttons[actual]->clearInnerText();
                        buttons[actual]->addLetter("X");
                    }
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                buttons[actual]->unselect();
                if (actual > 0 && actual != colorIndex)buttons[actual]->clearInnerText();
                if (actual == 0) actual = 10;
                else
                {
                    actual = (actual - 1) % 11;
                }
                buttons[actual]->select();
                if (actual >= 4 && actual <= 9)
                {
                    if (colorIndex != actual)
                    {
                        buttons[actual]->clearInnerText();
                        buttons[actual]->addLetter("X");
                    }
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                buttons[actual]->unselect();
                if (actual > 0 && actual != colorIndex)buttons[actual]->clearInnerText();
                if (actual == 0) actual = 10;
                else if (actual >= 3)
                {
                    actual = (actual - 3) % 11;
                }
                else actual = 0;
                buttons[actual]->select();
                if (actual >= 4 && actual <= 9)
                {
                    if (colorIndex != actual)
                    {
                        buttons[actual]->clearInnerText();
                        buttons[actual]->addLetter("X");
                    }
                }
            }
            else if (event2.type == sf::Event::TextEntered && actual == 0)
            {
                if ((event2.text.unicode >= 97 && event2.text.unicode <= 122) || (event2.text.unicode >= 65 && event2.text.unicode <= 90))
                {
                    if (buttons[0]->getInnerText() == "Tu wpisz swój nick...")buttons[0]->clearInnerText();
                    buttons[0]->addLetter(event2.text.unicode);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && backspace)
                {
                    buttons[0]->deleteLetter();
                    backspace = false;
                }
            }
        }
        window.clear();
        bg.setColor(sf::Color(255, 255, 255, 100));
        window.draw(bg);
        window.draw(text1);
        window.draw(text2);
        window.draw(text3);
        for (int i = 0; i < 11; i++)
            window.draw(*buttons[i]);
        window.display();
    }
    if (startGame)
    {
        int clr;
        if (colorIndex > 3 && colorIndex < 10)clr = colorIndex - 4;
        else
        {
            std::string seed = buttons[0]->getInnerText();
            auto getRandom = std::bind(std::uniform_int_distribution<int>(0, 5),
                std::mt19937(seed.length()));
            clr = getRandom();
        }
        game gra(buttons[0]->getInnerText(), clr, players, &window);
        s.stop();
        gra.serverGameplay();
        s.play();
    }
}

void GUI::subMenu1(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s)
{
    bool goBack = false;
    bool startGame = false;
    sf::Font font;
    font.loadFromFile("fonts/arial.ttf");
    sf::Text text1(L"Podaj nick: ", font);
    text1.setCharacterSize(30);
    text1.setPosition(100, 100);
    text1.setFillColor(sf::Color::Red);
    sf::Text text2(L"Podaj IP: ", font);
    text2.setCharacterSize(30);
    text2.setPosition(100, 200);
    text2.setFillColor(sf::Color::Red);
    sf::Text text3(L"Twój kolorek: ", font);
    text3.setCharacterSize(30);
    text3.setPosition(100, 300);
    text3.setFillColor(sf::Color::Red);
    int actual = 0;
    int colorIndex = 8;
    std::unique_ptr<button> buttons[9]
    {
        move(std::unique_ptr<button>(new button(resolution.x / 2.f,115.f,500.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button(resolution.x / 2.f,215.f,500.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) - 175.f,315.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f),315.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) + 175.f,315.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) - 175.f,415.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f),415.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f) + 175.f,415.f,150.f,50.f, "img/input.png", "img/input_2.png",1))),
        move(std::unique_ptr<button>(new button((resolution.x / 2.f),515.f,500.f,50.f, "img/menu2.png", "img/menu2_2.png")))
    };
    buttons[0]->addLetter("Tu wpisz swój nick...");
    buttons[1]->addLetter("Tu wpisz adres IP serwera...");
    buttons[2]->setColor(sf::Color::Red);
    buttons[3]->setColor(sf::Color::Green);
    buttons[4]->setColor(sf::Color::Blue);
    buttons[5]->setColor(sf::Color::Yellow);
    buttons[6]->setColor(sf::Color::Magenta);
    buttons[7]->setColor(sf::Color::Cyan);
    buttons[0]->select();
    sf::Event event;
    while (!goBack)
    {
        bool backspace = true;
        sf::String playerInput;
        sf::Text playerText;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                s.stop();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                goBack = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
            {
                if (actual > 1 && actual < 8)
                {
                    buttons[colorIndex]->clearInnerText();
                    buttons[actual]->clearInnerText();
                    buttons[actual]->addLetter("Wybrano");
                    colorIndex = actual;
                }
                if (actual == 8)
                {
                    startGame = true;
                    goBack = true;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && actual == 0)
            {
                buttons[actual]->unselect();
                actual++;
                buttons[actual]->select();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && actual == 1)
            {
                buttons[actual]->unselect();
                if (!validIp(buttons[1]->getInnerText()))
                {
                    buttons[1]->setColor(sf::Color::Red);
                }
                else
                {
                    buttons[1]->setColor(sf::Color::Green);
                }
                actual--;
                buttons[actual]->select();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && actual == 1)
            {
                buttons[actual]->unselect();
                if (!validIp(buttons[1]->getInnerText()))
                {
                    buttons[1]->setColor(sf::Color::Red);
                }
                else
                {
                    buttons[1]->setColor(sf::Color::Green);
                }
                actual++;
                buttons[actual]->select();
                if (actual != colorIndex)
                    buttons[actual]->addLetter("X");
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (actual == 2 || actual == 3 || actual == 4))
            {
                if (actual != colorIndex)
                    buttons[actual]->clearInnerText();
                buttons[actual]->unselect();
                actual = 1;
                buttons[actual]->select();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (actual == 2 || actual == 3 || actual == 4))
            {
                if (actual != colorIndex)
                    buttons[actual]->clearInnerText();
                buttons[actual]->unselect();
                actual += 3;
                buttons[actual]->select();
                if (actual != colorIndex)
                    buttons[actual]->addLetter("X");
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && (actual == 5 || actual == 6 || actual == 7))
            {
                if (actual != colorIndex)
                    buttons[actual]->clearInnerText();
                buttons[actual]->unselect();
                actual -= 3;
                buttons[actual]->select();
                if (actual != colorIndex)
                    buttons[actual]->addLetter("X");
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && (actual == 2 || actual == 5 || actual == 3 || actual == 6))
            {
                if (actual != colorIndex)
                    buttons[actual]->clearInnerText();
                buttons[actual]->unselect();
                actual++;
                buttons[actual]->select();
                if (actual != colorIndex)
                    buttons[actual]->addLetter("X");
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && (actual == 3 || actual == 4 || actual == 6 || actual == 7))
            {
                if (actual != colorIndex)
                    buttons[actual]->clearInnerText();
                buttons[actual]->unselect();
                actual--;
                buttons[actual]->select();
                if (actual != colorIndex)
                    buttons[actual]->addLetter("X");
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && (actual == 5 || actual == 6 || actual == 7))
            {
                if (actual != colorIndex)
                    buttons[actual]->clearInnerText();
                buttons[actual]->unselect();
                actual = 8;
                buttons[actual]->select();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && actual == 8)
            {
                buttons[actual]->unselect();
                actual = 5;
                buttons[actual]->select();
                if (actual != colorIndex)
                    buttons[actual]->addLetter("X");
            }
            else if (event.type == sf::Event::TextEntered && actual == 0)
            {
                if ((event.text.unicode >= 97 && event.text.unicode <= 122) || (event.text.unicode >= 65 && event.text.unicode <= 90))
                {
                    if (buttons[0]->getInnerText() == "Tu wpisz swój nick...")buttons[0]->clearInnerText();
                    buttons[0]->addLetter(event.text.unicode);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && backspace)
                {
                    buttons[0]->deleteLetter();
                    backspace = false;
                }
            }
            else if (event.type == sf::Event::TextEntered && actual == 1)
            {
                if ((event.text.unicode >= 48 && event.text.unicode <= 57) || event.text.unicode == 46)
                {
                    if (buttons[1]->getInnerText() == "Tu wpisz adres IP serwera...")buttons[1]->clearInnerText();
                    buttons[1]->addLetter(event.text.unicode);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace) && backspace)
                {
                    buttons[1]->deleteLetter();
                    backspace = false;
                }
            }
        }
        window.clear();
        bg.setColor(sf::Color(255, 255, 255, 100));
        window.draw(bg);
        window.draw(text1);
        window.draw(text2);
        window.draw(text3);
        for (int i = 0; i < 9; i++)
            window.draw(*buttons[i]);
        window.display();
    }
    if (startGame)
    {
        int clr;
        if (colorIndex > 1 && colorIndex < 8)clr = colorIndex - 2;
        else
        {
            std::string seed = buttons[0]->getInnerText();
            auto getRandom = std::bind(std::uniform_int_distribution<int>(0, 5),
                std::mt19937(seed.length()));
            clr = getRandom();
        }
        game gra(buttons[0]->getInnerText(), clr, sf::IpAddress(buttons[1]->getInnerText()), &window);
        if (!gra.connectionError)
        {
            s.stop();
            gra.serverGameplay();
        }
        s.play();
    }
    return;
}

void GUI::subMenu2(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s)
{
    sf::Event event;
    bool goBack = false;
    sf::Font font;
    font.loadFromFile("fonts/arial.ttf");
    sf::Text text("", font);
    sf::String temp(L"Nieoczywiste zasady:\n-od wiêzienia ochrania jednynie karta wylosowana na polu szansy\n-miasta mo¿na odkupiæ za dwukrotnoœæ ich wartoœci, czyli 2*(cena_zakupu + poziom_budynków*cena_jednego_budynku)\n-obiektów przemys³owych nie mo¿na odkupiæ ani ulepszyæ\n");
    temp += sf::String(L"\nJe¿eli sam nie wybierzesz kolorku, to zostanie on wylosowany\n\n");
    temp += sf::String(L"Gdy zabraknie Ci pieniêdzy na czynsz, op³acenie kary z wylosowanej szansy lub podatku\n-zostajesz bankrutem, je¿eli wartoœæ Twojego maj¹tku oraz stan konta s¹ mniejsze ni¿ kwota d³ugu\n");
    temp += sf::String(L"-Twój maj¹tek zostaje wyprzedany przez algorytm, je¿eli pozwoli on pokryæ d³ug\n");
    temp += sf::String(L"Sposób dzia³ania algorytmu:\n");
    temp += sf::String(L"Algorytm sortuje Twoje posiad³oœci od najtañszej do najdro¿szej \nNastêpnie zaczyna je sprzedawaæ tak d³ugo, a¿ nie uzbiera wymaganej liczby $\n");
    temp += sf::String(L"Je¿eli zostan¹ jakieœ $, to otrzymasz je na konto\n");
    temp += sf::String(L"Kurs sprzeda¿y wynosi 1.0\n");
    temp += sf::String(L"\nCo nie zosta³o zaimplementowane:\n");
    temp += sf::String(L"-mechanizmy licytacji\n-mechnizmy dotycz¹ce dubletów\n");
    temp += sf::String(L"\nW grê mo¿na graæ w sieci WAN, je¿eli gracz tworz¹cy grê posiada publiczny adres IP i otworzy port 53000\n");
    temp += sf::String(L"\nPola przemys³owe:\n");
    temp += sf::String(L"-im wiêcej posiadasz pól z poci¹gami, tym wiêkszy pobierasz czynsz\n");
    temp += sf::String(L"1 pole => 50 $\n");
    temp += sf::String(L"2 pola => 100 $\n");
    temp += sf::String(L"3 pola => 200 $\n");
    temp += sf::String(L"4 pola => 400 $\n");
    temp += sf::String(L"-czynsz na polach Elektrownia i Wodoci¹gi wynosi 10x liczba_ostatnio_wyrzuconych_kostek\n");
    text.setString(temp);
    text.setCharacterSize(30);
    text.setPosition(15.f, 15.f);
    text.setFillColor(sf::Color::Red);
    while (!goBack)
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                s.stop();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                goBack = true;
            }
        }
        window.clear();
        bg.setColor(sf::Color(255, 255, 255, 100));
        window.draw(bg);
        window.draw(text);
        window.display();
    }
    return;
}

void GUI::subMenu3(sf::RenderWindow & window, sf::Sprite & bg, sf::Sound& s)
{


    bool goBack = false;
    // Declare and load a font
    sf::Font font;
    font.loadFromFile("fonts/arial.ttf");
    // Create a text
    sf::Text text(L"Ustawienia grafiki\nTryb pe³noekranowy:", font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);
    text.setPosition(100.f, 350.f);

    std::unique_ptr<button> no(new button(resolution.x / 2.f, 400.f, 500.f, 50.f, "img/no.png", "img/no_2.png"));
    std::unique_ptr<button> yes(new button(resolution.x / 2.f, 500.f, 500.f, 50.f, "img/yes.png", "img/yes_2.png"));
    no->select();
    bool fullScreen = false;
    bool GoBack = false;
    bool clicked = false;
    sf::Event event;
    while (!GoBack)
    {
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                s.stop();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !clicked)
            {
                clicked = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                GoBack = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && clicked)
            {
                if (fullScreen)
                {
                    window.create(sf::VideoMode(resolution.x, resolution.y), "Eurobiznes", sf::Style::Fullscreen);
                    window.setFramerateLimit(60);
                    //window.setMouseCursorVisible(0);
                }
                else
                {
                    window.create(sf::VideoMode(resolution.x, resolution.y), "Eurobiznes");
                    window.setFramerateLimit(60);
                    //window.setMouseCursorVisible(0);
                }
                GoBack = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            {
                window.create(sf::VideoMode(resolution.x, resolution.y), "Eurobiznes", sf::Style::Fullscreen);
                window.setFramerateLimit(60);
                //window.setMouseCursorVisible(0);
                GoBack = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            {
                window.create(sf::VideoMode(resolution.x, resolution.y), "Eurobiznes");
                window.setFramerateLimit(60);
                //window.setMouseCursorVisible(0);
                GoBack = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                clicked = true;
                yes->select();
                fullScreen = true;
                no->unselect();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                clicked = true;
                no->select();
                fullScreen = false;
                yes->unselect();
            }
        }
        window.clear();
        if (GoBack)break;
        bg.setColor(sf::Color(255, 255, 255, 100));
        window.draw(bg);
        window.draw(*yes);
        window.draw(*no);
        window.draw(text);
        window.display();
    }
    return;
}


void GUI::subMenu4(sf::RenderWindow& window, sf::Sprite& bg, sf::Sound& s)
{
    sf::Event event;
    sf::SoundBuffer buff;
    buff.loadFromFile("sound/end.wav");
    sf::Sound sad;
    sad.setBuffer(buff);
    sad.play();
    std::unique_ptr<button> no(new button(resolution.x / 2.f, 400.f, 500.f, 50.f, "img/no.png", "img/no_2.png"));
    std::unique_ptr<button> yes(new button(resolution.x / 2.f, 500.f, 500.f, 50.f, "img/yes.png", "img/yes_2.png"));
    sf::Font font;
    font.loadFromFile("fonts/arial.ttf");
    sf::Text text(L"Czy na pewno chcesz opuœciæ grê?", font);
    text.setCharacterSize(30);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);
    text.setPosition(resolution.x / 2.f - 250.f, 300.f);
    no->select();
    bool quiting = false;
    bool fuGoBack = false;
    bool clicked = false;
    while (!fuGoBack)
    {
        s.pause();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !clicked)
            {
                clicked = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                fuGoBack = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && clicked)
            {
                if (quiting)
                {
                    s.stop();
                    window.close();
                    return;
                }
                else
                {
                    fuGoBack = true;
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
            {
                s.stop();
                window.close();
                return;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
            {
                fuGoBack = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                clicked = true;
                yes->select();
                quiting = true;
                no->unselect();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                clicked = true;
                no->select();
                quiting = false;
                yes->unselect();
            }
        }
        window.clear();
        if (fuGoBack)break;
        bg.setColor(sf::Color(255, 255, 255, 100));
        window.draw(bg);
        window.draw(*yes);
        window.draw(*no);
        window.draw(text);
        window.display();
    }
    sad.stop();
    s.play();
    return;
}