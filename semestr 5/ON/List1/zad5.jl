# Jędrzej Sajnóg 279701


using Printf


const x = [2.718281828, -3.141592654, 1.414213562, 0.5772156649, 0.3010299957]
const y =  [1486.2497, 878366.9879, -22.37492, 4773714.647, 0.000185049]

function forward_sum(x,y)
    if length(x) != length(y)
        throw(ArgumentError("two values must have the same length"))
    end
    sum = zero(eltype(x))
    for i in 1:1:length(x)
        sum = sum + x[i] * y[i]
    end
    return sum
end

function backward_sum(x,y)
    if length(x) != length(y)
        throw(ArgumentError("two values must have the same length"))
    end
    sum = zero(eltype(x))
    for i in reverse(1:1:length(x))
        sum = sum + x[i] * y[i]
    end
    return sum
end

function sum_single(x)
    sum = 0
    for i in 1:1:length(x)
        sum = sum + x[i]
    end
    return sum
end

function get_products(x,y)
    if length(x) != length(y)
        throw(ArgumentError("input vectors need to have the same length"))
    end
    products = []
    T = eltype(x)
    for i in 1:1:length(x)
        product = T(x[i] * y[i])
        append!(products, product)
    end
    return products
end


function sum_large_to_small(x,y)
    products = get_products(x,y)
    T = eltype(x)
    positives = T[]
    negatives = T[]
    for i in 1:1:length(products)
        if products[i] < zero(T)
            append!(negatives, products[i])
        else
            append!(positives, products[i])
        end
    end
    sort!(positives; rev = true)
    sort!(negatives)
    pos_sum = zero(T)
    neg_sum = zero(T)
    for p in positives; pos_sum += p; end
    for n in negatives; neg_sum += n; end
    s = pos_sum + neg_sum
    return s
end

function sum_small_to_large(x,y)
    products = get_products(x,y)
    T = eltype(x)
    positives = T[]
    negatives = T[]
    for i in 1:1:length(products)
        if products[i] < zero(T)
            append!(negatives, products[i])
        else
            append!(positives, products[i])
        end
    end
    sort!(positives)
    sort!(negatives; rev = true)
    pos_sum = zero(T)
    neg_sum = zero(T)
    for p in positives; pos_sum += p; end
    for n in negatives; neg_sum += n; end
    
    s = neg_sum + pos_sum
    return s
end

function delta(a,b)
    return abs(a-b)/abs(b)
end


types = [Float32, Float64] 

for T in types
    x_t = T.(x)
    y_t = T.(y)

    forward_s = forward_sum(x_t,y_t)
    backward_s = backward_sum(x_t,y_t)
    lts_s = sum_large_to_small(x_t,y_t)
    stl_s = sum_small_to_large(x_t,y_t)
    true_s = -1.00657107000000 * 10^(-11)

    println("========Current type: $T=========")
    println("x: $x_t, y: $y_t")
    println("True sum: $true_s")
    println("Forward: $forward_s, δ: $(delta(forward_s, true_s))")
    println("Backward: $backward_s, δ: $(delta(backward_s, true_s))")
    println("Large to small: $lts_s, δ: $(delta(lts_s, true_s))")
    println("Small to large: $stl_s, δ: $(delta(stl_s, true_s))")
    println("\n\n\n")
end