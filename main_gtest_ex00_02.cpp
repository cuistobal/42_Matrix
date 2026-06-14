#include <gtest/gtest.h>

#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <vector>

#include "ft_matrix.hpp"

// Exercise 01
template <vector::Numeric T, vector::Numeric U>
auto linear_combination(const std::vector<Vector<T>>& candidates,
                        const std::vector<U>& scalars) {
  using R = std::common_type_t<T, U>;

  if (candidates.size() != scalars.size()) {
    throw std::invalid_argument("Scalars and candidate sizes mismatch");
  }
  if (candidates.empty()) {
    throw std::invalid_argument("Empty linear combination");
  }

  const std::size_t dimensions = candidates.front().get_dimensions();
  std::vector<R> data(dimensions, R{});

  for (std::size_t i = 0; i < candidates.size(); ++i) {
    if (candidates[i].get_dimensions() != dimensions) {
      throw std::invalid_argument("Candidate size mismatch");
    }
    const auto& current = candidates[i].get_data();
    for (std::size_t j = 0; j < dimensions; ++j) {
      if constexpr (std::floating_point<R>) {
        data[j] = std::fma(static_cast<R>(current[j]), static_cast<R>(scalars[i]), data[j]);
      } else {
        data[j] += static_cast<R>(current[j]) * static_cast<R>(scalars[i]);
      }
    }
  }
  return Vector<R>(data);
}

// Exercise 02
template <typename U, typename V>
auto lerp(const U& u, const V& v, float t) {
  return u + ((v - u) * t);
}

template <typename T>
static void expect_vector_data(const Vector<T>& actual,
                               std::initializer_list<double> expected,
                               double eps = 1e-6) {
  ASSERT_EQ(actual.get_dimensions(), expected.size());
  const auto& data = actual.get_data();
  std::size_t i = 0;
  for (double value : expected) {
    if constexpr (std::floating_point<T>) {
      EXPECT_NEAR(static_cast<double>(data[i]), value, eps);
    } else {
      EXPECT_EQ(static_cast<double>(data[i]), value);
    }
    ++i;
  }
}

template <typename T>
static void expect_matrix_data(const Matrix<T>& actual,
                               std::size_t rows,
                               std::size_t cols,
                               std::initializer_list<double> expected,
                               double eps = 1e-6) {
  ASSERT_EQ(actual.get_rows(), rows);
  ASSERT_EQ(actual.get_cols(), cols);
  ASSERT_EQ(actual.get_data().size(), expected.size());
  const auto& data = actual.get_data();
  std::size_t i = 0;
  for (double value : expected) {
    if constexpr (std::floating_point<T>) {
      EXPECT_NEAR(static_cast<double>(data[i]), value, eps);
    } else {
      EXPECT_EQ(static_cast<double>(data[i]), value);
    }
    ++i;
  }
}

template <typename T>
class Exercise00VectorTypedTest : public ::testing::Test {};
using SupportedTypes = ::testing::Types<signed char, unsigned char, short, unsigned short,
                                        int, unsigned int, long, unsigned long,
                                        long long, unsigned long long, float, double, long double>;
TYPED_TEST_SUITE(Exercise00VectorTypedTest, SupportedTypes);

TYPED_TEST(Exercise00VectorTypedTest, AddsSubtractsAndScalesVectors) {
  using T = TypeParam;
  Vector<T> u{static_cast<T>(2), static_cast<T>(3)};
  Vector<T> v{static_cast<T>(5), static_cast<T>(7)};

  expect_vector_data(u + v, {7, 10});
  expect_vector_data(v - u, {3, 4});
  expect_vector_data(u * static_cast<T>(2), {4, 6});

  u += v;
  expect_vector_data(u, {7, 10});
  u -= v;
  expect_vector_data(u, {2, 3});
  u *= static_cast<T>(3);
  expect_vector_data(u, {6, 9});
}

TYPED_TEST(Exercise00VectorTypedTest, RejectsVectorShapeMismatch) {
  using T = TypeParam;
  Vector<T> u{static_cast<T>(1), static_cast<T>(2)};
  Vector<T> v{static_cast<T>(1), static_cast<T>(2), static_cast<T>(3)};

  EXPECT_THROW((void)(u + v), std::invalid_argument);
  EXPECT_THROW((void)(u - v), std::invalid_argument);
  EXPECT_THROW(u += v, std::invalid_argument);
  EXPECT_THROW(u -= v, std::invalid_argument);
}

template <typename T>
class Exercise00MatrixTypedTest : public ::testing::Test {};
TYPED_TEST_SUITE(Exercise00MatrixTypedTest, SupportedTypes);

TYPED_TEST(Exercise00MatrixTypedTest, AddsSubtractsAndScalesMatrices) {
  using T = TypeParam;
  Matrix<T> u{{static_cast<T>(1), static_cast<T>(2)}, {static_cast<T>(3), static_cast<T>(4)}};
  Matrix<T> v{{static_cast<T>(7), static_cast<T>(4)}, {static_cast<T>(2), static_cast<T>(2)}};

  expect_matrix_data(u + v, 2, 2, {8, 6, 5, 6});
  expect_matrix_data(v - u, 2, 2, {6, 2, -1, -2});
  expect_matrix_data(u * static_cast<T>(2), 2, 2, {2, 4, 6, 8});

  u += v;
  expect_matrix_data(u, 2, 2, {8, 6, 5, 6});
  u -= v;
  expect_matrix_data(u, 2, 2, {1, 2, 3, 4});
  u *= static_cast<T>(3);
  expect_matrix_data(u, 2, 2, {3, 6, 9, 12});
}

