#include "game.h"

void game::readSettings()
{
	std::filesystem::path path("settings/settings.txt");
	if (std::filesystem::exists(path))
	{
		std::ifstream input(path);
		std::string line;
		std::vector<std::string> settings;
		while (getline(input, line))
		{
			settings.push_back(line);
		}
		if (settings.size() == 8)
		{
			minutes = std::stoi(settings[1]);
			seconds = std::stoi(settings[3]);
			defaultMoney = std::stoi(settings[5]);
			speed = std::stof(settings[7]);
		}
	}
}
void game::sellProperties()
{
	int dlug = -players[actualPlayer]->money;
	std::vector<std::pair<int, int>> properties;
	for (int i = 1; i < 40; ++i)
	{
		if (fields[i]->getOwner() == actualPlayer)
		{
			properties.push_back(std::pair<int, int>{i, fields[i]->getValue()});
		}
	}
	/*sort(properties.begin(), properties.end(),
		[](std::pair<int, int> a, std::pair<int, int> b) {
			return (a.second < b.second);
		});*/
	std::ranges::sort(properties, [](std::pair<int, int> a, std::pair<int, int> b) {return (a.second < b.second);});
	sf::String temp(L"Sprzedane posiad³oœci:\n");
	if (properties[0].second >= dlug)
	{
		fields[properties[0].first]->sellField();
		players[actualPlayer]->money += properties[0].second;
		temp += fields[properties[0].first]->name + "\n";
		vindicationed.setString(temp);
		return;
	}
	else
		for (int i = 1; i < properties.size(); ++i)
		{
			properties[i].second += properties[i - 1].second;
			if (properties[i].second >= dlug)
			{
				players[actualPlayer]->money += properties[i].second;
				for (int j = 0; j <= i; j++)
				{
					fields[properties[j].first]->sellField();
					temp += "- " + fields[properties[j].first]->name + "\n";
				}
				vindicationed.setString(temp);
				break;
			}
		}
}

void game::startTimer()
{
	time_t now;
	now = time(NULL);
	bool loaded = false;
	while (!(minutes == 0 && seconds == 0))
	{
		time_t then;
		then = time(NULL);
		if (now != then)
		{
			now = then;
			if (seconds != 0)seconds--;
			else
			{
				minutes--;
				seconds += 59;
			}
			std::string sec="";
			if (seconds < 10)sec += "0";
			sec += std::to_string(seconds);
			std::string time = std::to_string(minutes) + ":" + sec;
			timer.setString(time);
		}
		if (minutes == 0 && seconds == 59 && !loaded)
		{
			sound.stop();
			buffer.loadFromFile("sound/noTime.wav");
			sound.play();
			sound.setLoop(false);
			loaded = true;
		}
	}
	return;
}

game::game(std::string n, int color, sf::IpAddress serwerIP, sf::RenderWindow* w)
{
	window = w;
	amISerwer = false;
	connectWithSerwer(n, color, serwerIP);
	if (!connectionError)
	{
		loadTexts();
		loadBoard();
		loadSound();
	}
}
game::game(std::string nick, int color, int numOfPlayers, sf::RenderWindow *w) : numberOfPlayers(numOfPlayers)
{
	window = w;
	seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	amISerwer = true;
	players.push_back(std::shared_ptr<player>(new player(nick)));
	players[0]->setColor(color);
		for (int i = 0; i < numberOfPlayers-1; ++i)connectWithPlayer(nick,color);
	loadTexts();
	loadBoard();
	loadSound();
}
int chance::seed = 0;
int chance::sizeBlue = 3;
int chance::sizeRed = 3;
std::vector<int> chance::blueNumbers{};
std::vector<int> chance::redNumbers{};

