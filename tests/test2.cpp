//
// Created by Евгений Григорьев on 20.04.2020.
//
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
