module InterpolationModule
export ilorazyRoznicowe, warNewton, newton_to_natural, rysujNnfx, create_czebyshev_xs, create_uniform_xs

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

function create_czebyshev_xs(n::Int, a::Float64, b::Float64)
    xs = []
    float_n = Float64(n) + 1.0
    for k in 1:(n+1)
        push!(xs,  cos((2*k-1)/(2*(float_n))*pi))
    end
    mid = 0.5*a + 0.5*b
    half = 0.5*b - 0.5*a

    return mid .+ (half .* xs)
end

function create_uniform_xs(a::Float64, b::Float64 ,n::Int)
    xs = []
    float_n = Float64(n)
    if (b < a)
        b, a = a, b
    end
    h = (b-a)/float_n
    for k in 0:n
        push!(xs, a + k*h)
    end
    return xs
end


function rysujNnfx(f, a::Float64, b::Float64, n::Int, m::Int, wezly::Symbol = :rownoodlegle)
    if wezly == :rownoodlegle
        x = create_uniform_xs(a,b,n)
    elseif wezly == :czebyszew
        x = create_czebyshev_xs(n,a,b)
    else 
        throw(ArgumentError("unknown x creation type"))
    end
    x = Float64.(x)
    y = []
    for x_value in x
        push!(y, f(x_value))
    end
    y = Float64.(y)
    differences = ilorazyRoznicowe(x, y)


    plot_x = []
    plot_y = []
    plot_true_y = []
    h = (b-a)/Float64(m)
    for k in 0:m
        curr_x = a + k*h
        push!(plot_x, curr_x)
        push!(plot_y, warNewton(x, differences, curr_x))
        push!(plot_true_y, f(curr_x))
    end


    gr()
    plot(plot_x,plot_true_y, label="true function")
    scatter!(x,y,label="points")
    p = plot!(plot_x, plot_y, label="interpolated function")
    return p
    # plot(true_plot, interpolated_plot)
end




end
