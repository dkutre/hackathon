Тестовое задание для хакатона.

Веб сервер отдающий файлы из текущего репозитория. 
Путь запроса должен начинаться с /get/. Ex: /get/file_name

Установка:

    git clone https://github.com/dkutre/hackathon.git
    ./build.sh
    
Запуск:

    cd bin
    ./hackathon

Используется:

    Boost.Asio, Filesystem;
    nodejs-http-parser;
    C++1x;
    Cmake
    




