#include <cassert>
#include <iterator>
#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include <sstream>

template <typename T>
class matrix2;

// Iterator pro jednotlive radky v prubehu iterovani pres sloupce.
// TODO - prejmenovat na column
template <typename T>
class matrix_column {
  matrix2<T>& _matrix;
  std::size_t _row = 0;
  std::size_t _col;

 public:
  using iterator = matrix_column<T>;
  using iterator_category = std::random_access_iterator_tag;

  using value_type = T;
  using reference = T&;
  using pointer = T*;
  using difference_type = std::ptrdiff_t;

  matrix_column(matrix2<T>& m, std::size_t col): _matrix(m), _col(col) {}

  reference operator*() { return _matrix.at(_row, _col); }

  iterator& operator++() { ++_row; return *this; }
  iterator operator++(int) { iterator tmp = *this; ++_row; return tmp; }

  iterator& operator--() { --_row; return *this; }
  iterator operator--(int) { iterator tmp = *this; --_row; return tmp; }

  bool operator==(const iterator& rhs) { return _matrix == rhs._matrix && _col == rhs._col && _row == rhs._row; }
  bool operator!=(const iterator& rhs) { return (*this == rhs); }

  iterator& operator+=(int n) { _row += n; return *this; }
  iterator& operator-=(int n) { _row -= n; return *this; }

  iterator operator+(int n) { auto tmp = *this; return tmp += n; }
  iterator operator-(int n) { auto tmp = *this; return tmp -= n; }

  difference_type operator-(const iterator& rhs) { return _row - rhs._row; }

  reference operator[](std::size_t n) { return *(*this + n); }

  bool operator<(const iterator& rhs) { return _row < rhs._row; }
  bool operator>(const iterator& rhs) { return _row > rhs._row; }

  bool operator<=(const iterator& rhs) { return _row <= rhs._row; }
  bool operator>=(const iterator& rhs) { return _row >= rhs._row; }

  iterator begin() { return *this; }
  iterator end() { return *this + _matrix.n(); }
};

// Iterator pro sloupce matice
template <typename T>
class matrix_column_iterator {
 private:
  matrix2<T>& _matrix;
  std::size_t _col = 0;

 public:
  using iterator = matrix_column_iterator<T>;
  using iterator_category = std::random_access_iterator_tag;

  using value_type = matrix_column<T>;
  using reference = value_type&;
  using pointer = value_type*;
  using difference_type = std::ptrdiff_t;

  matrix_column_iterator(matrix2<T>& m): _matrix(m) {}

  value_type operator*() { return matrix_column<T>(_matrix, _col); }

  iterator& operator++() { ++_col; return *this; }
  iterator operator++(int) { iterator tmp = *this; ++_col; return tmp; }

  iterator& operator--() { --_col; return *this; }
  iterator operator--(int) { iterator tmp = *this; --_col; return tmp; }

  bool operator==(const iterator& rhs) { return _matrix == rhs._matrix || _col == rhs._col; }
  bool operator!=(const iterator& rhs) { return !(*this == rhs); }

  iterator& operator+=(int n) { _col += n; return *this; }
  iterator& operator-=(int n) { _col -= n; return *this; }

  iterator operator+(int n) { auto tmp = *this; return tmp += n; }
  iterator operator-(int n) { auto tmp = *this; return tmp -= n; }

  difference_type operator-(const iterator& rhs) { return _col - rhs._col; }

  value_type operator[](std::size_t n) { return *(*this + n); }

  bool operator<(const iterator& rhs) { return _col < rhs._col; }
  bool operator>(const iterator& rhs) { return _col > rhs._col; }

  bool operator<=(const iterator& rhs) { return _col <= rhs._col; }
  bool operator>=(const iterator& rhs) { return _col >= rhs._col; }
};

// Muzeme pouzit pro "sezrani" znaku ze streamu. Pokud tam ocekavany znak neni,
// nastavi se failbit. rvalue referenci bereme proto, aby slo pouzit jen char literal.
std::istream& operator>>(std::istream& is, const char&& c) {
  char x; is >> x;
  if (x != c) is.setstate(std::ios::failbit);
  return is;
}

template <typename T>
class matrix2 {