void game::serverGameplay()
{
	if (amISerwer)
		sendPlayers();
	else
		getPlayers();
	setLabels();
	auto getRandom = std::bind(std::uniform_int_distribution<int>(1, 6),
		std::mt19937(seed));
	chance::seed = seed;
	readSettings();
	for (int i = 0; i < numberOfPlayers; ++i)
	{
		players[i]->pawn.setPosition(900.f + 38 * i, 956.f);
		players[i]->money = defaultMoney;
		playerMoney[i].setString(std::to_string(players[i]->money) + " $");
	}
	std::jthread timerThread(&game::startTimer, this);
	sf::Event event;
	std::jthread recive(std::move(&game::recive), this);
	std::jthread send(std::move(&game::send), this);
	std::jthread hide(std::move(&game::hideInfo), this, 5000);

	sound.play();
	while (!(minutes==0 && seconds==0 && bancrupts<(numberOfPlayers-1)))
	{
		for (int i = 0; i < numberOfPlayers; ++i)
		{
			if (players[i]->roundsInJail > 0)
			{
				players[i]->roundsInJail--;
			}
			else if (!players[i]->bankrupt)
			{
				playerNames[i].setStyle(sf::Text::Italic | sf::Text::Bold);
				if (players[i]->roundsInJail == 0)players[i]->inJail = false;
				actualPlayer = i;
				doneRecive = false;
				doneSend = false;
				if (i == myId)
				{
					showThrow = true;
					while (1)
					{
						DRAW;
						if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && window->hasFocus())
						//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
						{
							showThrow = false;
							lastThrow = getRandom() + getRandom();
							cube.play();
							if (!fields[(players[i]->position + lastThrow) % 40]->showButton(players, i, decision))sending = true;
							movePlayer(i);

							showYesNo = fields[players[i]->position]->showButton(players, i, decision);

							if (showYesNo)
							{
								yes->select();
								no->unselect();
								playerDecison = true;
								while (1)
								{
									DRAW;
									if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
									{
										playerDecison = true;
										sending = true;
										showYesNo = false;
										break;
									}
									else if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
									{
										playerDecison = false;
										sending = true;
										showYesNo = false;
										break;
									}
									else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
									{
										if (playerDecison)
										{
											no->select();
											yes->unselect();
										}
										else
										{
											yes->select();
											no->unselect();
										}
										playerDecison = !playerDecison;
										Sleep(100);
									}
									else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
									{
										sending = true;
										showYesNo = false;
										break;
									}
								}
							}
							fields[players[i]->position]->mainEventServer(players, i, playerDecison, information);
							if (players[i]->vindication)
							{
								vin.play();
								sellProperties();
								players[i]->vindication = false;
							}
							else if (players[i]->bankrupt)
							{
								banc.play();
								removePlayer();
								bancrupts++;
							}
							while (showInfo)
							{
								DRAW;
							}
							Sleep(300);
							showInfo = true;
							while (1)if (doneSend)break;
							break;
						}
						else if (event.type == sf::Event::Closed)
						{
							kill = true;
							Sleep(250);
							sound.stop();
							window->close();
						}
					}
				}
				else
				{
					showThrow = false;
					listen = true;
					while (1)
					{
						DRAW;
						if (event.type == sf::Event::Closed)
						{
							kill = true;
							Sleep(250);
							sound.stop();
							window->close();
						}
						if (doneRecive)
						{
							if (amISerwer)sending = true;
							lastThrow = getRandom() + getRandom();
							cube.play();
							movePlayer(i);
							if (amISerwer)while (1)if (doneSend)break;
							fields[players[i]->position]->mainEventServer(players, i, playerDecison, information);
							if (players[i]->vindication)
							{
								sellProperties();
								players[i]->vindication = false;
							}
							else if (players[i]->bankrupt)
							{
								removePlayer();
								bancrupts++;
							}
							while (showInfo)
							{
								DRAW;
							}
							Sleep(300);
							showInfo = true;
							break;
						}
					}
				}
				for (int j = 0; j < numberOfPlayers; ++j)playerMoney[j].setString(std::to_string(players[j]->money) + " $");

				playerNames[i].setStyle(sf::Text::Regular);
			}
		}
	}
	/*sort(players.begin(), players.end(),
		[](std::shared_ptr<player> a, std::shared_ptr<player> b) {
			return ((a->money+a->wealth) > (b->money + b->wealth));
		});*/
	std::ranges::sort(players, [](std::shared_ptr<player> a, std::shared_ptr<player> b) {
		return ((a->money + a->wealth) > (b->money + b->wealth));
		});
	kill = true;
	showThrow = false;
	showInfo = false;
	showEnd = true;
	setEndTextAndSound();
	sound.play();
	while (1)
	{
		DRAW;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		break;
		else if (event.type == sf::Event::Closed)
		{
			window->close();
		}
	}
	sound.stop();
}
std::vector<sf::Sound> field::sounds{};
std::vector<sf::SoundBuffer> field::buffers{};


