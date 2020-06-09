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
