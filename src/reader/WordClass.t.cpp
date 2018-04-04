#include "gtest/gtest.h"
#include "WordClass.h"
#include <set>
#include <sstream>

using namespace std;

// основные функции класса
TEST (WordClassTest, BasicMethods)
{
    // test 1
    WordClass testWord1("word", "definition");
    EXPECT_EQ ( "word", testWord1.getWord() );
    EXPECT_EQ ( "definition", testWord1.getDefinition() );

    // test 2
    WordClass testWord2("not word but phrase", "here is very long definition");
    EXPECT_EQ ( "not word but phrase", testWord2.getWord() );
    EXPECT_EQ ( "here is very long definition", testWord2.getDefinition() );
}

// проверка вывода (запись в поток)
TEST (WordClassTest, CheckOutput)
{
    // test 1
    WordClass testWord1("word", "definition");
    ostringstream out;
    out << testWord1;
    EXPECT_EQ ("word\tdefinition\n", out.str());

    // очистка потока
    out.str("");
    out.clear();

    // test 2
    WordClass testWord2("word", "long definition with many spaces");
    out << testWord2;
    EXPECT_EQ ("word\tlong definition with many spaces\n", out.str());

    // очистка потока
    out.str("");
    out.clear();

    // test 3
    WordClass testWord3("not word but phrase", "Long definition. With spaces... and other stuff!");
    out << testWord3;
    EXPECT_EQ ("not word but phrase\tLong definition. With spaces... and other stuff!\n", out.str());
}

// проверка ввода (чтение из потока)
TEST (WordClassTest, CheckInput)
{
    // test 1
    WordClass testWord1;
    istringstream in("word\tdefinition");
    in >> testWord1;
    EXPECT_EQ ( "word", testWord1.getWord() );
    EXPECT_EQ ( "definition", testWord1.getDefinition() );

    // очистка потока
    in.str("");
    in.clear();

    // test 2
    WordClass testWord2;
    in.str("not word but phrase\tlong definition with many spaces");
    in >> testWord2;
    EXPECT_EQ ( "not word but phrase", testWord2.getWord() );
    EXPECT_EQ ( "long definition with many spaces", testWord2.getDefinition() );

    // очистка потока
    in.str("");
    in.clear();

    // test 3
    WordClass testWord3;
    in.str("phrase, with - punctuation\tlong definition? with, punctuation!");
    in >> testWord3;
    EXPECT_EQ ( "phrase, with - punctuation", testWord3.getWord() );
    EXPECT_EQ ( "long definition? with, punctuation!", testWord3.getDefinition() );

    // очистка потока
    in.str("");
    in.clear();

    // test 4
    WordClass testWord4;
    in.str("word\tdefinition with new line\n");
    in >> testWord4;
    EXPECT_EQ ( "word", testWord4.getWord() );
    EXPECT_EQ ( "definition with new line", testWord4.getDefinition() );
}
