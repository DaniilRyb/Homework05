## Homework V

### Задание

```sh
% git remote remove origin
% hub create

% git push -u origin master
```

```sh
% cat >> CMakeLists.txt <<EOF
cmake_minimum_required(VERSION 3.10)
project(banking)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(account STATIC banking/Account.cpp)
target_include_directories(account
 PUBLIC \${CMAKE_CURRENT_SOURCE_DIR}/banking)

add_library(transaction STATIC banking/Transaction.cpp)
target_include_directories(account
 PUBLIC \${CMAKE_CURRENT_SOURCE_DIR}/banking)
 target_link_libraries(transaction account)
EOF
```

```sh
% mkdir third-party

% git submodule add https://github.com/google/googletest third-party/gtest

% cd third-party/gtest && git checkout release-1.10.0 && cd ../..
% git add third-party/gtest
```

```sh

% sed -i "" '/set(CMAKE_CXX_STANDARD_REQUIRED ON)/a\
option(BUILD_TESTS "Build tests" OFF)
' CMakeLists.txt

% cat >> CMakeLists.txt <<EOF

if(BUILD_TESTS)
  # Включить поддержку тестирования:
  enable_testing()
  add_subdirectory(third-party/gtest)
  # Создание списка файлов, соответствующих выражению и сохранение его в переменную
  file(GLOB \${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
  add_executable(check \${\${PROJECT_NAME}_TEST_SOURCES})
  target_link_libraries(check account transaction gtest_main gmock_main)
  # Добавление тестов к проекту
  add_test(NAME check COMMAND check)
endif()
EOF
```

```sh
% cat >> tests/test1.cpp <<EOF
#include <Account.h>
#include <gtest/gtest.h>
// Тест на проверку правильности конструктора
TEST(Account, Constructor)
{
Account a(2,300);

EXPECT_EQ(a.id(),2);
EXPECT_EQ(a.GetBalance(),300);
}
// Тест на проверку правильность изменения баланса
TEST(Account, ChangeBalance)
{
  Account a(2,300);
  a.Lock();
  a.ChangeBalance(100);
  EXPECT_EQ(a.GetBalance(),400);
}
// Тест на проверку состояния аккаунта(открыт/закрыт)
TEST(Account, Lock)
{
  Account a(2,300);
  a.Lock();
  a.ChangeBalance(100);
  a.Unlock();
  EXPECT_EQ(a.GetBalance(),400);
}
EOF
```

```sh
% cat >> tests/test2.cpp <<EOF

#include <Account.h>
#include <Transaction.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
// Создание mock-класса
class MockAccount : public Account {
public:
  MockAccount(){};
  MOCK_METHOD(int, GetBalance, (), (const, override));
  MOCK_METHOD(int, id, (), (const));
};
// Пример использования mock-объектов
TEST(Transaction, MakeTransaction) {

  MockAccount from;
  MockAccount to;
  Transaction transaction1;

  // Установка поведения
  EXPECT_CALL(from, id()).WillOnce(testing::Return(1));
  EXPECT_CALL(from, GetBalance()).WillOnce(testing::Return(1000));
  EXPECT_CALL(to, id()).WillOnce(testing::Return(2));
  EXPECT_CALL(to, GetBalance()).WillOnce(testing::Return(100));
  EXPECT_TRUE(transaction1.Make(Account(from.id(), from.GetBalance()),
                                Account(to.id(), to.GetBalance()), 500));
}
// Проценты за перевод
TEST(Transaction, Fee){
  Transaction transaction1;
  EXPECT_EQ(transaction1.fee(),1);
  transaction1.set_fee(3);
  EXPECT_EQ(transaction1.fee(),3);
  EXPECT_TRUE(transaction1.Make(Account(3, 1000),
                                Account(4,100), 500));
}
EOF
```

```sh

$ cmake -H. -B_build -DBUILD_TESTS=ON

$ cmake --build _build
Scanning dependencies of target account
[  6%] Building CXX object CMakeFiles/account.dir/banking/Account.cpp.o
...
[100%] Linking CXX executable check

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.30 sec
```

```sh
$ _build/check

# Запуск тестов с подробным выводом информации
$ cmake --build _build --target test -- ARGS=--verbose
Running tests...
...
100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.01 sec
```

```sh
% cat > .travis.yml <<EOF
language: cpp
os:
  - windows
jobs:
  include:
  - name: "Link an test"
    script:
    - cmake -H. -B_build -DBUILD_TESTS=ON
    - cmake --build _build
    - cmake --build _build --target test
    - _build/check
    - cmake --build _build --target test -- ARGS=--verbose
addons:
  apt:
    sources:
      - george-edison55-precise-backports
    packages:
      - cmake
      - cmake-data
EOF
```

```sh
% travis lint
Hooray, .travis.yml looks valid :)
% travis login --auto

% travis enable
Detected repository as Evgengrmit/hw05, is this correct? |yes| y
Evgengrmit/hw05: enabled :)
```
4. Настройте [Coveralls.io](https://coveralls.io/).
Обновление `CMakeLists.txt`
```sh
% sed -i "" '/add_executable(check ${${PROJECT_NAME}_TEST_SOURCES})/a\
target_compile_options(check PRIVATE --coverage)
target_link_libraries(check PRIVATE account transaction gtest_main gmock_main  --coverage)
' CMakeLists.txt
```
Перепишем сборочную процедуру на **TravisCI**.
```sh
% cat > .travis.yml <<EOF
language: cpp
os:
  - osx
jobs:
  include:
  - name: "Link an test"
    script:
    - cmake -H. -B_build -DBUILD_TESTS=ON
    - cmake --build _build
    - cmake --build _build --target test
    - _build/check
    - cmake --build _build --target test -- ARGS=--verbose
  - name: "Coveralls.io"
    before_install:
    - pyenv rehash
    - pip install cpp-coveralls
    - pyenv rehash
    script:
    - cmake -H. -B_build -DBUILD_TESTS=ON
    - cmake --build _build
    - ./_build/check
    after_success:
    - coveralls --root . -E ".*gtest.*" -E ".*CMakeFiles.*"

addons:
  apt:
    packages:
      - cmake
      - cmake-data

EOF
```
Проверка `.travis.yml` на ошибки
```sh
% travis lint
Hooray, .travis.yml looks valid :)
```
`add`, `commit`, `push`
```sh
% git add .
% git commit -m "Coveralls"
[master bd9e4cd] Coveralls
 2 files changed, 48 insertions(+), 3 deletions(-)
% git push origin master   
Enumerating objects: 7, done.
Counting objects: 100% (7/7), done.
Delta compression using up to 12 threads
Compressing objects: 100% (4/4), done.
Writing objects: 100% (4/4), 791 bytes | 791.00 KiB/s, done.
Total 4 (delta 3), reused 0 (delta 0)
remote: Resolving deltas: 100% (3/3), completed with 3 local objects.
To https://github.com/Evgengrmit/hw05.git
  9de08f9..bd9e4cd  master -> master
```
## Links

- [C++ CI: Travis, CMake, GTest, Coveralls & Appveyor](http://david-grs.github.io/cpp-clang-travis-cmake-gtest-coveralls-appveyor/)
- [Boost.Tests](http://www.boost.org/doc/libs/1_63_0/libs/test/doc/html/)
- [Catch](https://github.com/catchorg/Catch2)

```
Copyright (c) 2015-2020 The ISC Authors
```