void game::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(bg, states);
	for (auto& el : fields)
	{
		target.draw(*el, states);
	}
	for (int i = 0; i < numberOfPlayers;++i)
	{
		if (players[i]->inJail)target.draw(*players[i], states);
	}
	target.draw(*fields[10], states);
	for (int i = 0; i < numberOfPlayers; ++i)
	{
		if (!players[i]->inJail)target.draw(*players[i], states);
	}
	for (auto& el : playerRectangle)
	{
		target.draw(el, states);
	}
	for (auto& el : playerNames)
	{
		target.draw(el, states);
	}
	for (auto& el : playerMoney)
	{
		target.draw(el, states);
	}
	if (showYesNo)
	{
		target.draw(*yes, states);
		target.draw(*no, states);
		target.draw(decision, states);
	}
	else if (showThrow)
		target.draw(*go, states);
	if (showInfo)
	{
		target.draw(information, states);
		target.draw(vindicationed, states);
	}
	target.draw(logo, states);
	target.draw(logo2, states);
	target.draw(blueCard, states);
	target.draw(redCard, states);
	target.draw(timer, states);
	if (showEnd)
	{
		for(auto el : endTexts)target.draw(el, states);
	}
}

void game::getPlayers()
{
	sf::Packet packet;
	sf::Uint32 xd;
	players[0]->socket.receive(packet);
	for (int i = 1; i < numberOfPlayers; ++i)
	{
		std::string serverNick;
		packet >> serverNick;
		packet >> xd;
		int serwerColor = xd;
		players.push_back(std::shared_ptr<player>(new player(serverNick, i)));
		players[i]->setColor(serwerColor);
	}
	packet >> xd;
	myId = xd;
	packet >> xd;
	seed = xd;
}
void game::sendPlayers()
{
	for (int i = 1; i < players.size(); ++i)
	{
		sf::Packet packet;
		for (int j = 1; j < players.size(); ++j)
		{
			packet << players[j]->getNick();
			packet << players[j]->getColorUint32();
		}
		packet << i;
		packet << seed;
		players[i]->socket.send(packet);
	}
}
void game::connectWithSerwer(std::string nick, int c, sf::IpAddress serwerIP, int port)
{
	std::shared_ptr<player> temp(new player(0));
	sf::Socket::Status status = temp->socket.connect(serwerIP, port);
	if (status != sf::Socket::Done)
	{
		connectionError = true;
	}
	else
	{
		sf::Packet packet;
		sf::Uint32 xd = c;
		packet << nick;
		packet << xd;
		temp->socket.send(packet);
		packet.clear();
		std::string serverNick;
		temp->socket.receive(packet);
		packet >> serverNick;
		packet >> xd;
		packet >> numberOfPlayers;
		int serwerColor = xd;
		temp->setNick(serverNick);
		temp->setColor(serwerColor);
		players.push_back(temp);
		connectionError = false;
	}
	return;
}
void game::connectWithPlayer(std::string serverNick, int serverColor)
{
	sf::TcpListener listener;
	listener.listen(53000);
	std::shared_ptr<player> temp(new player(players.size()));
	std::string nick;
	sf::Uint32 xd;
	sf::Uint32 xdd;
	if (listener.accept(temp->socket) == sf::Socket::Done)
	{
		sf::Packet packet;
		temp->socket.receive(packet);
		packet >> nick;
		packet >> xd;
		int playerColor = xd;
		packet.clear();
		xd = serverColor;
		xdd = numberOfPlayers;
		packet << serverNick;
		packet << xd;
		packet << xdd;
		temp->socket.send(packet);
		temp->setNick(nick);
		temp->setColor(playerColor);
		players.push_back(temp);
		listener.close();
	}
	else
		connectionError = true;
	return;
}

sf::Color game::getColor(int i)
{
	switch (i)
	{
	case 0:
		return sf::Color::Red;
	case 1:
		return sf::Color::Green;
	case 2:
		return sf::Color::Blue;
	case 3:
		return sf::Color::Yellow;
	case 4:
		return sf::Color::Magenta;
	case 5:
		return sf::Color::Cyan;
	default:
		return sf::Color::Transparent;
	}
}