TYPED_TEST(Exercise00MatrixTypedTest, RejectsMatrixShapeMismatch) {
  using T = TypeParam;
  Matrix<T> u{{static_cast<T>(1), static_cast<T>(2)}, {static_cast<T>(3), static_cast<T>(4)}};
  Matrix<T> v{{static_cast<T>(1), static_cast<T>(2), static_cast<T>(3)}};

  EXPECT_THROW((void)(u + v), std::invalid_argument);
  EXPECT_THROW((void)(u - v), std::invalid_argument);
  EXPECT_THROW(u += v, std::invalid_argument);
  EXPECT_THROW(u -= v, std::invalid_argument);
}

TEST(Exercise00TypeInteractions, VectorOperationsPromoteMixedNumericTypes) {
  Vector<int> a{1, 2, 3};
  Vector<double> b{0.5, 1.5, 2.5};
  Vector<unsigned int> c{1u, 2u, 3u};

  static_assert(std::same_as<decltype(a + b), Vector<double>>);
  static_assert(std::same_as<decltype(c * 2.5), Vector<double>>);

  expect_vector_data(a + b, {1.5, 3.5, 5.5});
  expect_vector_data(b - a, {-0.5, -0.5, -0.5});
  expect_vector_data(c * 2.5, {2.5, 5.0, 7.5});
}

TEST(Exercise00TypeInteractions, MatrixOperationsPromoteMixedNumericTypes) {
  Matrix<int> a{{1, 2}, {3, 4}};
  Matrix<double> b{{0.5, 1.5}, {2.5, 3.5}};
  Matrix<unsigned int> c{{1u, 2u}, {3u, 4u}};

  static_assert(std::same_as<decltype(a + b), Matrix<double>>);
  static_assert(std::same_as<decltype(c * 2.5), Matrix<double>>);

  expect_matrix_data(a + b, 2, 2, {1.5, 3.5, 5.5, 7.5});
  expect_matrix_data(b - a, 2, 2, {-0.5, -0.5, -0.5, -0.5});
  expect_matrix_data(c * 2.5, 2, 2, {2.5, 5.0, 7.5, 10.0});
}

TEST(Exercise01LinearCombination, ComputesSubjectExamples) {
  Vector<float> e1{1.f, 0.f, 0.f};
  Vector<float> e2{0.f, 1.f, 0.f};
  Vector<float> e3{0.f, 0.f, 1.f};
  Vector<float> v1{1.f, 2.f, 3.f};
  Vector<float> v2{0.f, 10.f, -100.f};

  expect_vector_data(linear_combination(std::vector{e1, e2, e3}, std::vector{10.f, -2.f, 0.5f}),
                     {10, -2, 0.5});
  expect_vector_data(linear_combination(std::vector{v1, v2}, std::vector{10.f, -2.f}),
                     {10, 0, 230});
}

TEST(Exercise01LinearCombination, SupportsMixedScalarAndVectorTypes) {
  std::vector<Vector<int>> candidates{Vector<int>{1, 2, 3}, Vector<int>{4, 5, 6}};
  std::vector<double> scalars{0.5, 2.0};

  auto result = linear_combination(candidates, scalars);
  static_assert(std::same_as<decltype(result), Vector<double>>);
  expect_vector_data(result, {8.5, 11.0, 13.5});
}

TEST(Exercise01LinearCombination, RejectsIncoherentInputs) {
  std::vector<Vector<int>> candidates{Vector<int>{1, 2}, Vector<int>{3, 4}};
  std::vector<int> too_few_scalars{1};
  EXPECT_THROW((void)linear_combination(candidates, too_few_scalars), std::invalid_argument);

  std::vector<Vector<int>> mismatched_dimensions{Vector<int>{1, 2}, Vector<int>{3, 4, 5}};
  std::vector<int> scalars{1, 2};
  EXPECT_THROW((void)linear_combination(mismatched_dimensions, scalars), std::invalid_argument);

  std::vector<Vector<int>> empty_candidates{};
  std::vector<int> empty_scalars{};
  EXPECT_THROW((void)linear_combination(empty_candidates, empty_scalars), std::invalid_argument);
}

TEST(Exercise02Lerp, ComputesScalarExamples) {
  EXPECT_FLOAT_EQ(lerp(0.f, 1.f, 0.f), 0.f);
  EXPECT_FLOAT_EQ(lerp(0.f, 1.f, 1.f), 1.f);
  EXPECT_FLOAT_EQ(lerp(0.f, 1.f, 0.5f), 0.5f);
  EXPECT_FLOAT_EQ(lerp(21.f, 42.f, 0.3f), 27.3f);
}

TEST(Exercise02Lerp, ComputesVectorAndMatrixExamples) {
  expect_vector_data(lerp(Vector<float>{2.f, 1.f}, Vector<float>{4.f, 2.f}, 0.3f), {2.6, 1.3});
  expect_matrix_data(lerp(Matrix<float>{{2.f, 1.f}, {3.f, 4.f}},
                          Matrix<float>{{20.f, 10.f}, {30.f, 40.f}},
                          0.5f),
                     2, 2, {11, 5.5, 16.5, 22});
}

TEST(Exercise02Lerp, SupportsMixedTypesAndExtrapolation) {
  auto scalar = lerp(10, 20.0, 0.25f);
  static_assert(std::same_as<decltype(scalar), double>);
  EXPECT_DOUBLE_EQ(scalar, 12.5);

  expect_vector_data(lerp(Vector<int>{0, 10}, Vector<double>{10.0, 20.0}, 1.5f), {15, 25});
  expect_matrix_data(lerp(Matrix<int>{{0, 10}, {20, 30}},
                          Matrix<double>{{10, 20}, {30, 40}},
                          -0.5f),
                     2, 2, {-5, 5, 15, 25});
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
