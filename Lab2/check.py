import numpy as np
from scipy.interpolate import BarycentricInterpolator


class LagrangeInterpolation:
    def __init__(self, func, interval, n_values):
        self.func = func
        self.interval = interval
        self.n_values = n_values

    def select_nodes(self, nodes, n):
        mid = len(nodes) // 2
        selected_nodes = [nodes[mid]]  # 中间节点
        left_nodes = nodes[:mid]
        right_nodes = nodes[mid + 1 :]

        # 从左边和右边均匀选择节点
        left_selected = np.linspace(
            left_nodes[0], left_nodes[-1], n // 2, endpoint=False
        )
        right_selected = np.linspace(
            right_nodes[0], right_nodes[-1], n // 2, endpoint=True
        )

        return np.concatenate((left_selected, selected_nodes, right_selected))

    def equidistant_nodes(self, n):
        a, b = self.interval
        nodes = np.linspace(a, b, 2 * n + 1)
        return self.select_nodes(nodes, n)

    def chebyshev_nodes(self, n):
        a, b = self.interval
        nodes = np.array(
            [
                0.5 * (a + b)
                + 0.5 * (b - a) * np.cos((2 * i + 1) * np.pi / (2 * (2 * n + 1)))
                for i in range(2 * n + 1)
            ]
        )
        return self.select_nodes(nodes, n)

    def max_error(self, interpolator, test_points):
        true_values = self.func(test_points)
        interpolated_values = interpolator(test_points)
        return np.max(np.abs(true_values - interpolated_values))

    def run(self):
        test_points = np.linspace(self.interval[0], self.interval[1], 501)
        for n in self.n_values:
            equidistant_nodes = self.equidistant_nodes(n)
            chebyshev_nodes = self.chebyshev_nodes(n)

            equidistant_interpolator = BarycentricInterpolator(
                equidistant_nodes, self.func(equidistant_nodes)
            )
            chebyshev_interpolator = BarycentricInterpolator(
                chebyshev_nodes, self.func(chebyshev_nodes)
            )

            equidistant_error = self.max_error(equidistant_interpolator, test_points)
            chebyshev_error = self.max_error(chebyshev_interpolator, test_points)

            print(f"n = {n}, Nodes count: {len(equidistant_nodes)}")
            print(f"Equidistant nodes max error: {equidistant_error:.15e}")
            print(f"Chebyshev nodes max error: {chebyshev_error:.15e}")
            print()


def f(x):
    return 1 / (1 + x**2)


interval = [-5, 5]
n_values = [5, 10, 20, 40]

interpolation = LagrangeInterpolation(f, interval, n_values)
interpolation.run()
