#include "ft_matrix.hpp"

#include <array>
#include <cmath>
#include <format>
#include <sstream>
#include <type_traits>

namespace {

using VectorTypes = ::testing::Types<short, int, long, float, double, unsigned int>;
using MatrixTypes = ::testing::Types<short, int, long, float, double, unsigned int>;

template <typename T>
void expect_vector_data_eq(const Vector<T>& vector, const std::vector<T>& expected) {
  EXPECT_EQ(vector.get_dimensions(), expected.size());
  ASSERT_EQ(vector.get_data().size(), expected.size());
  for (size_t index = 0; index < expected.size(); ++index) {
    EXPECT_EQ(vector.get_data()[index], expected[index]);
  }
}

template <typename T>
void expect_matrix_data_eq(const Matrix<T>& matrix, size_t rows, size_t cols, const std::vector<T>& expected) {
  EXPECT_EQ(matrix.get_rows(), rows);
  EXPECT_EQ(matrix.get_cols(), cols);
  ASSERT_EQ(matrix.get_data().size(), expected.size());
  for (size_t index = 0; index < expected.size(); ++index) {
    EXPECT_EQ(matrix.get_data()[index], expected[index]);
  }
}

template <typename T>
void expect_matrix_data_eq(const Matrix<T>& matrix, const std::vector<T>& expected) {
  expect_matrix_data_eq(matrix, matrix.get_rows(), matrix.get_cols(), expected);
}

template <typename T>
class VectorTypedTest : public ::testing::Test {};

template <typename T>
class MatrixTypedTest : public ::testing::Test {};

}  // namespace

TYPED_TEST_SUITE(VectorTypedTest, VectorTypes);
TYPED_TEST_SUITE(MatrixTypedTest, MatrixTypes);

TYPED_TEST(VectorTypedTest, ConstructorsAndAccessors) {
  using T = TypeParam;

  Vector<T> from_list{T{1}, T{2}, T{3}};
  expect_vector_data_eq(from_list, {T{1}, T{2}, T{3}});

  std::array<T, 3> array_data{T{4}, T{5}, T{6}};
  Vector<T> from_range{array_data};
  expect_vector_data_eq(from_range, {T{4}, T{5}, T{6}});

  Vector<double> source{1.25, 2.75, 3.5};
  Vector<T> converted{source};
  expect_vector_data_eq(converted, {static_cast<T>(1.25), static_cast<T>(2.75), static_cast<T>(3.5)});
}

TYPED_TEST(VectorTypedTest, ArithmeticAcrossTypes) {
  using T = TypeParam;
  using R = std::common_type_t<T, int>;

  Vector<T> lhs{T{1}, T{2}, T{3}};
  Vector<int> rhs{4, 5, 6};

  expect_vector_data_eq(lhs + rhs, {R{5}, R{7}, R{9}});
  expect_vector_data_eq(rhs - lhs, {R{3}, R{3}, R{3}});
  expect_vector_data_eq(lhs * rhs, {R{4}, R{10}, R{18}});
  expect_vector_data_eq(lhs * 2.5f, {static_cast<std::common_type_t<T, float>>(2.5f), static_cast<std::common_type_t<T, float>>(5.0f), static_cast<std::common_type_t<T, float>>(7.5f)});
  expect_vector_data_eq(3 * lhs, {R{3}, R{6}, R{9}});
}

TYPED_TEST(VectorTypedTest, NormsAndComparison) {
  using T = TypeParam;

  Vector<T> values{T{3}, T{4}, T{12}};
  EXPECT_FLOAT_EQ(values.norm_1(), 19.0f);
  EXPECT_FLOAT_EQ(values.norm(), 13.0f);
  EXPECT_FLOAT_EQ(values.norm_inf(), 12.0f);

  Vector<T> equal_values{T{3}, T{4}, T{12}};
  Vector<T> smaller{T{3}, T{4}, T{11}};
  EXPECT_TRUE(values == equal_values);
  EXPECT_FALSE(values == smaller);
  EXPECT_EQ(values <=> smaller, std::partial_ordering::greater);
}

TYPED_TEST(VectorTypedTest, FormattingAndStreaming) {
  using T = TypeParam;

  Vector<T> vector{T{1}, T{2}, T{3}};
  EXPECT_EQ(std::format("{}", vector), "[ 1, 2, 3 ]\n");

  std::ostringstream stream;
  stream << vector;
  EXPECT_EQ(stream.str(), "[ 1, 2, 3 ]\n");
}

TEST(VectorHelpers, BasicOperationsAndDotLikeHelpers) {
  Vector<int> lhs{1, 2, 3};
  Vector<double> rhs{4.0, 5.0, 6.0};

  expect_vector_data_eq(add(lhs, rhs), {5.0, 7.0, 9.0});
  expect_vector_data_eq(sub(rhs, lhs), {3.0, 3.0, 3.0});
  expect_vector_data_eq(scl(lhs, 2.0), {2.0, 4.0, 6.0});

  EXPECT_DOUBLE_EQ(dot(lhs, rhs), 32.0);
  EXPECT_NEAR(angle_cos(lhs, rhs), 32.0f / (std::sqrt(14.0f) * std::sqrt(77.0f)), 1e-5f);
}

TEST(VectorHelpers, LinearCombinationAndInterpolation) {
  Vector<int> a{1, 2, 3};
  Vector<double> b{4.0, 5.0, 6.0};

  EXPECT_DOUBLE_EQ(linear_combination(3, 2.5), 7.5);
  EXPECT_DOUBLE_EQ(linear_interpolation(2, 10.0, 0.25f), 4.0);

  std::vector<Vector<int>> candidates{a, Vector<int>{7, 8, 9}};
  std::vector<double> scalars{0.25, 0.75};
  expect_vector_data_eq(linear_combination(candidates, scalars), {5.5, 6.5, 7.5});

  expect_vector_data_eq(linear_interpolation(a, b, 0.25f), {1.75, 2.75, 3.75});
}

