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

  T &at(std::size_t i, std::size_t j) { return _data.at(i).at(j); }

  const T &at(std::size_t i, std::size_t j) const { return _data.at(i).at(j); }

  // přičtení matice stejného rozměru
  matrix2<T> &operator+=(const matrix2<T> &rhs) {
    if (m() != rhs.m()) { throw std::range_error("Row size must be the same"); }
    if (n() != rhs.n()) { throw std::range_error("Column size must be the same"); }

    for (std::size_t i = 0; i < m(); i++) {
      for (std::size_t j = 0; j < n(); j++) {
        at(i, j) += rhs.at(i, j);
      }
    }

    return *this;
  }

  // přinásobení matice odpovídajících rozměrů (může změnit velikost matice)
  matrix2<T> &operator*=(const matrix2<T> &m);

  std::size_t m() const { return _m; }
  std::size_t n() const { return _n; }
  std::size_t size() const { return _m * _n; }

 private:
  std::vector<std::vector<T>> _data;
  int _m, _n;
};

template <typename T>
matrix2<T>::matrix2()
    : _m(0), _n(0) {}

template <typename T>
matrix2<T>::matrix2(std::size_t m, std::size_t n)
    : _data(m, std::vector<T>(n)), _m(m), _n(n) {}

// operátor vstupu (viz níže formát souboru matice)
template <typename T>

std::istream &operator>>(std::istream &str, matrix2<T> &m);

// operátor výstupu (viz níže formát souboru matice)
template <typename T>
std::ostream &operator<<(std::ostream &str, const matrix2<T> &m);

// operátory porovnání (najprve porovnat velikosti, pokud jsou stejné, pak
// obsah)
template <typename T>
bool operator==(const matrix2<T> &lhs, const matrix2<T> &rhs) {
  if (lhs.m() != rhs.m() || rhs.n() != rhs.n()) return false;

  for (std::size_t i = 0; i < lhs.m(); i++) {
    for (std::size_t j = 0; j < lhs.n(); j++) {
      if (lhs.at(i, j) != rhs.at(i, j)) {
        return false;
      }
    }
  }

  return true;
}

template <typename T>
bool operator!=(const matrix2<T> &l, const matrix2<T> &r) {
  return !(l == r);
}

// binární aritmetické operátory
template <typename T>
matrix2<T> operator+(const matrix2<T> &lhs, const matrix2<T> &rhs) {
  if (lhs.m() != rhs.m()) { throw std::range_error("Row size must be the same"); }
  if (lhs.n() != rhs.n()) { throw std::range_error("Column size must be the same"); }

  matrix2<T> result(lhs.m(), lhs.n());

  for (std::size_t i = 0; i < lhs.m(); i++) {
    for (std::size_t j = 0; j < lhs.n(); j++) {
      result.at(i, j) = lhs.at(i, j) + rhs.at(i, j);
    }
  }

  return result;
}
template <typename T>
matrix2<T> operator*(const matrix2<T> &l, const matrix2<T> &r);

int main() {
  {
    matrix2<int> m2(2, 3);

    m2.at(0, 0) = 5;
    assert(m2.at(0, 0) == 5);

    bool thrown = false;

    try {
      m2.at(2, 3);
    } catch (const std::out_of_range &e) {
      thrown = true;
    }

    assert(thrown);

    assert(m2.m() == 2);
    assert(m2.n() == 3);
  }

  {
    matrix2<int> m1(1, 1);
    m1.at(0, 0) = 3;
    matrix2<int> m2(1, 1);
    m2.at(0, 0) = 4;

    matrix2<int> m3 = m1 + m2;
    assert(m3.at(0, 0) == 7);
  }

  {
    matrix2<int> m1(1, 1);
    m1.at(0, 0) = 3;
    matrix2<int> m2(1, 1);
    m2.at(0, 0) = 4;

    m1 += m2;

    assert(m1.at(0, 0) == 7);
    assert(m2.at(0, 0) == 4);
  }

  {
    matrix2<int> m1(1, 1);
    m1.at(0, 0) = 3;

    matrix2<int> m2(1, 1);
    m2.at(0, 0) = 3;

    matrix2<int> m3(1, 2);

    assert(m1 == m2);

    m2.at(0, 0) = 4;
    assert(!(m1 == m2));

    assert(!(m1 == m3));
    assert(!(m2 == m3));

    assert(m1 != m3);
    assert(m2 != m3);
  }

  return 0;
}