void game::setEndTextAndSound()
{
	sf::Text temp;
	temp.setCharacterSize(50);
	temp.setFillColor(sf::Color::Black);
	temp.setFont(font);
	temp.setPosition(1280.f, 300.f);
	temp.setString("KONIEC GRY");
	endTexts.push_back(temp);
	temp.setCharacterSize(40);
	temp.setFont(font);
	temp.setPosition(1280.f, 400.f);
	if (players[0]->id == myId)
	{
		temp.setString(L"Wygra³eœ !!!");
		temp.setFillColor(sf::Color::Green);
		buffer.loadFromFile("sound/winTheme.wav");
		sound.setLoop(true);
	}
	else
	{
		temp.setString(L"Przegra³eœ !!!");
		temp.setFillColor(sf::Color::Red);
		buffer.loadFromFile("sound/lostTheme.wav");
	}
	sound.setBuffer(buffer);
	endTexts.push_back(temp);
	sf::String table = "Gracz\n";
	temp.setCharacterSize(20);
	temp.setFillColor(sf::Color::Black);
	for (int i = 0; i < numberOfPlayers; i++)
	{
		table += players[i]->nick + "\n";
		temp.setPosition(1280.f, 450.f + i * 50.f);
	}
	table += sf::String(L"Naciœnij ENTER, aby wróciæ do menu g³ównego");
	temp.setString(table);
	endTexts.push_back(temp);
	table = sf::String(L"Maj¹tek\n");
	for (int i = 0; i < numberOfPlayers; i++)
	{
		table += std::to_string(players[i]->money + players[i]->wealth) + " $\n";
		temp.setPosition(1500.f, 450.f + i * 50.f);
	}
	temp.setString(table);
	endTexts.push_back(temp);
}

void game::loadSound()
{
	buffer.loadFromFile("sound/gameTheme.wav");
	sound.setBuffer(buffer);
	sound.setLoop(true);
	sound.setVolume(80.f);
	buffer2.loadFromFile("sound/cube.wav");
	cube.setBuffer(buffer2);
	buffer3.loadFromFile("sound/vin.wav");
	vin.setBuffer(buffer3);
	buffer4.loadFromFile("sound/banct.wav");
	banc.setBuffer(buffer4);
	sf::SoundBuffer temp;
	temp.loadFromFile("sound/swk.wav");
	field::buffers.push_back(temp);
	temp.loadFromFile("sound/rent.wav");
	field::buffers.push_back(temp);
	temp.loadFromFile("sound/chance.wav");
	field::buffers.push_back(temp);
	temp.loadFromFile("sound/jail.wav");
	field::buffers.push_back(temp);
	for (int i = 0; i < field::buffers.size(); ++i)
	{
		field::sounds.push_back(sf::Sound(field::buffers[i]));
	}
}

void game::send()
{

	while (1)
	{
		sf::Packet p;
		if (playerDecison == true)p << true;
		else p << false;
		if (sending)
		{

			if (amISerwer)
				for (int j = 1; j < numberOfPlayers; j++)
				{
					if (j != actualPlayer)
					{
						players[j]->socket.send(p);
					}
				}
			else
			{
				players[0]->socket.send(p);
			}
			doneSend = true;
			sending = false;
		}
		if (kill)break;
		Sleep(200);
	}
}

void game::recive()
{
	while (1)
	{
		if (listen)
		{
			bool temp = false;
			sf::Packet p;
			if (amISerwer)
			{
				players[actualPlayer]->socket.receive(p);
			}
			else
			{
				players[0]->socket.receive(p);
			}
			p >> temp;
			playerDecison = temp;
			doneRecive = true;
			listen = false;
		}
		if (kill)break;
		Sleep(200);
	}
}

void game::hideInfo(int interwal)
{
	while (1)
	{
		if (showInfo)
		{
			Sleep(interwal);
			showInfo = false;
			vindicationed.setString("");
			information.setString("");
		}
		if (kill)break;
		Sleep(200);
	}
}

void game::removePlayer()
{
	players[actualPlayer]->money = 0;
	players[actualPlayer]->wealth = 0;
	for (int j = 1; j < 40; j++)
	{
		if (fields[j]->getOwner() == actualPlayer)
			fields[j]->sellField();
	}
}