 private:
  std::vector<std::vector<T>> _data;
  int _m, _n;

 public:
  // zkonstruuje prázdnou matici o rozměrech 0*0
  matrix2();

  // zkonstruuje 2D-matici o rozměrech M*N
  matrix2(std::size_t m, std::size_t n);

  // splnění požadavků na kontejner - základní iterátory iterují přes všechny
  // prvky matice, nejprve po řádcích, pak po sloupcích v řádce, size() vrací
  // počet prvků v matici

  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using size_tag = std::size_t;

  using row_value_type = std::vector<T>;
  using difference_type = typename std::vector<row_value_type>::difference_type;

  using row_iterator = typename std::vector<row_value_type>::iterator;
  using const_row_iterator = typename std::vector<row_value_type>::const_iterator;

  // splnění požadavků na iterativní kontejner řádků - typy i funkce budou mít
  // prefix row_ (např. row_value_type, row_cbegin), row_size vrací M
  row_iterator row_begin() { return begin(_data); }
  const_row_iterator row_begin() const { return begin(_data); }
  const_row_iterator row_cbegin() const { return begin(_data); }

  row_iterator row_end() { return end(_data); }
  const_row_iterator row_end() const { return end(_data); }
  const_row_iterator row_cend() const { return end(_data); }

  inline std::size_t row_size() const { return m(); }

  // splnění požadavků na iterativní kontejner sloupců - typy i funkce budou mít
  // prefix column_ (např. column_value_type, column_cbegin), column_size vrací N
  // using column_value_type = row_iterator<T>;
  using column_iterator = matrix_column_iterator<T>;
  using const_column_iterator = const matrix_column_iterator<T>;

  column_iterator column_begin() { return matrix_column_iterator<T>(*this); }
  const_column_iterator column_begin() const { return matrix_column_iterator<T>(*this); }
  const_column_iterator column_cbegin() const { return matrix_column_iterator<T>(*this); }

  column_iterator column_end() { return matrix_column_iterator<T>(*this) + n(); }
  const_column_iterator column_end() const { return matrix_column_iterator<T>(*this) + n(); }
  const_column_iterator column_cend() const { return matrix_column_iterator<T>(*this) + n(); }

  inline std::size_t column_size() const { return m(); }

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

  inline std::size_t m() const { return _m; }
  inline std::size_t n() const { return _n; }
  inline std::size_t size() const { return _m * _n; }
  inline std::size_t max_size() const { return size(); }
  inline bool empty() const { return _m == 0 && _n == 0; }

  void swap(matrix2<T> b) {
    std::swap(_data, b._data);
    std::swap(_m, b._m);
    std::swap(_n, b._data);
  }

  void resize(int m, int n) {
    _m = m;
    _n = n;
    _data.resize(m, std::vector<T>(n));
  }
};

template <typename T>
matrix2<T>::matrix2()
    : _m(0), _n(0) {}

template <typename T>
matrix2<T>::matrix2(std::size_t m, std::size_t n)
    : _data(m, std::vector<T>(n)), _m(m), _n(n) {}

// operátor vstupu (viz níže formát souboru matice)
template <typename T>
std::istream &operator>>(std::istream &is, matrix2<T> &m) {
  int M, N;
  is >> '{' >> M >> N;

  m.resize(M, N);

  for (int i = 0; i < M; i++) {
    is >> '{';

    for (int j = 0; j < N; j++) {
      is >> m.at(i, j);
      if (j < N - 1) is >> ',';
    }

    is >> '}';
  }

  is >> '}';

  return is;
}

