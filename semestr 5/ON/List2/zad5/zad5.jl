# Jędrzej Sajnóg 279701


r = 3
p0 = 0.01

types = [Float32, Float64]

function simulate_growth(p0,n,r, T)
    p = T(p0)
    for i in 1:1:n
        p = p + T(r*T(p*T((1-T(p)))))
    end
    return p
end


for T in types
    println("========== Type: $T =============")

    p_true = simulate_growth(T(p0),40,r, T)
    println("p calucalted in a standard way: $p_true")
    p_cutoff = simulate_growth(T(p0),10,r, T)
    println("p calucalted after cutoff (10-th round): $p_cutoff")
    p_cutoff = round(p_cutoff, digits=3, RoundDown)
    # println(p_cutoff)
    p_cutoff = simulate_growth(T(p_cutoff), 30, r, T)
    println("p calucalted after cutoff (40-th round): $p_cutoff")
end