void game::loadTextures()
{
	logoTexture.loadFromFile("img/logo.png");
	logo.setTexture(logoTexture);
	logo.setOrigin(308.f, 55.f);
	logo.setScale(1, 1);
	logo.setTextureRect(sf::IntRect(1, 1, 615, 110));
	logo.rotate(-45.f);
	logo.setPosition(500.f, 500.f);
	logoTexture2.loadFromFile("img/logo2.png");
	logo2.setTexture(logoTexture2);
	logo2.setOrigin(303.5, 39.f);
	logo2.setScale(1, 1);
	logo2.setTextureRect(sf::IntRect(1, 1, 607, 78));
	logo2.rotate(-45.f);
	logo2.setPosition(560.f, 560.f);
	redCardTexture.loadFromFile("img/redCard.png");
	redCard.setTexture(redCardTexture);
	redCard.setOrigin(86.f, 120.f);
	redCard.setScale(1, 1);
	redCard.setTextureRect(sf::IntRect(1, 1, 172, 240));
	redCard.rotate(-135.f);
	redCard.setPosition(700.f, 700.f);
	blueCardTexture.loadFromFile("img/blueCard.png");
	blueCard.setTexture(blueCardTexture);
	blueCard.setOrigin(86.f, 120.f);
	blueCard.setScale(1, 1);
	blueCard.setTextureRect(sf::IntRect(1, 1, 172, 240));
	blueCard.rotate(-135.f);
	blueCard.setPosition(370.f, 370.f);

	no = std::move(std::unique_ptr<button>(new button(1500.f, 500.f, 500.f, 50.f, "img/no.png", "img/no_2.png")));
	yes = std::move(std::unique_ptr<button>(new button(1500.f, 400.f, 500.f, 50.f, "img/yes.png", "img/yes_2.png")));
	go = std::move(std::unique_ptr<button>(new button(1500.f, 300.f, 500.f, 50.f, "img/throw.png", "img/throw.png")));

	bgTexture.loadFromFile("img/bgGame.jpg");
	bg.setTexture(bgTexture);
	bg.setScale(1, 1);
	bg.setTextureRect(sf::IntRect(1, 1, 1920, 1080));
}

void game::loadTexts()
{
	font.loadFromFile("fonts/arial.ttf");
	timer.setCharacterSize(25);
	timer.setFillColor(sf::Color::White);
	std::string time = std::to_string(minutes) + ":" + std::to_string(seconds);
	timer.setString(time);
	timer.setFont(font);
	timer.setPosition(250.f, 250.f);
	information.setCharacterSize(20);
	information.setFillColor(sf::Color::Black);
	information.setFont(font);
	information.setPosition(1280.f, 200.f);
	information.setString(L"Tu powinny byæ informacje");
	decision.setCharacterSize(20);
	decision.setFillColor(sf::Color::Black);
	decision.setFont(font);
	decision.setPosition(1280.f, 300.f);
	vindicationed.setCharacterSize(20);
	vindicationed.setFillColor(sf::Color::Black);
	vindicationed.setFont(font);
	vindicationed.setPosition(1280.f, 800.f);
}