// operátor výstupu (viz níže formát souboru matice)
template <typename T>
std::ostream &operator<<(std::ostream &os, const matrix2<T> &m) {
  os << "{" << m.m() << " " << m.n() << std::endl;

  for (auto it = m.row_begin(); it != m.row_end(); it++) {
    os << "{";
    for (auto i : *it) {
      os << i << ",";
    }
    os << "}" << std::endl;
  }

  os << "}";

  return os;
}

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
matrix2<T> operator*(matrix2<T> &lhs, matrix2<T> &rhs) {
  if (lhs.n() != rhs.m()) {
    std::stringstream ss;
    ss << "Invalid matrix size for matrix multiplication. ";
    ss << "lhs.n: " << lhs.n();
    ss << " != rhs.m: " << rhs.m();;

    throw std::range_error(ss.str());
  }

  matrix2<T> result(lhs.m(), rhs.n());

  typename matrix2<T>::const_row_iterator lit = lhs.row_begin();
  typename matrix2<T>::column_iterator rit = rhs.column_begin();

  for (auto i = result.row_begin(); i != result.row_end(); i++) {
    typename matrix2<T>::row_iterator::value_type result_row = *i;

    for (auto j = result_row.begin(); j != result_row.end(); j++) {

      std::size_t k = lhs.n();
      // Explicitni typy pro kontrolu, aby bylo jasne s cim se kde iteruje
      std::vector<T> left = *lit;
      matrix_column<T> right = *rit;

      typename std::vector<T>::iterator x = left.begin();
      typename matrix_column<T>::iterator y = right.begin();

      T value{};
      while (k--) {
        std::cout << *x << " * " << *y << std::endl;
        value += (*x) * (*y);
        x++;
        y++;
      }

      T& cell = *j;
      cell = value;

      std::cout << "\t" << value << std::endl;

      rit++;
    }

    lit++;
  }

  return result;
}

int main() {
  {
    std::stringstream ss("hello");

    ss >> 'h';
    assert(ss.good());

    ss >> 'a';
    assert(!ss.good());
  }

  {
    matrix2<int> m{};
    std::stringstream ss("{ 2 3\n{ 1, 2, 3 }\n{ 4, 5, 6 }\n}");
    ss >> m;

    assert(m.m() == 2);
    assert(m.n() == 3);

    assert(m.at(0, 0) == 1);
    assert(m.at(0, 1) == 2);
    assert(m.at(0, 2) == 3);
    assert(m.at(1, 0) == 4);
    assert(m.at(1, 1) == 5);
    assert(m.at(1, 2) == 6);
  }

  {
    matrix2<int> m{};
    std::stringstream is("{ 2 3\n{ 1, 2, 3 }\n{ 4, 5, 6 }\n}");
    is >> m;

    std::stringstream os;
    os << m;
    assert(os.str() == "{2 3\n{1,2,3,}\n{4,5,6,}\n}");
    // std::cout << os.str() << std::endl;
  }

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

  {
    // Iterator test
    matrix2<int> m{};
    std::stringstream ss("{ 2 3\n{ 1, 2, 3 }\n{ 4, 5, 6 }\n}");
    ss >> m;

    auto it = m.column_begin();

    auto c1 = *it++;
    assert(*c1++ == 1);
    assert(*c1++ == 4);

    auto c2 = *it++;
    assert(*c2++ == 2);
    assert(*c2++ == 5);

    auto c3 = *it++;
    assert(*c3++ == 3);
    assert(*c3++ == 6);

    c3--; c3--;
    assert(*c3 == 3);

    auto c4 = *(it - 2);
    auto c5 = *(it - 2);

    assert(c4 == c5);

    auto c6 = m.column_begin();
    c6 += 1;
    auto c7 = m.column_begin();
    c7++;

    assert(c6 == c7);

    c6--;
    c6 += 2;

    assert(c6 > c7);
    assert(c6 >= c7);

    assert(c7 < c6);
    assert(c7 <= c6);

    c7++;

    assert(c6 >= c7);
    assert(c6 == c7);
    assert(c7 <= c6);
  }

  {
    // Iterator test
    matrix2<int> m{};
    std::stringstream ss("{ 2 3\n{ 1, 2, 3 }\n{ 4, 5, 6 }\n}");
    ss >> m;

    auto it = m.column_begin();
    assert((it + 3) == m.column_end());
  }

  {
    // Nasobeni matic
    matrix2<int> m1{};
    std::stringstream ss1("{ 1 3\n{ 1, 4, 6 }}");
    ss1 >> m1;

    matrix2<int> m2{};
    std::stringstream ss2("{ 3 2\n{ 2, 3}\n {5, 8}\n{7, 9}\n}");
    ss2 >> m2;

    auto m3 = m1 * m2;
    std::cout << m3 << std::endl;
    assert(m3.at(0, 0) == 64);
    assert(m3.at(0, 1) == 89);
  }

  return 0;
}
