module InterpolationModule
export ilorazyRoznicowe, warNewton, newton_to_natural, rysujNnfx

using Plots

function ilorazyRoznicowe(x::Vector{Float64}, f::Vector{Float64})
    differences = copy(f)
    for i in 1:length(x)-1
        for j in length(x):-1:i+1
            differences[j] = (differences[j] - differences[j-1]) / (x[j] - x[j - i])
        end
    end
    return differences
end


function warNewton(x::Vector{Float64}, fx::Vector{Float64}, t::Float64)
    product = fx[length(fx)]
    for i in (length(fx)-1):-1:1
        product = product * (t - x[i]) + fx[i]
    end
    return product
end

function newton_to_natural(c, x)
    n = length(c)
    a = zeros(Float64, n)
    prod = zeros(Float64, n)

    a[1] = c[1]
    prod[1] = 1.0

    for k in 2:n
        for i in (k-1):-1:1
            prod[i+1] = prod[i] - x[k-1] * prod[i+1]
        end
        prod[1] = -x[k-1] * prod[1]

        for i in 1:k
            a[i] += c[k] * prod[i]
        end
    end

    return a
end



function rysujNnfx(f, a::Float64, b::Float64, n::Int, m::Int, wezly::Symbol = :rownoodlegle)
    if wezly == :rownoodlegle
        x = create_uniform_xs(a,b,n)
    elseif wezly == :czebyszew
        x = create_czebyshev_xs(n)
    else 
        throw(ArgumentError("unknown x creation type"))
    end
    y = []
    for _ in 1:length(x)
        push!(y, f(x))
    end
    differences = ilorazyRoznicowe(x, y)

    plot_x = []
    plot_y = []
    h = (b-a)/Float64(m)
    for k in 1:m
        curr_x = a + k*h
        push!(plot_x, curr_x)
        push!(plot_y, warNewton(x, differences, curr_x))
    end

    gr()
    true_func = plot(x,y, label="true function")
    inter_func = plot(plot_x, plot_y, label="interpolated function")
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

end