void game::setLabels()
{
	for (int i = 0; i < numberOfPlayers; ++i)
	{
		sf::Text temp;
		sf::Text temp2;
		temp.setCharacterSize(30);
		temp2.setCharacterSize(30);
		temp.setFillColor(sf::Color::Black);
		temp2.setFillColor(sf::Color::Black);
		temp.setString(players[i]->nick);
		std::string m = std::to_string(players[i]->money) + " $";
		temp2.setString(m);
		temp.setFont(font);
		temp2.setFont(font);
		temp.setPosition(1100.f, 20 + 30.f * i);
		float w = temp2.getLocalBounds().width;
		temp2.setOrigin(w, 0);
		temp2.setPosition(1700.f, 20 + 30.f * i);
		sf::RectangleShape temp3;
		temp3.setSize(sf::Vector2f(650, 30));
		temp3.setFillColor(getColor(players[i]->color));
		temp3.setPosition(1100, 25 + 30.f * i);
		playerNames.push_back(temp);
		playerMoney.push_back(temp2);
		playerRectangle.push_back(temp3);
	}
}
void game::movePlayer(int id)
{
	int oldPosition = players[id]->position;
	float scale = 1.0f;
	int counter = 0;
	fields[oldPosition]->leavePlace(id);
	players[id]->position += lastThrow;
	players[id]->lastMove = lastThrow;
	if (players[id]->position > 40)
	{
		players[id]->money += 400;
		playerMoney[id].setString(std::to_string(players[id]->money) + " $");
	}
	players[id]->position = players[id]->position % 40;
	sf::Vector2f newPosition = fields[players[id]->position]->getPlace(id);
	float dx;
	float dy;
	sf::Vector2f currentPosition = players[id]->pawn.getPosition();
	dx = newPosition.x - currentPosition.x;
	dy = newPosition.y - currentPosition.y;
	players[id]->pawn.setTexture(players[id]->pawnTexture2);
	sf::Event event;
	if (oldPosition >= 0 && oldPosition <= 10 || oldPosition >= 20 && oldPosition <= 30)
	{
		if (dx < 0) players[id]->velocity.x = -speed;
		else players[id]->velocity.x = speed;
		players[id]->velocity.y = 0.0;
		while (1)
		{
			if (dx == 0)break;
			DRAW;
			counter++;
			if (counter == 10)
			{
				counter = 0;
				scale = -scale;
				players[id]->pawn.setScale(scale, 1.f);
			}
			players[id]->pawn.move(players[id]->velocity);
			if (players[id]->pawn.getPosition().x > newPosition.x - speed && players[id]->pawn.getPosition().x < newPosition.x + speed)break;
		}
		players[id]->velocity.x = 0.0;
		if (dy < 0) players[id]->velocity.y = -speed;
		else players[id]->velocity.y = speed;
		while (1)
		{
			if (dy == 0)break;
			DRAW;
			counter++;
			if (counter == 10)
			{
				counter = 0;
				scale = -scale;
				players[id]->pawn.setScale(scale, 1.f);
			}
			players[id]->pawn.move(players[id]->velocity);
			if (players[id]->pawn.getPosition().y > newPosition.y - speed && players[id]->pawn.getPosition().y < newPosition.y + speed)break;
		}

	}
	else
	{
		if (dy < 0) players[id]->velocity.y = -speed;
		else players[id]->velocity.y = speed;
		players[id]->velocity.x = 0.0;
		while (1)
		{
			if (dy == 0)break;
			DRAW;
			counter++;
			if (counter == 10)
			{
				counter = 0;
				scale = -scale;
				players[id]->pawn.setScale(scale, 1.f);
			}
			players[id]->pawn.move(players[id]->velocity);
			if (players[id]->pawn.getPosition().y > newPosition.y - speed && players[id]->pawn.getPosition().y < newPosition.y + speed)break;
		}
		if (dx < 0) players[id]->velocity.x = -speed;
		else players[id]->velocity.x = speed;
		players[id]->velocity.y = 0.0;
		while (1)
		{
			if (dx == 0)break;
			DRAW;
			counter++;
			if (counter == 10)
			{
				counter = 0;
				scale = -scale;
				players[id]->pawn.setScale(scale, 1.f);
			}
			players[id]->pawn.move(players[id]->velocity);
			if (players[id]->pawn.getPosition().x > newPosition.x - speed && players[id]->pawn.getPosition().x < newPosition.x + speed)break;
		}

	}
	players[id]->pawn.setTexture(players[id]->pawnTexture);
}