TEST(VectorHelpers, DeathChecks) {
  EXPECT_DEATH(([] {
    Vector<int> lhs{1, 2};
    Vector<int> rhs{1, 2, 3};
    (void)(lhs + rhs);
  }()), "Unable to add vectors with different sizes");

  EXPECT_DEATH(([] {
    Vector<int> lhs{1, 2};
    Vector<int> rhs{1};
    (void)dot(lhs, rhs);
  }()), "Unable to compute dot product of vectors with different sizes");
}

TYPED_TEST(MatrixTypedTest, ConstructorsAndAccessors) {
  using T = TypeParam;

  Matrix<T> from_nested{{T{1}, T{2}}, {T{3}, T{4}}};
  expect_matrix_data_eq(from_nested, 2, 2, {T{1}, T{2}, T{3}, T{4}});

  std::vector<T> flat{T{5}, T{6}, T{7}, T{8}};
  Matrix<T> from_flat{flat, 2, 2};
  expect_matrix_data_eq(from_flat, 2, 2, {T{5}, T{6}, T{7}, T{8}});

  Matrix<double> source{{1.5, 2.5}, {3.5, 4.5}};
  Matrix<T> converted{source};
  expect_matrix_data_eq(converted, 2, 2, {static_cast<T>(1.5), static_cast<T>(2.5), static_cast<T>(3.5), static_cast<T>(4.5)});
}

TYPED_TEST(MatrixTypedTest, ArithmeticAndComparison) {
  using T = TypeParam;
  using R = std::common_type_t<T, int>;

  Matrix<T> lhs{{T{1}, T{2}}, {T{3}, T{4}}};
  Matrix<int> rhs{{5, 6}, {7, 8}};
  Matrix<T> equal_matrix{{T{1}, T{2}}, {T{3}, T{4}}};
  Matrix<T> smaller_matrix{{T{1}, T{2}}, {T{3}, T{3}}};

  expect_matrix_data_eq(lhs + rhs, {R{6}, R{8}, R{10}, R{12}});
  expect_matrix_data_eq(rhs - lhs, {R{4}, R{4}, R{4}, R{4}});
  expect_matrix_data_eq(lhs * 2.0f, {static_cast<std::common_type_t<T, float>>(2.0f), static_cast<std::common_type_t<T, float>>(4.0f), static_cast<std::common_type_t<T, float>>(6.0f), static_cast<std::common_type_t<T, float>>(8.0f)});

  EXPECT_TRUE(lhs == equal_matrix);
  EXPECT_FALSE(lhs == smaller_matrix);
  EXPECT_EQ(lhs <=> smaller_matrix, std::partial_ordering::greater);
}

TYPED_TEST(MatrixTypedTest, MultiplicationAndFormatting) {
  using T = TypeParam;

  Matrix<T> lhs{{T{1}, T{2}, T{3}}, {T{4}, T{5}, T{6}}};
  Matrix<int> rhs{{7, 8}, {9, 10}, {11, 12}};

  expect_matrix_data_eq(lhs * rhs, {58, 64, 139, 154});
  EXPECT_EQ(std::format("{}", lhs), "[\n [1, 2, 3],\n [4, 5, 6]\n]\n");
}

TEST(MatrixHelpers, LinearInterpolationAndDot) {
  Matrix<int> lhs{{1, 2}, {3, 4}};
  Matrix<double> rhs{{5.0, 6.0}, {7.0, 8.0}};

  EXPECT_DOUBLE_EQ(dot(lhs, rhs), 70.0);
  EXPECT_NEAR(linear_interpolation(0, 10.0, 0.6f), 6.0, 1e-5);

  expect_matrix_data_eq(linear_interpolation(lhs, rhs, 0.5f), 2, 2, {3.0, 4.0, 5.0, 6.0});
}

TEST(MatrixHelpers, DeathChecks) {
  EXPECT_DEATH(([] {
    Matrix<int> lhs{{1, 2}, {3, 4}};
    Matrix<int> rhs{{1, 2, 3}, {4, 5, 6}};
    (void)(lhs + rhs);
  }()), "Unable to add matrices with different shapes");

  EXPECT_DEATH(([] {
    Matrix<int> lhs{{1, 2}, {3, 4}};
    Matrix<int> rhs{{1, 2}, {3, 4}, {5, 6}};
    (void)(lhs * rhs);
  }()), "Unable to multiply matrices with incompatible shapes");
}

TEST(Complex, ConstructorsComparisonAndFormatting) {
  Complex<int> value{3, -2};
  EXPECT_EQ(value.get_real(), 3);
  EXPECT_EQ(value.get_imag(), -2);

  Complex<double> converted{value};
  EXPECT_DOUBLE_EQ(converted.get_real(), 3.0);
  EXPECT_DOUBLE_EQ(converted.get_imag(), -2.0);

  Complex<int> equal_value{3, -2};
  Complex<int> smaller_value{2, 10};
  Complex<int> different_value{3, 2};
  EXPECT_TRUE(value == equal_value);
  EXPECT_FALSE(value == different_value);
  EXPECT_TRUE((value <=> smaller_value) == std::partial_ordering::greater);

  EXPECT_EQ(std::format("{}", value), "3 -2");
}

TEST(MatrixAndVectorEdgeCases, EmptyContainersAndZeroShape) {
  Matrix<int> empty_matrix{std::vector<std::vector<int>>{}};
  expect_matrix_data_eq(empty_matrix, 0, 0, {});

  Vector<int> vector{std::vector<int>{}};
  expect_vector_data_eq(vector, {});
}