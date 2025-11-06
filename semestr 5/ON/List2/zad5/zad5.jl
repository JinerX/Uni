r = 3
p0 = 0.01

function simulate_growth(p0,n,r)
    p = p0
    for i in 1:1:n
        p = p + r*p*(1-p)
    end
    return p
end

final_p = simulate_growth(p0,1000,r)
println("Final p: $final_p")