void game::loadBoard()
{
	loadTextures();
	//dolny rzad
	fields.push_back(std::unique_ptr<field>(std::move(new special(150.f, 981.f, 981.f, 400, "Start gry"))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Saloniki", "Grecja", 120, { 5,40,60,280,320,500 }, 100, sf::Color::Yellow, 863.f, 981.f))));
	fields.push_back(std::unique_ptr<field>(std::move(new chance(781.f, 981.f, sf::Color::Blue))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Ateny", "Grecja", 120, { 10,40,120,360,640,900 }, 100, sf::Color::Yellow, 699.f, 981.f))));
	fields.push_back(std::unique_ptr<field>(std::move(new special(80, 617.f, 981.f, 0, "Darmowy\nparking"))));
	fields.push_back(std::unique_ptr<field>(std::move(new industry(L"Poci¹gi S", 535.f, 981.f))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Neapol", L"W³ochy", 200, { 15,60,180,540,800,1100 }, 100, sf::Color::Red, 453.f, 981.f))));
	fields.push_back(std::unique_ptr<field>(std::move(new chance(371.f, 981.f, sf::Color::Red))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Mediolan", L"W³ochy", 200, { 15,60,180,540,800,1100 }, 100, sf::Color::Red, 289.f, 981.f))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Rzym", L"W³ochy", 240, { 20,80,200,600,900,1200 }, 100, sf::Color::Red, 207.f, 981.f))));
	fields.push_back(std::unique_ptr<field>(std::move(new jail(89.f, 981.f))));
	//lewy
	fields.push_back(std::unique_ptr<field>(std::move(new city("Barcelona", "Hiszpania", 280, { 20,100,300,900,1250,1500 }, 200, sf::Color::Magenta, 89.f, 863.f, 90))));
	fields.push_back(std::unique_ptr<field>(std::move(new industry(L"Elektrownia", 89.f, 781.f, 'P', 300, 90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Sewilla", "Hiszpania", 280, { 20,100,300,900,1250,1500 }, 200, sf::Color::Magenta, 89.f, 699.f, 90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Madryt", "Hiszpania", 320, { 25,120,360,1000,1400,1800 }, 200, sf::Color::Magenta, 89.f, 617.f, 90))));
	fields.push_back(std::unique_ptr<field>(std::move(new industry(L"Poci¹gi W", 89.f, 535.f, 'x', 400, 90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Liverpool", "Anglia", 360, { 30,140,400,1100,1500,1900 }, 200, sf::Color(255, 66, 85), 89.f, 453.f, 90))));
	fields.push_back(std::unique_ptr<field>(std::move(new chance(89.f, 371.f, sf::Color::Blue, 90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Glasgow", "Anglia", 360, { 30,140,400,1100,1500,1900 }, 200, sf::Color(255, 66, 85), 89.f, 289.f, 90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Londyn", "Anglia", 400, { 35,160,440,1200,1600,2000 }, 200, sf::Color(255, 66, 85), 89.f, 207.f, 90))));
	//gorny rzad
	fields.push_back(std::unique_ptr<field>(std::move(new special(150.f, 89.f, 89.f, 0, L"Darmowy parking"))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Rotterdam", "Benelux", 440, { 35,180,500,1400,1750,2100 }, 300, sf::Color::Green, 207.f, 89.f, 180))));
	fields.push_back(std::unique_ptr<field>(std::move(new chance(289.f, 89.f, sf::Color::Red))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Bruksela", "Benelux", 440, { 35,180,500,1400,1750,2100 }, 300, sf::Color::Green, 371.f, 89.f, 180))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Amsterdam", "Benelux", 480, { 40,200,600,1500,1850,2200 }, 300, sf::Color::Green, 453.f, 89.f, 180))));
	fields.push_back(std::unique_ptr<field>(std::move(new industry(L"Poci¹gi S", 535.f, 89.f, 'x', 400, 180))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Malmo", "Szwecja", 520, { 45,220,660,1600,1950,2300 }, 300, sf::Color::Cyan, 617.f, 89.f, 180))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Goteborg", "Szwecja", 520, { 45,220,660,1600,1950,2300 }, 300, sf::Color::Cyan, 699.f, 89.f, 180))));
	fields.push_back(std::unique_ptr<field>(std::move(new industry(L"Wodaci¹gi", 781.f, 89.f, 'W', 300, 180))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Sztokholm", "Szwecja", 560, { 50,240,720,1700,2050,2400 }, 300, sf::Color::Cyan, 863.f, 89.f, 180))));
	fields.push_back(std::unique_ptr<field>(std::move(new fastTravell(981.f, 89.f))));
	//prawy rzad
	fields.push_back(std::unique_ptr<field>(std::move(new city("Frankfurt", "RFN", 600, { 55,260,780,1900,2200,2550 }, 400, sf::Color(88, 88, 88), 981.f, 207.f, -90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Kolonia", "RFN", 600, { 55,260,780,1900,2200,2550 }, 400, sf::Color(88, 88, 88), 981.f, 289.f, -90))));
	fields.push_back(std::unique_ptr<field>(std::move(new chance(981.f, 371.f, sf::Color::Blue, -90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Bonn", "RFN", 640, { 60,300,900,2000,2400,2800 }, 400, sf::Color(88, 88, 88), 981.f, 453.f, -90))));
	fields.push_back(std::unique_ptr<field>(std::move(new industry(L"Poci¹gi W", 981.f, 535.f, 'x', 400, -90))));
	fields.push_back(std::unique_ptr<field>(std::move(new chance(981.f, 617.f, sf::Color::Red, -90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city("Innsbruck", "Austria", 700, { 70,350,1000,2200,2600,3000 }, 400, sf::Color(197, 38, 255), 981.f, 699.f, -90))));
	fields.push_back(std::unique_ptr<field>(std::move(new special(80.f, 981.f, 781.f, -200, "PCC", -90))));
	fields.push_back(std::unique_ptr<field>(std::move(new city(L"Wiedeñ", "Austria", 800, { 100,400,1200,2800,3400,4000 }, 400, sf::Color(197, 38, 255), 981.f, 863.f, -90))));
}