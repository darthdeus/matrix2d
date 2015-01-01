#include <cassert>
#include <iostream>
#include <vector>

template <typename T>

class matrix2 {
 public:
  // zkonstruuje prázdnou matici o rozměrech 0*0
  matrix2();

  // zkonstruuje 2D-matici o rozměrech M*N
  matrix2(std::size_t m, std::size_t n);

  // splnění požadavků na kontejner - základní iterátory iterují přes všechny
  // prvky matice, nejprve po řádcích, pak po sloupcích v řádce, size() vrací
  // počet prvků v matici

  // splnění požadavků na iterativní kontejner řádků - typy i funkce budou mít
  // prefix row_ (např. row_value_type, row_cbegin), row_size vrací M

  // splnění požadavků na iterativní kontejner sloupců - typy i funkce budou mít
  // prefix column_ (např. column_value_type, column_cbegin), column_size vrací
  // N

  // přímý přístup na jednotlivé prvky - 0-based (rozsahy <0;M-1> a <0;N-1>)

  T &at(std::size_t i, std::size_t j) {
    return _data.at(i).at(j);
  }

  const T &at(std::size_t i, std::size_t j) const {
    return _data.at(i).at(j);
  }

  // přičtení matice stejného rozměru
  matrix2<T> &operator+=(const matrix2<T> &m);

  // přinásobení matice odpovídajících rozměrů (může změnit velikost matice)
  matrix2<T> &operator*=(const matrix2<T> &m);

 private:
  std::vector<std::vector<T>> _data;
};

template <typename T>
matrix2<T>::matrix2() {}

template <typename T>
matrix2<T>::matrix2(std::size_t m, std::size_t n):
  _data(m, std::vector<T>(n)) {

}

// operátor vstupu (viz níže formát souboru matice)
template <typename T>

std::istream &operator>>(std::istream &str, matrix2<T> &m);

// operátor výstupu (viz níže formát souboru matice)
template <typename T>

std::ostream &operator<<(std::ostream &str, const matrix2<T> &m);
// operátory porovnání (najprve porovnat velikosti, pokud jsou stejné, pak
// obsah)

template <typename T>
bool operator==(const matrix2<T> &l, const matrix2<T> &r);

template <typename T>
bool operator!=(const matrix2<T> &l, const matrix2<T> &r);

// binární aritmetické operátory
template <typename T>
matrix2<T> operator+(const matrix2<T> &l, const matrix2<T> &r);

template <typename T>
matrix2<T> operator*(const matrix2<T> &l, const matrix2<T> &r);

int main() {
  matrix2<int> m;
  matrix2<int> m2(5, 5);
  matrix2<int> m3(5, 10);

  m3.at(0,0) = 5;
  assert(m3.at(0, 0) == 5);

  return 0;
}

