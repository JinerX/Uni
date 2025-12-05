using Plots

function rysujNnfx(f, a::Float64, b::Float64, n::Int, wezly::Symbol = :rownoodlegle, m::Int)
    if wezly == :rownoodlegle
        x = create_uniform_xs(a,b,n)
    elseif wezly == :czebyszew
        x = create_czebyshev_xs(n)
    else 
        throw(ArgumentError("unknown x creation type"))
    end

end

function create_uniform_xs(a::Float64, b::Float64 ,n::Int)
    xs = []
    float_n = Float64(n)
    if (b < a)
        b, a = a, b
    end
    h = (b-a)/float_n
    for k in 1:n
        push!(xs, a + k*h)
    end
    return xs
end

function create_czebyshev_xs(n::Int)
    xs = []
    float_n = Float64(n)
    for k in 1:n
        push!(x,  cos((2*k-1)/(2*float_n)*pi))
    end
    return xs
end