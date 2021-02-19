Казантаев Алексей 213

На сайте labs.play-with-docker.com после входа в аккаунт нужно нажать на иконку гаечного ключа рядом с шестеренкой, там выбрать template 1 Worker - 1 Manager

Клиент-серверное приложение
_________________________________________________

Для запуска работы сервера (в manager) :
1) docker pull -a alexeykaz/asvk
2) docker run -p 8000:8000 -it [Image ID of the server] /bin/bash
3) python3 server.py
_________________________________________________

Для запуска работы клиента (в worker) :
1) docker pull -a alexeykaz/asvk
2) docker run -p 8000:8000 -it [Image ID of the client] /bin/bash
3) git clone https://github.com/alexey-kaz/asvk-xml.git 
4) cd asvk-xml
5) cp *.xml /client
6) cd ..
7) python3 client.py [Manager instance IP address]

// 3-6 команды отвечают за то, чтобы в контейнере с client.py оказались тесты для отправки серверу. В моем случае я копировал свой GitHub репозиторий с ними, потом переместил их из созданной директории asvk-xml в ту, где находится client.py.

// Важно, чтобы сервер был запущен первым, иначе клиент выдает ошибку Connection refused

// ID образов клиента и сервера можно найти командой docker images

// Квадратные скобки в командах не вводить