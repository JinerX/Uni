include("../approx_module/approx_module.jl")
using Test
using .ApproxTools
const AT = ApproxTools

# --------------------------
# Helper test functions
# --------------------------

f1(x) = x^2 - 2
df1(x) = 2x

f2(x) = exp(x) - 3
df2(x) = exp(x)

# --------------------------
# Tests for mbisekcji
# --------------------------

@testset "Bisection: mbisekcji" begin
    # Correct root for x^2 - 2 on [1,2]
    r, v, it, err = AT.mbisekcji(f1, 1.0, 2.0, 1e-10, 1e-10)
    @test isapprox(r, sqrt(2), atol=1e-8)
    @test isapprox(v, 0.0, atol=1e-8)
    @test err == 0
    @test it > 0

    # Wrong interval → error
    r, v, it, err = AT.mbisekcji(f1, 2.0, 3.0, 1e-10, 1e-10)
    @test isnan(r)
    @test err == 1
    @test it == 0
end

# --------------------------
# Tests for mstycznych (Newton)
# --------------------------

@testset "Newton: mstycznych" begin
    # Converges on well-behaved function
    r, v, it, err = AT.mstycznych(f1, df1, 2.0, 1e-10, 1e-10, 50)
    @test isapprox(r, sqrt(2), atol=1e-8)
    @test isapprox(v, 0.0, atol=1e-8)
    @test err == 0
    @test it > 0

    # Derivative near zero → error=2
    # Try starting at x=0 for f(x)=x^2 → f'(0)=0
    g(x) = x^3 + 1
    dg(x) = 3x^2

    r, v, it, err = AT.mstycznych(g, dg, 1e-20, 1e-8, 1e-8, 10)
    @test err == 2

    # Max iterations reached → error=1
    # Choose a bad starting point for f1
    r, v, it, err = AT.mstycznych(f1, df1, 100.0, 1e-10, 1e-10, 3)
    @test err == 1
end

# --------------------------
# Tests for msiecznych (Secant)
# --------------------------

@testset "Secant: msiecznych" begin
    # Should converge to sqrt(2)
    r, v, it, err = AT.msiecznych(f1, 1.0, 2.0, 1e-10, 1e-10, 50)
    @test isapprox(r, sqrt(2), atol=1e-8)
    @test isapprox(v, 0.0, atol=1e-8)
    @test err == 0
    @test it > 0

    # Check stopping when f(x) small at start
    r, v, it, err = AT.msiecznych(f1, sqrt(2), 2.0, 1e-10, 1e-10, 50)
    @test err == 0
    @test it == 0
    @test isapprox(v, 0.0, atol=1e-10)

    # Check division-by-small-difference case → error=2
    g(x) = x^3 + 1
    r, v, it, err = AT.msiecznych(g, 1.0, 1.0 + 1e-12, 1e-10, 1e-10, 10)
    @test err == 2

    # Max iteration exit → error=1
    # Use f(x) = x^3 with starting points that go nowhere
    q(x) = x^3
    r, v, it, err = AT.msiecznych(q, 100.0, 101.0, 1e-15, 1e-15, 3)
    @test err == 1
end

# ----------------------------------------
# Tests for f3(x) = sin(x) + 10x
# ----------------------------------------

f3(x) = sin(x)
df3(x) = cos(x)

@testset "Tests for f3 = sin(x)" begin

    # There is a single root near x ≈ 0 (because sin(0)=0)
    # Solve f3(x) = 0 → sin(x) = -10x → small negative x
    # Numerical root ≈ -0.099833... (you can verify externally)
    expected_root = 0

    # --- Bisection ---
    # Choose interval [-1, 1]; f3(-1) < 0, f3(1) > 0 → sign change
    r, v, it, err = AT.mbisekcji(f3, -1.0, 1.0, 1e-10, 1e-10)
    @test err == 0
    println(r)
    @test isapprox(r, expected_root, atol=1e-8)
    @test isapprox(v, 0.0, atol=1e-8)
    @test it > 0

    # --- Newton’s Method ---
    # Start close to the expected root
    r, v, it, err = AT.mstycznych(f3, df3, 0.0, 1e-10, 1e-10, 50)
    @test err == 0
    @test isapprox(r, expected_root, atol=1e-8)
    @test isapprox(v, 0.0, atol=1e-8)

    # Derivative never vanishes (df3(x) = cos(x) + 10 > 9), so no error=2 case
    # Check max iteration error with a terrible starting point
    r, v, it, err = AT.mstycznych(f3, df3, 1000.0, 1e-10, 1e-10, 2)
    @test err == 1

    # --- Secant Method ---
    # Use two initial points on opposite sides of the real root
    r, v, it, err = AT.msiecznych(f3, -1.0, 1.0, 1e-10, 1e-10, 50)
    @test err == 0
    @test isapprox(r, expected_root, atol=1e-8)
    @test isapprox(v, 0.0, atol=1e-8)
    @test it > 0

    # Secant should also converge if starting points are close to the root
    r, v, it, err = AT.msiecznych(f3, -0.2, 0.2, 1e-10, 1e-10, 50)
    @test err == 0
    @test isapprox(r, expected_root, atol=1e-8)

end
