# Jędrzej Sajnóg 279701


using LinearAlgebra

# using(Pkg)

# Pkg.add("LinearAlgebra")

include("hilb.jl")
include("matcond.jl")

ns = 5:5:20
cs = [0,1,3,7,12,16]

# A_hilb = hilb(5)
# A_rand = matcond(5,10.0)

function vector_delta(x_pred, x_true)
    denom = norm(x_true)
    numer = norm(x_true - x_pred)
    if denom == 0
        throw("denominator is 0")
    end
    return numer/denom
end


for n in ns
    println("=============n: $n================")
    println("Hilbert matrix")
    x = ones(n)
    A_hilb = hilb(n)
    
    b = A_hilb * x
    x_hilb_gauss = A_hilb\b
    x_hilb_gauss_delta = vector_delta(x_hilb_gauss, x)
    x_hilb_inv = inv(A_hilb)*b
    x_hilb_inv_delta = vector_delta(x_hilb_inv, x)

    # println("x_gauss: $x_hilb_gauss, δ: $x_hilb_gauss_delta")
    # println("x_inv: $x_hilb_inv, δ: $x_hilb_inv_delta")
    println("condition number: $(cond(A_hilb))")
    println("rank: $(rank(A_hilb))")
    println("gauss method, δ: $x_hilb_gauss_delta")
    println("inverse matrix method, δ: $x_hilb_inv_delta")

    # println("Hilbert matrix:\n $A_hilb")

    println("Random matrix")
    for c in cs
        println("c: $c")
        A_cond = matcond(n, Float64(10^c))
        b = A_cond * x
        x_cond_gauss = A_cond \ b
        x_cond_inv = inv(A_cond) * b 
        x_cond_gauss_delta = vector_delta(x_cond_gauss, x)
        x_cond_inv_delta = vector_delta(x_cond_inv, x)

        # println("x_gauss: $x_cond_gauss, δ: $x_cond_gauss_delta")
        # println("x_inv: $x_cond_inv, δ: $x_cond_inv_delta")
        println("condition number: $(cond(A_cond))")
        println("rank: $(rank(A_cond))")
        println("gauss method, δ: $x_cond_gauss_delta")
        println("inverse matrix method, δ: $x_cond_inv_delta")
    end
end