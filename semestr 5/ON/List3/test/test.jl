#Jędrzej Sajnóg, indeks: 279701


include("../approx_module/approx_module.jl")
using Test
using .ApproxTools
const AT = ApproxTools

# ============================================================
# Helper functions
# ============================================================

f1(x) = x^2 - 2
df1(x) = 2x

f2(x) = exp(x) - 3
df2(x) = exp(x)

f3(x) = sin(x)
df3(x) = cos(x)

# A flat function near zero (for Newton/ secant error cases)
g(x) = x^3 + 1
dg(x) = 3x^2

q(x) = x^3


# ============================================================
# Tests for mbisekcji (Bisection method)
# ============================================================

@testset "Bisection: mbisekcji" begin
    @testset "Basic correctness" begin
        r, v, it, err = AT.mbisekcji(f1, 1.0, 2.0, 1e-10, 1e-10)
        @test err == 0
        @test it > 0
        @test isapprox(r, sqrt(2), atol=1e-8)
        @test isapprox(v, 0.0, atol=1e-8)
    end

    @testset "Invalid interval (no sign change)" begin
        r, v, it, err = AT.mbisekcji(f1, 2.0, 3.0, 1e-10, 1e-10)
        @test err == 1
        @test isnan(r)
        @test it == 0
    end

    @testset "Root exactly on boundary" begin
        # f1(√2) = 0, so use interval [sqrt(2), 3]
        r, v, it, err = AT.mbisekcji(f1, sqrt(2), 3.0, 1e-10, 1e-10)
        @test err == 1
        @test isequal(r, NaN)
    end
end


# ============================================================
# Tests for mstycznych (Newton's method)
# ============================================================

@testset "Newton: mstycznych" begin
    @testset "Basic convergence" begin
        r, v, it, err = AT.mstycznych(f1, df1, 2.0, 1e-10, 1e-10, 50)
        @test err == 0
        @test it > 0
        @test isapprox(r, sqrt(2), atol=1e-8)
        @test isapprox(v, 0.0, atol=1e-8)
    end

    @testset "Derivative near zero → error=2" begin
        r, v, it, err = AT.mstycznych(g, dg, 1e-20, 1e-8, 1e-8, 10)
        @test err == 2
    end

    @testset "Max iteration reached → error=1" begin
        r, v, it, err = AT.mstycznych(f1, df1, 100.0, 1e-10, 1e-10, 3)
        @test err == 1
    end

    @testset "Starting at exact root" begin
        r, v, it, err = AT.mstycznych(f1, df1, sqrt(2), 1e-10, 1e-10, 50)
        @test err == 0
        @test it == 0
        @test isapprox(v, 0.0, atol=1e-12)
    end
end


# ============================================================
# Tests for msiecznych (Secant method)
# ============================================================

@testset "Secant: msiecznych" begin
    @testset "Basic convergence" begin
        r, v, it, err = AT.msiecznych(f1, 1.0, 2.0, 1e-10, 1e-10, 50)
        @test err == 0
        @test it > 0
        @test isapprox(r, sqrt(2), atol=1e-8)
        @test isapprox(v, 0.0, atol=1e-8)
    end

    @testset "Initial point already a root" begin
        r, v, it, err = AT.msiecznych(f1, sqrt(2), 2.0, 1e-10, 1e-10, 50)
        @test err == 0
        @test it == 0
        @test isapprox(v, 0.0, atol=1e-12)
    end

    @testset "f1(x0) ≈ f1(x1) → division-by-small → error=2" begin
        r, v, it, err = AT.msiecznych(g, 1.0, 1.0 + 1e-12, 1e-10, 1e-10, 10)
        @test err == 2
    end

    @testset "Max iterations reached → error=1" begin
        r, v, it, err = AT.msiecznych(q, 100.0, 101.0, 1e-15, 1e-15, 3)
        @test err == 1
    end
end


# ============================================================
# Tests for f3(x) = sin(x)
# ============================================================

@testset "Function f3(x) = sin(x)" begin
    expected_root = 0.0

    @testset "Bisection" begin
        r, v, it, err = AT.mbisekcji(f3, -1.0, 1.0, 1e-10, 1e-10)
        @test err == 0
        @test it > 0
        @test isapprox(r, expected_root, atol=1e-8)
        @test isapprox(v, 0.0, atol=1e-8)
    end

    @testset "Newton" begin
        r, v, it, err = AT.mstycznych(f3, df3, 0.1, 1e-10, 1e-10, 50)
        @test err == 0
        @test isapprox(r, expected_root, atol=1e-8)
        @test isapprox(v, 0.0, atol=1e-8)

        # bad starting point → max iterations
        r, v, it, err = AT.mstycznych(f3, df3, 1000.0, 1e-10, 1e-10, 2)
        @test err == 1
    end

    @testset "Secant" begin
        r, v, it, err = AT.msiecznych(f3, -1.0, 1.0, 1e-10, 1e-10, 50)
        @test err == 0
        @test it > 0
        @test isapprox(r, expected_root, atol=1e-8)

        r, v, it, err = AT.msiecznych(f3, -0.2, 0.2, 1e-10, 1e-10, 50)
        @test err == 0
        @test isapprox(r, expected_root, atol=1e-8)
    end
end
