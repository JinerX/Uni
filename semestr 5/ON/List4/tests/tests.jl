# Jędrzej Sajnóg 279701
using Test
using InterpolationModule

@testset "InterpolationModule - simple tests" begin

    @testset "ilorazyRoznicowe" begin
        xs1 = [0.0, 1.0, 2.0]
        f1  = [0.0, 1.0, 4.0]
        expected1 = [0.0, 1.0, 1.0]
        @test isapprox(ilorazyRoznicowe(xs1, f1), expected1; atol=1e-12)

        xs2 = [2.0, 3.0]
        f2  = [5.0, 8.0]       
        expected2 = [5.0, 3.0]
        @test isapprox(ilorazyRoznicowe(xs2, f2), expected2; atol=1e-12)
    end

    @testset "warNewton" begin
       
        x = [0.0, 1.0, 2.0]
        c = [0.0, 1.0, 1.0]  
        @test isapprox(warNewton(x, c, 1.5), 2.25; atol=1e-12) 

       
        x2 = [2.0, 3.0]
        c2 = [5.0, 3.0]      
        @test isapprox(warNewton(x2, c2, 2.5), 6.5; atol=1e-12) 
    end

    @testset "newton_to_natural" begin
       
        x = [0.0, 1.0, 2.0]
        c = [0.0, 1.0, 1.0]
        expected_nat = [0.0, 0.0, 1.0]  
        @test isapprox(newton_to_natural(c, x), expected_nat; atol=1e-12)

       
        x2 = [2.0, 3.0]
        c2 = [5.0, 3.0]
        expected_nat2 = [-1.0, 3.0]     
        @test isapprox(newton_to_natural(c2, x2), expected_nat2; atol=1e-12)
    end

    @testset "create_czebyshev_xs" begin
       
        xs_ref = [cos(pi/6), cos(pi/2), cos(5pi/6)]
        @test isapprox(create_czebyshev_xs(2, -1.0, 1.0), xs_ref; atol=1e-12)

       
        xs_ref_scaled = 2 .+ xs_ref
        @test isapprox(create_czebyshev_xs(2, 1.0, 3.0), xs_ref_scaled; atol=1e-12)
    end

    @testset "create_uniform_xs" begin
       
        @test isapprox(create_uniform_xs(0.0, 1.0, 4), [0.0, 0.25, 0.5, 0.75, 1.0]; atol=1e-12)

       
        @test isapprox(create_uniform_xs(5.0, 1.0, 2), [1.0, 3.0, 5.0]; atol=1e-12)
    end

end
