types = [Float16, Float32, Float64]



#=

This returns the exact correct value because in IEEE-754 (and in binary in general) the smallest possible increment is a power of 2 so halving gives an exact value 
therefore we do not need to scan the values between prev and add

=#

function find_epsilon(type)
    a = 1
    x = convert(type, a)
    add = x
    prev = add
    while x + add > 1
        prev = add
        add = add / 2
    end
    return prev
end



for type in types
    calc_eps = find_epsilon(type)
    true_eps = eps(type)
    println("Type: $type, calculated epsilon: $calc_eps, true_epsilon: $true_eps")
end