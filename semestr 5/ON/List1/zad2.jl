# Jędrzej Sajnóg 279701


types = [Float16, Float32, Float64]

for T in types
    three = T(3)
    four = T(4)
    approx = three * (four / three - one(T)) - one(T)
    true_eps = eps(T)
    println("Type: $T, true epsilon: $true_eps, approx: $approx")    
end