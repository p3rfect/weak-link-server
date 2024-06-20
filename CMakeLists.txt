cmake_minimum_required(VERSION 3.20)
project(WeakLink)

set(CMAKE_CXX_STANDARD 23)

add_executable(WeakLink main.cpp Networking/WebsocketServer.cpp Networking/WebsocketServer.h Entities/User.cpp Entities/User.h Data/User/IUserRepository.h Data/User/PostgresUserRepository.cpp Data/User/PostgresUserRepository.h Entities/Game.cpp Entities/Game.h Services/UserService.cpp Services/UserService.h Data/Game/IGameRepository.h Data/Game/InMemoryGameRepository.cpp Data/Game/InMemoryGameRepository.h Services/GameService.cpp Services/GameService.h Adapters/JsonUserAdapter.cpp Adapters/JsonUserAdapter.h Adapters/JsonGameAdapter.cpp Adapters/JsonGameAdapter.h Services/ServerManager.cpp Services/ServerManager.h Services/ImageService.cpp Services/ImageService.h)
find_package(libpqxx REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(jsoncpp REQUIRED)
target_link_libraries(WeakLink PRIVATE pqxx ${PostgreSQL_LIBRARIES} jsoncpp)