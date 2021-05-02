#include "Test/Test.hpp"
#include "Memory/Memory.hpp"
#include "Maths/Vector4.hpp"
#include <cmath>

int main() {

	test(DE::Vector4);

	DE::Vector4 a(0, 1, 2, 3);
	DE::Vector4 b(4, 5, 6, 7);
	DE::Vector4 c(1, 2, 2, 3);
	DE::Vector4 d;

	test_show(a);
	test_show(b);
	test_show(c);
	test_show(d);

	test_title("OPERATORS");

	test_expected(a + a, DE::Vector4(0 + 0, 1 + 1, 2 + 2, 3 + 3));
	test_expected(a - a, DE::Vector4(0 - 0, 1 - 1, 2 - 2, 3 - 3));
	test_expected(b * a, DE::Vector4(4 * 0, 5 * 1, 6 * 2, 7 * 3));
	test_expected(b / DE::Vector4(1, 1, 2, 3), DE::Vector4(4 / 1.0f, 5 / 1.0f, 6 / 2.0f, 7 / 3.0f));
	test_expected(DE::Vector4(a) -= a, DE::Vector4(0 - 0, 1 - 1, 2 - 2, 3 - 3));
	test_expected(DE::Vector4(a) += a, DE::Vector4(0 + 0, 1 + 1, 2 + 2, 3 + 3));
	test_expected(DE::Vector4(a) *= a, DE::Vector4(0 * 0, 1 * 1, 2 * 2, 3 * 3));
	test_expected(DE::Vector4(a) /= DE::Vector4(1, 1, 2, 3), DE::Vector4(0 / 1.0f, 1 / 1.0f, 2 / 2.0f, 3 / 3.0f));
	test_expected_bool(a == a, true);
	test_expected_bool(a == b, false);
	test_expected_bool(a != a, false);
	test_expected_bool(a != b, true);

	test_title("ARRAY ACCESS");

	test_expected_float(a[0], 0);
	test_expected_float(a[1], 1);
	test_expected_float(a[2], 2);
	test_expected_float(a[3], 3);

	test_title("FUNCTIONS");

	test_expected(DE::Vector4(a).add(a), DE::Vector4(0 + 0, 1 + 1, 2 + 2, 3 + 3));
	test_expected(DE::Vector4(a).sub(a), DE::Vector4(0 - 0, 1 - 1, 2 - 2, 3 - 3));
	test_expected(DE::Vector4(a).mul(DE::Vector4(2, 2, 2, 2)), DE::Vector4(0 * 2, 1 * 2, 2 * 2, 3 * 2));
	test_expected(DE::Vector4(a).div(DE::Vector4(2, 2, 2, 2)), DE::Vector4(0 / 2.0f, 1 / 2.0f, 2 / 2.0f, 3 / 2.0f));
	test_expected(DE::Vector4(a).add(1), DE::Vector4(0 + 1, 1 + 1, 2 + 1, 3 + 1));
	test_expected(DE::Vector4(a).sub(1), DE::Vector4(0 - 1, 1 - 1, 2 - 1, 3 - 1));
	test_expected(DE::Vector4(a).mul(2), DE::Vector4(0 * 2, 1 * 2, 2 * 2, 3 * 2));
	test_expected(DE::Vector4(a).div(2), DE::Vector4(0 / 2.0f, 1 / 2.0f, 2 / 2.0f, 3 / 2.0f));

	test_show(c.div(2));
	test_expected(c, DE::Vector4(1 / 2.0f, 2 / 2.0f, 2 / 2.0f, 3 / 2.0f));

	test_expected(DE::Vector4(b).lerp(a, 0.5f), DE::Vector4(b) += (a - b) * 0.5f);
	test_expected(DE::Vector4(b).lerp(a, 0.1f), DE::Vector4(b) += (a - b) * 0.1f);
	test_expected(DE::Vector4(b).lerp(a, 0.823f), DE::Vector4(b) += (a - b) * 0.823f);
	test_expected_float(a.dot(a), a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w);
	test_expected_float(a.dot(b), a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
	test_expected_float(a.sqrlen(), a.dot(a));
	test_expected_float(a.len(), sqrtf(a.sqrlen()));
	test_expected_float(a.max(), 3);
	test_expected_float(a.min(), 0);
	test_expected_float(a.sqrdst(b), (a - b).dot(a - b));
	test_expected_float(a.dst(b), sqrtf(a.sqrdst(b)));
	test_expected_bool(a.eq(b, 0), false);
	test_expected_bool(a.eq(b, 10), true);
	test_expected(DE::Vector4(a).nor(), DE::Vector4(a).div(a.len()));

	test_expected_float_eps(DE::Vector4(a).clamp(0.1f).len(), 0.1f, 0.1f);
	test_expected_float_eps(DE::Vector4(a).clamp(0.2f).len(), 0.2f, 0.1f);
	test_expected_float_eps(DE::Vector4(a).clamp(0.3f).len(), 0.3f, 0.1f);
	test_expected_float_eps(DE::Vector4(a).clamp(0.4f).len(), 0.4f, 0.1f);
	test_expected_float_eps(DE::Vector4(a).clamp(0.5f).len(), 0.5f, 0.1f);

	summary();

	return 0;
}
